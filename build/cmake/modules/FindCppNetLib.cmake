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
#  Description: Find CppNetLib include directory and libraries.
#
#  CPPNETLIB_INCLUDE_DIR    -> where to find boost/network.hpp and other header files.
#  CPPNETLIB_URI_LIBRARY    -> uri library to link to.
#  CPPNETLIB_CLIENT_LIBRARY -> client library to link to.
#  CPPNETLIB_SERVER_LIBRARY -> server library to link to.
#  CPPNETLIB_FOUND          -> True if CppNetLib found.
#

find_path(CPPNETLIB_INCLUDE_DIR boost/network.hpp
          PATHS /usr
                /usr/local
          PATH_SUFFIXES include)

if(UNIX)
  find_library(CPPNETLIB_URI_LIBRARY
               NAMES cppnetlib-uri
               PATHS /usr
                     /usr/local
               PATH_SUFFIXES lib)

  find_library(CPPNETLIB_CLIENT_LIBRARY
               NAMES cppnetlib-client-connections
               PATHS /usr
                     /usr/local
               PATH_SUFFIXES lib)
               
  find_library(CPPNETLIB_SERVER_LIBRARY
               NAMES cppnetlib-server-parsers
               PATHS /usr
                     /usr/local
               PATH_SUFFIXES lib)

elseif(WIN32)

  find_library(CPPNETLIB_URI_LIBRARY_RELEASE
               NAMES cppnetlib-uri
               PATHS /usr
                     /usr/local
               PATH_SUFFIXES lib)

  find_library(CPPNETLIB_URI_LIBRARY_DEBUG
               NAMES cppnetlib-urid cppnetlib-uri_d
               PATHS /usr
                     /usr/local
               PATH_SUFFIXES lib)

  find_library(CPPNETLIB_CLIENT_LIBRARY_RELEASE
               NAMES cppnetlib-client-connections
               PATHS /usr
                     /usr/local
               PATH_SUFFIXES lib)

  find_library(CPPNETLIB_CLIENT_LIBRARY_DEBUG
               NAMES cppnetlib-client-connectionsd cppnetlib-client-connections_d
               PATHS /usr
                     /usr/local
               PATH_SUFFIXES lib)

  find_library(CPPNETLIB_SERVER_LIBRARY_RELEASE
               NAMES cppnetlib-server-parsers
               PATHS /usr
                     /usr/local
               PATH_SUFFIXES lib)

  find_library(CPPNETLIB_SERVER_LIBRARY_DEBUG
               NAMES cppnetlib-server-parsersd cppnetlib-server-parsers_d
               PATHS /usr
                     /usr/local
               PATH_SUFFIXES lib)

  if(CPPNETLIB_URI_LIBRARY_RELEASE AND CPPNETLIB_URI_LIBRARY_DEBUG)
    set(CPPNETLIB_URI_LIBRARY optimized ${CPPNETLIB_URI_LIBRARY_RELEASE} debug ${CPPNETLIB_URI_LIBRARY_DEBUG})
  elseif(CPPNETLIB_URI_LIBRARY_RELEASE)
    set(CPPNETLIB_URI_LIBRARY optimized ${CPPNETLIB_URI_LIBRARY_RELEASE} debug ${CPPNETLIB_URI_LIBRARY_RELEASE})
  elseif(CPPNETLIB_URI_LIBRARY_DEBUG)
    set(CPPNETLIB_URI_LIBRARY optimized ${CPPNETLIB_URI_LIBRARY_DEBUG} debug ${CPPNETLIB_URI_LIBRARY_DEBUG})
  endif()

  if(CPPNETLIB_CLIENT_LIBRARY_RELEASE AND CPPNETLIB_CLIENT_LIBRARY_DEBUG)
    set(CPPNETLIB_CLIENT_LIBRARY optimized ${CPPNETLIB_CLIENT_LIBRARY_RELEASE} debug ${CPPNETLIB_CLIENT_LIBRARY_DEBUG})
  elseif(CPPNETLIB_CLIENT_LIBRARY_RELEASE)
    set(CPPNETLIB_CLIENT_LIBRARY optimized ${CPPNETLIB_CLIENT_LIBRARY_RELEASE} debug ${CPPNETLIB_CLIENT_LIBRARY_RELEASE})
  elseif(CPPNETLIB_CLIENT_LIBRARY_DEBUG)
    set(CPPNETLIB_CLIENT_LIBRARY optimized ${CPPNETLIB_CLIENT_LIBRARY_DEBUG} debug ${CPPNETLIB_CLIENT_LIBRARY_DEBUG})
  endif()

  if(CPPNETLIB_SERVER_LIBRARY_RELEASE AND CPPNETLIB_SERVER_LIBRARY_DEBUG)
    set(CPPNETLIB_SERVER_LIBRARY optimized ${CPPNETLIB_SERVER_LIBRARY_RELEASE} debug ${CPPNETLIB_SERVER_LIBRARY_DEBUG})
  elseif(CPPNETLIB_SERVER_LIBRARY_RELEASE)
    set(CPPNETLIB_SERVER_LIBRARY optimized ${CPPNETLIB_SERVER_LIBRARY_RELEASE} debug ${CPPNETLIB_SERVER_LIBRARY_RELEASE})
  elseif(CPPNETLIB_SERVER_LIBRARY_DEBUG)
    set(CPPNETLIB_SERVER_LIBRARY optimized ${CPPNETLIB_SERVER_LIBRARY_DEBUG} debug ${CPPNETLIB_SERVER_LIBRARY_DEBUG})
  endif()

endif()

set(CPPNETLIB_LIBRARIES CPPNETLIB_URI_LIBRARY CPPNETLIB_CLIENT_LIBRARY CPPNETLIB_SERVER_LIBRARY)

include(FindPackageHandleStandardArgs)

FIND_PACKAGE_HANDLE_STANDARD_ARGS(CppNetLib DEFAULT_MSG CPPNETLIB_LIBRARIES CPPNETLIB_INCLUDE_DIR)

mark_as_advanced(CPPNETLIB_INCLUDE_DIR CPPNETLIB_URI_LIBRARY CPPNETLIB_CLIENT_LIBRARY CPPNETLIB_SERVER_LIBRARY)
