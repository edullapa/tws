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

// STL
#include <algorithm>
#include <memory>
#include <iostream>

// Boost
#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/format.hpp>
#include <boost/lexical_cast.hpp>

// SciDB
//#include <SciDBAPI.h>

// RapidJSON
#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>

#include <terralib/xml/Writer.h>
#include <terralib/xml/AbstractWriterFactory.h>
#include <terralib/xml/ReaderFactory.h>


void tws::wcs::get_capabilities_functor::operator()(const tws::core::http_request& request,
                                                    tws::core::http_response& response)
{

  // TODO: generate in memory and retrieve instead saving in file
  std::shared_ptr<te::xml::AbstractWriter> writer(te::xml::AbstractWriterFactory::make());
  writer->setURI("capabilities.xml");
  writer->setRootNamespaceURI("http://www.opengis.net/wcs/2.0");
  writer->writeStartDocument("UTF", "no");
  writer->writeStartElement("wcs:Capabilities");
  writer->writeEndElement("wcs:Capabilities");
  writer->writeToFile();

//  std::shared_ptr<te::xml::Reader> xml_reader(te::xml::ReaderFactory::make());
//  xml_reader->read("capabilities.xml");
  std::string output;

//  while(xml_reader->next())
  {
//    output.append(xml_reader->getElementLocalName());
//    std::cout << "Element Name: " << reader->getElementLocalName() << " -> ";
//    std::cout << "Node Type: "; NodeTypePrinter(reader->getNodeType());

//    if(reader->getNodeType() == te::xml::START_ELEMENT && reader->hasAttrs())
//    {
//      std::cout << "  Attributes" << std::endl;
//      for(std::size_t i = 0; i < reader->getNumberOfAttrs(); ++i)
//        std::cout << "  * [" << reader->getAttrLocalName(i) << " = " << reader->getAttr(i) << "]" << std::endl;
//    }

//    if(reader->getNodeType() == te::xml::VALUE)
//      std::cout << "  * Element value: " << reader->getElementValue() << std::endl;
  }

  response.add_header("Content-Type", "application/xml");
  response.add_header("Access-Control-Allow-Origin", "*");
  std::string message = "{\"status\": 1}";
  response.set_content(output.c_str(), output.size());
//  response.set_content(message.c_str(), message.size());

//// output result
//  rapidjson::Document::AllocatorType allocator;

//  rapidjson::Document doc;

//  doc.SetObject();

//  rapidjson::Value jarrays(rapidjson::kArrayType);

//  rapidjson::StringBuffer str_buff;

//  rapidjson::Writer<rapidjson::StringBuffer> writer(str_buff);

//  doc.Accept(writer);

//  const char* p_str_buff = str_buff.GetString();

//  response.add_header("Content-Type", "application/json");
//  response.add_header("Access-Control-Allow-Origin", "*");
//  response.set_content(p_str_buff, str_buff.Size());
}

void
tws::wcs::describe_coverage_functor::operator()(const tws::core::http_request& request,
                                                tws::core::http_response& response)
{
// output result
  rapidjson::Document::AllocatorType allocator;

  rapidjson::Document doc;

  doc.SetObject();

  rapidjson::Value jarrays(rapidjson::kArrayType);

  rapidjson::StringBuffer str_buff;

  rapidjson::Writer<rapidjson::StringBuffer> writer(str_buff);

  doc.Accept(writer);

  const char* p_str_buff = str_buff.GetString();

  response.add_header("Content-Type", "application/json");
  response.add_header("Access-Control-Allow-Origin", "*");
  response.set_content(p_str_buff, str_buff.Size());
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

