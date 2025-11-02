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

#ifndef _LEAFBAR_TASKMANAGER_H
#define _LEAFBAR_TASKMANAGER_H

// TQt
#include <tqdict.h>
#include <tqintdict.h>

// TDE
#include <twinmodule.h>

// Leafbar
#include "applet.h"

typedef TQValueList<WId> WIdList;

class LeafbarPanel;
class LeafbarTaskContainer;
class LeafbarTask;

class LeafbarTaskMan : public LeafbarApplet {
  TQ_OBJECT

  public:
    LeafbarTaskMan(LeafbarPanel *panel, TDEConfig *cfg);
    virtual ~LeafbarTaskMan();

    static TQString className(WId w);
    static TQString classClass(WId w);

    bool valid() { return true; }
    KWinModule *twin();

    bool autoSaveGroupers() { return m_autoSaveGroupers; }
    bool defaultExpandGroupers() { return m_defaultExpandGroupers; }
    bool showAllDesktops() { return m_showAllDesktops; }
    bool showDesktopNumber() { return m_showDesktopNumber; }
    bool bigGrouperIcons() { return m_bigGrouperIcons; }
    bool showTaskIcons() { return m_showTaskIcons; }

  public slots:
    void addWindow(WId w);
    void removeWindow(WId w);
    void updateWindow(WId w, unsigned int changes);
    void savePinnedApplications();
    void updateTaskVisibility();
    void relayout();

  signals:
    void windowActivated(WId w);

  private:
    TQDict<LeafbarTaskContainer> m_containers;
    TQIntDict<LeafbarTask> m_tasks;
    KWinModule *m_twin;

    bool m_autoSaveGroupers, m_defaultExpandGroupers,
         m_showTaskIcons, m_showAllDesktops, m_showDesktopNumber,
         m_bigGrouperIcons;

    void addContainer(LeafbarTaskContainer *c);
    void removeContainer(LeafbarTaskContainer *c);

  private slots:
    void slotPinChanged(bool pinned);
    void reconfigure();

  friend class LeafbarTaskContainer;
};

#endif // _LEAFBAR_TASKMANAGER_H

/* kate: replace-tabs true; tab-width 2; */