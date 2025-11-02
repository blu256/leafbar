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

// TDE
#include <dcopclient.h>

// Leafbar
#include "settings.h"
#include "utils.h"
#include "application.h"
#include "application.moc"

LeafbarApp::LeafbarApp(PanelPosition pos) : DCOPObject("LeafbarIface")
{
    disableSessionManagement();

    if (!tdeApp->dcopClient()->isRegistered())
    {
        tdeApp->dcopClient()->registerAs("leafbar");
    }
    tdeApp->dcopClient()->setDefaultObject("LeafbarIface");

    LeafbarSettings::instance("leafbarrc");

    m_panel = new LeafbarPanel();
    if (pos != PanelPosition::Saved)
    {
        m_panel->setPosition(pos, true);
    }
    setTopWidget(m_panel);
    m_panel->show();
}

LeafbarApp::~LeafbarApp()
{
    ZAP(m_panel)
}

bool LeafbarApp::ping()
{
    return true;
}

void LeafbarApp::reconfigure()
{
    m_panel->reconfigure();
}

void LeafbarApp::reloadApplet(TQString applet)
{
    m_panel->reloadApplet(applet);
}

void LeafbarApp::quit()
{
    tdeApp->quit();
}

/* kate: replace-tabs true; tab-width 4; */