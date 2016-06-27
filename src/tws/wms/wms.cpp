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
#include "../geoarray/timeline.hpp"
#include "../geoarray/timeline_manager.hpp"
#include "../scidb/connection.hpp"
#include "../scidb/connection_pool.hpp"
#include "../scidb/utils.hpp"
#include "data_types.hpp"
#include "wms_manager.hpp"
#include "xml_serializer.hpp"

// STL
#include <algorithm>
#include <memory>
#include <tuple>

// Boost
#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/format.hpp>
#include <boost/lexical_cast.hpp>

// SciDB
#include <SciDBAPI.h>

// RapidXml
#include <rapidxml/rapidxml.hpp>
#include <rapidxml/rapidxml_print.hpp>

// GD
#include <gd.h>

// TerraLib
#include <terralib/geometry/Envelope.h>
#include <terralib/raster/Grid.h>
#include <terralib/srs/Converter.h>

namespace tws
{
  namespace wms
  {
    struct get_map_request_parameters
    {
      std::string version;
      std::vector<std::string> layers;
      std::vector<std::string> styles;
      std::string crs;
      te::gm::Envelope bbox;
      uint32_t width;
      uint32_t height;
      std::string format;
      std::string time_point;
    };

    typedef std::tuple<const layer_t*, const style_t*, const tws::geoarray::geoarray_t*, const tws::geoarray::timeline*> layer_tuple_t;

    get_map_request_parameters
    decode_get_map_request(const tws::core::query_string_t& qstr);

    std::vector<layer_tuple_t>
    valid(const get_map_request_parameters& parameters);

    void render(const layer_tuple_t& ltuple,
                const get_map_request_parameters& parameters,
                gdImagePtr img);

//    std::size_t
//    compute_time_index(const tws::geoarray::geoarray_t* geoarray,
//                       const tws::geoarray::timeline* tl,
//                       const get_map_request_parameters& parameters);

  } // end namespace wms
}   // end namespace tws

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
    throw tws::core::http_request_error() << tws::error_description("GetMap operation requires the following parameters: \"VERSION\", \"LAYERS\", \"CRS\", \"BBOX\", \"WIDTH\", \"HEIGHT\", \"FORMAT\", \"TIME\".");

// parse plain text query string to a std::map
  tws::core::query_string_t qstr = tws::core::expand(qstring);

// parse parameters to a struct
  get_map_request_parameters parameters = decode_get_map_request(qstr);

// valid parameters
  std::vector<tws::wms::layer_tuple_t> layers_to_render = valid(parameters);

// now... let's render the selected layers!
  std::unique_ptr<gdImage, decltype(&gdImageDestroy)> img(gdImageCreateTrueColor(parameters.width, parameters.height), gdImageDestroy);

  for(const auto& ltuple : layers_to_render)
  {
    render(ltuple, parameters, img.get());
  }

  int png_size = 0;

  std::unique_ptr<void, decltype(&gdFree)> png_img(gdImagePngPtr(img.get(), &png_size), gdFree);

  response.add_header("Content-Type", "image/png");
  response.add_header("Access-Control-Allow-Origin", "*");
  response.set_content((char*)(png_img.get()), png_size);
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
    s_op.description = "List the metadata, describing the layers avaliable for visualization.";
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
  tws::geoarray::geoarray_manager::instance();
  tws::geoarray::timeline_manager::instance();
  wms_manager::instance();
}

tws::wms::get_map_request_parameters
tws::wms::decode_get_map_request(const tws::core::query_string_t& qstr)
{
  get_map_request_parameters parameters;
// get service version
  tws::core::query_string_t::const_iterator it = qstr.find("VERSION");
  tws::core::query_string_t::const_iterator it_end = qstr.end();

  if(it == it_end || it->second.empty())
    throw tws::core::http_request_error() << tws::error_description("Error on GetMap operation: \"VERSION\" parameter is missing.");

  parameters.version = it->second;

// get layers
  it = qstr.find("LAYERS");

  if(it == it_end)
    throw tws::core::http_request_error() << tws::error_description("Error on GetMap operation: \"LAYERS\" parameter is missing.");

  boost::split(parameters.layers, it->second, boost::is_any_of(","));

// get layers
    it = qstr.find("STYLES");

    if(it == it_end)
      throw tws::core::http_request_error() << tws::error_description("Error on GetMap operation: \"STYLES\" parameter is missing.");

    boost::split(parameters.styles, it->second, boost::is_any_of(","));

// retrieve crs
  it = qstr.find("CRS");

  if(it == it_end || it->second.empty())
    throw tws::core::http_request_error() << tws::error_description("Error on GetMap operation: \"CRS\" parameter is missing.");

  parameters.crs = it->second;

// retrieve bounding box
  it = qstr.find("BBOX");

  if(it == it_end || it->second.empty())
    throw tws::core::http_request_error() << tws::error_description("Error on GetMap operation: \"BBOX\" parameter is missing.");

  std::vector<std::string> str_bbox;

  boost::split(str_bbox, it->second, boost::is_any_of(","));

  if(str_bbox.size() != 4)
  {
    boost::format err_msg("Error on GetMap operation: invalid bounding box '%1%'.");
    throw tws::core::http_request_error() << tws::error_description((err_msg % it->second).str());
  }

  parameters.bbox.m_llx = std::stod(str_bbox[0]);
  parameters.bbox.m_lly = std::stod(str_bbox[1]);
  parameters.bbox.m_urx = std::stod(str_bbox[2]);
  parameters.bbox.m_ury = std::stod(str_bbox[3]);

// output image size
  it = qstr.find("WIDTH");

  if(it == it_end || it->second.empty())
    throw tws::core::http_request_error() << tws::error_description("Error on GetMap operation: \"WIDTH\" parameter is missing.");

  parameters.width = std::stoul(it->second);

  it = qstr.find("HEIGHT");

  if(it == it_end || it->second.empty())
    throw tws::core::http_request_error() << tws::error_description("Error on GetMap operation: \"HEIGHT\" parameter is missing.");

  parameters.height = std::stoul(it->second);

// output image format
  it = qstr.find("FORMAT");

  if(it == it_end || it->second.empty())
    throw tws::core::http_request_error() << tws::error_description("Error on GetMap operation: \"FORMAT\" parameter is missing.");

  parameters.format = it->second;

// retrieve time instant
  it = qstr.find("TIME");

  parameters.time_point = (it != it_end) ? it->second : std::string("");

  return parameters;
}

std::vector<tws::wms::layer_tuple_t>
tws::wms::valid(const get_map_request_parameters& parameters)
{
// check WMS version
  if(parameters.version != "1.3.0")
  {
    boost::format err_msg("Error on GetMap operation: invalid service version '%1%'.");
    throw tws::core::http_request_error() << tws::error_description((err_msg % parameters.version).str());
  }

// check if layer list is not empty
  if(parameters.layers.empty())
    throw tws::core::http_request_error() << tws::error_description("Error on GetMap operation: empty layer list.");

// artificial check: currently, we are accepting just one layer per request
  if(parameters.layers.size() > 1) // TODO: remove this restriction in near future!
    throw tws::core::http_request_error() << tws::error_description("Error on GetMap operation: this WMS implementation can only render one array per time.");

// check if style-list size is the same as layer-list size
  if(parameters.layers.size() != parameters.styles.size())
    throw tws::core::http_request_error() << tws::error_description("Error on GetMap operation: the requested layer list must have the same size as style list.");

// valid layer list and styles and prepare rendering data
  std::vector<tws::wms::layer_tuple_t> result;

  const capabilities_t& wms_capabilities = tws::wms::wms_manager::instance().capabilities();

  const layer_t& root_layer = wms_capabilities.capability.layer;

  for(std::size_t i = 0; i < parameters.layers.size(); ++i)
  {
// find requested layer in the server layer list
    const std::string& layer_name = parameters.layers[i];

    const auto& it_layer = std::find_if(root_layer.layers.begin(), root_layer.layers.end(), [&layer_name]
                                                                                            (const layer_t& l)
                                                                                            { return l.name == layer_name; });

    if(it_layer == root_layer.layers.end())
    {
      boost::format err_msg("Error on GetMap operation: could not find layer '%1%'.");
      throw tws::core::http_request_error() << tws::error_description((err_msg % layer_name).str());
    }

    const layer_t& current_layer = *it_layer;

// find the requested layer style
    const std::string& style_name = parameters.styles[i];

    const auto& it_style = std::find_if(current_layer.styles.begin(), current_layer.styles.end(), [&style_name]
                                                                                                  (const style_t& s)
                                                                                                  { return s.name == style_name; });

    if(it_style == current_layer.styles.end())
    {
      boost::format err_msg("Error on GetMap operation: could not find layer style named '%1%'.");
      throw tws::core::http_request_error() << tws::error_description((err_msg % style_name).str());
    }

    const auto& current_layer_style = *it_style;

// find layer's original geoarray and time-line info
    const tws::geoarray::timeline& tl = tws::geoarray::timeline_manager::instance().get(layer_name);

    const tws::geoarray::geoarray_t& geo_array = tws::geoarray::geoarray_manager::instance().get(layer_name);

// check if it is a 3D array
    if(geo_array.dimensions.size() != 3)
    {
      boost::format err_msg("Error on GetMap operation: layer '%1%' is not a spatio-temporal array.");
      throw tws::core::http_request_error() << tws::error_description((err_msg % layer_name).str());
    }

// create tuple info
    layer_tuple_t ltuple = std::make_tuple(&current_layer, &current_layer_style , &geo_array, &tl);

    result.push_back(ltuple);
  }

// check output image size
  if(parameters.width > wms_capabilities.service.max_width)
    throw tws::core::http_request_error() << tws::error_description("Error on GetMap operation: \"WIDTH\" parameter is out-of bounds.");

  if(parameters.height > wms_capabilities.service.max_height)
    throw tws::core::http_request_error() << tws::error_description("Error on GetMap operation: \"HEIGHT\" parameter is out-of bounds.");

// check requested output format
  if(std::find_if(wms_capabilities.capability.request.get_map.format.begin(),
                  wms_capabilities.capability.request.get_map.format.end(),
                  [&parameters](const std::string& f){ return (f == parameters.format); }) == wms_capabilities.capability.request.get_map.format.end())
  {
    boost::format err_msg("Error on GetMap operation: image output format '%1%' is not valid.");
    throw tws::core::http_request_error() << tws::error_description((err_msg % parameters.format).str());
  }

// ok: return rendering info computed during validation
  return result;
}

void
tws::wms::render(const layer_tuple_t& ltuple,
                 const get_map_request_parameters& parameters,
                 gdImagePtr img)
{
  const layer_t* layer = std::get<0>(ltuple);
  const style_t* style = std::get<1>(ltuple);
  const tws::geoarray::geoarray_t* garray = std::get<2>(ltuple);
  const tws::geoarray::timeline* tline = std::get<3>(ltuple);

// compute time-index
  //std::size_t time_idx = compute_time_index(garray, tline, parameters);
  std::size_t time_idx = parameters.time_point.empty() ? tline->index(tline->time_points().front()) : tline->index(parameters.time_point);

// get connection
  std::unique_ptr<tws::scidb::connection> conn(tws::scidb::connection_pool::instance().get());

// prepare the query string
  std::string str_aql = "SELECT " + style->colors[0] + " "
                      + "FROM between(" + layer->name + ", "
                                        + std::to_string(garray->dimensions[0].min_idx) + ","
                                        + std::to_string(garray->dimensions[1].min_idx) + ","
                                        + std::to_string(time_idx) + ","
                                        + std::to_string(garray->dimensions[0].min_idx + (parameters.width - 1)) + ","
                                        + std::to_string(garray->dimensions[1].min_idx + (parameters.height - 1)) + ","
                                        + std::to_string(time_idx)
                                        + ")";

// execute query
    boost::shared_ptr< ::scidb::QueryResult > qresult = conn->execute(str_aql, false);

    if((qresult.get() == nullptr) || (qresult->array.get() == nullptr))
    {
      boost::format err_msg("Error querying layer: '%1%'!");
      throw tws::core::http_request_error() << tws::error_description((err_msg % layer->name).str());
    }

    const ::scidb::ArrayDesc& array_desc = qresult->array->getArrayDesc();
    const ::scidb::Attributes& array_attributes = array_desc.getAttributes(true);
    const ::scidb::AttributeDesc& attr = array_attributes.front();

    std::shared_ptr<::scidb::ConstArrayIterator> array_it = qresult->array->getConstIterator(attr.getId());

    while(!array_it->end())
    {

      const ::scidb::ConstChunk& chunk = array_it->getChunk();

      std::shared_ptr< ::scidb::ConstChunkIterator > chunk_it = chunk.getConstIterator();

      while(!chunk_it->end())
      {
        const ::scidb::Value& v = chunk_it->getItem();

        const ::scidb::Coordinates& coords = chunk_it->getPosition();

        int color = v.getUint8();

        int gray = gdTrueColorAlpha(color, color, color, 0);

        int col = coords[0];
        int row = coords[1];
        //int t = coords[2];

        gdImageSetPixel(img, col, row, gray);

        ++(*chunk_it);
      }

      ++(*array_it);

    }


}

//std::size_t
//tws::wms::compute_time_index(const tws::geoarray::geoarray_t* geoarray,
//                             const tws::geoarray::timeline* tl,
//                             const get_map_request_parameters& parameters)
//{
//  std::size_t time_idx = parameters.time_point.empty() ? tl->index(tl->time_points().front()) : tl->index(parameters.time_point);

//  time_idx += geoarray->dimensions[2].min_idx;

//  return time_idx;
//}
