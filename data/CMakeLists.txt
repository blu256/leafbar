################################################################################
# Leafbar - a DeskBar-style panel for TDE                                      #
# Copyright (C) 2023-2025 Philippe Mavridis <philippe.mavridis@yandex.com>     #
#                                                                              #
# Improvements and feedback are welcome!                                       #
# This software is licensed under the terms of the GNU GPL v3 license.         #
################################################################################

install(
  FILES iconify.png maximize.png close.png pindown.png pinup.png
  DESTINATION ${DATA_INSTALL_DIR}/leafbar/pics
)

tde_install_icons()