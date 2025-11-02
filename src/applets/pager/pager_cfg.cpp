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

// TQt
#include <tqlayout.h>
#include <tqcheckbox.h>
#include <tqtabwidget.h>
#include <tqvbox.h>
#include <tqwhatsthis.h>

// TDE
#include <tdeconfig.h>
#include <tdelocale.h>

// Leafbar
#include "pager_cfg.h"
#include "pager_cfg.moc"

LeafbarPagerConfig::LeafbarPagerConfig(TQWidget *parent, const TQString& cfg)
: LeafbarAppletConfig(parent, cfg)
{
    new TQVBoxLayout(this);
    m_tabs = new TQTabWidget(this);
    layout()->add(m_tabs);

    // Appearance
    TQVBox *appearance = new TQVBox(m_tabs);

    m_inline = new TQCheckBox(i18n("Show desktops in single row"), appearance);
    m_miniature = new TQCheckBox(i18n("Show desktop miniatures"), appearance);

    connect(m_inline, TQ_SIGNAL(toggled(bool)), TQ_SIGNAL(changed()));
    connect(m_miniature, TQ_SIGNAL(toggled(bool)), TQ_SIGNAL(changed()));

    m_tabs->addTab(appearance, i18n("Appearance"));
}

LeafbarPagerConfig::~LeafbarPagerConfig()
{
}

void LeafbarPagerConfig::reset()
{
    m_inline->setChecked(true);
    m_miniature->setChecked(false);
}

void LeafbarPagerConfig::load()
{
    auto cfg = config();
    cfg->setGroup("Pager");

    // Appearance
    m_inline->setChecked(cfg->readBoolEntry("ShowInline", true));
    m_miniature->setChecked(cfg->readBoolEntry("ShowMiniature", true));
}

void LeafbarPagerConfig::save()
{
    auto cfg = config();
    cfg->setGroup("Pager");

    // Appearance
    cfg->writeEntry("ShowInline", m_inline->isChecked());
    cfg->writeEntry("ShowMiniature", m_miniature->isChecked());

    cfg->sync();
}

// kate: replace-tabs true; tab-width 4; indent-width 4;