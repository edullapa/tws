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
  \file tws/wms/xml_serializer.cpp

  \brief Utility functions for serializing WMS Capabality object to a XML.

  \author Roger Victor
 */

// TWS
#include "xml_serializer.hpp"

void
tws::wms::write(const capabilities_t& capabilities, rapidxml::xml_document<>& xml_capabilities)
{
// xml declaration
  rapidxml::xml_node<>* decl = xml_capabilities.allocate_node(rapidxml::node_declaration);
  decl->append_attribute(xml_capabilities.allocate_attribute("version", "1.0"));
  decl->append_attribute(xml_capabilities.allocate_attribute("encoding", "utf-8"));
  xml_capabilities.append_node(decl);

// root node
  rapidxml::xml_node<>* root = xml_capabilities.allocate_node(rapidxml::node_element, "WMS_Capabilities");

  rapidxml::xml_attribute<>* attr = xml_capabilities.allocate_attribute("version", "1.3.0");
  root->append_attribute(attr);

  attr = xml_capabilities.allocate_attribute("xmlns", "http://www.opengis.net/wms");
  root->append_attribute(attr);

  attr = xml_capabilities.allocate_attribute("xmlns:xlink", "http://www.w3.org/1999/xlink");
  root->append_attribute(attr);

  attr = xml_capabilities.allocate_attribute("xmlns:xsi", "http://www.w3.org/2001/XMLSchema-instance");
  root->append_attribute(attr);

  attr = xml_capabilities.allocate_attribute("xsi:schemaLocation", "http://www.opengis.net/wms http://schemas.opengis.net/wms/1.3.0/capabilities_1_3_0.xsd");
  root->append_attribute(attr);

  root->append_node(write_service(capabilities.service, xml_capabilities));

  root->append_node(write_capability(capabilities.capability, xml_capabilities));

  xml_capabilities.append_node(root);
}

rapidxml::xml_node<>*
tws::wms::write_service(const service_t service, rapidxml::xml_document<>& xml_capabilities)
{
  rapidxml::xml_node<>* service_node = xml_capabilities.allocate_node(rapidxml::node_element, "Service");

  rapidxml::xml_node<>* node = xml_capabilities.allocate_node(rapidxml::node_element, "Name", service.name.c_str());

  service_node->append_node(node);

  node = xml_capabilities.allocate_node(rapidxml::node_element, "Title", service.title.c_str());

  service_node->append_node(node);

  node = xml_capabilities.allocate_node(rapidxml::node_element, "Abstract", service.abstract.c_str());

  service_node->append_node(node);

  service_node->append_node(write_keyword_list(service.keyword_list, xml_capabilities));

  service_node->append_node(write_online_resource(service.online_resource, xml_capabilities));

  service_node->append_node(write_contact_information(service.contact_information, xml_capabilities));

  node = xml_capabilities.allocate_node(rapidxml::node_element, "Fees", service.fees.c_str());

  service_node->append_node(node);

  node = xml_capabilities.allocate_node(rapidxml::node_element, "AccessConstraints", service.access_constraints.c_str());

  service_node->append_node(node);

  node = xml_capabilities.allocate_node(rapidxml::node_element, "LayerLimit", std::to_string(service.layer_limit).c_str());

  service_node->append_node(node);

  node = xml_capabilities.allocate_node(rapidxml::node_element, "MaxWidth", std::to_string(service.max_width).c_str());

  service_node->append_node(node);

  node = xml_capabilities.allocate_node(rapidxml::node_element, "MaxHeight", std::to_string(service.max_height).c_str());

  service_node->append_node(node);

  return service_node;
}

rapidxml::xml_node<>*
tws::wms::write_contact_information(const contact_information_t contact_information, rapidxml::xml_document<>& xml_capabilities)
{
  rapidxml::xml_node<>* contact_information_node = xml_capabilities.allocate_node(rapidxml::node_element, "ContactInformation");

  rapidxml::xml_node<>* contact_person_node = xml_capabilities.allocate_node(rapidxml::node_element, "ContactPersonPrimary");

  rapidxml::xml_node<>* node = xml_capabilities.allocate_node(rapidxml::node_element, "ContactPerson", contact_information.contact_person_primary.contact_person.c_str());

  contact_person_node->append_node(node);

  node = xml_capabilities.allocate_node(rapidxml::node_element, "ContactOrganization", contact_information.contact_person_primary.contact_organization.c_str());

  contact_person_node->append_node(node);

  contact_information_node->append_node(contact_person_node);

  node = xml_capabilities.allocate_node(rapidxml::node_element, "ContactPosition", contact_information.contact_position.c_str());

  contact_information_node->append_node(node);

  rapidxml::xml_node<>* contact_address_node = xml_capabilities.allocate_node(rapidxml::node_element, "ContactAddress");

  node = xml_capabilities.allocate_node(rapidxml::node_element, "AddressType", contact_information.contact_address.address_type.c_str());

  contact_address_node->append_node(node);

  node = xml_capabilities.allocate_node(rapidxml::node_element, "Address", contact_information.contact_address.address.c_str());

  contact_address_node->append_node(node);

  node = xml_capabilities.allocate_node(rapidxml::node_element, "City", contact_information.contact_address.city.c_str());

  contact_address_node->append_node(node);

  node = xml_capabilities.allocate_node(rapidxml::node_element, "StateOrProvince", contact_information.contact_address.state_or_province.c_str());

  contact_address_node->append_node(node);

  node = xml_capabilities.allocate_node(rapidxml::node_element, "PostCode", contact_information.contact_address.post_code.c_str());

  contact_address_node->append_node(node);

  node = xml_capabilities.allocate_node(rapidxml::node_element, "Country", contact_information.contact_address.country.c_str());

  contact_address_node->append_node(node);

  contact_information_node->append_node(contact_address_node);

  node = xml_capabilities.allocate_node(rapidxml::node_element, "ContactVoiceTelephone", contact_information.contact_voice_telephone.c_str());

  contact_information_node->append_node(node);

  node = xml_capabilities.allocate_node(rapidxml::node_element, "ContactElectronicMailAddress", contact_information.contact_electronic_mail_address.c_str());

  contact_information_node->append_node(node);

  return contact_information_node;
}

rapidxml::xml_node<>*
tws::wms::write_capability(const capability_t capability, rapidxml::xml_document<>& xml_capabilities)
{
  rapidxml::xml_node<>* capability_node = xml_capabilities.allocate_node(rapidxml::node_element, "Capability");

  capability_node->append_node(write_request(capability.request, xml_capabilities));

  rapidxml::xml_node<>* exception_node = xml_capabilities.allocate_node(rapidxml::node_element, "Exception");

  for(auto format: capability.exception_formats)
    exception_node->append_node(xml_capabilities.allocate_node(rapidxml::node_element, "Format", format.c_str()));

  capability_node->append_node(exception_node);

  capability_node->append_node(write_layer(capability.layer, xml_capabilities));

  return capability_node;
}

rapidxml::xml_node<>*
tws::wms::write_request(const request_t request, rapidxml::xml_document<>& xml_capabilities)
{
  rapidxml::xml_node<>* request_node = xml_capabilities.allocate_node(rapidxml::node_element, "Request");

  rapidxml::xml_node<>* get_capabilities_node = xml_capabilities.allocate_node(rapidxml::node_element, "GetCapabilities");

  request_node->append_node(write_operation(request.get_capabilities, get_capabilities_node, xml_capabilities));

  rapidxml::xml_node<>* get_map_node = xml_capabilities.allocate_node(rapidxml::node_element, "GetMap");

  request_node->append_node(write_operation(request.get_map, get_map_node, xml_capabilities));

  rapidxml::xml_node<>* get_feature_info_node = xml_capabilities.allocate_node(rapidxml::node_element, "GetFeatureInfo");

  request_node->append_node(write_operation(request.get_feature_info, get_feature_info_node, xml_capabilities));

  return request_node;
}

rapidxml::xml_node<>*
tws::wms::write_operation(const operation_t operation, rapidxml::xml_node<>* operation_node, rapidxml::xml_document<>& xml_capabilities)
{
  for(auto format: operation.format)
    operation_node->append_node(xml_capabilities.allocate_node(rapidxml::node_element, "Format", format.c_str()));

  rapidxml::xml_node<>* dcp_type_node = xml_capabilities.allocate_node(rapidxml::node_element, "DCPType");

  rapidxml::xml_node<>* http_node = xml_capabilities.allocate_node(rapidxml::node_element, "HTTP");

  rapidxml::xml_node<>* get_node = xml_capabilities.allocate_node(rapidxml::node_element, "Get");

  get_node->append_node(write_online_resource(operation.dcp_type.http.get, xml_capabilities));

  http_node->append_node(get_node);

  if (!operation.dcp_type.http.post.xlink_href.empty())
  {
    rapidxml::xml_node<>* post_node = xml_capabilities.allocate_node(rapidxml::node_element, "Post");

    post_node->append_node(write_online_resource(operation.dcp_type.http.post, xml_capabilities));

    http_node->append_node(post_node);
  }

  dcp_type_node->append_node(http_node);

  operation_node->append_node(dcp_type_node);

  return operation_node;
}

rapidxml::xml_node<>*
tws::wms::write_layer(const layer_t layer, rapidxml::xml_document<>& xml_capabilities)
{
  rapidxml::xml_node<>* layer_node = xml_capabilities.allocate_node(rapidxml::node_element, "Layer");

  rapidxml::xml_node<>* node = xml_capabilities.allocate_node(rapidxml::node_element, "Name", layer.name.c_str());

  layer_node->append_node(node);

  for(auto crs: layer.crs)
    layer_node->append_node(xml_capabilities.allocate_node(rapidxml::node_element, "CRS", crs.c_str()));

  for(auto bounding_box: layer.bounding_box)
    layer_node->append_node(write_bounding_box(bounding_box, xml_capabilities));

  for(auto sublayer: layer.layers)
    layer_node->append_node(write_layer(sublayer, xml_capabilities));

  return layer_node;
}

rapidxml::xml_node<>*
tws::wms::write_keyword_list(const std::vector<keyword_t> keyword_list, rapidxml::xml_document<>& xml_capabilities)
{
  rapidxml::xml_node<>* keyword_list_node = xml_capabilities.allocate_node(rapidxml::node_element, "KeywordList");

  for(auto keyword: keyword_list)
    keyword_list_node->append_node(xml_capabilities.allocate_node(rapidxml::node_element, "Keyword", keyword.vocabulary.c_str()));

  return keyword_list_node;
}

rapidxml::xml_node<>*
tws::wms::write_online_resource(const online_resource_t online_resource, rapidxml::xml_document<>& xml_capabilities)
{
  rapidxml::xml_node<>* online_resource_node = xml_capabilities.allocate_node(rapidxml::node_element, "OnlineResource");

  rapidxml::xml_attribute<>* attr = xml_capabilities.allocate_attribute("xmlns:xlink", "http://www.w3.org/1999/xlink");
  online_resource_node->append_attribute(attr);

  attr = xml_capabilities.allocate_attribute("xlink:type", online_resource.xlink_type.c_str());
  online_resource_node->append_attribute(attr);

  attr = xml_capabilities.allocate_attribute("xlink:href", online_resource.xlink_href.c_str());
  online_resource_node->append_attribute(attr);

  return online_resource_node;
}

rapidxml::xml_node<>*
tws::wms::write_bounding_box(const bounding_box_t bounding_box, rapidxml::xml_document<>& xml_capabilities)
{
  rapidxml::xml_node<>* bounding_box_node = xml_capabilities.allocate_node(rapidxml::node_element, "BoundingBox");

  rapidxml::xml_attribute<>* attr = xml_capabilities.allocate_attribute("CRS", bounding_box.crs.c_str());
  bounding_box_node->append_attribute(attr);

// TODO: fix the conversion of double to string

//  attr = xml_capabilities.allocate_attribute("minx", std::to_string(bounding_box.min_x).c_str());
//  bounding_box_node->append_attribute(attr);

//  attr = xml_capabilities.allocate_attribute("miny", std::to_string(bounding_box.min_y).c_str());
//  bounding_box_node->append_attribute(attr);

//  attr = xml_capabilities.allocate_attribute("maxx", std::to_string(bounding_box.max_x).c_str());
//  bounding_box_node->append_attribute(attr);

//  attr = xml_capabilities.allocate_attribute("maxy", std::to_string(bounding_box.max_y).c_str());
//  bounding_box_node->append_attribute(attr);

//  attr = xml_capabilities.allocate_attribute("resx", std::to_string(bounding_box.res_x).c_str());
//  bounding_box_node->append_attribute(attr);

//  attr = xml_capabilities.allocate_attribute("resy", std::to_string(bounding_box.res_y).c_str());
//  bounding_box_node->append_attribute(attr);

  return bounding_box_node;
}
