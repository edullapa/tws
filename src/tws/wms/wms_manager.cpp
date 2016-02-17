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
  \file tws/wms/layer_manager.cpp

  \brief A singleton for controlling the list of map layers to be served.

  \author Roger Victor
 */

// TWS
#include "wms_manager.hpp"
#include "../core/utils.hpp"
#include "data_types.hpp"

// Rapidjson
#include <rapidjson/document.h>

// STL
#include <memory>


struct tws::wms::wms_manager::impl
{
  impl()
    : json_file(nullptr)
  {

  }

  ~impl()
  {
  }

  std::shared_ptr<rapidjson::Document> json_file;
};

tws::wms::wms_manager::wms_manager()
  : pimpl_(nullptr)
{
  pimpl_ = new impl;

  std::string wms_file = tws::core::find_in_app_path("share/tws/config/wms.json");
  pimpl_->json_file.reset(tws::core::open_json_file(wms_file));
}

tws::wms::wms_manager::~wms_manager()
{
  delete pimpl_;
}

tws::wms::wms_manager& tws::wms::wms_manager::instance()
{
  static wms_manager instance;

  return instance;
}

tws::wms::capabilities_t tws::wms::wms_manager::capabilities()
{
  capabilities_t capabilities;
  capabilities.service = service();
  
  return capabilities;
}

tws::wms::service_t tws::wms::wms_manager::service()
{
  try
  {
    service_t service;
    const rapidjson::Value& service_object = (*pimpl_->json_file)["service"];
    if (!service_object.IsObject())
    {
      throw tws::parser_error() << tws::error_description("Could not find service in wms config file");
    }

    const rapidjson::Value& name = service_object["name"];
    if (!name.IsString())
    {
      throw tws::parser_error() << tws::error_description("Could not find service name in wms config file");
    }
    service.name = name.GetString();

    const rapidjson::Value& title = service_object["title"];
    if (!title.IsString())
      service.title = "";
    else
      service.title = title.GetString();

    const rapidjson::Value& abstract = service_object["abstract"];
    if (!abstract.IsString())
    {
      throw tws::parser_error() << tws::error_description("Could not find service abstract in wms config file");
    }
    service.abstract = abstract.GetString();
    
    const rapidjson::Value& contact_information_object = service_object["contact_information"];
    if (!contact_information_object.IsObject())
    {
      throw tws::parser_error() << tws::error_description("Could not find contact information in wms config file");
    }

    contact_information_t contact_information;
    const rapidjson::Value& contact_position = contact_information_object["contact_position"];
    if (!contact_position.IsString())
    {
      throw tws::parser_error() << tws::error_description("Could not find contact position in wms config file");
    }

    contact_information.contact_position = contact_position.GetString();

    service.contact_information = contact_information;

    return service;
  }
  catch(...)
  {
    throw;
  }
}
