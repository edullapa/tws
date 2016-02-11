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
#  Description: Find RapidXml include directory.
#
#  RAPIDXML_INCLUDE_DIR - where to find rapidxml/rapidxml.hpp and other header files.
#  RAPIDXML_FOUND       - True if RapidXml found.
#

find_path(RAPIDXML_INCLUDE_DIR rapidxml/rapidxml.hpp
          PATHS /usr
                /usr/local
          PATH_SUFFIXES include
                        include/rapidxml)

include(FindPackageHandleStandardArgs)

FIND_PACKAGE_HANDLE_STANDARD_ARGS(RapidXml DEFAULT_MSG RAPIDXML_INCLUDE_DIR)

mark_as_advanced(RAPIDXML_INCLUDE_DIR)
