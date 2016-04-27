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
#include "../core/http_request.hpp"
#include "../core/http_response.hpp"
#include "../core/service_operations_manager.hpp"
#include "../core/utils.hpp"
#include "../geoarray/geoarray_manager.hpp"
#include "data_types.hpp"
#include "wms_manager.hpp"
#include "xml_serializer.hpp"

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

// GD
#include <gd.h>

void
tws::wms::get_capabilities_functor::operator()(const tws::core::http_request& request,
                                               tws::core::http_response& response)
{
  const rapidxml::xml_document<>& xml_capabilities = tws::wms::wms_manager::instance().xml_capabilities();

// output result
  std::string str_buff;

  rapidxml::print(std::back_inserter(str_buff), xml_capabilities, 0);

  const char* p_str_buff = str_buff.c_str();

  response.add_header("Content-Type", "application/xml");
  response.add_header("Access-Control-Allow-Origin", "*");
  response.set_content(p_str_buff, str_buff.size());
}

void
tws::wms::get_map_functor::operator()(const tws::core::http_request& request,
                                      tws::core::http_response& response)
{
// get client query string
  const char* qstring = request.query_string();

  if(qstring == nullptr)
    throw tws::core::http_request_error() << tws::error_description("GetMap operation requires the following parameters: \"LAYERS\", \"BBOX\", \"WIDTH\", \"HEIGHT\".");

// parse plain text query string to a std::map
  tws::core::query_string_t qstr = tws::core::expand(qstring);

// which layers?
  tws::core::query_string_t::const_iterator it = qstr.find("LAYERS");
  tws::core::query_string_t::const_iterator it_end = qstr.end();

  if(it == it_end)
    throw tws::core::http_request_error() << tws::error_description("GetMap error: \"LAYERS\" parameter is missing!");

  std::vector<std::string> layers;

  boost::split(layers, it->second, boost::is_any_of(","));

  if(layers.empty())
    throw tws::core::http_request_error() << tws::error_description("check GetCapabilities operation: please, inform at least one layer.");

// valid layer list
  const capabilities_t& capabilities = tws::wms::wms_manager::instance().capabilities();

  for(const auto& layer_name : layers)
  {
    std::vector<layer_t>::const_iterator it = std::find_if(capabilities.capability.layer.layers.begin(),
                                                           capabilities.capability.layer.layers.end(),
                                                           [&layer_name](const layer_t& layer){ return (layer.name == layer_name); });

    if(it == capabilities.capability.layer.layers.end())
    {
      boost::format err_ms("Layer '%1%' is not valid in GetMap!");
      throw tws::core::http_request_error() << tws::error_description((err_ms % layer_name).str());
    }
  }

// retrieve bounding box
  it = qstr.find("BBOX");

  if(it == it_end || it->second.empty())
    throw tws::core::http_request_error() << tws::error_description("check GetMap operation: \"BBOX\" parameter is missing!");

  std::vector<std::string> str_bbox;

  boost::split(str_bbox, it->second, boost::is_any_of(","));

  if(str_bbox.size() != 4)
  {
    boost::format err_ms("Invalid bounding box: '%1%'.");
    throw tws::core::http_request_error() << tws::error_description((err_ms % it->second).str());
  }

  bounding_box_t bbox;
  bbox.min_x = std::stod(str_bbox[0]);
  bbox.min_y = std::stod(str_bbox[1]);
  bbox.max_x = std::stod(str_bbox[2]);
  bbox.max_y = std::stod(str_bbox[3]);

// output image size
  it = qstr.find("WIDTH");

  if(it == it_end || it->second.empty())
    throw tws::core::http_request_error() << tws::error_description("check GetMap operation: \"WIDTH\" parameter is missing!");

  const uint32_t width = std::stoul(it->second);

  it = qstr.find("HEIGHT");

  if(it == it_end || it->second.empty())
    throw tws::core::http_request_error() << tws::error_description("check GetMap operation: \"HEIGHT\" parameter is missing!");

  const uint32_t height = std::stoul(it->second);

// create a GD Image
  gdImagePtr img =  gdImageCreateTrueColor(width, height);

  int red = gdTrueColorAlpha(255, 0, 0, 0);

  for(uint32_t i = 0; i != height; ++i)
    for(uint32_t j = 0; j != width; ++j)
      gdImageSetPixel(img, j, i, red);

  int png_size = 0;

  void* png_img = gdImagePngPtr(img, &png_size);

  response.add_header("Content-Type", "image/png");
  response.add_header("Access-Control-Allow-Origin", "*");
  response.set_content((char*)png_img, png_size);

  gdFree(png_img);

  gdImageDestroy(img);
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
