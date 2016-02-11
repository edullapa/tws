#!/bin/bash
#
#  Copyright (C) 2014 National Institute For Space Research (INPE) - Brazil.
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
#  Description: Install all required software for TerraLib Web Services on Linux Ubuntu 12.04.
#
#  Author: Gilberto Ribeiro de Queiroz <gribeiro@dpi.inpe.br>
#
#
#  Example:
#  $ TWS_DIR="/Users/gribeiro/MyLibs" ./install-3rdparty-macosx-el-capitan.sh
#

echo "*******************************************************************************"
echo "* TerraLib Web Services 3rd-party Libraries Installer for Mac OS X El Capitan *"
echo "*******************************************************************************"
echo ""
sleep 1s

#
# Valid parameter val or abort script
#
function valid()
{
  if [ $1 -ne 0 ]; then
    echo $2
    echo ""
    exit
  fi
}


#
# Check for tws-3rdparty-macosx-el-capitan.tar.gz
#
if [ ! -f ./tws-3rdparty-macosx-el-capitan.tar.gz ]; then
  echo "Please, make sure to have tws-3rdparty-macosx-el-capitan.tar.gz in the current directory!"
  echo ""
  exit
fi


#
# Extract packages
#
echo "extracting packages..."
echo ""
sleep 1s

tar xzvf tws-3rdparty-macosx-el-capitan.tar.gz
valid $? "Error: could not extract 3rd party libraries (tws-3rdparty-macosx-el-capitan.tar.gz)"

echo "packages extracted!"
echo ""
sleep 1s


#
# Go to 3rd party libraries dir
#
cd tws-3rdparty-macosx-el-capitan
valid $? "Error: could not enter 3rd-party libraries dir (tws-3rdparty-macosx-el-capitan)"


#
# Check installation dir
#
if [ "$TWS_DIR" == "" ]; then
  TWS_DIR="/opt/tws"
fi

export PATH="$PATH:$TWS_DIR/bin"
export LD_LIBRARY_PATH="$PATH:$TWS_DIR/lib"

echo "installing 3rd-party libraries to '$TWS_DIR' ..."
echo ""
sleep 1s


#
# RapidJSON
#
if [ ! -f "$TWS_DIR/include/rapidjson/rapidjson.h" ]; then
  echo "installing RapidJSON..."
  echo ""
  sleep 1s

  if [ ! -d "$TWS_DIR" ]; then
    mkdir $TWS_DIR
  fi

  tar xzvf rapidjson-0.11.tar.gz
  valid $? "Error: could not uncompress rapidjson-0.11.tar.gz!"

  cd rapidjson
  valid $? "Error: could not enter rapidjson!"

  mv include/rapidjson $TWS_DIR/include/

  cd ..
fi


#
# RapidXml
#
if [ ! -f "$TWS_DIR/include/rapidxml/rapidxml.hpp" ]; then
  echo "installing RapidXml..."
  echo ""
  sleep 1s

  if [ ! -d "$TWS_DIR" ]; then
    mkdir $TWS_DIR
  fi

  if [ ! -d "$TWS_DIR/include/rapidxml" ]; then
    mkdir -p $TWS_DIR/include/rapidxml
  fi

  unzip rapidxml-1.13.zip
  valid $? "Error: could not uncompress rapidxml-1.13.zip!"

  cd rapidxml-1.13
  valid $? "Error: could not enter rapidxml!"

  mv *.hpp $TWS_DIR/include/rapidxml/

  cd ..
fi


#
# Finished!
#
clear
echo "*******************************************************************************"
echo "* TerraLib Web Services 3rd-party Libraries Installer for Mac OS X El Capitan *"
echo "*******************************************************************************"
echo ""
echo "finished successfully!"
