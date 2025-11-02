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

#ifndef _LEAFBAR_KICKER_APPLET_H
#define _LEAFBAR_KICKER_APPLET_H

// TDE
#include <kpanelapplet.h>

// Leafbar
#include "applet.h"
#include "applet_db.h"

class LeafbarPanel;
class KLibrary;

class LeafbarKickerAppletInterface : public KPanelApplet
{
  public:
    int heightFor(int width); // HACK
};

class LeafbarKickerApplet : public LeafbarApplet
{
  TQ_OBJECT

  public:
    LeafbarKickerApplet(LeafbarPanel *parent, AppletData data);
    virtual ~LeafbarKickerApplet();

    virtual bool valid();
    virtual TQString lastErrorMessage();

  protected:
    void about();
    void preferences();
    void resizeEvent(TQResizeEvent *);
    void loadApplet();
    void unloadApplet();

  private slots:
    virtual void reconfigure();
    void slotUpdateGeometry();

  private:
    AppletData m_data;
    KLibrary *m_lib;
    LeafbarKickerAppletInterface *m_applet;
    bool m_valid;
};


#endif // _LEAFBAR_KICKER_APPLET_H

/* kate: replace-tabs true; tab-width 2; */