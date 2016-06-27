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
  \file tws/wms/xml_serializer.hpp

  \brief Utility functions for serializing WMS Capabality object to a XML.

  \author Roger Victor
 */

#ifndef __TWS_WMS_XML_SERIALIZER_HPP__
#define __TWS_WMS_XML_SERIALIZER_HPP__

// TWS
#include "data_types.hpp"

// RapidXml
#include <rapidxml/rapidxml.hpp>

namespace tws
{
  namespace wms
  {

    void write(const capabilities_t& capabilities, rapidxml::xml_document<>& xml_capabilities);

    rapidxml::xml_node<>* write_service(const service_t service, rapidxml::xml_document<>& xml_capabilities);

    rapidxml::xml_node<>* write_contact_information(const contact_information_t contact_information, rapidxml::xml_document<>& xml_capabilities);

    rapidxml::xml_node<>* write_capability(const capability_t capability, rapidxml::xml_document<>& xml_capabilities);

    rapidxml::xml_node<>* write_request(const request_t request, rapidxml::xml_document<>& xml_capabilities);

    rapidxml::xml_node<>* write_operation(const operation_t operation, rapidxml::xml_node<>* operation_node, rapidxml::xml_document<>& xml_capabilities);

    rapidxml::xml_node<>* write_layer(const layer_t layer, rapidxml::xml_document<>& xml_capabilities);

    rapidxml::xml_node<>* write_keyword_list(const std::vector<keyword_t> keyword_list, rapidxml::xml_document<>& xml_capabilities);

    rapidxml::xml_node<>* write_online_resource(const online_resource_t online_resource, rapidxml::xml_document<>& xml_capabilities);

    rapidxml::xml_node<>* write_bounding_box(const bounding_box_t bounding_box, rapidxml::xml_document<>& xml_capabilities);

  }  // end namespace wms
}    // end namespace tws

#endif  // __TWS_WMS_XML_SERIALIZER_HPP__
