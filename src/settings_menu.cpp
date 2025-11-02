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
#include <tdeapplication.h>

// Leafbar
#include "panel.h"
#include "settings_menu.h"
#include "settings_menu.moc"

LeafbarSettingsMenu::LeafbarSettingsMenu(LeafbarPanel *panel)
  : LeafbarMenu(panel, KServiceGroup::group("Settings/"))
{
    tdecontrol = KService::serviceByDesktopName("kcontrol");
    if (tdecontrol->isValid()) {
        addItem(tdecontrol, this, TQ_SLOT(launchKControl()), KCONTROL_ITEM, 0);
        insertSeparator(1);
    }
}

LeafbarSettingsMenu::~LeafbarSettingsMenu() {
}

void LeafbarSettingsMenu::launchKControl() {
    panel()->launch(tdecontrol);
}

/* kate: replace-tabs true; tab-width 4; */