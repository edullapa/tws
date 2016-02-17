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
#include "utils.hpp"

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

    rapidxml::xml_node<> *root = initialize_wcs_xml_namespaces(doc);

    {
      // Service Identification node (service metadata)
      rapidxml::xml_node<>* service_identification_node = doc.allocate_node(rapidxml::node_element, "ows:ServiceIdentification");

      {
        // Temp code. This metadata should be get from json file
        rapidxml::xml_node<>* node = doc.allocate_node(rapidxml::node_element, "ows:Title", capabilities.identification.title.c_str());

        service_identification_node->append_node(node);

        node = doc.allocate_node(rapidxml::node_element, "ows:Abstract", capabilities.identification.abstract.c_str());

        service_identification_node->append_node(node);

        node = doc.allocate_node(rapidxml::node_element, "ows:ServiceType", capabilities.identification.service_type.c_str());

        service_identification_node->append_node(node);

        node = doc.allocate_node(rapidxml::node_element, "ows:ServiceTypeVersion", "2.0.1");

        service_identification_node->append_node(node);

        for(auto profile: capabilities.identification.profiles)
        {
          service_identification_node->append_node(doc.allocate_node(rapidxml::node_element, "ows:Profile", profile.c_str()));
        }
      }

      // Service Provider node (service metadata)
      rapidxml::xml_node<>* service_provider_node = doc.allocate_node(rapidxml::node_element, "ows:ServiceProvider");

      {
        // Temp code. This metadata should be get from json file
        rapidxml::xml_node<>* node = doc.allocate_node(rapidxml::node_element, "ows:ProviderName", capabilities.provider.name.c_str());

        service_provider_node->append_node(node);

        node = doc.allocate_node(rapidxml::node_element, "ows:ProviderSite");

        rapidxml::xml_attribute<>* attr = doc.allocate_attribute("href", capabilities.provider.site.c_str());
        node->append_attribute(attr);

        service_provider_node->append_node(node);

        // ServiceContact node
        rapidxml::xml_node<>* service_contact_node = doc.allocate_node(rapidxml::node_element, "ows:ServiceContact");

        node->append_node(service_contact_node);
      }

      // Service Provider node (service metadata)
      rapidxml::xml_node<>* operations_metadata_node = doc.allocate_node(rapidxml::node_element, "ows:OperationsMetadata");

      {
        // getcapabilities
        rapidxml::xml_node<>* node = doc.allocate_node(rapidxml::node_element, "ows:Operation");

        rapidxml::xml_attribute<>* attr = doc.allocate_attribute("name", "GetCapabilities");
        node->append_attribute(attr);

        operations_metadata_node->append_node(node);

        // describe coverage
        node = doc.allocate_node(rapidxml::node_element, "ows:Operation");

        attr = doc.allocate_attribute("name", "DescribeCoverage");
        node->append_attribute(attr);

        operations_metadata_node->append_node(node);

        // getcoverage
        node = doc.allocate_node(rapidxml::node_element, "ows:Operation");

        attr = doc.allocate_attribute("name", "GetCoverage");
        node->append_attribute(attr);

        operations_metadata_node->append_node(node);
      }

      // wcs:ServiceMetadata node
      rapidxml::xml_node<>* service_metadata_node = doc.allocate_node(rapidxml::node_element, "wcs:ServiceMetadata");
      for(const auto& format: capabilities.metadata.formats_supported)
        service_metadata_node->append_node(doc.allocate_node(rapidxml::node_element, "wcs:formatSupported", format.c_str()));

      rapidxml::xml_node<>* contents_node = doc.allocate_node(rapidxml::node_element, "wcs:Contents");

      // retrieve the list of registered geo-arrays
      std::vector<std::string> arrays = tws::geoarray::geoarray_manager::instance().list_arrays();
      for(const auto& array: arrays)
      {
        rapidxml::xml_node<>* summary = doc.allocate_node(rapidxml::node_element, "wcs:CoverageSummary");
        summary->append_node(doc.allocate_node(rapidxml::node_element, "wcs:CoverageId", array.c_str()));
        // todo: set it from file
        summary->append_node(doc.allocate_node(rapidxml::node_element, "wcs:CoverageSubType", "GridCoverage"));

        contents_node->append_node(summary);
      }

      root->append_node(service_identification_node);
      root->append_node(service_provider_node);
      root->append_node(operations_metadata_node);
      root->append_node(service_metadata_node);
      root->append_node(contents_node);
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
  describe_coverage_t describe = wcs_manager::instance().describe_coverage();

  // create DescribeCoverage document
  rapidxml::xml_document<> doc;

  rapidxml::xml_node<> *root = initialize_wcs_xml_namespaces(doc);

  doc.append_node(root);

  for(const auto& coverage: describe.coverages_description)
  {
    rapidxml::xml_node<>* node = doc.allocate_node(rapidxml::node_element, "wcs:CoverageDescription", coverage.id.c_str());
    {
      rapidxml::xml_node<>* bounded_by_node = doc.allocate_node(rapidxml::node_element, "bounded_by");
      rapidxml::xml_node<>* envelope_node = doc.allocate_node(rapidxml::node_element, "Envelope");

      rapidxml::xml_attribute<>* envelope_attr = doc.allocate_attribute("axisLabels", coverage.bounded_by.envelope.axis.c_str());
      envelope_node->append_attribute(envelope_attr);

      // todo: check. its printing "<"
      envelope_attr = doc.allocate_attribute("srsDimension", std::to_string(coverage.bounded_by.envelope.dimension).c_str());
      envelope_node->append_attribute(envelope_attr);

//      rapidxml::xml_attribute<>* attribute = doc.allocate_attribute("srsName", "")
      envelope_node->append_node(doc.allocate_node(rapidxml::node_element, "lowerCorner", coverage.bounded_by.envelope.min.c_str()));
      envelope_node->append_node(doc.allocate_node(rapidxml::node_element, "upperCorner", coverage.bounded_by.envelope.max.c_str()));

      bounded_by_node->append_node(envelope_node);

      node->append_node(bounded_by_node);
    }

    node->append_node(doc.allocate_node(rapidxml::node_element, "wcs:CoverageId", coverage.id.c_str()));

    rapidxml::xml_node<>* domain_set_node  = doc.allocate_node(rapidxml::node_element, "gml:domainSet");
    {
      rapidxml::xml_node<>* grid_node  = doc.allocate_node(rapidxml::node_element, "gml:Grid");
      rapidxml::xml_attribute<>* attr = doc.allocate_attribute("srsDimension", std::to_string(coverage.bounded_by.envelope.dimension).c_str());
      grid_node->append_attribute(attr);

      rapidxml::xml_node<>* limits_node  = doc.allocate_node(rapidxml::node_element, "gml:limits");
      rapidxml::xml_node<>* grid_envelope_node  = doc.allocate_node(rapidxml::node_element, "gml:GridEnvelope");

      grid_envelope_node->append_node(doc.allocate_node(rapidxml::node_element, "low", coverage.bounded_by.envelope.min.c_str()));
      grid_envelope_node->append_node(doc.allocate_node(rapidxml::node_element, "high", coverage.bounded_by.envelope.max.c_str()));

      limits_node->append_node(grid_envelope_node);
      grid_node->append_node(limits_node);

      domain_set_node->append_node(grid_node);
    }

    node->append_node(domain_set_node);

    rapidxml::xml_node<>* range_type_node  = doc.allocate_node(rapidxml::node_element, "gmlcov:rangeType");
    {
      rapidxml::xml_node<>* data_record = doc.allocate_node(rapidxml::node_element, "swe:DataRecord");
      for(const auto& attribute: coverage.geoarray.attributes)
      {
        rapidxml::xml_node<>* swe_field = doc.allocate_node(rapidxml::node_element, "swe:Field");
        rapidxml::xml_attribute<>* attr = doc.allocate_attribute("name", attribute.name.c_str());
        swe_field->append_attribute(attr);

        rapidxml::xml_node<>* swe_quantity = doc.allocate_node(rapidxml::node_element, "swe:Quantity");
        swe_quantity->append_node(doc.allocate_node(rapidxml::node_element, "swe:description", attribute.description.c_str()));

        rapidxml::xml_node<>* swe_constraint = doc.allocate_node(rapidxml::node_element, "swe:constraint");
        rapidxml::xml_node<>* swe_allowed_values = doc.allocate_node(rapidxml::node_element, "swe:AllowedValues");

        // todo: check it.  Its printing wcs description tag.  ????
//        std::string range = std::to_string(attribute.valid_range.min_val) + " " + std::to_string(attribute.valid_range.max_val);

//        swe_allowed_values->append_node(doc.allocate_node(rapidxml::node_element, "swe:interval", range.c_str()));

        swe_constraint->append_node(swe_allowed_values);

        swe_quantity->append_node(swe_constraint);

        swe_field->append_node(swe_quantity);

        data_record->append_node(swe_field);
      }

      range_type_node->append_node(data_record);
    }

    node->append_node(range_type_node);

    root->append_node(node);
  }

  // output result
  std::string str_buff;

  rapidxml::print(std::back_inserter(str_buff), doc, 0);

  const char* p_str_buff = str_buff.c_str();

  response.add_header("Content-Type", "application/xml");
  response.add_header("Access-Control-Allow-Origin", "*");
  response.set_content(p_str_buff, str_buff.size());
}

void tws::wcs::get_coverage_functor::operator()(const tws::core::http_request& request,
                                                tws::core::http_response& response)
{
  const char* query_string = request.query_string();
  // temp code
  if (query_string == nullptr)
  {
    throw tws::missing_argument_error() << tws::error_description("Arguments missing. OGC services requires at least three arguments: \"service\", \"version\" e \"request\"");
  }

  tws::core::query_string_t arguments = tws::core::expand(query_string);

  const auto& coverage_id = arguments.find("coverageID");
  if (coverage_id == arguments.end())
    throw tws::missing_argument_error() << tws::error_description("Missing \"coverageID\" argument");

  ////  std::string str_q_aql("SELECT " + attrs + " FROM " + coverage.from_clause);
  ////
  ////  if(!coverage.where_clause.empty())
  ////    str_q_aql += " WHERE " + coverage.where_clause;
  ////
  ////  boost::format q_aql(str_q_aql);
  ////  q_aql.bind_arg(1, pixel_col);
  ////  q_aql.bind_arg(2, pixel_row);
  ////  q_aql.bind_arg(3, start_time_pos);
  ////  q_aql.bind_arg(4, pixel_col);
  ////  q_aql.bind_arg(5, pixel_row);
  ////  q_aql.bind_arg(6, end_time_pos);
  //
  //// get a connection to retrieve the time series
  //  std::unique_ptr<tws::scidb::connection> conn(tws::scidb::connection_pool::instance().get());
  //
  ////  boost::shared_ptr< ::scidb::QueryResult > qresult = conn->execute(q_aql.str(), false);
  //
  ////  if((qresult.get() == nullptr) || (qresult->array.get() == nullptr))
  ////    throw tws::core::http_request_error() << tws::error_description("no query result returned after querying database.");
  //
  ////  const ::scidb::ArrayDesc& array_desc = qresult->array->getArrayDesc();
  //
  ////  const ::scidb::Attributes& array_attributes = array_desc.getAttributes(true);
  //
  ////  const std::size_t nattributes = array_attributes.size();
  //  const std::size_t nattributes = queried_attributes.size();

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

