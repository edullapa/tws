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
  \file tws/wms/wms.cpp

  \brief Web Map Service.

  \author Roger Victor
 */

// TWS
#include "wms.hpp"
#include "data_types.hpp"
#include "wms_manager.hpp"
#include "../core/http_request.hpp"
#include "../core/http_response.hpp"
#include "../core/service_operations_manager.hpp"
#include "../core/utils.hpp"
#include "../geoarray/geoarray_manager.hpp"

// STL
#include <algorithm>
#include <memory>

// Boost
#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/format.hpp>
#include <boost/lexical_cast.hpp>

// SciDB
//#include <SciDBAPI.h>

// RapidXml
#include <rapidxml/rapidxml.hpp>
#include <rapidxml/rapidxml_print.hpp>

void
tws::wms::get_capabilities_functor::operator()(const tws::core::http_request& request,
                                               tws::core::http_response& response)
{
  capabilities_t capabilities = tws::wms::wms_manager::instance().capabilities();
  
  try
  {
    
    // create Capabilities document
    rapidxml::xml_document<> doc;
    
    rapidxml::xml_node<>* root = doc.allocate_node(rapidxml::node_element, "WMS_Capabilities");
    
    rapidxml::xml_attribute<>* attr = doc.allocate_attribute("version", "1.3.0");
    root->append_attribute(attr);
    
    attr = doc.allocate_attribute("xmlns:xlink", "http://www.w3.org/1999/xlink");
    root->append_attribute(attr);

    {
      // Service node (service metadata)
      rapidxml::xml_node<>* service_node = doc.allocate_node(rapidxml::node_element, "Service");

      {
        // Temp code. This metadata should be get from json file
        rapidxml::xml_node<>* node = doc.allocate_node(rapidxml::node_element, "Name", capabilities.service.name.c_str());

        service_node->append_node(node);
        
        node = doc.allocate_node(rapidxml::node_element, "Title", capabilities.service.title.c_str());

        service_node->append_node(node);

        node = doc.allocate_node(rapidxml::node_element, "Abstract", capabilities.service.abstract.c_str());

        service_node->append_node(node);

        // KeywordList node
        rapidxml::xml_node<>* keyword_list_node = doc.allocate_node(rapidxml::node_element, "KeywordList");

        for(auto keyword: capabilities.service.keyword_list)
        {
          keyword_list_node->append_node(doc.allocate_node(rapidxml::node_element, "Keyword", keyword.vocabulary.c_str()));
        }

        service_node->append_node(keyword_list_node);

        node = doc.allocate_node(rapidxml::node_element, "OnlineResource");

        rapidxml::xml_attribute<>* attr = doc.allocate_attribute("xlink:type", capabilities.service.online_resource.xlink_type.c_str());
        node->append_attribute(attr);

        attr = doc.allocate_attribute("xlink:href", capabilities.service.online_resource.xlink_href.c_str());
        node->append_attribute(attr);

        service_node->append_node(node);

        // Contact Information node
        rapidxml::xml_node<>* contact_information_node = doc.allocate_node(rapidxml::node_element, "ContactInformation");

        {
          // Temp code. This metadata should be get from json file
          rapidxml::xml_node<>* contact_person_node = doc.allocate_node(rapidxml::node_element, "ContactPersonPrimary");

          rapidxml::xml_node<>* node = doc.allocate_node(rapidxml::node_element, "ContactPerson", capabilities.service.contact_information.contact_person_primary.contact_person.c_str());

          contact_person_node->append_node(node);

          node = doc.allocate_node(rapidxml::node_element, "ContactOrganization", capabilities.service.contact_information.contact_person_primary.contact_organization.c_str());

          contact_person_node->append_node(node);

          contact_information_node->append_node(contact_person_node);

          node = doc.allocate_node(rapidxml::node_element, "ContactPosition", capabilities.service.contact_information.contact_position.c_str());

          contact_information_node->append_node(node);

          rapidxml::xml_node<>* contact_address_node = doc.allocate_node(rapidxml::node_element, "ContactAddress");

          node = doc.allocate_node(rapidxml::node_element, "AddressType", capabilities.service.contact_information.contact_address.address_type.c_str());

          contact_address_node->append_node(node);

          node = doc.allocate_node(rapidxml::node_element, "Address", capabilities.service.contact_information.contact_address.address.c_str());

          contact_address_node->append_node(node);

          node = doc.allocate_node(rapidxml::node_element, "City", capabilities.service.contact_information.contact_address.city.c_str());

          contact_address_node->append_node(node);

          node = doc.allocate_node(rapidxml::node_element, "StateOrProvince", capabilities.service.contact_information.contact_address.state_or_province.c_str());

          contact_address_node->append_node(node);

          node = doc.allocate_node(rapidxml::node_element, "PostCode", capabilities.service.contact_information.contact_address.post_code.c_str());

          contact_address_node->append_node(node);

          node = doc.allocate_node(rapidxml::node_element, "Country", capabilities.service.contact_information.contact_address.country.c_str());

          contact_address_node->append_node(node);

          contact_information_node->append_node(contact_address_node);

          node = doc.allocate_node(rapidxml::node_element, "ContactVoiceTelephone", capabilities.service.contact_information.contact_voice_telephone.c_str());

          contact_information_node->append_node(node);

          node = doc.allocate_node(rapidxml::node_element, "ContactElectronicMailAddress", capabilities.service.contact_information.contact_electronic_mail_address.c_str());

          contact_information_node->append_node(node);
        }

        service_node->append_node(contact_information_node);

        node = doc.allocate_node(rapidxml::node_element, "Fees", capabilities.service.fees.c_str());

        service_node->append_node(node);

        node = doc.allocate_node(rapidxml::node_element, "AccessConstraints", capabilities.service.access_constraints.c_str());

        service_node->append_node(node);

        node = doc.allocate_node(rapidxml::node_element, "LayerLimit", std::to_string(capabilities.service.layer_limit).c_str());

        service_node->append_node(node);

        node = doc.allocate_node(rapidxml::node_element, "MaxWidth", std::to_string(capabilities.service.max_width).c_str());

        service_node->append_node(node);

        node = doc.allocate_node(rapidxml::node_element, "MaxHeight", std::to_string(capabilities.service.max_height).c_str());

        service_node->append_node(node);
      }

      // Capability node
      rapidxml::xml_node<>* capability_node = doc.allocate_node(rapidxml::node_element, "Capability");

      {
        // Request node
        rapidxml::xml_node<>* request_node = doc.allocate_node(rapidxml::node_element, "Request");

        {

        }

        capability_node->append_node(request_node);
      }

      root->append_node(service_node);

      root->append_node(capability_node);
    }

    doc.append_node(root);

    // output result
    std::string str_buff;

    rapidxml::print(std::back_inserter(str_buff), doc, 0);

    const char* p_str_buff = str_buff.c_str();

    response.add_header("Content-Type", "application/xml");
    response.add_header("Access-Control-Allow-Origin", "*");
    response.set_content(p_str_buff, str_buff.size());
  }
  catch(...)
  {

  }
}

void
tws::wms::get_map_functor::operator()(const tws::core::http_request& request,
                                      tws::core::http_response& response)
{

}

void
tws::wms::get_feature_info_functor::operator()(const tws::core::http_request& request,
                                           tws::core::http_response& response)
{

}

void
tws::wms::register_operations()
{
  tws::core::service_metadata service;

  service.name = "wms";

// 1st WMS operation: GetCapabilities
  {
    tws::core::service_operation s_op;

    s_op.name = "GetCapabilities";
    s_op.description = "List the metadata, describing the layers avaliable for vizualization.";
    s_op.handler = get_capabilities_functor();

    service.operations.push_back(s_op);
  }

// 2nd WMS operation: GetMap
  {
    tws::core::service_operation s_op;

    s_op.name = "GetMap";
    s_op.description = "Request the server to render a map giving a list of layers.";
    s_op.handler = get_map_functor();

    service.operations.push_back(s_op);
  }

// 3rd WMS operation: GetFeatureInfo
  {
    tws::core::service_operation s_op;

    s_op.name = "GetFeatureInfo";
    s_op.description = "Retrieve information about an element of a particular layer.";
    s_op.handler = get_feature_info_functor();

    service.operations.push_back(s_op);
  }

  tws::core::service_operations_manager::instance().insert(service);
}

void
tws::wms::initialize_operations()
{

}
