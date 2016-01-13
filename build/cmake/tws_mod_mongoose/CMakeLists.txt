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
#  CMake scripts for TerraLib Web Services
#
#  Description: Script for generating the build of Mongoose support.
#
#  Author: Gilberto Ribeiro de Queiroz
#

include_directories(${TWS_ABSOLUTE_ROOT_DIR}/third-party/mongoose)

if(TWS_RAPIDJSON_ENABLED)
  include_directories(${RAPIDJSON_INCLUDE_DIR})
endif()

add_definitions(-DMONGOOSE_NO_FILESYSTEM)

file(GLOB TWS_SRC_MONGOOSE_FILES ${TWS_ABSOLUTE_ROOT_DIR}/third-party/mongoose/*.c)
file(GLOB TWS_HDR_MONGOOSE_FILES ${TWS_ABSOLUTE_ROOT_DIR}/third-party/mongoose/*.h)

file(GLOB TWS_SRC_FILES ${TWS_ABSOLUTE_ROOT_DIR}/src/tws/mongoose/*.cpp)
file(GLOB TWS_HDR_FILES ${TWS_ABSOLUTE_ROOT_DIR}/src/tws/mongoose/*.hpp)


# TODO: add link with -pthread
#       add MONGOOSE_NO_LOGGING as a directive
add_library(tws_mod_mongoose SHARED ${TWS_SRC_FILES} ${TWS_HDR_FILES}
                                    ${TWS_SRC_MONGOOSE_FILES} ${TWS_HDR_MONGOOSE_FILES})

target_link_libraries(tws_mod_mongoose tws_mod_core
                                       ${Boost_FILESYSTEM_LIBRARY}
                                       ${Boost_SYSTEM_LIBRARY})

set_target_properties(tws_mod_mongoose
                      PROPERTIES VERSION ${TWS_VERSION_MAJOR}.${TWS_VERSION_MINOR}
                                 SOVERSION ${TWS_VERSION_MAJOR}.${TWS_VERSION_MINOR})

install(TARGETS tws_mod_mongoose
        EXPORT tws-targets
        RUNTIME DESTINATION ${TWS_DESTINATION_RUNTIME} COMPONENT runtime
        LIBRARY DESTINATION ${TWS_DESTINATION_LIBRARY} COMPONENT runtime
        ARCHIVE DESTINATION ${TWS_DESTINATION_ARCHIVE} COMPONENT runtime)

install(FILES ${TWS_HDR_FILES}
        DESTINATION ${TWS_DESTINATION_HEADERS}/tws/mongoose COMPONENT devel)

#install(FILES ${TWS_ABSOLUTE_ROOT_DIR}/share/tws/config/mongoose_web_server.json
#        DESTINATION ${TWS_DESTINATION_SHARE}/tws/config COMPONENT runtime)

#install(FILES ${TWS_ABSOLUTE_ROOT_DIR}/share/tws/plugins/mongoose_plugin.json
#        DESTINATION ${TWS_DESTINATION_SHARE}/tws/plugins COMPONENT runtime)

export(TARGETS tws_mod_mongoose APPEND FILE ${CMAKE_BINARY_DIR}/tws-exports.cmake)
