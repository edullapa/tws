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
#          Matheus Cavassan Zaglia
#          Roger Victor
#

cmake_minimum_required(VERSION 2.8.12)

project(tws)

#
# main variables for the build script
#

# variables that control the system versioning
set(TWS_VERSION_MAJOR 0)
set(TWS_VERSION_MINOR 1)
set(TWS_VERSION_PATCH 0)
set(TWS_VERSION_STATUS "")
set(TWS_VERSION_STATUS_FULL "")
set(TWS_VERSION_STRING "${TWS_VERSION_MAJOR}.${TWS_VERSION_MINOR}.${TWS_VERSION_PATCH}${TWS_VERSION_STATUS_FULL}")
set(TWS_VERSION "0x000100")

# variables storing absolute paths
set(TWS_ABSOLUTE_ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../..)

# variables that control paths
set(TWS_BASE_DESTINATION_DIR "")

if(NOT DEFINED TWS_DIR_VAR_NAME)
  set(TWS_DIR_VAR_NAME "TWS_HOME" CACHE STRING "Name of an environment variable with the base installation path of TerraLib GeoWeb Services")
endif()

# variable that defines the path to data directory used in the unittests
if(NOT DEFINED TWS_DATA_DIR)
  set(TWS_DATA_DIR "${CMAKE_BINARY_DIR}/data" CACHE PATH "Path to a directory containing the data used in the examples and/or in the unit test")
endif()

# variables that control install locations
if(NOT DEFINED TWS_DESTINATION_PLUGINS)
  set(TWS_DESTINATION_PLUGINS "${TWS_BASE_DESTINATION_DIR}share/tws/plugins")
endif()

if(NOT DEFINED TWS_DESTINATION_EXAMPLES)
  set(TWS_DESTINATION_EXAMPLES "${TWS_BASE_DESTINATION_DIR}share/tws/examples")
endif()

if(NOT DEFINED TWS_DESTINATION_UNITTEST)
  set(TWS_DESTINATION_UNITTEST "${TWS_BASE_DESTINATION_DIR}share/tws/unittest")
endif()

if(NOT DEFINED TWS_DESTINATION_INTEGRATIONTEST)
  set(TWS_DESTINATION_INTEGRATIONTEST "${TWS_BASE_DESTINATION_DIR}share/tws/integrationtest")
endif()

if(NOT DEFINED TWS_DESTINATION_LIBRARY)
  set(TWS_DESTINATION_LIBRARY "${TWS_BASE_DESTINATION_DIR}lib")
endif()

if(NOT DEFINED TWS_DESTINATION_RUNTIME)
  set(TWS_DESTINATION_RUNTIME "${TWS_BASE_DESTINATION_DIR}bin")
endif()

if(NOT DEFINED TWS_DESTINATION_ARCHIVE)
  set(TWS_DESTINATION_ARCHIVE "${TWS_BASE_DESTINATION_DIR}lib")
endif()

if(NOT DEFINED TWS_DESTINATION_HEADERS)
  set(TWS_DESTINATION_HEADERS "${TWS_BASE_DESTINATION_DIR}include")
endif()

if(NOT DEFINED TWS_DESTINATION_SHARE)
  set(TWS_DESTINATION_SHARE "${TWS_BASE_DESTINATION_DIR}share")
endif()

#
# global definitions, includes and config
#

# Common definitions to all modules
add_definitions(-DBOOST_ALL_NO_LIB -DBOOST_FILESYSTEM_VERSION=3 -DBOOST_UBLAS_TYPE_CHECK=0 -DBOOST_LOG_DYN_LINK)

# Specific definitions per platform
if(APPLE)
  add_definitions(-ftemplate-depth-1024)
endif()

# For each CMake version we need to check some policies
# Note: automatically link executables with qtmain.lib on Windows when it has Qt dependency.
#if("${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION}.${CMAKE_PATCH_VERSION}" VERSION_GREATER  2.8.9)
#  cmake_policy(SET CMP0020 NEW)
#endif()

#if(${CMAKE_MAJOR_VERSION} GREATER 2)
#  cmake_policy(SET CMP0026 OLD)
#  cmake_policy(SET CMP0042 NEW)
#  cmake_policy(SET CMP0043 NEW)
#endif()

#if(${CMAKE_VERSION} VERSION_GREATER "3.1.2")
#  cmake_policy(SET CMP0053 OLD)
#endif()

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

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DPROJECT_ROOT='\"${TWS_ABSOLUTE_ROOT_DIR}/src\"'")

# Turn on multi process compilation on Windows
if(MSVC)
  add_definitions(/MP)
  set(CMAKE_DEBUG_POSTFIX "d")
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

find_package(SciDB REQUIRED)

find_package(terralib REQUIRED)

find_package(RapidJSON REQUIRED)

find_package(RapidXml)


#
# add include targets
#
include_directories(${Boost_INCLUDE_DIR})

#
# build options
#
# warning: please, do not change the order below!
#
option(TWS_MOD_CORE_ENABLED "Build TWS CORE library?" ON)

CMAKE_DEPENDENT_OPTION(TWS_MOD_MONGOOSE_ENABLED "Build Mongoose Web Server support?" ON "TWS_MOD_CORE_ENABLED" OFF)

CMAKE_DEPENDENT_OPTION(TWS_MOD_GEOARRAY_ENABLED "Build Geo-Array metadata support?" ON "TWS_MOD_CORE_ENABLED" OFF)

CMAKE_DEPENDENT_OPTION(TWS_MOD_SCIDB_ENABLED "Build SciDB support for web services?" ON "TWS_MOD_CORE_ENABLED" OFF)

CMAKE_DEPENDENT_OPTION(TWS_MOD_WMS_ENABLED "Build Application Web Map Service?" ON "TWS_MOD_GEOARRAY_ENABLED;RAPIDXML_FOUND" OFF)

CMAKE_DEPENDENT_OPTION(TWS_MOD_WTSS_ENABLED "Build Application Web Time Series Data Service?" ON "TWS_MOD_GEOARRAY_ENABLED" OFF)

CMAKE_DEPENDENT_OPTION(TWS_MOD_WCS_ENABLED "Build Application Web Coverage Service?" ON "TWS_MOD_GEOARRAY_ENABLED;RAPIDXML_FOUND" OFF)

CMAKE_DEPENDENT_OPTION(TWS_APP_SERVER_ENABLED "Build Application Web Server?" ON "TWS_MOD_CORE_ENABLED" OFF)


#
# process TWS configuration files
#
configure_file(${TWS_ABSOLUTE_ROOT_DIR}/src/tws/build_config.hpp.in
               ${CMAKE_BINARY_DIR}/tws_build_config.hpp @ONLY)

configure_file(${TWS_ABSOLUTE_ROOT_DIR}/src/tws/config.hpp.in
               ${CMAKE_BINARY_DIR}/tws_config.hpp @ONLY)

configure_file(${TWS_ABSOLUTE_ROOT_DIR}/src/tws/version.hpp.in
               ${CMAKE_BINARY_DIR}/tws_version.hpp @ONLY)


#
# build modules
#
if(TWS_MOD_CORE_ENABLED)
  add_subdirectory(tws_mod_core)
endif()

if(TWS_MOD_GEOARRAY_ENABLED)
  add_subdirectory(tws_mod_geoarray)
endif()

if(TWS_MOD_SCIDB_ENABLED)
  add_subdirectory(tws_mod_scidb)
endif()

if(TWS_MOD_WMS_ENABLED)
  add_subdirectory(tws_mod_wms)
endif()

if(TWS_MOD_WCS_ENABLED)
  add_subdirectory(tws_mod_wcs)
endif()

if(TWS_MOD_WTSS_ENABLED)
  add_subdirectory(tws_mod_wtss)
endif()

if(TWS_MOD_MONGOOSE_ENABLED)
  add_subdirectory(tws_mod_mongoose)
endif()

if(TWS_APP_SERVER_ENABLED)
  add_subdirectory(tws_app_server)
endif()


#
# install and targets export
#
file(GLOB TWS_HDR_FILES ${TWS_ABSOLUTE_ROOT_DIR}/src/tws/*.hpp)

install(FILES ${CMAKE_BINARY_DIR}/tws_config.hpp
              ${CMAKE_BINARY_DIR}/tws_build_config.hpp
              ${CMAKE_BINARY_DIR}/tws_version.hpp
              ${TWS_HDR_FILES}
        DESTINATION ${TWS_DESTINATION_HEADERS}/tws COMPONENT devel)

install(EXPORT tws-targets
        DESTINATION ${TWS_DESTINATION_LIBRARY}/cmake/tws-${TWS_VERSION_STRING}
        FILE tws-exports.cmake
        COMPONENT devel)


#
# CPack
#
set(CPACK_SOURCE_INSTALLED_DIRECTORIES "${TWS_ABSOLUTE_ROOT_DIR};/")

include(CPack)

# defining installation types, components and groups
cpack_add_install_type(full DISPLAY_NAME "Complete")
cpack_add_install_type(minimalistic DISPLAY_NAME "Minimalistic")

cpack_add_component_group(general_group
                          DISPLAY_NAME "Applications"
                          DESCRIPTION "TWS web servers, web clients and GUI applications"
                          EXPANDED)

cpack_add_component_group(devel_group
                          DISPLAY_NAME "Development Tools"
                          DESCRIPTION "Development Tools: header files, libraries and cmake stuffs"
                          EXPANDED)

cpack_add_component(runtime
                    DISPLAY_NAME "Runtime Libraries and Executables"
                    DESCRIPTION "Install only necessary shared libraries (or DLLs) and executables"
                    GROUP general_group
                    INSTALL_TYPES full minimalistic)

cpack_add_component(devel
                    DISPLAY_NAME "Development package"
                    DESCRIPTION "Install header files, cmake stuffs and additional shared libraries"
                    GROUP devel_group
                    INSTALL_TYPES full minimalistic)


