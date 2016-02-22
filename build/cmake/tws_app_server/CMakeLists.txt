#
#  Copyright (C) 2014 National Institute For Space Research (INPE) - Brazil.
#
#  This file is part of TWS.
#
#  TWS is free software: you can
#  redistribute it and/or modify it under the terms of the
#  GNU Lesser General Public License as published by
#  the Free Software Foundation, either version 3 of the License,
#  or (at your option) any later version.
#
#  TWS is distributed in the hope that
#  it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
#  GNU Lesser General Public License for more details.
#
#  You should have received a copy of the GNU Lesser General Public License
#  along with TWS. See LICENSE. If not, write to
#  e-sensing team at <esensning-team@dpi.inpe.br>.
#
#
#  CMake scripts for TerraLib GeoWeb Services
#
#  Description: Script for generating the Web Server.
#
#  Author: Gilberto Ribeiro de Queiroz <gribeiro@dpi.inpe.br>
#

include_directories(${terralib_INCLUDE_DIRS})
include_directories(${RAPIDJSON_INCLUDE_DIR})

file(GLOB TWS_SRC_FILES ${TWS_ABSOLUTE_ROOT_DIR}/src/tws/app_server/*.cpp)
file(GLOB TWS_HDR_FILES ${TWS_ABSOLUTE_ROOT_DIR}/src/tws/app_server/*.hpp)

add_executable(tws_app_server ${TWS_SRC_FILES} ${TWS_HDR_FILES})

target_link_libraries(tws_app_server tws_mod_core
                                     terralib_mod_plugin
                                     terralib_mod_common
                                     ${Boost_FILESYSTEM_LIBRARY}
                                     ${Boost_SYSTEM_LIBRARY})

install(TARGETS tws_app_server
        EXPORT tws-targets
        RUNTIME DESTINATION ${TWS_DESTINATION_RUNTIME} COMPONENT runtime
        LIBRARY DESTINATION ${TWS_DESTINATION_LIBRARY} COMPONENT runtime
        ARCHIVE DESTINATION ${TWS_DESTINATION_ARCHIVE} COMPONENT runtime)

export(TARGETS tws_app_server APPEND FILE ${CMAKE_BINARY_DIR}/tws-exports.cmake)

