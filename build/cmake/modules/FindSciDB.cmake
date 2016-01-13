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
#  Author: Gilberto Ribeiro de Queiroz <gribeiro@dpi.inpe.br>
#
#  Description: Find SciDB include directory and C++ client library.
#
#  SCIDB_INCLUDE_DIR    -> where to find SciDBAPI.h and other header files.
#  SCIDB_CLIENT_LIBRARY -> C++ client library to link to.
#  SCIDB_FOUND          -> True if SciDB C++ client library is found.
#

find_path(SCIDB_INCLUDE_DIR SciDBAPI.h
          PATHS /usr
                /usr/local
                /opt/scidb/14.3
                /opt/scidb/13.12
          PATH_SUFFIXES include
                        scidb/include)

find_library(SCIDB_CLIENT_LIBRARY
             NAMES scidbclient
             PATHS /usr
                   /usr/local
                   /opt/scidb/14.3
                   /opt/scidb/13.12
             PATH_SUFFIXES lib
                           scidb/lib)

include(FindPackageHandleStandardArgs)

FIND_PACKAGE_HANDLE_STANDARD_ARGS(SciDB DEFAULT_MSG SCIDB_CLIENT_LIBRARY SCIDB_INCLUDE_DIR)

mark_as_advanced(SCIDB_INCLUDE_DIR SCIDB_CLIENT_LIBRARY)
