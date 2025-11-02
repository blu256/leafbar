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

#ifndef _LEAFBAR_APPLET_SELECTOR_H
#define _LEAFBAR_APPLET_SELECTOR_H

// TQt
#include <tqlistbox.h>
#include <tqpainter.h>

// TDE
#include <tdeactionselector.h>

// Leafbar
#include "applet_db.h"

/* --- LeafbarAppletItem class ------------------------------------------------ */
class LeafbarAppletItem : public TQListBoxPixmap {
  public:
    LeafbarAppletItem(AppletData &appletData);
    AppletData& data() { return m_data; }
    int height(const TQListBox *) const;
    int height(const TQFontMetrics fm) const;

  protected:
    void paint(TQPainter *);

  private:
    AppletData& m_data;
};

/* --- LeafbarAppletSelector class -------------------------------------------- */
class LeafbarAppletSelector : public TDEActionSelector {
  TQ_OBJECT

  public:
    LeafbarAppletSelector(TQWidget *parent, const char *name = 0);

    void insertApplet(AppletData &applet);
    void insertActiveApplet(AppletData &applet, int index = -1);

    bool selectedApplet(AppletData &applet);

  signals:
    void changed();
    void activeSelectionChanged();
};

#endif // _LEAFBAR_APPLET_SELECTOR_H

/* kate: replace-tabs true; tab-width 2; */