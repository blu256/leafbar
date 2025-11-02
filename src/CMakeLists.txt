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
    ${CMAKE_CURRENT_SOURCE_DIR}/external/dmctl
    ${TDE_INCLUDE_DIR}
    ${TQT_INCLUDE_DIRS}
)

link_directories(
    ${TQT_LIBRARY_DIRS}
)

### libleafbar (library) #########################################################
tde_add_library(
  leafbar SHARED AUTOMOC

  SOURCES
    applet.cpp
    applet_panel_extension.cpp
    applet_config.cpp
    settings.kcfgc

  LINK
    tdecore-shared

  DESTINATION ${LIB_INSTALL_DIR}
)

### leafbar (executable) #########################################################
set(LEAFBAR_OPTIONAL_SOURCES "")
if (${WITH_KICKER_APPLETS})
  list(APPEND LEAFBAR_OPTIONAL_SOURCES
    kicker_applet.cpp
    kickerSettings.kcfgc
  )
endif()

tde_add_executable(
  leafbar AUTOMOC

  SOURCES
    main.cpp
    application.cpp
    panel.cpp
    applet_db.cpp
    menu_button.cpp
    menu.cpp
    root_menu.cpp
    settings_menu.cpp
    leafbariface.skel
    ${LEAFBAR_OPTIONAL_SOURCES}

  LINK
    tdecore-shared
    tdeui-shared
    tdeio-shared
    leafbar-shared
    dmctl-static

  DESTINATION ${BIN_INSTALL_DIR}
)

### kcm_leafbar (module) #########################################################
tde_add_kpart(
  kcm_leafbar AUTOMOC

  SOURCES
    tdecm_leafbar.cpp
    applet_db.cpp
    applet_selector.cpp
    applet_config_dialog.cpp
    settings.kcfgc

  LINK
    tdecore-shared
    tdeui-shared
    tdeio-shared

  DESTINATION ${PLUGIN_INSTALL_DIR}
)

### applets ####################################################################
add_subdirectory(applets)
add_subdirectory(external/dmctl)

### headers ####################################################################
install(
  FILES applet.h applet_config.h
  DESTINATION ${INCLUDE_INSTALL_DIR}/leafbar/
)

### translated desktop files ###################################################
tde_create_translated_desktop(leafbar.desktop
    PO_DIR leafbar-desktops)
tde_create_translated_desktop(leafbar_config.desktop
    PO_DIR leafbar-desktops)

# kate: replace-tabs true; tab-width 2;