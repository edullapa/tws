#
#  Copyright (C) 2014-2014 National Institute For Space Research (INPE) - Brazil.
#
#  This file is part of the TerraLib Web Services.
#
#  TerraLib Web Services is free software: you can redistribute it and/or modify
#  it under the terms of the GNU General Public License version 3 as
#  published by the Free Software Foundation.
#
#  TerraLib Web Services is distributed  "AS-IS" in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY OF ANY KIND; without even the implied warranty
#  of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
#  GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License along
#  with TerraLib Web Services. See COPYING. If not, see <http://www.gnu.org/licenses/gpl-3.0.html>.
#
#
#  CMake scripts for TerraLib Web Services
#
#  Author: Gilberto Ribeiro de Queiroz
#
#  Description: Find LibGD include directory and library.
#
#  LIBGD_INCLUDE_DIR    -> where to find gd.h and other header files.
#  LIBGD_LIBRARY        -> LibGD library to link to.
#  LIBGD_FOUND          -> True if LibGD is found.
#

find_path(LIBGD_INCLUDE_DIR gd.h
          PATHS /usr
                /usr/local
          PATH_SUFFIXES include
                        libgd/include)

find_library(LIBGD_LIBRARY
             NAMES gd
             PATHS /usr
                   /usr/local
             PATH_SUFFIXES lib
                           libgd/lib)

include(FindPackageHandleStandardArgs)

FIND_PACKAGE_HANDLE_STANDARD_ARGS(LibGD DEFAULT_MSG LIBGD_LIBRARY LIBGD_INCLUDE_DIR)

mark_as_advanced(LIBGD_INCLUDE_DIR LIBGD_LIBRARY)
