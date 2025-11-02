################################################################################
# Leafbar - a DeskBar-style panel for TDE                                      #
# Copyright (C) 2023-2025 Philippe Mavridis <philippe.mavridis@yandex.com>     #
#                                                                              #
# Improvements and feedback are welcome!                                       #
# This software is licensed under the terms of the GNU GPL v3 license.         #
################################################################################

include_directories(
    ${CMAKE_BINARY_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${TDE_INCLUDE_DIR}
    ${TQT_INCLUDE_DIRS}
    ${CMAKE_SOURCE_DIR}
)

link_directories(
    ${TQT_LIBRARY_DIRS}
)

### leafbar_applet_systray (module) ############################################
tde_add_kpart(leafbar_applet_systray AUTOMOC
  SOURCES
    system_tray.cpp
    status_widget.cpp
    system_graph.cpp
    system_tray_cfg.cpp

  LINK
    tdecore-shared
    tdeui-shared
    tdeio-shared
    leafbar-shared

  DESTINATION ${PLUGIN_INSTALL_DIR}
)

### desktop file ###############################################################
tde_create_translated_desktop(
  SOURCE system_tray.desktop
  DESTINATION ${DATA_INSTALL_DIR}/leafbar/applets
  PO_DIR leafbar-applets-desktops
)

# kate: replace-tabs true; tab-width 2;