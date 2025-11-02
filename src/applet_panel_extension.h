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

#ifndef _LEAFBAR_APPLET_PANEL_EXTENSION_H
#define _LEAFBAR_APPLET_PANEL_EXTENSION_H

// TQt
#include <tqobject.h>

// TDE
#include <kservice.h>
#include <kurl.h>

#define panelExt LeafbarAppletPanelExtension::instance()

class LeafbarAppletPanelExtension : public TQObject {
  TQ_OBJECT

  public:
    static LeafbarAppletPanelExtension *instance();

    void popup(TQString icon, TQString caption, TQString message);
    void launch(KService::Ptr service, KURL::List urls = KURL::List());

  signals:
    void popupRequest(TQString icon, TQString caption, TQString message);
    void launchRequest(KService::Ptr service, KURL::List urls = KURL::List());

  private:
    LeafbarAppletPanelExtension();
    ~LeafbarAppletPanelExtension();
};

#endif // _LEAFBAR_H

/* kate: replace-tabs true; tab-width 2; */