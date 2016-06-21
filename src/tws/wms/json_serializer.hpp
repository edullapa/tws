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
  \file tws/wms/json_serializer.hpp

  \brief Utility functions for reading the WMS Capabality information from a JSON document.

  \author Roger Victor
 */

#ifndef __TWS_WMS_JSON_SERIALIZER_HPP__
#define __TWS_WMS_JSON_SERIALIZER_HPP__

// TWS
#include "config.hpp"
#include "data_types.hpp"

// RapidJSON
#include <rapidjson/document.h>

namespace tws
{
  namespace wms
  {

    capabilities_t read_capabilities(const rapidjson::Value& jcapabilities);

    service_t read_service(const rapidjson::Value& jservice);

    contact_information_t read_contact_information(const rapidjson::Value& jcontact_information);

    contact_person_primary_t read_contact_person_primary(const rapidjson::Value& jcontact_person_primary);

    contact_address_t read_contact_address(const rapidjson::Value& jcontact_address);

    capability_t read_capability(const rapidjson::Value& jformats);

    request_t read_request(const rapidjson::Value& jrequest);

    operation_t read_operation(const rapidjson::Value& joperation);

    dcp_type_t read_dcp_type(const rapidjson::Value& jdcp_type);

    http_t read_http(const rapidjson::Value& jhttp);

    std::vector<keyword_t> read_keyword_list(const rapidjson::Value& jkeyword_list);

    std::vector<std::string> read_formats(const rapidjson::Value& jformats);

    online_resource_t read_online_resource(const rapidjson::Value& jonline_resource);

    std::vector<std::string> read_crs(const rapidjson::Value& jcrs_list);

    std::vector<bounding_box_t> read_bounding_box(const rapidjson::Value& jbounding_box_list);

    layer_t read_layer(const rapidjson::Value& jlayer);

    std::vector<style_t> read_styles(const rapidjson::Value& jstyles);

    style_t read_style(const rapidjson::Value& jstyle);

  }  // end namespace wms
}    // end namespace tws


#endif  // __TWS_WMS_JSON_SERIALIZER_HPP__
