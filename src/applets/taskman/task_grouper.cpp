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

  Improvements and feedback are welcome!
*******************************************************************************/

// TQt
#include <tqobjectlist.h>
#include <tqpainter.h>
#include <tqstyle.h>
#include <tqtimer.h>

// TDE
#include <tdeapplication.h>
#include <tdestandarddirs.h>
#include <kiconloader.h>
#include <tdepopupmenu.h>
#include <kpropertiesdialog.h>
#include <tdelocale.h>
#include <kdebug.h>

// Leafbar
#include "task.h"
#include "task_container.h"
#include "task_grouper.h"
#include "task_grouper.moc"

static TQPixmap *pxLink = nullptr;

LeafbarTaskGrouper::LeafbarTaskGrouper(LeafbarTaskContainer *parent, TDEConfig *cfg, TQString name)
  : LeafbarTaskButton(parent, LeafbarTaskButton::Grouper),
    m_config(cfg),
    m_expanded(true),
    m_pinned(false)
{
    updateStaticPixmaps();

    loadState();
}

LeafbarTaskGrouper::~LeafbarTaskGrouper()
{
}

LeafbarTaskMan* LeafbarTaskGrouper::manager() const
{
    return container()->manager();
}

#define ICON(iconVar, icon, group, size) \
    if (!iconVar || iconVar->isNull()) iconVar = new TQPixmap( \
        tdeApp->iconLoader()->loadIcon( \
            icon, group, size.height(), TDEIcon::DefaultState, nullptr, true \
        ) \
    );
#define ICON_SMALL(iconVar, icon) \
    ICON(iconVar, icon, TDEIcon::Small, smallIconSize())
#define ICON_BIG(iconVar, icon) \
    ICON(iconVar, icon, TDEIcon::Panel, bigIconSize())

void LeafbarTaskGrouper::updateStaticPixmaps()
{
    ICON_BIG(pxLink, tdeApp->iconLoader()->theme()->linkOverlay())
}
#undef ICON
#undef ICON_SMALL
#undef ICON_BIG

TQString LeafbarTaskGrouper::name()
{
    return container()->application();
}

TQPixmap LeafbarTaskGrouper::icon()
{
    TQPixmap groupIcon = container()->groupIcon();
    return groupIcon.isNull() ? defaultIcon() : groupIcon;
}

bool LeafbarTaskGrouper::autoExpand()
{
    m_config->setGroup("TaskGroupers");
    return m_config->readBoolEntry(container()->applicationClass(),
                                   manager()->defaultExpandGroupers());
}

void LeafbarTaskGrouper::setAutoExpand(bool autoExpand)
{
    m_config->setGroup("TaskGroupers");
    m_config->writeEntry(container()->applicationClass(), autoExpand);
    m_config->sync();
}

void LeafbarTaskGrouper::toggleAutoExpand()
{
    setAutoExpand(!autoExpand());
}

void LeafbarTaskGrouper::saveState()
{
    setAutoExpand(expanded());
}

void LeafbarTaskGrouper::loadState()
{
    setExpanded(autoExpand(), true);
}

void LeafbarTaskGrouper::setExpanded(bool expanded, bool dontSave)
{
    m_expanded = expanded;
    setOn(m_expanded);

    TQTimer::singleShot(0, this, TQ_SLOT(repaint()));

    // hide/show tasks
    FOREACH_TASK(t, container()->tasks())
    {
        t->updateVisibility();
    }
    END_FOREACH_TASK

    if (!dontSave && manager()->autoSaveGroupers())
    {
        saveState();
    }
}

bool LeafbarTaskGrouper::pinnable()
{
    return (container()->service() != nullptr);
}

void LeafbarTaskGrouper::pin()
{
    if (!pinnable()) return;
    m_pinned = true;
    TQTimer::singleShot(0, this, TQ_SLOT(repaint()));
    emit pinChanged(true);
}

void LeafbarTaskGrouper::unpin()
{
    m_pinned = false;
    TQTimer::singleShot(0, this, TQ_SLOT(repaint()));
    emit pinChanged(false);
}

TQFont LeafbarTaskGrouper::font()
{
    return container()->isActive() ? boldFont() : normalFont() ;
}

TQColorGroup LeafbarTaskGrouper::colors()
{
    TQColorGroup cg = LeafbarTaskButton::colors();
    if (container()->isActive())
    {
      TQColor highlight = blendColors(cg.button(), cg.highlight());
      cg.setColor(TQColorGroup::Button,     highlight);
      cg.setColor(TQColorGroup::Background, highlight);
      cg.setColor(TQColorGroup::ButtonText, cg.highlightedText());
    }
    return cg;
}

void LeafbarTaskGrouper::drawButton(TQPainter *p)
{
    LeafbarTaskButton::drawButton(p);

    // arrow
    if (container()->count() > 0 &&
        (manager()->showAllDesktops() || container()->currentDesktopCount() > 0))
    {
        int h = height();
        TQRect r(width() - h, 0, h, h);
        style().drawPrimitive(
          expanded() ? TQStyle::PE_ArrowUp : TQStyle::PE_ArrowDown,
          p, r, colorGroup()
        );
    }

    // link
    if (m_pinned)
    {
        TQPoint origin = TQPoint(iconOffset());
        p->drawPixmap(origin, *pxLink);
    }
}

void LeafbarTaskGrouper::contextMenuEvent(TQContextMenuEvent *cme)
{
    TDEPopupMenu ctx, opts;
    int item;

    if (pinnable())
    {
      if (pinned())
      {
          ctx.insertItem(TQPixmap(locate("data", "leafbar/pics/pindown.png")),
                        i18n("Unpin application"), this, TQ_SLOT(unpin()));
      }
      else
      {
          ctx.insertItem(TQPixmap(locate("data", "leafbar/pics/pinup.png")),
                        i18n("Pin application"), this, TQ_SLOT(pin()));
      }
      ctx.insertSeparator();
    }

    if (!manager()->autoSaveGroupers())
    {
        TQString label = expanded()
            ? i18n("Auto-expand this application")
            : i18n("Don't auto-expand this application");
        item = opts.insertItem(label, this, TQ_SLOT(toggleAutoExpand()));
        opts.setItemChecked(item, autoExpand() == expanded());
    }
    ctx.insertItem(SmallIcon("configure"), i18n("Grouper options"), &opts);

    if (container()->tasks().count())
    {
        item = ctx.insertItem(SmallIcon("taskbar"), i18n("Minimize all"),
                              container(), TQ_SLOT(toggleIconifiedAll()));
        ctx.setItemChecked(item, container()->allIconified());

        item = ctx.insertItem(SmallIcon("close"), i18n("Close all"),
                              container(), TQ_SLOT(closeAll()));
    }

    KService::Ptr s = container()->service();
    if (s && s->desktopEntryPath() != TQString::null)
    {
        ctx.insertSeparator();
        ctx.insertItem(SmallIcon("document-properties"), i18n("Properties"),
                       this, TQ_SLOT(showPropertiesDialog()));
    }

    if (ctx.count())
    {
        ctx.exec(cme->globalPos());
        cme->accept();
    }
}

void LeafbarTaskGrouper::mousePressEvent(TQMouseEvent *me)
{
    switch (me->button())
    {
        case LeftButton:
            if (!container()->tasks().count()) return;

            if (me->state() & (ControlButton | AltButton))
            {
                container()->closeAll();
            }
            else if (!(me->state() & ControlButton))
            {
                setExpanded(!expanded());
            }
            return;

        case MidButton:
            container()->toggleIconifiedAll();
            return;

        default:
            me->ignore();
    }
}
void LeafbarTaskGrouper::mouseDoubleClickEvent(TQMouseEvent *me) {
    if (me->button() == LeftButton && container()->service()) {
        panelExt->launch(container()->service());

        // HACK the second click gets consumed
        if (container()->tasks().count() && !(me->state() & ControlButton)) {
            toggle();
        }
    }
    me->ignore();
}

void LeafbarTaskGrouper::showPropertiesDialog() {
    const KURL path = container()->desktopPath();
    if (!path.isValid()) { // should never happen actually
        kdWarning() << container()->service()->name()
                    << "does not have a desktop entry!" << endl;
        return;
    }
    KPropertiesDialog *d = new KPropertiesDialog(KURL(path), this);
    auto c = container();
    connect(d, TQ_SIGNAL(applied()),
            c, TQ_SLOT(update()));
    connect(d, TQ_SIGNAL(saveAs(const KURL&, KURL&)),
            c, TQ_SLOT(slotDesktopFileChanged(const KURL&, KURL&)));
}

/* kate: replace-tabs true; tab-width 4; */