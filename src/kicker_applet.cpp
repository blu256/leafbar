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

  WARNING | EXPERIMENTAL FEATURE
  This source code is subject to API changes and might even be entirely
  removed at some point in the future.

  Improvements and feedback are welcome!
*******************************************************************************/

// TQt
#include <tqlayout.h>

// TDE
#include <klibloader.h>

// Leafbar
#include "panel.h"
#include "settings.h"
#include "kicker_applet.h"
#include "kicker_applet.moc"

// HACK
int LeafbarKickerAppletInterface::heightFor(int width)
{
    kdDebug() <<"hforw" << endl;
    setPosition(KPanelApplet::pTop);
    orientationChange(TQt::Horizontal);
    int h = heightForWidth(width);
    kdDebug() << "w = " << width << ", h = " << h << endl;
    return h;
}

LeafbarKickerApplet::LeafbarKickerApplet(LeafbarPanel *parent, AppletData data)
  : LeafbarApplet(parent, nullptr, data.id),
    m_data(data),
    m_lib(nullptr),
    m_applet(nullptr),
    m_valid(false)
{
    setFrameStyle(TQFrame::WinPanel | TQFrame::Sunken);
    setLineWidth(2);

    new TQHBoxLayout(this);
    loadApplet();
    if (!m_applet)
    {
        return;
    }

    setSizePolicy(TQSizePolicy::Fixed, TQSizePolicy::Fixed);
    slotUpdateGeometry();

    m_valid = true;
    m_applet->show();

    LeafbarAppletPanelExtension::instance();
    connect(parent, TQ_SIGNAL(appletsReconfigure()), TQ_SLOT(reconfigure()));
}

LeafbarKickerApplet::~LeafbarKickerApplet()
{
    unloadApplet();
}

bool LeafbarKickerApplet::valid()
{
    return m_valid;
}

TQString LeafbarKickerApplet::lastErrorMessage()
{
    return TQString::null;
}

void LeafbarKickerApplet::resizeEvent(TQResizeEvent *e)
{
    slotUpdateGeometry();
}

void LeafbarKickerApplet::slotUpdateGeometry()
{
    TQSize newSize(
        LeafbarSettings::panelWidth(),
        m_applet->heightFor(LeafbarSettings::panelWidth())
    );
    kdDebug() << m_data.name << " updateGeometry: " << size() << " => " << newSize << endl;
    setFixedSize(newSize);
    emit updateGeometry();
}

void LeafbarKickerApplet::loadApplet()
{
    m_lib = KLibLoader::self()->library(m_data.libPath());
    void *init = m_lib->symbol("init");
    LeafbarKickerAppletInterface *(*c)(TQWidget *, const TQString&, KPanelApplet::Type) =
        (LeafbarKickerAppletInterface *(*)(TQWidget *, const TQString&, KPanelApplet::Type))init;

    m_applet = c(this, "leafbar_" + m_data.id + "_rc", KPanelApplet::Stretch);
    connect(m_applet, TQ_SIGNAL(updateLayout()), TQ_SLOT(slotUpdateGeometry()));

    layout()->add(m_applet);
}

void LeafbarKickerApplet::unloadApplet()
{
    delete m_applet;
    m_applet = nullptr;

    KLibLoader::self()->unloadLibrary(m_data.libPath());
}

/// slot
void LeafbarKickerApplet::reconfigure()
{
//     m_applet->loadSettings();
}

void LeafbarKickerApplet::about()
{
//     m_applet->about();
}

void LeafbarKickerApplet::preferences()
{
//     m_applet->preferences();
}

/* kate: replace-tabs true; tab-width 4; */