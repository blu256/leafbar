/*******************************************************************************
  Leafbar - a DeskBar-style panel for TDE
  Copyright (C) 2023-2025 Philippe Mavridis <philippe.mavridis@yandex.com>

  This program is free software: you can redistribute it and/or modify it under
  the terms of the GNU General Public License as published by the Free Software
  Foundation, either version 3 of the License, or (at your option) any later
  version.

  This program is distributed in the hope that it will be useful, but WITHOUT
  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
  FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
  You should have received a copy of the GNU General Public License along with
  this program. If not, see <http://www.gnu.org/licenses/>.

  Based on the Kicker system tray applet.
  Copyright (c) 2000-2001 Matthias Ettrich <ettrich@kde.org>
                2000-2001 Matthias Elter   <elter@kde.org>
                2001      Carsten Pfeiffer <pfeiffer@kde.org>
                2001      Martijn Klingens <mklingens@yahoo.com>
                2004      Aaron J. Seigo   <aseigo@kde.org>
                2010      Timothy Pearson  <kb9vqf@pearsoncomputing.net>

  Improvements and feedback are welcome!
*******************************************************************************/

// TQt
#include <tqwhatsthis.h>
#include <tqcheckbox.h>
#include <tqlabel.h>
#include <tqvbox.h>

// TDE
#include <tdeapplication.h>
#include <tdeaboutdata.h>
#include <tdelocale.h>
#include <kdebug.h>

// Leafbar
#include "status_widget.h"
#include "system_tray_cfg.h"
#include "utils.h"
#include "version.h"
#include "system_tray.h"
#include "system_tray.moc"

extern "C"
{
    TDE_EXPORT LeafbarApplet *init(LeafbarPanel *parent, TDEConfig *cfg)
    {
        return new LeafbarSysTray(parent, cfg);
    }

    TDE_EXPORT TDEAboutData *about()
    {
        auto about = new TDEAboutData(
            "leafbar_system_tray",
            I18N_NOOP("System Tray"),
            leafbar::version,
            I18N_NOOP("Area for resident application icons"),
            TDEAboutData::License_GPL_V3,
            leafbar::copyright,
            I18N_NOOP("Based on the Kicker system tray applet.")
        );
        about->addCredit("Matthias Ettrich", I18N_NOOP("Kicker applet developer") " (2000-2001)", "ettrich@kde.org");
        about->addCredit("Matthias Elter",   I18N_NOOP("Kicker applet developer") " (2000-2001)", "elter@kde.org");
        about->addCredit("Carsten Pfeiffer", I18N_NOOP("Kicker applet developer") " (2001)",      "pfeiffer@kde.org");
        about->addCredit("Martijn Klingens", I18N_NOOP("Kicker applet developer") " (2001)",      "mklingens@yahoo.com");
        about->addCredit("Aaron J. Seigo",   I18N_NOOP("Kicker applet developer") " (2004)",      "aseigo@kde.org");
        about->addCredit("Timothy Pearson",  I18N_NOOP("Kicker applet developer") " (2010)",      "kb9vqf@pearsoncomputing.net");
        return about;
    }

    TDE_EXPORT TQWidget *config(TQWidget *parent, const TQString& cfg)
    {
        return new LeafbarSysTrayConfig(parent, cfg);
    }
}

LeafbarSysTray::LeafbarSysTray(LeafbarPanel *parent, TDEConfig *cfg)
  : LeafbarApplet(parent, cfg, "LeafbarSystray"),
    m_layout(nullptr),
    m_cols(0),
    m_doingRelayout(false),
    m_icon_size(22),
    m_icon_padding(1),
    m_margin(5),
    m_valid(false),
    m_status(nullptr)
{
    setSizePolicy(TQSizePolicy::MinimumExpanding, TQSizePolicy::Maximum);
    setPaletteBackgroundColor(colorGroup().button().dark(110));
    setFrameStyle(TQFrame::StyledPanel|TQFrame::Sunken);
    setLineWidth(1);
    setMidLineWidth(3);
    setMargin(5);

    m_twin = new KWinModule(this);

    const WIdList systrayWindows = m_twin->systemTrayWindows();
    for (WIdList::ConstIterator it = systrayWindows.begin();
         it  != systrayWindows.end(); ++it)
    {
        embedWindow(*it, true);
    }

    connect(m_twin, TQ_SIGNAL(systemTrayWindowAdded(WId)),
            this,   TQ_SLOT(trayWindowAdded(WId)));

    connect(m_twin, TQ_SIGNAL(systemTrayWindowRemoved(WId)),
            this,   TQ_SLOT(updateTrayWindows()));

    connect(tdeApp, TQ_SIGNAL(tdedisplayPaletteChanged()),
            this,   TQ_SLOT(paletteChanged()));

    m_valid = acquireSystemTray();
    if (!m_valid) {
        m_error = i18n("Unable to acquire system tray!");
    }

    TQWhatsThis::add(this, i18n("The system tray provides status information, "
                                "quick access to some functions and a place "
                                "to conveniently store application windows "
                                "from the task list."));

    relayout(true);
}

LeafbarSysTray::~LeafbarSysTray()
{
    ZAP(m_status);
}

TQSize LeafbarSysTray::sizeHint() const
{
    return m_layout->sizeHint();
}

void LeafbarSysTray::reconfigure()
{
    relayout(true);
}

bool LeafbarSysTray::acquireSystemTray()
{
    TQCString screenstr;
    screenstr.setNum(tqt_xscreen());
    TQCString trayatom = "_NET_SYSTEM_TRAY_S" + screenstr;

    Display *d = tqt_xdisplay();
    net_system_tray_selection = XInternAtom(d, trayatom, false);
    net_system_tray_opcode = XInternAtom(d, "_NET_SYSTEM_TRAY_OPCODE", false);

    XSetSelectionOwner(d, net_system_tray_selection, winId(), CurrentTime);

    if (XGetSelectionOwner(d, net_system_tray_selection) == winId())
    {
        WId root = tqt_xrootwin();

        XClientMessageEvent xe;
        xe.type = ClientMessage;
        xe.window = root;

        xe.message_type = XInternAtom(d, "MANAGER", False);
        xe.format = 32;
        xe.data.l[0] = CurrentTime;
        xe.data.l[1] = net_system_tray_selection;
        xe.data.l[2] = winId();

        // manager-specific data
        xe.data.l[3] = 0;
        xe.data.l[4] = 0;

        XSendEvent(d, root, False, StructureNotifyMask, (XEvent *)&xe);
        return true;
    }
    return false;
}

void LeafbarSysTray::embedWindow(WId w, bool tde_tray)
{
    TrayEmbed *ew = new TrayEmbed(tde_tray, this);
    ew->setAutoDelete(false);

    if (tde_tray) {
        Display *d = tqt_xdisplay();

        static Atom tde_tray_embed_atom =
        XInternAtom(d, "_TDE_SYSTEM_TRAY_EMBEDDING", False);

        XChangeProperty(d, w, tde_tray_embed_atom, tde_tray_embed_atom, 32,
                        PropModeReplace, nullptr, 0);
        ew->embed(w);
        XDeleteProperty(d, w, tde_tray_embed_atom);
    }
    else ew->embed(w);

    if (ew->embeddedWinId() == 0)
    {
        ZAP(ew)
        return;
    }

    m_tray.append(ew);
    relayout(true);

    connect(ew, TQ_SIGNAL(embeddedWindowDestroyed()), TQ_SLOT(updateTrayWindows()));
    ew->setFixedSize(iconSize());
    ew->show();
}

bool LeafbarSysTray::isWinManaged(WId w)
{
    TrayEmbedList::const_iterator it;
    for (it = m_tray.begin(); it != m_tray.end(); ++it) {
        if ((*it)->embeddedWinId() == w) {
            return true;
        }
    }
    return false;
}

void LeafbarSysTray::trayWindowAdded(WId w)
{
    if (isWinManaged(w)) return;

    embedWindow(w, true);
    relayout(true);
}

void LeafbarSysTray::updateTrayWindows()
{
    TrayEmbedList::iterator it = m_tray.begin();
    while (it != m_tray.end()) {
        WId w = (*it)->embeddedWinId();
        if ((w == 0) ||
            ((*it)->tdeTray() && !m_twin->systemTrayWindows().contains(w)))
        {
            (*it)->hide();
            (*it)->deleteLater();
            it = m_tray.erase(it);
        }
        else ++it;
    }
    relayout(true);
}

void LeafbarSysTray::resizeEvent(TQResizeEvent *)
{
    relayout();
}

bool LeafbarSysTray::x11Event(XEvent *xe)
{
    if (xe->type == ClientMessage)
    {
        if (xe->xclient.message_type == net_system_tray_opcode &&
            xe->xclient.data.l[1] == SYSTRAY_REQUEST_DOCK)
        {
            WId w = xe->xclient.data.l[2];

            // ignore event if window is already managed by us
            if (isWinManaged(w)) {
                return true;
            }

            embedWindow(w, false);
            return true;
        }
    }
    return false;
}

void LeafbarSysTray::relayout(bool force)
{
    if (m_doingRelayout) return;

    TQSize icon = iconSize();
    int width = static_cast<TQWidget *>(parent())->width() - margin();
    int cols = width / (icon.width() + iconPadding());
    int rows = m_tray.size() / cols;
    if (m_cols == cols && !force) return;

    m_doingRelayout = true;
    delete m_layout;
    m_layout = new TQGridLayout(this, rows, cols, margin(), iconPadding());

    int col = 0, row = 0;
    m_cols = cols;

    config()->setGroup("SysTray");
    if (config()->readBoolEntry("EnableStatusWidget", true))
    {
        if (!m_status)
        {
            m_status = new LeafbarStatusWidget(this);
        }
        m_layout->addMultiCellWidget(m_status, 0, 0, 0, m_cols);
        row += 1;
    }

    TrayEmbedList::const_iterator it;
    for (it = m_tray.begin(); it != m_tray.end(); ++it)
    {
        m_layout->addWidget((*it), row, col, TQt::AlignCenter);
        ++col;
        if (col >= m_cols)
        {
            ++row;
            col = 0;
        }
    }
    if (col != 0) {
        m_layout->addItem(new TQSpacerItem(icon.width(), icon.height(),
                                           TQSizePolicy::Expanding,
                                           TQSizePolicy::Fixed), row, col);
    }

    m_doingRelayout = false;
}

void LeafbarSysTray::paletteChanged()
{
    TrayEmbedList::const_iterator it;
    for (it = m_tray.begin(); it != m_tray.end(); ++it)
    {
        XClearArea(x11Display(), (*it)->embeddedWinId(), 0, 0, 0, 0, True);
    }
}

TrayEmbed::TrayEmbed(bool _tde_tray, TQWidget *parent)
  : QXEmbed(parent), tde_tray(_tde_tray)
{
    setSizePolicy(TQSizePolicy::Fixed, TQSizePolicy::Fixed);
}

TrayEmbed::~TrayEmbed() {}

/* kate: replace-tabs true; tab-width 4; */