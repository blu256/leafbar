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

#ifndef _LEAFBAR_TASK_GROUPER_H
#define _LEAFBAR_TASK_GROUPER_H

// Leafbar
#include "task_button.h"

class LeafbarTaskContainer;
class LeafbarTaskMan;

class LeafbarTaskGrouper : public LeafbarTaskButton {
  TQ_OBJECT

  public:
    LeafbarTaskGrouper(LeafbarTaskContainer *parent, TDEConfig *cfg, TQString name);
    ~LeafbarTaskGrouper();

    TQString name();
    TQPixmap icon();

    bool expanded() { return m_expanded; }
    void setExpanded(bool expanded, bool dontSave = false);

    bool autoExpand();
    void setAutoExpand(bool autoExpand);

    static void updateStaticPixmaps();

    bool pinnable();
    bool pinned() { return m_pinned; }

    LeafbarTaskMan *manager() const;

  public slots:
    void pin();
    void unpin();
    void showPropertiesDialog();
    void saveState();
    void loadState();
    void toggleAutoExpand();

  protected:
    TQFont font();
    TQColorGroup colors();
    void drawButton(TQPainter *p);
    void contextMenuEvent(TQContextMenuEvent *);
    void mousePressEvent(TQMouseEvent *);
    void mouseDoubleClickEvent(TQMouseEvent *);

  private:
    TDEConfig *m_config;
    bool m_expanded;
    bool m_pinned;

  signals:
    void pinChanged(bool pinned);
};

#endif // _LEAFBAR_TASK_GROUPER_H

/* kate: replace-tabs true; tab-width 2; */