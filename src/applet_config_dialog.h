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

#ifndef _LEAFBAR_APPLET_CONFIG_DIALOG_H
#define _LEAFBAR_APPLET_CONFIG_DIALOG_H

// TDE
#include <kdialogbase.h>

// Leafbar
#include "applet_db.h"

class LeafbarAppletConfigDialog : public KDialogBase
{
    TQ_OBJECT

    public:
        LeafbarAppletConfigDialog(TQWidget *parent, AppletData applet);
        ~LeafbarAppletConfigDialog();

        bool changed();

    protected slots:
        virtual void slotOk();
        virtual void slotApply();
        virtual void slotDefault();
        virtual void slotChanged();

    private slots:
        void reconfigure();

    private:
        AppletData m_applet;
        LeafbarAppletConfig *m_config;
        bool m_changed;
};

#endif // _LEAFBAR_APPLET_CONFIG_DIALOG_H