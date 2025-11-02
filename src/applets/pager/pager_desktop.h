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


#ifndef _LEAFBAR_PAGER_DESKTOP_H
#define _LEAFBAR_PAGER_DESKTOP_H

// TQt
#include <tqframe.h>

class TQLabel;
class TQFrame;
class LeafbarPager;

class LeafbarPagerDesktop : public TQFrame
{
  public:
    LeafbarPagerDesktop(LeafbarPager *pager, int desktop, const TQPoint &viewport);
    ~LeafbarPagerDesktop();

    bool isCurrent();

  protected:
    virtual void paintEvent(TQPaintEvent *pe);
    virtual void mouseReleaseEvent(TQMouseEvent *me);
    virtual void drawContents(TQPainter *p);

  private:
    LeafbarPager *m_pager;
    KPixmap *m_miniature;
    int m_desktopNo;
    const TQPoint& m_viewport;
};

#endif // _LEAFBAR_PAGER_DESKTOP_H

/* kate: replace-tabs true; tab-width 2; */