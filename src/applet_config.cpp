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
#include <tdeconfig.h>

// Leafbar
#include "applet_config.h"
#include "applet_config.moc"

LeafbarAppletConfig::LeafbarAppletConfig(TQWidget *parent, const TQString &cfg)
: TQFrame(parent)
{
    m_cfg = new TDEConfig(cfg);
}

LeafbarAppletConfig::~LeafbarAppletConfig()
{
    delete m_cfg;
}