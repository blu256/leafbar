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

// TQt
#include <tqlayout.h>
#include <tqgroupbox.h>
#include <tqbuttongroup.h>
#include <tqcheckbox.h>
#include <tqradiobutton.h>
#include <tqpushbutton.h>
#include <tqspinbox.h>
#include <tqlabel.h>
#include <tqvbox.h>
#include <tqhbox.h>
#include <tqwhatsthis.h>

// TDE
#include <tdeaboutapplication.h>
#include <tdeglobalsettings.h>
#include <tdeapplication.h>
#include <kgenericfactory.h>
#include <tdestandarddirs.h>
#include <tdedesktopfile.h>
#include <tdemessagebox.h>
#include <kiconloader.h>
#include <kjanuswidget.h>
#include <kdialog.h>
#include <tdelocale.h>
#include <dcopref.h>
#include <tdeio/netaccess.h>

// Leafbar
#include "applet_db.h"
#include "applet_selector.h"
#include "applet_config.h"
#include "applet_config_dialog.h"
#include "settings.h"
#include "version.h"
#include "tdecm_leafbar.h"
#include "tdecm_leafbar.moc"

typedef KGenericFactory<LeafbarConfig, TQWidget> LeafbarConfigFactory;
K_EXPORT_COMPONENT_FACTORY(kcm_leafbar, LeafbarConfigFactory("kcm_leafbar"))

static const char description[] = I18N_NOOP("Leafbar panel configuration module");

LeafbarConfig::LeafbarConfig(TQWidget *parent, const char *name, const TQStringList &)
  : TDECModule(LeafbarConfigFactory::instance(), parent, name)
{
    LeafbarSettings::instance("leafbarrc");

    TDEAboutData *about = new TDEAboutData(I18N_NOOP("kcm_leafbar"),
                          I18N_NOOP("LeafbarConfig"), leafbar::version, description,
                          TDEAboutData::License_GPL_V3, leafbar::copyright);
    setAboutData(about);

    TDEGlobal::locale()->insertCatalogue("leafbar");
    TDEGlobal::dirs()->addResourceType("autostart", "share/autostart");

    setQuickHelp( i18n("<h1>Leafbar</h1> This control module can be used to enable and configure"
        " Leafbar. Leafbar is a BeOS-style panel for TDE. If you choose to use Leafbar, Kicker will"
        " be automatically disabled."));

    new TQVBoxLayout(this);

    m_groupBox = new TQGroupBox(this);
    layout()->setAutoAdd(true);

    new TQVBoxLayout(m_groupBox);
    m_groupBox->layout()->setAutoAdd(true);

#define ICON(x) TDEGlobal::iconLoader()->loadIcon(x, TDEIcon::Panel)
    m_tabWidget = new KJanusWidget(m_groupBox, 0, KJanusWidget::IconList);
    TQVBox *tabGeneral = m_tabWidget->addVBoxPage(i18n("General"),
                                                  "General settings",
                                                  ICON("configure"));

    TQVBox *tabLook = m_tabWidget->addVBoxPage((const TQString)"Appearance",
                                               "Appearance", ICON("icons"));

    TQVBox *tabApplets = m_tabWidget->addVBoxPage(i18n("Applets"),
                                                  "Applets", ICON("kicker"));

#define VBOX_PAGE_PREPARE(tab, stretch) \
    tab->layout()->setAutoAdd(false); \
    if (stretch) ((TQVBoxLayout *)tab->layout())->addStretch();

    // General tab
    m_autostart = new TQCheckBox("Start Leafbar automatically on startup", tabGeneral);
    TQWhatsThis::add(m_autostart, "Start the Leafbar panel automatically when the user session starts.");

    m_replaceKicker = new TQCheckBox("Replace Kicker panel", tabGeneral);
    m_replaceKicker->setEnabled(m_autostart->isOn());
    TQWhatsThis::add(m_replaceKicker, "Don't run the Kicker panel if Leafbar is set to start along with the user session.");

    tabGeneral->layout()->add(m_autostart);
    tabGeneral->layout()->add(m_replaceKicker);

    connect(m_autostart, TQ_SIGNAL(toggled(bool)), m_replaceKicker, TQ_SLOT(setEnabled(bool)));
    connect(m_autostart, TQ_SIGNAL(toggled(bool)), TQ_SLOT(changed()));
    connect(m_replaceKicker, TQ_SIGNAL(toggled(bool)), TQ_SLOT(changed()));

    VBOX_PAGE_PREPARE(tabGeneral, true)

    // Appearance tab
    TQHBox *posBox = new TQHBox(tabLook);
    TQLabel *posLabel = new TQLabel(i18n("Panel position: "), posBox);

    m_grpPos = new TQButtonGroup(posBox);
    m_grpPos->setSizePolicy(TQSizePolicy::Maximum, TQSizePolicy::Fixed);

    new TQHBoxLayout(m_grpPos);
    m_grpPos->layout()->setAutoAdd(true);

    TQRadioButton *posTopLeft  = new TQRadioButton(i18n("Top left"), m_grpPos);
    TQRadioButton *posTopRight = new TQRadioButton(i18n("Top right"), m_grpPos);

    TQHBox *widthBox = new TQHBox(tabLook);
    TQLabel *widthLabel = new TQLabel(i18n("Panel width: "), widthBox);
    m_width = new TQSpinBox(100, 500, 1, widthBox);

    tabLook->layout()->add(posBox);
    tabLook->layout()->add(widthBox);

    connect(m_grpPos,    TQ_SIGNAL(clicked(int)),      TQ_SLOT(changed()));
    connect(m_width,     TQ_SIGNAL(valueChanged(int)), TQ_SLOT(changed()));

    VBOX_PAGE_PREPARE(tabLook, true)

    // Applets tab
    TQHBox *appletHBox = new TQHBox(tabApplets);
    m_appletSelector = new LeafbarAppletSelector(appletHBox);

    TQVBox *appletActions = new TQVBox(appletHBox);
    appletActions->setSizePolicy(TQSizePolicy::Maximum, TQSizePolicy::Fixed);
    appletActions->setMargin(KDialog::marginHint());

    m_appletAbout = new TQPushButton(ICON("help-about"), i18n("About"), appletActions);
    m_appletAbout->setEnabled(false);

    m_appletConfig = new TQPushButton(ICON("configure"), i18n("Options"), appletActions);
    m_appletConfig->setEnabled(false);

    tabApplets->layout()->add(appletHBox);

    connect(m_appletAbout,    TQ_SIGNAL(clicked()), TQ_SLOT(appletAbout()));
    connect(m_appletConfig,   TQ_SIGNAL(clicked()), TQ_SLOT(appletConfig()));
    connect(m_appletSelector, TQ_SIGNAL(changed()), TQ_SLOT(changed()));
    connect(m_appletSelector, TQ_SIGNAL(activeSelectionChanged()),
                              TQ_SLOT(appletSelected()));

    VBOX_PAGE_PREPARE(tabApplets, false)

#undef VBOX_PAGE_PREPARE
#undef ICON

    // Final touches
    setButtons(Help|Apply);
    load();
    show();
}

/// slot
void LeafbarConfig::appletSelected()
{
    AppletData applet;
    if (m_appletSelector->selectedApplet(applet))
    {
        m_appletAbout->setEnabled(true);
        m_appletConfig->setEnabled(applet.hasConfig);
        changed();
    }
}

/// slot
void LeafbarConfig::changed()
{
    TDECModule::changed();
}

/// slot
void LeafbarConfig::appletAbout()
{
    AppletData applet;
    if (m_appletSelector->selectedApplet(applet))
    {
        auto aboutDlg = new TDEAboutApplication(applet.about, this);
        aboutDlg->exec();
        delete aboutDlg;
    }
}

/// slot
void LeafbarConfig::appletConfig()
{
    AppletData applet;
    if (m_appletSelector->selectedApplet(applet) && applet.hasConfig)
    {
        auto configDlg = new LeafbarAppletConfigDialog(this, applet);
        configDlg->exec();
        delete configDlg;
    }
}

void LeafbarConfig::load() {
    LeafbarSettings::self()->readConfig();

    // General tab
    m_autostart->setChecked(LeafbarSettings::autostart());
    m_replaceKicker->setChecked(LeafbarSettings::replaceKicker());

    // Appearance tab
    m_grpPos->setButton(LeafbarSettings::position());
    m_width->setValue(LeafbarSettings::panelWidth());

    // Applets tab
    loadApplets();
}

void LeafbarConfig::save() {
    // Try to install autostart files if needed
    if (m_autostart->isChecked())
    {
        if (m_replaceKicker->isChecked())
        {
            if (!uninstallAutostart() || !installPanelAutostart())
            {
                m_autostart->setChecked(false);
            }
        }
        else
        {
            if (!uninstallPanelAutostart() || !installAutostart())
            {
                m_autostart->setChecked(false);
            }
        }
    }
    else if (!m_autostart->isChecked())
    {
        if (!uninstallAutostart() || !uninstallPanelAutostart())
        {
            m_autostart->setChecked(true);
        }
    }

    bool autoStartPolicyChanged = (LeafbarSettings::autostart() != m_autostart->isChecked());
    bool kickerPolicyChanged = (LeafbarSettings::replaceKicker() != m_replaceKicker->isChecked());

    // General tab
    LeafbarSettings::setAutostart(m_autostart->isChecked());
    LeafbarSettings::setReplaceKicker(m_replaceKicker->isChecked());

    // Appearance tab
    LeafbarSettings::setPosition(m_grpPos->selectedId());
    LeafbarSettings::setPanelWidth(m_width->value());

    // Applets tab
    TQStringList applets;
    TQListBoxItem *item = m_appletSelector->selectedListBox()->firstItem();

    // Prompt if the user wants to start/kill Leafbar right away
    if (autoStartPolicyChanged)
    {
        if (m_autostart->isChecked() && !leafbarAlive())
        {
            int result = KMessageBox::questionYesNo(
                this,
                i18n("<qt>You have chosen to enable the Leafbar panel."
                     " Would you like to start it immediately?"
                     "<br><br>Note: even if you select 'No', Leafbar will "
                     "still be started the next time you log in.</qt>")
            );

            if (result == KMessageBox::Yes)
            {
                tdeApp->startServiceByDesktopName("leafbar");
            }
        }
        else if (!m_autostart->isChecked() && leafbarAlive())
        {
            int result = KMessageBox::questionYesNo(
                this,
                i18n("<qt>You have choosed to disable the Leafbar panel."
                     " Would you like to stop it immediately?</qt>")
            );

            if (result == KMessageBox::Yes)
            {
                DCOPRef leafbar("leafbar", "LeafbarIface");
                leafbar.call("quit()");
            }
        }
    }

    if (autoStartPolicyChanged || kickerPolicyChanged)
    {
        if (m_autostart->isChecked() && m_replaceKicker->isChecked())
        {
            if (kickerAlive())
            {
                DCOPRef kicker("kicker", "kicker");
                kicker.call("quit()");
            }
        }
        else if (!kickerAlive())
        {
            tdeApp->tdeinitExec("kicker");
        }
    }

    // Update applets
    while (item)
    {
        applets << static_cast<LeafbarAppletItem *>(item)->data().id;
        item = item->next();
    }
    LeafbarSettings::setApplets(applets);

    // Save and ask Leafbar to re-read configuration
    LeafbarSettings::self()->writeConfig();

    DCOPRef leafbar("leafbar", "LeafbarIface");
    leafbar.call("reconfigure()");
}

void LeafbarConfig::loadApplets()
{
    TQStringList active = LeafbarSettings::applets();
    LeafbarAppletDB *appletdb = LeafbarAppletDB::instance();
    TQValueList<TQCString> applets = appletdb->applets();
    TQValueList<TQCString>::iterator it;
    for (it = applets.begin(); it != applets.end(); ++it)
    {
        TQCString id((*it));
        AppletData &applet = (*appletdb)[id];
        int index;
        if ((index = active.findIndex(id)) != -1)
        {
            m_appletSelector->insertActiveApplet(applet, index);
        }
        else
        {
            m_appletSelector->insertApplet(applet);
        }
    }
}

bool LeafbarConfig::kickerAlive()
{
    DCOPRef leafbar("kicker", "default");
    DCOPReply reply = leafbar.call("panelPosition()");
    return reply.isValid();
}

bool LeafbarConfig::leafbarAlive()
{
    DCOPRef leafbar("leafbar", "LeafbarIface");
    DCOPReply reply = leafbar.call("ping()");
    return reply.isValid() && (bool)reply == true;
}

const KURL LeafbarConfig::autostartPath()
{
    KURL autostartPath = KURL::fromPathOrURL(TDEGlobalSettings::autostartPath());
    autostartPath.addPath("leafbar.desktop");
    return autostartPath;
}

bool LeafbarConfig::autostartInstalled()
{
    return TQFile::exists(autostartPath().path());
}

bool LeafbarConfig::installAutostart()
{
    if (autostartInstalled()) return true;

    const KURL &src = KURL::fromPathOrURL(locate("xdgdata-apps", "tde/leafbar.desktop"));
    const KURL &dst = autostartPath();
    bool ok = TDEIO::NetAccess::file_copy(src, dst, -1, true, false, this);
    if (!ok)
    {
        KMessageBox::detailedError(
            this,
            i18n("Unable to install autostart file '%1'.").arg(dst.path()),
            TDEIO::NetAccess::lastErrorString()
        );
    }
    return ok;
}

bool LeafbarConfig::uninstallAutostart()
{
    if (!autostartInstalled()) return true;

    const KURL &path = autostartPath();
    bool ok = TDEIO::NetAccess::del(path, this);
    if (!ok)
    {
        KMessageBox::detailedError(
            this,
            i18n("Unable to delete autostart file '%1'.").arg(path.path()),
            TDEIO::NetAccess::lastErrorString()
        );
    }
    return ok;
}

const KURL LeafbarConfig::panelAutostartPath()
{
    return KURL::fromPathOrURL(locateLocal("autostart", "panel.desktop"));
}

bool LeafbarConfig::panelAutostartInstalled()
{
    const KURL &path = panelAutostartPath();
    if (!TQFile::exists(path.path())) return false;

    TDEDesktopFile panelAutostart(path.path(), true);
    return panelAutostart.readEntry("Exec") == "leafbar";
}

bool LeafbarConfig::installPanelAutostart()
{
    if (panelAutostartInstalled()) return true;

    const KURL &path = panelAutostartPath();
    if (TQFile::exists(path.path()))
    {
        const KURL &bup = KURL::fromPathOrURL(path.path() + ".bup");
        bool ok = TDEIO::NetAccess::file_move(path, bup, -1, true, false, this);
        if (!ok)
        {
            KMessageBox::detailedError(
                this,
                i18n("Unable to backup autostart file '%1'. It will not be overwritten.")
                    .arg(path.path()),
                TDEIO::NetAccess::lastErrorString()
            );
            return false;
        }
    }

    TDEDesktopFile panelAutostart(path.path());
    panelAutostart.writeEntry("Type", "Service");
    panelAutostart.writeEntry("Exec", "leafbar");
    panelAutostart.writeEntry("OnlyShowIn", "TDE;");
    panelAutostart.writeEntry("X-TDE-autostart-after", "kdesktop");
    panelAutostart.writeEntry("X-TDE-autostart-phase", "0");
    panelAutostart.sync();
    return true;
}

bool LeafbarConfig::uninstallPanelAutostart()
{
    if (!panelAutostartInstalled()) return true;

    const KURL &path = panelAutostartPath();
    bool ok = TDEIO::NetAccess::del(path, this);
    if (!ok)
    {
        KMessageBox::detailedError(
            this,
            i18n("Unable to delete autostart file '%1'.").arg(path.path()),
            TDEIO::NetAccess::lastErrorString()
        );
    }
    return ok;
}

/* kate: replace-tabs true; tab-width 4; */