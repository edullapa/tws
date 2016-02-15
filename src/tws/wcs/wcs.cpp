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
  \file tws/wcs/wcs.cpp

  \brief Web Coverage Service implementation.

  \author Raphael Willian da Costa
 */

// TWS
#include "wcs.hpp"
#include "../core/http_request.hpp"
#include "../core/http_response.hpp"
#include "../core/service_operations_manager.hpp"
#include "../core/utils.hpp"
#include "../geoarray/geoarray_manager.hpp"
#include "wcs_manager.hpp"
#include "data_types.hpp"

// STL
#include <algorithm>
#include <iterator>
#include <memory>
#include <iostream>

// Boost
#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/format.hpp>
#include <boost/lexical_cast.hpp>

// SciDB
//#include <SciDBAPI.h>

// RapidXml
#include <rapidxml/rapidxml_print.hpp>
#include <rapidxml/rapidxml.hpp>

void tws::wcs::get_capabilities_functor::operator()(const tws::core::http_request& request,
                                                    tws::core::http_response& response)
{
  capabilities_t capabilities = tws::wcs::wcs_manager::instance().capabilities();

  try
  {

    // create Capabilities document
    rapidxml::xml_document<> doc;

    rapidxml::xml_node<> *root = doc.allocate_node(rapidxml::node_element, "Capabilities");

    rapidxml::xml_attribute<>* attr = doc.allocate_attribute("version", "2.0.1");
    root->append_attribute(attr);

    {
      // Service Identification node (service metadata)
      rapidxml::xml_node<>* serviceIdentificationNode = doc.allocate_node(rapidxml::node_element, "ServiceIdentification");

      {
        // Temp code. This metadata should be get from json file
        rapidxml::xml_node<>* node = doc.allocate_node(rapidxml::node_element, "Title", capabilities.identification.title.c_str());

        serviceIdentificationNode->append_node(node);

        node = doc.allocate_node(rapidxml::node_element, "Abstract", capabilities.identification.abstract.c_str());

        serviceIdentificationNode->append_node(node);

        node = doc.allocate_node(rapidxml::node_element, "ServiceType", capabilities.identification.service_type.c_str());

        serviceIdentificationNode->append_node(node);

        node = doc.allocate_node(rapidxml::node_element, "ServiceTypeVersion", "2.0.1");

        serviceIdentificationNode->append_node(node);

        for(auto profile: capabilities.identification.profiles)
        {
          serviceIdentificationNode->append_node(doc.allocate_node(rapidxml::node_element, "Profile", profile.c_str()));
        }
      }

      // Service Provider node (service metadata)
      rapidxml::xml_node<>* serviceProviderNode = doc.allocate_node(rapidxml::node_element, "ServiceProvider");

      {
        // Temp code. This metadata should be get from json file
        rapidxml::xml_node<>* node = doc.allocate_node(rapidxml::node_element, "ProviderName", capabilities.provider.name.c_str());

        serviceProviderNode->append_node(node);

        node = doc.allocate_node(rapidxml::node_element, "ProviderSite");

        rapidxml::xml_attribute<>* attr = doc.allocate_attribute("href", capabilities.provider.site.c_str());
        node->append_attribute(attr);

        serviceProviderNode->append_node(node);

        // ServiceContact node
        rapidxml::xml_node<>* serviceContactNode = doc.allocate_node(rapidxml::node_element, "ServiceContact");

        node->append_node(serviceContactNode);
      }

      // Service Provider node (service metadata)
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
      for(const auto& format: capabilities.metadata.formats_supported)
        serviceMetadataNode->append_node(doc.allocate_node(rapidxml::node_element, "formatSupported", format.c_str()));

      rapidxml::xml_node<>* contentsaNode = doc.allocate_node(rapidxml::node_element, "Contents");

      // retrieve the list of registered geo-arrays
      std::vector<std::string> arrays = tws::geoarray::geoarray_manager::instance().list_arrays();
      for(auto array: arrays)
      {
        rapidxml::xml_node<>* summary = doc.allocate_node(rapidxml::node_element, "CoverageSummary");
        summary->append_node(doc.allocate_node(rapidxml::node_element, "CoverageId", array.c_str()));
        // todo: set it from file
        summary->append_node(doc.allocate_node(rapidxml::node_element, "CoverageSubType", "GridCoverage"));

        contentsaNode->append_node(summary);
      }

      root->append_node(serviceIdentificationNode);
      root->append_node(serviceProviderNode);
      root->append_node(operationsMetadataNode);
      root->append_node(serviceMetadataNode);
      root->append_node(contentsaNode);
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
tws::wcs::describe_coverage_functor::operator()(const tws::core::http_request& request,
                                                tws::core::http_response& response)
{
//// output result
//  rapidjson::Document::AllocatorType allocator;
//
//  rapidjson::Document doc;
//
//  doc.SetObject();
//
//  rapidjson::Value jarrays(rapidjson::kArrayType);
//
//  rapidjson::StringBuffer str_buff;
//
//  rapidjson::Writer<rapidjson::StringBuffer> writer(str_buff);
//
//  doc.Accept(writer);
//
//  const char* p_str_buff = str_buff.GetString();
//
//  response.add_header("Content-Type", "application/json");
//  response.add_header("Access-Control-Allow-Origin", "*");
//  response.set_content(p_str_buff, str_buff.Size());
}

void tws::wcs::get_coverage_functor::operator()(const tws::core::http_request& request,
                                                tws::core::http_response& response)
{

}

void tws::wcs::register_operations()
{
  tws::core::service_metadata service;

  service.name = "wcs";

// 1st WCS operation: GetCapabilities
  {
    tws::core::service_operation s_op;

    s_op.name = "GetCapabilities";
    s_op.description = ".......";
    s_op.handler = get_capabilities_functor();

    service.operations.push_back(s_op);
  }

// 2nd WCS operation: DescribeCoverage
  {
    tws::core::service_operation s_op;

    s_op.name = "DescribeCoverage";
    s_op.description = "...";
    s_op.handler = describe_coverage_functor();

    service.operations.push_back(s_op);
  }

// 3rd WCS operation: GetCoverage
  {
    tws::core::service_operation s_op;

    s_op.name = "GetCoverage";
    s_op.description = "....";
    s_op.handler = get_coverage_functor();

    service.operations.push_back(s_op);
  }

  tws::core::service_operations_manager::instance().insert(service);
}

void tws::wcs::initialize_operations()
{

}

