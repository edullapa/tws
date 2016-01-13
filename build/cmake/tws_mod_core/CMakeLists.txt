#
#   Copyright (C) 2014 National Institute For Space Research (INPE) - Brazil.
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
#  Description: Main CMake script for TWS build system.
#
#  Author: Gilberto Ribeiro de Queiroz
#          Eduardo Llapa Rodriguez
#          Matheus Cavassan Zaglia
#          Roger Victor
#

file(GLOB TWS_SRC_FILES ${TWS_ABSOLUTE_ROOT_DIR}/src/tws/core/*.cpp)
file(GLOB TWS_HDR_FILES ${TWS_ABSOLUTE_ROOT_DIR}/src/tws/core/*.hpp)

source_group("Source Files"  FILES ${TWS_SRC_FILES})
source_group("Header Files"  FILES ${TWS_HDR_FILES})

add_library(tws_mod_core SHARED ${TWS_HDR_FILES} ${TWS_SRC_FILES})

target_link_libraries(tws_mod_core   ${Boost_SYSTEM_LIBRARY}
                                     ${Boost_DATE_TIME_LIBRARY}
                                     ${Boost_CHRONO_LIBRARY}
                                     ${Boost_TIMER_LIBRARY}
                                     ${Boost_FILESYSTEM_LIBRARY}
                                     ${Boost_PROGRAM_OPTIONS_LIBRARY}
                                     ${Boost_THREAD_LIBRARY})

install(TARGETS tws_mod_core
        EXPORT tws-targets
        RUNTIME DESTINATION ${TWS_DESTINATION_RUNTIME} COMPONENT runtime
        LIBRARY DESTINATION ${TWS_DESTINATION_LIBRARY} COMPONENT runtime
        ARCHIVE DESTINATION ${TWS_DESTINATION_ARCHIVE} COMPONENT runtime)
