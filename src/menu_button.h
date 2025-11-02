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

#ifndef _LEAFBAR_MENU_BUTTON_H
#define _LEAFBAR_MENU_BUTTON_H

// TDE
#include <kpushbutton.h>

// Leafbar
#include "panel.h"

class LeafbarMenu;

class LeafbarMenuBtn : public KPushButton {
  TQ_OBJECT

  public:
    LeafbarMenuBtn(LeafbarPanel *panel);
    virtual ~LeafbarMenuBtn();

    LeafbarPanel *panel() { return static_cast<LeafbarPanel *>(parent()); }

  public slots:
    void showMenu();

  protected:
    void mousePressEvent(TQMouseEvent *);
    TQSize sizeHint() const;

  private:
    LeafbarMenu *m_menu;
};

#endif // _LEAFBAR_MENU_BUTTON_H

/* kate: replace-tabs true; tab-width 2; */