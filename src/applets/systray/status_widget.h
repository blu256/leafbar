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

#ifndef _LEAFBAR_STATUS_WIDGET_H
#define _LEAFBAR_STATUS_WIDGET_H

// TQt
#include <tqframe.h>

// Leafbar
#include "system_tray.h"

enum DateTimeFormat {
    FormatDateTime,
    FormatDateTimeShort,
    FormatDateTimeShortSecs,
    FormatDateTimeSecs,
    FormatDate,
    FormatDateShort,
    FormatTime,
    FormatTimeShort,
    DATETIMEFORMAT_END
};

class TQLabel;
class TQTimer;

class LeafbarSystemGraph;

class LeafbarStatusWidget : public TQFrame {
  TQ_OBJECT

  public:
    LeafbarStatusWidget(LeafbarSysTray *tray);
    virtual ~LeafbarStatusWidget();

    LeafbarSysTray *sysTray() const {
        return static_cast<LeafbarSysTray *>(parent());
    }

  public slots:
    void configureDateTime();
    void configureDateTimeFormat();

  protected:
    void mousePressEvent(TQMouseEvent *e);

  private slots:
    void updateClock();
    void copyDateTime(int choice);

  private:
    TQLabel *m_clock;
    TQTimer *m_clockTimer;
    LeafbarSystemGraph *m_cpuGraph;
    LeafbarSystemGraph *m_memGraph;

    const TQString formatDateTime(TQDateTime dt, DateTimeFormat f);
};

#endif // _LEAFBAR_STATUS_WIDGET_H

/* kate: replace-tabs true; tab-width 2; */