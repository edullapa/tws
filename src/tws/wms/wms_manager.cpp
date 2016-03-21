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
  \file tws/wms/wms_manager.cpp

  \brief A singleton for controlling the structs containing wms configuration.

  \author Roger Victor
 */

// TWS
#include "wms_manager.hpp"
#include "../core/utils.hpp"
#include "data_types.hpp"
#include "json_serializer.hpp"

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

  std::unique_ptr<rapidjson::Document> json_file;
  tws::wms::capabilities_t capabilities;
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
  const rapidjson::Value& jcapabilities = (*pimpl_->json_file)["wms_capabilities"];

  pimpl_->capabilities = read_capabilities(jcapabilities);

  return pimpl_->capabilities;
}
