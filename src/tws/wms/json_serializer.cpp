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
  \file tws/wms/json_serializer.cpp

  \brief Utility functions for reading the WMS Capabality information from a JSON document.

  \author Roger Victor
 */

// TWS
#include "json_serializer.hpp"
#include "exception.hpp"


tws::wms::capabilities_t
tws::wms::read_capabilities(const rapidjson::Value& jcapabilities)
{
  if(!jcapabilities.IsObject())
      throw tws::parser_error() << tws::error_description("error parsing wms_capabilities.");

  const rapidjson::Value& jservice = jcapabilities["service"];
  const rapidjson::Value& jcapability = jcapabilities["capability"];

  capabilities_t capabilities;

  capabilities.service = read_service(jservice);
  capabilities.capability = read_capability(jcapability);
  capabilities.version = "1.3.0";

  return capabilities;
}

tws::wms::service_t
tws::wms::read_service(const rapidjson::Value& jservice)
{
  service_t service;
  return service;
}

tws::wms::capability_t
tws::wms::read_capability(const rapidjson::Value& jcapability)
{
  capability_t capability;
  return capability;
}

