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
  \file tws/wtss/wtss.cpp

  \brief Web Time Series Data Service.

  \author Gilberto Ribeiro de Queiroz
 */

// TWS
#include "wtss.hpp"
#include "../core/http_request.hpp"
#include "../core/http_response.hpp"
#include "../core/service_operations_manager.hpp"
#include "../core/utils.hpp"
#include "../geoarray/geoarray_manager.hpp"
#include "../geoarray/timeline.hpp"
#include "../geoarray/timeline_manager.hpp"
#include "../geoarray/utils.hpp"
#include "../scidb/connection.hpp"
#include "../scidb/connection_pool.hpp"
#include "../scidb/utils.hpp"
#include "utils.hpp"

// STL
#include <algorithm>
//#include <chrono>
//#include <iostream>
#include <iterator>
#include <memory>
#include <string>

// Boost
#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/filesystem.hpp>
#include <boost/format.hpp>
#include <boost/lexical_cast.hpp>


// SciDB
#include <SciDBAPI.h>

// RapidJSON
#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>

// TerraLib
#include <terralib/geometry/Envelope.h>
#include <terralib/raster/Grid.h>
#include <terralib/srs/Converter.h>

namespace tws
{
  namespace wtss
  {
    struct timeseries_request_parameters
    {
      std::string cv_name;
      std::vector<std::string> queried_attributes;
      double longitude;
      double latitude;
      std::string start_time_point;
      std::string end_time_point;
    };

    struct timeseries_validated_parameters
    {
      std::vector<std::size_t> attribute_positions;
      const tws::geoarray::geoarray_t* geo_array;
      const tws::geoarray::timeline* timeline;
      std::size_t start_time_idx;
      std::size_t end_time_idx;
      int64_t pixel_col;
      int64_t pixel_row;
      double pixel_center_longitude;
      double pixel_center_latitude;
    };

    timeseries_request_parameters
    decode_timeseries_request(const tws::core::query_string_t& qstr);

    timeseries_validated_parameters
    valid(const timeseries_request_parameters& parameters);

    void
    compute_time_series(const timeseries_request_parameters& parameters,
                        const timeseries_validated_parameters& vparameters,
                        rapidjson::Document::AllocatorType& allocator,
                        rapidjson::Value& jattributes);

    void
    prepare_timeseries_response(const timeseries_request_parameters& parameters,
                                const timeseries_validated_parameters& vparameters,
                                rapidjson::Document& doc,
                                rapidjson::Value& jattributes,
                                rapidjson::Document::AllocatorType& allocator);

  }  // end namespace wtss
}    // end namespace tws

void
tws::wtss::list_coverages_functor::operator()(const tws::core::http_request& request,
                                              tws::core::http_response& response)
{
// retrieve the list of registered geo-arrays
  std::vector<std::string> arrays = tws::geoarray::geoarray_manager::instance().list_arrays();

// output result
  rapidjson::Document::AllocatorType allocator;

  rapidjson::Document doc;

  doc.SetObject();

  rapidjson::Value jarrays(rapidjson::kArrayType);

  tws::core::copy_string_array(arrays.begin(), arrays.end(), jarrays, allocator);

  doc.AddMember("coverages", jarrays, allocator);

  rapidjson::StringBuffer str_buff;

  rapidjson::Writer<rapidjson::StringBuffer> writer(str_buff);

  doc.Accept(writer);

  const char* p_str_buff = str_buff.GetString();

  response.add_header("Content-Type", "application/json");
  response.add_header("Access-Control-Allow-Origin", "*");
  response.set_content(p_str_buff, str_buff.Size());
}

void
tws::wtss::describe_coverage_functor::operator()(const tws::core::http_request& request,
                                                 tws::core::http_response& response)
{
  const char* qstring = request.query_string();

  if(qstring == nullptr)
    throw tws::core::http_request_error() << tws::error_description("describe_coverage operation requires the parameter: \"name\".");

  tws::core::query_string_t qstr = tws::core::expand(qstring);

// which coverage?
  tws::core::query_string_t::const_iterator it = qstr.find("name");
  tws::core::query_string_t::const_iterator it_end = qstr.end();

  if(it == it_end)
    throw tws::core::http_request_error() << tws::error_description("check describe_coverage operation: \"name\" parameter is missing!");

// retrieve the coverage
  const tws::geoarray::geoarray_t& cv = tws::geoarray::geoarray_manager::instance().get(it->second);

// output result: JSON document
  rapidjson::Document::AllocatorType allocator;

  rapidjson::Document doc;

  doc.SetObject();

  write(cv, doc, allocator);

  rapidjson::StringBuffer str_buff;

  rapidjson::Writer<rapidjson::StringBuffer> writer(str_buff);

  doc.Accept(writer);

  const char* p_str_buff = str_buff.GetString();

  response.add_header("Content-Type", "application/json");
  response.add_header("Access-Control-Allow-Origin", "*");
  response.set_content(p_str_buff, str_buff.Size());
}

void
tws::wtss::time_series_functor::operator()(const tws::core::http_request& request,
                                           tws::core::http_response& response)
{
// get client query string
  const char* qstring = request.query_string();

  if(qstring == nullptr)
    throw tws::core::http_request_error() << tws::error_description("time_series operation requires the following parameters: \"coverage\", \"attributes\", \"latitude\", \"longitude\", \"start\", \"end\".");

// parse plain text query string to a std::map
  tws::core::query_string_t qstr = tws::core::expand(qstring);

// parse parameters to a struct
  timeseries_request_parameters parameters = tws::wtss::decode_timeseries_request(qstr);

// valid parameters
  timeseries_validated_parameters vparameters = valid(parameters);

// prepare the JSON root document
  rapidjson::Document::AllocatorType allocator;

  rapidjson::Value jattributes(rapidjson::kArrayType);

// compute timeseries for queried coverage attributes
  compute_time_series(parameters, vparameters, allocator, jattributes);

// prepare the return document
  rapidjson::Document doc;

  doc.SetObject();

  prepare_timeseries_response(parameters, vparameters, doc, jattributes, allocator);

// send response
  rapidjson::StringBuffer str_buff;

  rapidjson::Writer<rapidjson::StringBuffer> writer(str_buff);

  doc.Accept(writer);

  const char* p_str_buff = str_buff.GetString();

  //response.add_header("Content-Length", boost::lexical_cast<std::string>(content.size()).c_str());
  response.add_header("Content-Type", "application/json");
  response.add_header("Access-Control-Allow-Origin", "*");
  response.set_content(p_str_buff, str_buff.Size());
}

void
tws::wtss::register_operations()
{
  tws::core::service_metadata service;

  service.name = "wtss";

// 1st WTSS operation: list available coverages
  {
    tws::core::service_operation s_op;

    s_op.name = "list_coverages";
    s_op.description = "List all coverages from remote sensing data products available in the server";
    s_op.handler = list_coverages_functor();

    service.operations.push_back(s_op);
  }

// 2nd WTSS operation: get metadata about a specific coverage
  {
    tws::core::service_operation s_op;

    s_op.name = "describe_coverage";
    s_op.description = "Retrieve the metadata of a specific coverage";
    s_op.handler = describe_coverage_functor();

    service.operations.push_back(s_op);
  }

// 3rd WTSS operation: time_series retrieval operation
  {
    tws::core::service_operation s_op;

    s_op.name = "time_series";
    s_op.description = "Retrieve the time series for a given location in a particular coverage";
    s_op.handler = time_series_functor();

    service.operations.push_back(s_op);
  }

  tws::core::service_operations_manager::instance().insert(service);
}

void
tws::wtss::initialize_operations()
{
  tws::geoarray::geoarray_manager::instance();
  tws::geoarray::timeline_manager::instance();
}

tws::wtss::timeseries_request_parameters
tws::wtss::decode_timeseries_request(const tws::core::query_string_t& qstr)
{
  timeseries_request_parameters parameters;

// get coverage name
  tws::core::query_string_t::const_iterator it = qstr.find("coverage");
  tws::core::query_string_t::const_iterator it_end = qstr.end();

  if(it == it_end)
    throw tws::core::http_request_error() << tws::error_description("Error on time_series operation: \"coverage\" parameter is missing.");

  parameters.cv_name = it->second;

// get queried attributes
  it = qstr.find("attributes");

  if(it == it_end)
  {
    boost::format err_msg("Error on time_series operation: \"attributes\" parameter is missing for coverage '%1%'.");

    throw tws::core::http_request_error() << tws::error_description((err_msg % parameters.cv_name).str());
  }

  boost::split(parameters.queried_attributes, it->second, boost::is_any_of(","));

// extract longitude
  it = qstr.find("longitude");

  if(it == it_end || it->second.empty())
    throw tws::core::http_request_error() << tws::error_description("Error on time_series operation: \"longitude\" parameter is missing.");

  parameters.longitude = boost::lexical_cast<double>(it->second);

// extract latitude
  it = qstr.find("latitude");

  if(it == it_end || it->second.empty())
    throw tws::core::http_request_error() << tws::error_description("check time_series operation: \"latitude\" parameter is missing.");

  parameters.latitude = boost::lexical_cast<double>(it->second);

// extract start and end times if any
  it = qstr.find("start");

  parameters.start_time_point = (it != it_end) ? it->second : std::string("");

  it = qstr.find("end");

  parameters.end_time_point = (it != it_end) ? it->second : std::string("");

// ok: finished extracting parameters
  return parameters;
}

tws::wtss::timeseries_validated_parameters
tws::wtss::valid(const timeseries_request_parameters& parameters)
{
  timeseries_validated_parameters vparameters;

// retrieve the underlying geoarray
  vparameters.geo_array = & (tws::geoarray::geoarray_manager::instance().get(parameters.cv_name));

// valid queried attributes
  if(parameters.queried_attributes.empty())
  {
    boost::format err_msg("Error on time_series operation: please, inform at least one attribute coverage '%1%'.");

    throw tws::core::http_request_error() << tws::error_description((err_msg % parameters.cv_name).str());
  }

  for(const std::string& attr_name : parameters.queried_attributes)
  {
    const std::vector<tws::geoarray::attribute_t>::const_iterator it = std::find_if(vparameters.geo_array->attributes.begin(),
                                                                       vparameters.geo_array->attributes.end(),
                                                                       [&attr_name](const tws::geoarray::attribute_t& attr){ return (attr.name == attr_name); });

    const std::vector<tws::geoarray::attribute_t>::const_iterator it_end = vparameters.geo_array->attributes.end();

    if(it == it_end)
    {
      boost::format err_msg("Error on time_series operation: attribute '%1%' doesn't belong to coverage '%2%'.");
      throw tws::core::http_request_error() << tws::error_description((err_msg % attr_name % parameters.cv_name).str());
    }

    std::size_t pos = std::distance(it, it_end);

    vparameters.attribute_positions.push_back(pos);
  }

// valid queried time-interval
  vparameters.timeline = & (tws::geoarray::timeline_manager::instance().get(parameters.cv_name));

  vparameters.start_time_idx = parameters.start_time_point.empty() ? vparameters.timeline->index(vparameters.timeline->time_points().front()) : vparameters.timeline->index(parameters.start_time_point);

  vparameters.end_time_idx = parameters.end_time_point.empty() ? vparameters.timeline->index(vparameters.timeline->time_points().back()) : vparameters.timeline->index(parameters.end_time_point);

  if(vparameters.end_time_idx < vparameters.start_time_idx)
  {
    boost::format err_msg("Error on time_series operation: invalid time range [%1%, %2%].");

    throw tws::core::http_request_error() << tws::error_description((err_msg % parameters.start_time_point % parameters.end_time_point).str());
  }

// prepare SRS conversor that allows to go from lat/long to array projection system and then come back to lat/long
// TODO: do this only if a transformation is needed!
//  if(vparameters.geo_array->geo_extent.spatial.crs_code != 4326)

  te::srs::Converter srs_conv(4326, vparameters.geo_array->geo_extent.spatial.crs_code);

  srs_conv.convert(parameters.longitude, parameters.latitude, vparameters.pixel_center_longitude, vparameters.pixel_center_latitude);

// check if x and y values are within coverage boundary
  if(!intersects(vparameters.pixel_center_longitude, vparameters.pixel_center_latitude, vparameters.geo_array->geo_extent.spatial.extent))
  {
    boost::format err_msg("Error on time_series operation: queried longitude '%1%' or latitude '%2%' is outof coverage '%3%' extent (%4%, %5%, %6%, %7%).");

    throw tws::core::http_request_error() << tws::error_description((err_msg % parameters.longitude % parameters.latitude % parameters.cv_name
                                                                     % vparameters.geo_array->geo_extent.spatial.extent.xmin
                                                                     % vparameters.geo_array->geo_extent.spatial.extent.xmax
                                                                     % vparameters.geo_array->geo_extent.spatial.extent.ymin
                                                                     % vparameters.geo_array->geo_extent.spatial.extent.ymax).str());
  }

// compute pixel location from input Lat/Long WGS84 coordinate
  te::rst::Grid array_grid(vparameters.geo_array->dimensions[0].max_idx - vparameters.geo_array->dimensions[0].min_idx + 1,
                           vparameters.geo_array->dimensions[1].max_idx - vparameters.geo_array->dimensions[1].min_idx + 1,
                           vparameters.geo_array->geo_extent.spatial.resolution.x,
                           vparameters.geo_array->geo_extent.spatial.resolution.y,
                           new te::gm::Envelope(vparameters.geo_array->geo_extent.spatial.extent.xmin,
                                                vparameters.geo_array->geo_extent.spatial.extent.ymin,
                                                vparameters.geo_array->geo_extent.spatial.extent.xmax,
                                                vparameters.geo_array->geo_extent.spatial.extent.ymax),
                           vparameters.geo_array->geo_extent.spatial.crs_code);

  double dpixel_col = 0.0;
  double dpixel_row = 0.0;

  array_grid.geoToGrid(vparameters.pixel_center_longitude, vparameters.pixel_center_latitude, dpixel_col, dpixel_row);

  vparameters.pixel_col = static_cast<int64_t>(dpixel_col);
  vparameters.pixel_row = static_cast<int64_t>(dpixel_row);

// check if row and col are within array dimension ranges!
  if(!is_in_spatial_range(vparameters.pixel_col, vparameters.pixel_row, vparameters.geo_array->dimensions))
  {
    boost::format err_msg("Error on timeseries operation: queried \"col\" (%1%) or \"row\" (%2%) are not within the array dimension ranges col=[%3%,%4%] or row=[%5%, %6%].");

    throw tws::core::http_request_error() << tws::error_description((err_msg % vparameters.pixel_col % vparameters.pixel_row
                                                                     % vparameters.geo_array->dimensions[0].min_idx
                                                                     % vparameters.geo_array->dimensions[0].max_idx
                                                                     % vparameters.geo_array->dimensions[1].min_idx
                                                                     % vparameters.geo_array->dimensions[1].max_idx).str());
  }

// then compute the location of the center of the pixel
  array_grid.gridToGeo(vparameters.pixel_col, vparameters.pixel_row, vparameters.pixel_center_longitude, vparameters.pixel_center_latitude);

// get back from sinu (or any other CRS) to lat/long
  srs_conv.invert(vparameters.pixel_center_longitude, vparameters.pixel_center_latitude, vparameters.pixel_center_longitude, vparameters.pixel_center_latitude);

  return vparameters;
}

void
tws::wtss::compute_time_series(const timeseries_request_parameters& parameters,
                               const timeseries_validated_parameters& vparameters,
                               rapidjson::Document::AllocatorType& allocator,
                               rapidjson::Value& jattributes)
{
  const std::size_t ntime_pts = vparameters.end_time_idx - vparameters.start_time_idx + 1;

  const std::size_t nattributes = parameters.queried_attributes.size();

// get a connection from the pool in order to retrieve the time series data
  std::unique_ptr<tws::scidb::connection> conn(tws::scidb::connection_pool::instance().get());

// iterate through each queried attribute
  for(std::size_t i = 0; i != nattributes; ++i)
  {
    const auto& attr_name = parameters.queried_attributes[i];

    const std::size_t& attr_pos = vparameters.attribute_positions[i];

// the scidb query string
    std::string str_afl = "project( between(" + parameters.cv_name + ", "
                          + std::to_string(vparameters.pixel_col) + "," + std::to_string(vparameters.pixel_row) + "," + std::to_string(vparameters.start_time_idx) + ","
                          + std::to_string(vparameters.pixel_col) + "," + std::to_string(vparameters.pixel_row) + "," + std::to_string(vparameters.end_time_idx) + "), "
                          + attr_name + ")";

    boost::shared_ptr< ::scidb::QueryResult > qresult = conn->execute(str_afl, true);

    if((qresult.get() == nullptr) || (qresult->array.get() == nullptr))
    {
      rapidjson::Value jattribute(rapidjson::kObjectType);

      jattribute.AddMember("attribute", attr_name.c_str(), allocator);

      rapidjson::Value jvalues(rapidjson::kArrayType);

      jattribute.AddMember("values", jvalues, allocator);

      jattributes.PushBack(jattribute, allocator);

      continue; // no query result returned after querying database.
    }

    std::vector<double> values(ntime_pts, vparameters.geo_array->attributes[attr_pos].missing_value);

    const ::scidb::ArrayDesc& array_desc = qresult->array->getArrayDesc();
    const ::scidb::Attributes& array_attributes = array_desc.getAttributes(true);
    const ::scidb::AttributeDesc& attr = array_attributes.front();

    std::shared_ptr< ::scidb::ConstArrayIterator > array_it = qresult->array->getConstIterator(attr.getId());

    fill_time_series(values, ntime_pts, array_it.get(), attr.getType(), 2, -(vparameters.start_time_idx));

    rapidjson::Value jattribute(rapidjson::kObjectType);

    jattribute.AddMember("attribute", attr_name.c_str(), allocator);

    rapidjson::Value jvalues(rapidjson::kArrayType);

    tws::core::copy_numeric_array(values.begin(), values.end(), jvalues, allocator);

    jattribute.AddMember("values", jvalues, allocator);

    jattributes.PushBack(jattribute, allocator);
  }
}

void
tws::wtss::prepare_timeseries_response(const timeseries_request_parameters& parameters,
                                       const timeseries_validated_parameters& vparameters,
                                       rapidjson::Document& doc,
                                       rapidjson::Value& jattributes,
                                       rapidjson::Document::AllocatorType& allocator)
{
// prepare result part in response
  rapidjson::Value jresult(rapidjson::kObjectType);

  jresult.AddMember("attributes", jattributes, allocator);

// add timeline in the response
  rapidjson::Value jtimeline(rapidjson::kArrayType);

  std::size_t init_pos = vparameters.timeline->pos(vparameters.start_time_idx);
  std::size_t fin_pos = vparameters.timeline->pos(vparameters.end_time_idx);

  tws::core::copy_string_array(std::begin(vparameters.timeline->time_points()) + init_pos,
                               std::begin(vparameters.timeline->time_points()) + (fin_pos + 1),
                               jtimeline, allocator);

  jresult.AddMember("timeline", jtimeline, allocator);

// add the pixel center location in response
  rapidjson::Value jcenter(rapidjson::kObjectType);
  jcenter.AddMember("latitude", vparameters.pixel_center_latitude, allocator);
  jcenter.AddMember("longitude", vparameters.pixel_center_longitude, allocator);
  jresult.AddMember("center_coordinates", jcenter, allocator);

// prepare the query part in response
  rapidjson::Value jquery(rapidjson::kObjectType);

  jquery.AddMember("coverage", parameters.cv_name.c_str(), allocator);

  rapidjson::Value jqattributes(rapidjson::kArrayType);

  tws::core::copy_string_array(parameters.queried_attributes.begin(), parameters.queried_attributes.end(), jqattributes, allocator);

  jquery.AddMember("attributes", jqattributes, allocator);

  jquery.AddMember("latitude", parameters.latitude, allocator);

  jquery.AddMember("longitude", parameters.longitude, allocator);

  doc.AddMember("result", jresult, allocator);
  doc.AddMember("query", jquery, allocator);
}
