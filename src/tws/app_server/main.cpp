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
#include "../core/logger.hpp"
#include "../core/utils.hpp"

// TerraLib
#include <terralib/common/TerraLib.h>
#include <terralib/core/logger/Logger.h>
#include <terralib/core/translator/Translator.h>
#include <terralib/plugin/PluginInfo.h>
#include <terralib/plugin/PluginManager.h>
#include <terralib/plugin/Utils.h>

// STL
#include <cstdlib>
#include <iostream>
#include <memory>

// Boost
#include <boost/format.hpp>

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
  
  std::unique_ptr<te::plugin::PluginInfo> info(te::plugin::GetInstalledPlugin(plugins_path + "/tws.http_server.mongoose.teplg"));
  te::plugin::PluginManager::getInstance().load(*info);
  
  info.reset(te::plugin::GetInstalledPlugin(plugins_path + "/tws.wms.teplg"));
  te::plugin::PluginManager::getInstance().load(*info);

  //info = te::plugin::GetInstalledPlugin(plugins_path + "/tws.wcs.teplg");
  //te::plugin::PluginManager::getInstance().load(info);

  info.reset(te::plugin::GetInstalledPlugin(plugins_path + "/tws.wtss.teplg"));
  te::plugin::PluginManager::getInstance().load(*info);

  //te::plugin::PluginManager::getInstance().loadAll();
}

void UnloadModules()
{
  te::plugin::PluginManager::getInstance().unloadAll();
}

int main(int argc, char *argv[])
{
// look for tws_app_server config file
  std::string config_file_name = tws::core::find_in_app_path("share/tws/config/tws_app_server.json");

  if(config_file_name.empty())
  {
    std::cerr << TE_TR("Could not find tws_app_server configuration file: 'tws_app_server.json'.");

    return EXIT_FAILURE;
  }

  try
  {
// read config file
    std::unique_ptr<rapidjson::Document> config_doc(tws::core::open_json_file(config_file_name));

    if(!config_doc->IsObject() || config_doc->IsNull())
      throw tws::parse_error() << tws::error_description(TE_TR("error in array entry name in metadata."));

// get log file information
    const rapidjson::Value& jlog_file = (*config_doc)["log_file"];

    std::string log_file = jlog_file.GetString();

// init logger
    TE_ADD_LOGGER(TWS_LOGGER, log_file, TWS_LOGGER_FORMAT);

// init TerraLib and TWS frameworks
    TWS_LOG_INFO(TE_TR("Starting TerraLib GeoWeb Services..."));

    TerraLib::getInstance().initialize();

    tws::core::init_terralib_web_services();
    
    LoadModules();

// start default htp server
    const rapidjson::Value& jhttp_server = (*config_doc)["http_server"];

    std::string http_server = jhttp_server.GetString();

    std::unique_ptr<tws::core::http_server> server = tws::core::http_server_builder::instance().build(http_server);

    server->start();

    UnloadModules();

    TerraLib::getInstance().finalize();

    TWS_LOG_INFO(TE_TR("Finished TerraLib GeoWeb Services!"));
  }
  catch(const boost::exception& e)
  {
    if(const std::string* d = boost::get_error_info<tws::error_description>(e))
    {
      boost::format err_msg(TE_TR("the following error has occurred: %1%."));

      TWS_LOG_ERROR((err_msg % *d).str());
    }
    else
    {
      TWS_LOG_ERROR(TE_TR("an unknown error has occurred"));
    }

    return EXIT_FAILURE;
  }
  catch(const std::exception& e)
  {
    boost::format err_msg(TE_TR("the following error has occurred: %1%."));

    TWS_LOG_ERROR((err_msg % e.what()).str());

    return EXIT_FAILURE;
  }
  catch(...)
  {
    TWS_LOG_ERROR(TE_TR("an unknown error has occurred."));

    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
