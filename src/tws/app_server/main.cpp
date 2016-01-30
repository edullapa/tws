/*
  Copyright (C) 2014 National Institute For Space Research (INPE) - Brazil.
 
  This file is part of the TerraLib GeoWeb Services.
 
  TerraLib GeoWeb Services is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License version 3 as
  published by the Free Software Foundation.
 
  TerraLib GeoWeb Services is distributed  "AS-IS" in the hope that it will be useful,
  but WITHOUT ANY WARRANTY OF ANY KIND; without even the implied warranty
  of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
  GNU Lesser General Public License for more details.
 
  You should have received a copy of the GNU Lesser General Public License along
  with TerraLib Web Services. See COPYING. If not, see <http://www.gnu.org/licenses/lgpl-3.0.html>.
 */

/*!
  \file tws/app_server/main.cpp

  \brief An HTTP Web Server for TerraLig GeoWeb Services.

  \author Gilberto Ribeiro de Queiroz
 */

// TWS
#include "../core/http_server.hpp"
#include "../core/http_server_builder.hpp"
#include "../core/utils.hpp"

// TerraLib
#include <terralib/common/TerraLib.h>
#include <terralib/plugin/PluginManager.h>
#include <terralib/plugin/Utils.h>

// STL
#include <cstdlib>
#include <iostream>
#include <memory>

//#ifdef TWS_MOD_CPPNETLIB_ENABLED
//  #define TWS_DEFAULT_WEB_SERVER "cppnetlib"
//#elif defined(TWS_MOD_MONGOOSE_ENABLED)
//  #define TWS_DEFAULT_WEB_SERVER "mongoose"
//#else
//  #error "No Web Server Support Built: check if mongoose or cppnetlib modules were built!"
//#endif

void LoadModules()
{
  std::string plugins_path = tws::core::find_in_app_path("share/tws/plugins");
  
  te::plugin::PluginInfo* info = te::plugin::GetInstalledPlugin(plugins_path + "/tws.http_server.mongoose.teplg");
  te::plugin::PluginManager::getInstance().add(info);
  
  info = te::plugin::GetInstalledPlugin(plugins_path + "/tws.wms.teplg");
  te::plugin::PluginManager::getInstance().add(info);

  //info = te::plugin::GetInstalledPlugin(plugins_path + "/tws.wtss.teplg");
  //te::plugin::PluginManager::getInstance().add(info);
  
  te::plugin::PluginManager::getInstance().loadAll();
}

void UnloadModules()
{
  te::plugin::PluginManager::getInstance().unloadAll();
}

int main(int argc, char *argv[])
{
  std::cout << "\nStarting TerraLib GeoWeb Services...\n" << std::endl;
  
  TerraLib::getInstance().initialize();
  
  

  try
  {
    tws::core::init_terralib_web_services();
    
    LoadModules();

    //std::unique_ptr<tws::core::http_server> server = tws::core::http_server_builder::instance().build(TWS_DEFAULT_WEB_SERVER);
    std::unique_ptr<tws::core::http_server> server = tws::core::http_server_builder::instance().build("mongoose");

    server->start();

    UnloadModules();
  }
  catch(const boost::exception& e)
  {
    if(const std::string* d = boost::get_error_info<tws::error_description>(e))
      std::cout << "\n\nthe following error has occurried: " << *d << std::endl;
    else
      std::cout << "\n\nan unknown error has occurried" << std::endl;

    return EXIT_FAILURE;
  }
  catch(const std::exception& e)
  {
    std::cout << "\n\nthe following error has occurried: " << e.what() << std::endl;

    return EXIT_FAILURE;
  }
  catch(...)
  {
    std::cout << "\n\nan unknown error has occurried." << std::endl;

    return EXIT_FAILURE;
  }
  
  
  TerraLib::getInstance().finalize();

  std::cout << "\nFinished TerraLib GeoWeb Services!\n" << std::endl;

  return EXIT_SUCCESS;
}
