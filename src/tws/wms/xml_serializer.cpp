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

  {
    // Service node (service metadata)
    rapidxml::xml_node<>* service_node = xml_capabilities.allocate_node(rapidxml::node_element, "Service");

    {
      // Temp code. This metadata should be get from json file
      rapidxml::xml_node<>* node = xml_capabilities.allocate_node(rapidxml::node_element, "Name", capabilities.service.name.c_str());

      service_node->append_node(node);

      node = xml_capabilities.allocate_node(rapidxml::node_element, "Title", capabilities.service.title.c_str());

      service_node->append_node(node);

      node = xml_capabilities.allocate_node(rapidxml::node_element, "Abstract", capabilities.service.abstract.c_str());

      service_node->append_node(node);

      // KeywordList node
      rapidxml::xml_node<>* keyword_list_node = xml_capabilities.allocate_node(rapidxml::node_element, "KeywordList");

      for(auto keyword: capabilities.service.keyword_list)
      {
        keyword_list_node->append_node(xml_capabilities.allocate_node(rapidxml::node_element, "Keyword", keyword.vocabulary.c_str()));
      }

      service_node->append_node(keyword_list_node);

      node = xml_capabilities.allocate_node(rapidxml::node_element, "OnlineResource");

      rapidxml::xml_attribute<>* attr = xml_capabilities.allocate_attribute("xmlns:xlink", "http://www.w3.org/1999/xlink");
      node->append_attribute(attr);

      attr = xml_capabilities.allocate_attribute("xlink:type", capabilities.service.online_resource.xlink_type.c_str());
      node->append_attribute(attr);

      attr = xml_capabilities.allocate_attribute("xlink:href", capabilities.service.online_resource.xlink_href.c_str());
      node->append_attribute(attr);

      service_node->append_node(node);

      // Contact Information node
      rapidxml::xml_node<>* contact_information_node = xml_capabilities.allocate_node(rapidxml::node_element, "ContactInformation");

      {
        // Temp code. This metadata should be get from json file
        rapidxml::xml_node<>* contact_person_node = xml_capabilities.allocate_node(rapidxml::node_element, "ContactPersonPrimary");

        rapidxml::xml_node<>* node = xml_capabilities.allocate_node(rapidxml::node_element, "ContactPerson", capabilities.service.contact_information.contact_person_primary.contact_person.c_str());

        contact_person_node->append_node(node);

        node = xml_capabilities.allocate_node(rapidxml::node_element, "ContactOrganization", capabilities.service.contact_information.contact_person_primary.contact_organization.c_str());

        contact_person_node->append_node(node);

        contact_information_node->append_node(contact_person_node);

        node = xml_capabilities.allocate_node(rapidxml::node_element, "ContactPosition", capabilities.service.contact_information.contact_position.c_str());

        contact_information_node->append_node(node);

        rapidxml::xml_node<>* contact_address_node = xml_capabilities.allocate_node(rapidxml::node_element, "ContactAddress");

        node = xml_capabilities.allocate_node(rapidxml::node_element, "AddressType", capabilities.service.contact_information.contact_address.address_type.c_str());

        contact_address_node->append_node(node);

        node = xml_capabilities.allocate_node(rapidxml::node_element, "Address", capabilities.service.contact_information.contact_address.address.c_str());

        contact_address_node->append_node(node);

        node = xml_capabilities.allocate_node(rapidxml::node_element, "City", capabilities.service.contact_information.contact_address.city.c_str());

        contact_address_node->append_node(node);

        node = xml_capabilities.allocate_node(rapidxml::node_element, "StateOrProvince", capabilities.service.contact_information.contact_address.state_or_province.c_str());

        contact_address_node->append_node(node);

        node = xml_capabilities.allocate_node(rapidxml::node_element, "PostCode", capabilities.service.contact_information.contact_address.post_code.c_str());

        contact_address_node->append_node(node);

        node = xml_capabilities.allocate_node(rapidxml::node_element, "Country", capabilities.service.contact_information.contact_address.country.c_str());

        contact_address_node->append_node(node);

        contact_information_node->append_node(contact_address_node);

        node = xml_capabilities.allocate_node(rapidxml::node_element, "ContactVoiceTelephone", capabilities.service.contact_information.contact_voice_telephone.c_str());

        contact_information_node->append_node(node);

        node = xml_capabilities.allocate_node(rapidxml::node_element, "ContactElectronicMailAddress", capabilities.service.contact_information.contact_electronic_mail_address.c_str());

        contact_information_node->append_node(node);
      }

      service_node->append_node(contact_information_node);

      node = xml_capabilities.allocate_node(rapidxml::node_element, "Fees", capabilities.service.fees.c_str());

      service_node->append_node(node);

      node = xml_capabilities.allocate_node(rapidxml::node_element, "AccessConstraints", capabilities.service.access_constraints.c_str());

      service_node->append_node(node);

      node = xml_capabilities.allocate_node(rapidxml::node_element, "LayerLimit", std::to_string(capabilities.service.layer_limit).c_str());

      service_node->append_node(node);

      node = xml_capabilities.allocate_node(rapidxml::node_element, "MaxWidth", std::to_string(capabilities.service.max_width).c_str());

      service_node->append_node(node);

      node = xml_capabilities.allocate_node(rapidxml::node_element, "MaxHeight", std::to_string(capabilities.service.max_height).c_str());

      service_node->append_node(node);
    }

    // Capability node
    rapidxml::xml_node<>* capability_node = xml_capabilities.allocate_node(rapidxml::node_element, "Capability");

    {
      // Request node
      rapidxml::xml_node<>* request_node = xml_capabilities.allocate_node(rapidxml::node_element, "Request");

      {
        // GetCapabilities node
        rapidxml::xml_node<>* get_capabilities_node = xml_capabilities.allocate_node(rapidxml::node_element, "GetCapabilities");

        {
          // Temp code. This metadata should be get from json file
          for(auto format: capabilities.capability.request.get_capabilities.format)
          {
            get_capabilities_node->append_node(xml_capabilities.allocate_node(rapidxml::node_element, "Format", format.c_str()));
          }

          rapidxml::xml_node<>* dcp_type_node = xml_capabilities.allocate_node(rapidxml::node_element, "DCPType");

          rapidxml::xml_node<>* http_node = xml_capabilities.allocate_node(rapidxml::node_element, "HTTP");

          rapidxml::xml_node<>* get_node = xml_capabilities.allocate_node(rapidxml::node_element, "Get");
          {
            rapidxml::xml_node<>* node = xml_capabilities.allocate_node(rapidxml::node_element, "OnlineResource");

            rapidxml::xml_attribute<>* attr = xml_capabilities.allocate_attribute("xlink:type", capabilities.capability.request.get_capabilities.dcp_type.http.get.xlink_type.c_str());
            node->append_attribute(attr);

            attr = xml_capabilities.allocate_attribute("xlink:href", capabilities.capability.request.get_capabilities.dcp_type.http.get.xlink_href.c_str());
            node->append_attribute(attr);

            get_node->append_node(node);
          }

          http_node->append_node(get_node);

          rapidxml::xml_node<>* post_node = xml_capabilities.allocate_node(rapidxml::node_element, "Post");
          {
            rapidxml::xml_node<>* node = xml_capabilities.allocate_node(rapidxml::node_element, "OnlineResource");

            rapidxml::xml_attribute<>* attr = xml_capabilities.allocate_attribute("xlink:type", capabilities.capability.request.get_capabilities.dcp_type.http.post.xlink_type.c_str());
            node->append_attribute(attr);

            attr = xml_capabilities.allocate_attribute("xlink:href", capabilities.capability.request.get_capabilities.dcp_type.http.post.xlink_href.c_str());
            node->append_attribute(attr);

            post_node->append_node(node);
          }

          http_node->append_node(post_node);

          dcp_type_node->append_node(http_node);

          get_capabilities_node->append_node(dcp_type_node);
        }

        request_node->append_node(get_capabilities_node);

        // GetMap node
        rapidxml::xml_node<>* get_map_node = xml_capabilities.allocate_node(rapidxml::node_element, "GetMap");

        {
          // Temp code. This metadata should be get from json file
          for(auto format: capabilities.capability.request.get_map.format)
          {
            get_map_node->append_node(xml_capabilities.allocate_node(rapidxml::node_element, "Format", format.c_str()));
          }

          rapidxml::xml_node<>* dcp_type_node = xml_capabilities.allocate_node(rapidxml::node_element, "DCPType");

          rapidxml::xml_node<>* http_node = xml_capabilities.allocate_node(rapidxml::node_element, "HTTP");

          rapidxml::xml_node<>* get_node = xml_capabilities.allocate_node(rapidxml::node_element, "Get");

          rapidxml::xml_node<>* node = xml_capabilities.allocate_node(rapidxml::node_element, "OnlineResource");

          rapidxml::xml_attribute<>* attr = xml_capabilities.allocate_attribute("xlink:type", capabilities.capability.request.get_map.dcp_type.http.get.xlink_type.c_str());
          node->append_attribute(attr);

          attr = xml_capabilities.allocate_attribute("xlink:href", capabilities.capability.request.get_map.dcp_type.http.get.xlink_href.c_str());
          node->append_attribute(attr);

          get_node->append_node(node);

          http_node->append_node(get_node);

          dcp_type_node->append_node(http_node);

          get_map_node->append_node(dcp_type_node);
        }

        request_node->append_node(get_map_node);

        // GetFeatureInfo node
        rapidxml::xml_node<>* get_feature_info_node = xml_capabilities.allocate_node(rapidxml::node_element, "GetFeatureInfo");

        {
          // Temp code. This metadata should be get from json file
          for(auto format: capabilities.capability.request.get_feature_info.format)
          {
            get_feature_info_node->append_node(xml_capabilities.allocate_node(rapidxml::node_element, "Format", format.c_str()));
          }

          rapidxml::xml_node<>* dcp_type_node = xml_capabilities.allocate_node(rapidxml::node_element, "DCPType");

          rapidxml::xml_node<>* http_node = xml_capabilities.allocate_node(rapidxml::node_element, "HTTP");

          rapidxml::xml_node<>* get_node = xml_capabilities.allocate_node(rapidxml::node_element, "Get");

          rapidxml::xml_node<>* node = xml_capabilities.allocate_node(rapidxml::node_element, "OnlineResource");

          rapidxml::xml_attribute<>* attr = xml_capabilities.allocate_attribute("xlink:type", capabilities.capability.request.get_feature_info.dcp_type.http.get.xlink_type.c_str());
          node->append_attribute(attr);

          attr = xml_capabilities.allocate_attribute("xlink:href", capabilities.capability.request.get_feature_info.dcp_type.http.get.xlink_href.c_str());
          node->append_attribute(attr);

          get_node->append_node(node);

          http_node->append_node(get_node);

          dcp_type_node->append_node(http_node);

          get_feature_info_node->append_node(dcp_type_node);
        }

        request_node->append_node(get_feature_info_node);
      }

      capability_node->append_node(request_node);

      // Exception node
      rapidxml::xml_node<>* exception_node = xml_capabilities.allocate_node(rapidxml::node_element, "Exception");

      {
        // Temp code. This metadata should be get from json file
        for(auto format: capabilities.capability.exception_formats)
        {
          exception_node->append_node(xml_capabilities.allocate_node(rapidxml::node_element, "Format", format.c_str()));
        }
      }

      capability_node->append_node(exception_node);

      // Layer node
      rapidxml::xml_node<>* layer_node = xml_capabilities.allocate_node(rapidxml::node_element, "Layer");

      {
        // Temp code. This metadata should be get from json file
        rapidxml::xml_node<>* node = xml_capabilities.allocate_node(rapidxml::node_element, "Name", capabilities.capability.layer.name.c_str());

        layer_node->append_node(node);

        // subLayers node
        for(auto layer: capabilities.capability.layer.layers)
        {
          rapidxml::xml_node<>* sub_layer_node = xml_capabilities.allocate_node(rapidxml::node_element, "Layer");

          sub_layer_node->append_node(xml_capabilities.allocate_node(rapidxml::node_element, "Name", layer.name.c_str()));

          layer_node->append_node(sub_layer_node);
        }
      }

      capability_node->append_node(layer_node);
    }

    root->append_node(service_node);

    root->append_node(capability_node);
  }

  xml_capabilities.append_node(root);
}
