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
#include <kdebug.h>

// Leafbar
#include "applet_panel_extension.h"
#include "applet_panel_extension.moc"

static LeafbarAppletPanelExtension *leafbarAppletPanelExtension = nullptr;
LeafbarAppletPanelExtension *LeafbarAppletPanelExtension::instance() {
    if (!leafbarAppletPanelExtension) {
        leafbarAppletPanelExtension = new LeafbarAppletPanelExtension();
    }
    return leafbarAppletPanelExtension;
}

LeafbarAppletPanelExtension::LeafbarAppletPanelExtension()
: TQObject()
{}

LeafbarAppletPanelExtension::~LeafbarAppletPanelExtension() {}

void LeafbarAppletPanelExtension::popup(TQString icon, TQString caption,
                                      TQString message)
{
    emit popupRequest(icon, caption, message);
}

void LeafbarAppletPanelExtension::launch(KService::Ptr service, KURL::List urls)
{
    emit launchRequest(service, urls);
}

/* kate: replace-tabs true; tab-width 4; */