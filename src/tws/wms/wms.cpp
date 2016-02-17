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
    
    rapidxml::xml_node<> *root = doc.allocate_node(rapidxml::node_element, "WMS_Capabilities");
    
    rapidxml::xml_attribute<>* attr = doc.allocate_attribute("version", "1.3.0");
    root->append_attribute(attr);
    
    {
      // Service Identification node (service metadata)
      rapidxml::xml_node<>* serviceNode = doc.allocate_node(rapidxml::node_element, "Service");

      {
        // Temp code. This metadata should be get from json file
        rapidxml::xml_node<>* node = doc.allocate_node(rapidxml::node_element, "Name", capabilities.service.name.c_str());

        serviceNode->append_node(node);
        
        node = doc.allocate_node(rapidxml::node_element, "Title", capabilities.service.title.c_str());

        serviceNode->append_node(node);

        node = doc.allocate_node(rapidxml::node_element, "Abstract", capabilities.service.abstract.c_str());

        serviceNode->append_node(node);
      }

      // Service Provider node (service metadata)
      rapidxml::xml_node<>* contactInformationNode = doc.allocate_node(rapidxml::node_element, "Contact Information");

      {
        // Temp code. This metadata should be get from json file
        rapidxml::xml_node<>* node = doc.allocate_node(rapidxml::node_element, "Contact Position", capabilities.service.contact_information.contact_position.c_str());

        serviceNode->append_node(node);
      }

      /* // Service Provider node (service metadata)
      rapidxml::xml_node<>* operationsMetadataNode = doc.allocate_node(rapidxml::node_element, "OperationsMetadata");

      {
        // getcapabilities
        rapidxml::xml_node<>* node = doc.allocate_node(rapidxml::node_element, "Operation");

        rapidxml::xml_attribute<>* attr = doc.allocate_attribute("name", "GetCapabilities");
        node->append_attribute(attr);

        operationsMetadataNode->append_node(node);

        // describe coverage
        node = doc.allocate_node(rapidxml::node_element, "Operation");

        attr = doc.allocate_attribute("name", "DescribeCoverage");
        node->append_attribute(attr);

        operationsMetadataNode->append_node(node);

        // getcoverage
        node = doc.allocate_node(rapidxml::node_element, "Operation");

        attr = doc.allocate_attribute("name", "GetCoverage");
        node->append_attribute(attr);

        operationsMetadataNode->append_node(node);
      }

      // wcs:ServiceMetadata node
      rapidxml::xml_node<>* serviceMetadataNode = doc.allocate_node(rapidxml::node_element, "ServiceMetadata");
      serviceMetadataNode->append_node(doc.allocate_node(rapidxml::node_element, "formatSupported", "application/xml"));
      serviceMetadataNode->append_node(doc.allocate_node(rapidxml::node_element, "formatSupported", "image/tiff"));
      serviceMetadataNode->append_node(doc.allocate_node(rapidxml::node_element, "formatSupported", "image/hdf"));

      rapidxml::xml_node<>* contentsaNode = doc.allocate_node(rapidxml::node_element, "Contents");

      // retrieve the list of registered geo-arrays
      std::vector<std::string> arrays = tws::geoarray::geoarray_manager::instance().list_arrays();
      for(const auto& array: arrays)
      {
        rapidxml::xml_node<>* summary = doc.allocate_node(rapidxml::node_element, "CoverageSummary");
        summary->append_node(doc.allocate_node(rapidxml::node_element, "CoverageId", array.c_str()));
        // todo: set it from file
        summary->append_node(doc.allocate_node(rapidxml::node_element, "CoverageSubType", "GridCoverage"));

        contentsaNode->append_node(summary);
      } */

      root->append_node(serviceNode);
      root->append_node(contactInformationNode);
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

