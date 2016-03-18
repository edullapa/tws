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

    capability_t read_capability(const rapidjson::Value& jcapability);

  }  // end namespace wms
}    // end namespace tws


#endif  // __TWS_WMS_JSON_SERIALIZER_HPP__
