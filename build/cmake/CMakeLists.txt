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

cmake_minimum_required(VERSION 2.8.12)

project(tws)

#
# main variables for the build script
#

# variables that control the system versioning
set(TWS_VERSION_MAJOR 1)
set(TWS_VERSION_MINOR 0)
set(TWS_VERSION_PATCH 0)
set(TWS_VERSION_STATUS "alpha")
set(TWS_VERSION_STATUS_FULL "-alpha1")
set(TWS_VERSION_STRING "${TWS_VERSION_MAJOR}.${TWS_VERSION_MINOR}.${TWS_VERSION_PATCH}${TWS_VERSION_STATUS_FULL}")
set(TWS_VERSION "0x010000")

# variables storing absolute paths
set(TWS_ABSOLUTE_ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../..)

set(TWS_BASE_DESTINATION_DIR "")

if(NOT DEFINED TWS_DESTINATION_RUNTIME)
  set(TWS_DESTINATION_RUNTIME "${TWS_BASE_DESTINATION_DIR}bin")
endif()


#
# global definitions, includes and config
#

# Common definitions to all modules
add_definitions(-DBOOST_ALL_NO_LIB -DBOOST_FILESYSTEM_VERSION=3 -DBOOST_UBLAS_TYPE_CHECK=0)

# Specific definitions per platform
if(APPLE)
  add_definitions(-ftemplate-depth-1024)
endif()

# Build will generate files into a single folder
if((CMAKE_GENERATOR STREQUAL Xcode) OR MSVC)
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
  set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
  set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
else()
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
  set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
  set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
endif()

# Where to find modules (find procedures)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/modules")

# Compiler options: warning and C++ 11
if(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX OR (CMAKE_CXX_COMPILER MATCHES ".*clang"))
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -pedantic -std=c++11")
endif()

# Turn on multi process compilation on Windows
if(MSVC)
  add_definitions(/MP)
endif()

# Enable CTest
enable_testing()

# Build tree directory will be needed in all projects.
include_directories(${CMAKE_BINARY_DIR})


#
# including CMake's auxiliary files
#

include(CMakeDependentOption)


#
# first check of libraries
#

find_package(Boost 1.54.0 REQUIRED system date_time chrono log timer filesystem program_options thread)

if(NOT Boost_FOUND)
  message(FATAL_ERROR "TWS: could not find required Boost libraries!")
endif()

#
# build options
#
# warning: please, do not change the order below!
#

option(TWS_MOD_CORE_ENABLED "Build TWS CORE library?" ON)


#
# process TWS configuration files
#

#configure_file(${CMAKE_SOURCE_DIR}/terrama2-config-pkg.cmake.in
#               ${CMAKE_BINARY_DIR}/pkg/terrama2-config.cmake @ONLY)

#configure_file(${CMAKE_SOURCE_DIR}/terrama2-config-version.cmake.in
#               ${CMAKE_BINARY_DIR}/terrama2-config-version.cmake @ONLY)

#configure_file(${CMAKE_SOURCE_DIR}/terrama2-config.cmake.in
#               ${CMAKE_BINARY_DIR}/terrama2-config.cmake @ONLY)

#configure_file(${TWS_ABSOLUTE_ROOT_DIR}/src/terrama2/Config.hpp.in
#               ${CMAKE_BINARY_DIR}/terrama2_config.hpp @ONLY)

#configure_file(${TWS_ABSOLUTE_ROOT_DIR}/src/terrama2/Defines.hpp.in
#               ${CMAKE_BINARY_DIR}/terrama2_defines.hpp @ONLY)

configure_file(${TWS_ABSOLUTE_ROOT_DIR}/src/tws/Version.hpp.in
               ${CMAKE_BINARY_DIR}/tws_version.hpp @ONLY)

#configure_file(${TWS_ABSOLUTE_ROOT_DIR}/COPYING ${CMAKE_BINARY_DIR}/COPYING.txt COPYONLY)


#
# build modules
#

if(TWS_MOD_CORE_ENABLED)
  add_subdirectory(tws_mod_core)
endif()


#
# install and targets export
#

#file(GLOB TWS_HDR_FILES ${TWS_ABSOLUTE_ROOT_DIR}/src/terrama2/*.hpp)

#install(FILES ${CMAKE_BINARY_DIR}/terrama2_config.hpp
#              ${CMAKE_BINARY_DIR}/terrama2_version.hpp
#              ${TWS_HDR_FILES}
#        DESTINATION ${TWS_DESTINATION_HEADERS}/terrama2 COMPONENT devel)

#install(FILES ${TWS_ABSOLUTE_ROOT_DIR}/src/terrama2.hpp
#        DESTINATION ${TWS_DESTINATION_HEADERS} COMPONENT devel)

#install(FILES ${TWS_BINARY_DIR}/pkg/terrama2-config.cmake
#              ${TWS_BINARY_DIR}/terrama2-config-version.cmake
#        DESTINATION ${TWS_DESTINATION_LIBRARY}/cmake/terrama2-${TWS_VERSION_STRING} COMPONENT devel)

#install(EXPORT terrama2-targets
#        DESTINATION ${TWS_DESTINATION_LIBRARY}/cmake/terrama2-${TWS_VERSION_STRING}
#        FILE terrama2-exports.cmake
#        COMPONENT devel)

#install(DIRECTORY ${TWS_ABSOLUTE_ROOT_DIR}/share/terrama2/config
#        DESTINATION ${TWS_DESTINATION_SHARE}/terrama2 COMPONENT runtime)

#install(DIRECTORY ${TWS_ABSOLUTE_ROOT_DIR}/share/terrama2/icons
#        DESTINATION ${TWS_DESTINATION_SHARE}/terrama2 COMPONENT runtime)


#
# Track dependencies
#
#if(TWS_TRACK_3RDPARTY_DEPENDENCIES)
  #set(CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS_SKIP ON)

  #include(InstallRequiredSystemLibraries)

  #install(PROGRAMS ${CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS}
  #        DESTINATION bin
  #        COMPONENT runtime)

  #add_subdirectory(fix_bundle)
#endif()


#
# CPack
#

#set(CPACK_SOURCE_INSTALLED_DIRECTORIES "${TWS_ABSOLUTE_ROOT_DIR};/")

#configure_file(${TWS_SOURCE_DIR}/terrama2-cpack-options.cmake.in
#               ${TWS_BINARY_DIR}/terrama2CPackOptions.cmake
#               @ONLY)

#set(CPACK_PROJECT_CONFIG_FILE ${TWS_BINARY_DIR}/terrama2CPackOptions.cmake)

#include(CPack)

# defining installation types, components and groups
#cpack_add_install_type(full DISPLAY_NAME "Complete")
#cpack_add_install_type(minimalistic DISPLAY_NAME "Minimalistic")

#cpack_add_component_group(general_group
#                          DISPLAY_NAME "Applications"
#                          DESCRIPTION "TWS web servers, web clients and GUI applications"
#                          EXPANDED)

#cpack_add_component_group(devel_group
#                          DISPLAY_NAME "Development Tools"
#                          DESCRIPTION "Development Tools: header files, libraries and cmake stuffs"
#                          EXPANDED)

#cpack_add_component(runtime
#                    DISPLAY_NAME "Runtime Libraries and Executables"
#                    DESCRIPTION "Install only necessary shared libraries (or DLLs) and executables"
#                    GROUP general_group
#                    INSTALL_TYPES full minimalistic)

#cpack_add_component(devel
#                    DISPLAY_NAME "Development package"
#                    DESCRIPTION "Install header files, cmake stuffs and additional shared libraries"
#                    GROUP devel_group
#                    INSTALL_TYPES full minimalistic)

#
# Uninstall target
#

#configure_file("${CMAKE_CURRENT_SOURCE_DIR}/cmake_uninstall.cmake.in"
#               "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
#               IMMEDIATE @ONLY)

#add_custom_target(uninstall COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake)