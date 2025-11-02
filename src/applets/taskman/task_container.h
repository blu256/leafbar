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

#ifndef _LEAFBAR_TASK_CONTAINER_H
#define _LEAFBAR_TASK_CONTAINER_H

// TDE
#include <kservice.h>
#include <kurl.h>

// TQt
#include <tqvbox.h>

// Leafbar
#include "task_manager.h"
#include "task_grouper.h"

#define FOREACH_TASK(x, tasks) \
  TQObjectList taskList = tasks; \
  TQObjectListIt it(taskList); \
  LeafbarTask *x; \
  while ((x = static_cast<LeafbarTask *>(it.current())) != nullptr) {
#define END_FOREACH_TASK ++it; }

class LeafbarTask;

class LeafbarTaskContainer : public TQVBox {
  TQ_OBJECT

  public:
    LeafbarTaskContainer(LeafbarTaskMan *parent, TQString wclass, TQString aclass);
    ~LeafbarTaskContainer();

    // Constructor for pinned applications
    LeafbarTaskContainer(LeafbarTaskMan *parent, KService::Ptr service, TQString aclass);

    TQString windowClass()      const { return m_wclass; }
    TQString applicationClass() const { return m_aclass; }
    TQString application()      const { return m_appname; }
    TQPixmap groupIcon();

    TQObjectList tasks() const;
    TQObjectList visibleTasks() const;
    TQObjectList currentDesktopTasks() const;

    uint count() const;
    uint visibleCount() const;
    uint currentDesktopCount() const;

    KService::Ptr service() { return m_service; }
    const KURL desktopPath();

    bool isActive()   { return m_active; }
    bool isPinned()   { return m_grouper->pinned(); }
    bool isPinnable() { return m_grouper->pinnable(); }

    bool allIconified();

    LeafbarTaskMan *manager() const { return static_cast<LeafbarTaskMan *>(parent()); }
    LeafbarTaskGrouper *grouper() const { return m_grouper; }
    KWinModule *twin() { return manager()->twin(); }

  public slots:
    void reconfigure();
    void update();
    void updateTaskVisibility();
    void updateActiveTask(WId w);
    void toggleIconifiedAll();
    void closeAll();

    void slotPinChanged(bool pinned);
    void slotDesktopFileChanged(const KURL&, KURL&);

  signals:
    void pinChanged(bool pinned);

  protected:
    TQSize sizeHint() const;

  private:
    KService::Ptr m_service;
    TQString m_wclass, m_aclass, m_appname;
    LeafbarTaskGrouper *m_grouper;
    bool m_active;

  private slots:
    void findService();
    void repaintAll();
};

#endif // _LEAFBAR_TASK_CONTAINER_H

/* kate: replace-tabs true; tab-width 2; */