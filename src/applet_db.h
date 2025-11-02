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

#ifndef _LEAFBAR_APPLET_DB_H
#define _LEAFBAR_APPLET_DB_H

// TQt
#include <tqfile.h>

// TDE
#include <tqmap.h>

// Leafbar
#include <config.h>

class TDEAboutData;
class LeafbarApplet;

struct AppletData
{
    bool loaded = false;
    TQString desktopFile;
#ifdef WITH_KICKER_APPLETS
    bool isKickerApplet = false;
#endif

    TQCString id;
    TQString name;
    TQString icon;
    TQString library;
    TQString comment;
    TDEAboutData *about;
    bool hasConfig;
    bool unique;
    LeafbarApplet *ptr = nullptr;

    bool valid()
    {
        return loaded && !id.isNull() && !name.isNull() && !library.isNull();
    }

    const char *libPath()
    {
        return TQFile::encodeName(library);
    }
};

typedef TQMap<TQCString, AppletData> AppletMap;

class LeafbarAppletDB : public TQObject
{
  public:
    static LeafbarAppletDB *instance();

    bool contains(TQString id);

    TQValueList<TQCString> applets();

    AppletData &operator[](TQString id);

    static int licenseFromString(TQString str);

  protected:
    bool loadApplet(AppletData &data);

  private:
    AppletMap m_applets;
    LeafbarAppletDB();
    ~LeafbarAppletDB() = default;
};

#endif // _LEAFBAR_APPLETDB_H

/* kate: replace-tabs true; tab-width 2; */