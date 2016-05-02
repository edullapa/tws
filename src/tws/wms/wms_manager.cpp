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
#include "xml_serializer.hpp"

// Rapidjson
#include <rapidjson/document.h>

// STL
#include <memory>

struct tws::wms::wms_manager::impl
{
  tws::wms::capabilities_t capabilities;
  rapidxml::xml_document<> xml_doc;
};

tws::wms::wms_manager&
tws::wms::wms_manager::instance()
{
  static wms_manager instance;

  return instance;
}

const tws::wms::capabilities_t&
tws::wms::wms_manager::capabilities() const
{
  return pimpl_->capabilities;
}

const rapidxml::xml_document<>&
tws::wms::wms_manager::xml_capabilities() const
{
  return pimpl_->xml_doc;
}

tws::wms::wms_manager::wms_manager()
  : pimpl_(nullptr)
{
  pimpl_ = new impl;

  std::string wms_file = tws::core::find_in_app_path("share/tws/config/wms.json");

  if(wms_file.empty())
    throw tws::file_exists_error() << tws::error_description("Could not locate file 'share/tws/config/wms.json'.");

  std::unique_ptr<rapidjson::Document> jdocument(tws::core::open_json_file(wms_file));

  if(!jdocument->HasMember("wms_capabilities"))
    throw tws::parse_error() << tws::error_description("Could not locate wms_capabilities key in file 'share/tws/config/wms.json'.");

  const rapidjson::Value& jcapabilities = (*jdocument)["wms_capabilities"];

  pimpl_->capabilities = read_capabilities(jcapabilities);

  write(pimpl_->capabilities, pimpl_->xml_doc);
}

tws::wms::wms_manager::~wms_manager()
{
  delete pimpl_;
}
