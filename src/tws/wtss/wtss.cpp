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
#include "../scidb/connection.hpp"
#include "../scidb/connection_pool.hpp"
#include "../scidb/utils.hpp"
#include "timeline.hpp"
#include "timeline_manager.hpp"
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
  const char* qstring = request.query_string();

  if(qstring == nullptr)
    throw tws::core::http_request_error() << tws::error_description("time_series operation requires the following parameters: \"coverage\", \"attributes\", \"latitude\", \"longitude\", \"start\", \"end\".");

  tws::core::query_string_t qstr = tws::core::expand(qstring);

// which coverage?
  tws::core::query_string_t::const_iterator it = qstr.find("coverage");
  tws::core::query_string_t::const_iterator it_end = qstr.end();

  if(it == it_end)
    throw tws::core::http_request_error() << tws::error_description("check time_series operation: \"coverage\" parameter is missing!");

// retrieve the coverage
  const tws::geoarray::geoarray_t& cv = tws::geoarray::geoarray_manager::instance().get(it->second);

// which attributes
  it = qstr.find("attributes");

  if(it == it_end)
  {
    boost::format err_ms("check time_series operation: \"attributes\" parameter is missing!");

    throw tws::core::http_request_error() << tws::error_description((err_ms % cv.name).str());
  }

  std::vector<std::string> queried_attributes;

  boost::split(queried_attributes, it->second, boost::is_any_of(","));

  if(queried_attributes.empty())
  {
    boost::format err_ms("check time_series operation for coverage '%1%': please, inform at least one attribute.");

    throw tws::core::http_request_error() << tws::error_description((err_ms % cv.name).str());
  }

  // check if attributes are valid
  for(const std::string& attr_name : queried_attributes)
  {
      std::vector<tws::geoarray::attribute_t>::const_iterator it = std::find_if(cv.attributes.begin(),
                                                                                cv.attributes.end(),
                                                                                [&attr_name](const tws::geoarray::attribute_t& attr){ return (attr.name == attr_name); });

      if(it == cv.attributes.end())
      {
        boost::format err_ms("attribute '%1%' doesn't belong to coverage '%2%'!");
        throw tws::core::http_request_error() << tws::error_description((err_ms % attr_name % cv.name).str());
      }

  }

  // extract longitude
  it = qstr.find("longitude");

  if(it == it_end || it->second.empty())
    throw tws::core::http_request_error() << tws::error_description("check time_series operation: \"longitude\" parameter is missing!");

  const double longitude = boost::lexical_cast<double>(it->second);

  // extract latitude
  it = qstr.find("latitude");

  if(it == it_end || it->second.empty())
    throw tws::core::http_request_error() << tws::error_description("check time_series operation: \"latitude\" parameter is missing!");

  const double latitude = boost::lexical_cast<double>(it->second);

// extract start and end times if any
  it = qstr.find("start");

  const std::string start_time = (it != it_end) ? it->second : std::string("");

  const timeline& tl = timeline_manager::instance().get(cv.name);

  std::size_t start_time_idx = start_time.empty() ? tl.index(tl.time_points().front()) : tl.index(start_time);

  it = qstr.find("end");

  const std::string end_time = (it != it_end) ? it->second : std::string("");

  std::size_t end_time_idx = end_time.empty() ? tl.index(tl.time_points().back()) : tl.index(end_time);

  if(end_time_idx < start_time_idx)
    throw tws::core::http_request_error() << tws::error_description("invalid time range!");

  std::size_t ntime_pts = end_time_idx - start_time_idx + 1;

// benchmark
  //std::chrono::time_point<std::chrono::steady_clock> start, end;

  //start = std::chrono::steady_clock::now();

// TODO: check time interval and get a valid time range

// prepare SRS conversor that allows to go from lat/long to array projection system and then come back to lat/long
  te::srs::Converter srs_conv(4326, cv.geo_extent.spatial.crs_code);

  double x = 0.0;
  double y = 0.0;

  srs_conv.convert(longitude, latitude, x, y); // degrees to radians

// TODO: check if x and y values are within coverage boundary

// compute pixel location from input Lat/Long WGS84 coordinate
  te::rst::Grid array_grid(cv.dimensions[0].max_idx - cv.dimensions[0].min_idx + 1,
                           cv.dimensions[1].max_idx - cv.dimensions[1].min_idx + 1,
                           cv.geo_extent.spatial.resolution.x, cv.geo_extent.spatial.resolution.y,
                           new te::gm::Envelope(cv.geo_extent.spatial.extent.xmin, cv.geo_extent.spatial.extent.ymin,
                                                cv.geo_extent.spatial.extent.xmax, cv.geo_extent.spatial.extent.ymax),
                           cv.geo_extent.spatial.crs_code);

  double dpixel_col = 0.0;
  double dpixel_row = 0.0;

  array_grid.geoToGrid(x, y, dpixel_col, dpixel_row);

  int64_t pixel_col = static_cast<int64_t>(dpixel_col);
  int64_t pixel_row = static_cast<int64_t>(dpixel_row);

// TODO: check if row and col are within array dimension ranges!

  // then compute the location of the center of the pixel
  array_grid.gridToGeo(pixel_col, pixel_row, x, y);

  // get back from sinu to lat/long
  srs_conv.invert(x, y, x, y);

  //end = std::chrono::steady_clock::now();

  //std::chrono::duration<double> elapsed_time = end - start;

  //std::cout << "\n\tSRS conversion time: " << elapsed_time.count() << "s" << std::endl;

  //start = std::chrono::steady_clock::now();

  // get a connection from the pool in order to retrieve the time series data
  std::unique_ptr<tws::scidb::connection> conn(tws::scidb::connection_pool::instance().get());

  //end = std::chrono::steady_clock::now();

  //elapsed_time = end - start;

  //std::cout << "\tRetrieving a database connection: " << elapsed_time.count() << "s" << std::endl;

  // prepare the JSON root document
  rapidjson::Document::AllocatorType allocator;

  rapidjson::Value jattributes(rapidjson::kArrayType);

// iterate through each queried attribute
  for(const auto& attr_name : queried_attributes)
  {
// TODO: fix the query string when we have the time range
    std::string str_afl = "project( between(" + cv.name + ", "
                        + std::to_string(pixel_col) + "," + std::to_string(pixel_row) + "," + std::to_string(start_time_idx) + ","
                        + std::to_string(pixel_col) + "," + std::to_string(pixel_row) + "," + std::to_string(end_time_idx) + "), "
                        + attr_name + ")";

    //start = std::chrono::steady_clock::now();

    boost::shared_ptr< ::scidb::QueryResult > qresult = conn->execute(str_afl, true);

    //end = std::chrono::steady_clock::now();

    //elapsed_time = end - start;

    //std::cout << "\tQuery: " << str_afl << "; executed in " << elapsed_time.count() << "s" << std::endl;

    //start = std::chrono::steady_clock::now();

    if((qresult.get() == nullptr) || (qresult->array.get() == nullptr))
    {
      rapidjson::Value jattribute(rapidjson::kObjectType);

      jattribute.AddMember("attribute", attr_name.c_str(), allocator);

      rapidjson::Value jvalues(rapidjson::kArrayType);

      jattribute.AddMember("values", jvalues, allocator);

      jattributes.PushBack(jattribute, allocator);

      continue; // no query result returned after querying database.
    }

    std::vector<double> values;
    values.reserve(ntime_pts);

    const ::scidb::ArrayDesc& array_desc = qresult->array->getArrayDesc();
    const ::scidb::Attributes& array_attributes = array_desc.getAttributes(true);
    const ::scidb::AttributeDesc& attr = array_attributes.front();

    std::shared_ptr< ::scidb::ConstArrayIterator > array_it = qresult->array->getConstIterator(attr.getId());

    tws::scidb::fill(values, array_it.get(), attr.getType());

    //end = std::chrono::steady_clock::now();

    //elapsed_time = end - start;

    //std::cout << "\tTraversing array in " << elapsed_time.count() << "s" << std::endl;

    if(values.size() != ntime_pts)
    {
      boost::format err_msg("error retrieving time series for geoarray '%1%', attribute '%2%': number of expected values was '%3%', found '%4%'!");
      throw tws::core::http_request_error() << tws::error_description((err_msg % cv.name % attr_name % ntime_pts % values.size()).str());
    }

    rapidjson::Value jattribute(rapidjson::kObjectType);

    jattribute.AddMember("attribute", attr_name.c_str(), allocator);

    rapidjson::Value jvalues(rapidjson::kArrayType);

    tws::core::copy_numeric_array(values.begin(), values.end(), jvalues, allocator);

    jattribute.AddMember("values", jvalues, allocator);

    jattributes.PushBack(jattribute, allocator);
  }

// prepare result part in response
  rapidjson::Value jresult(rapidjson::kObjectType);

  jresult.AddMember("attributes", jattributes, allocator);

// add timeline in the response
  rapidjson::Value jtimeline(rapidjson::kArrayType);
  tws::core::copy_string_array(std::begin(tl.time_points()) + start_time_idx,  std::begin(tl.time_points()) + (start_time_idx + ntime_pts),
                               jtimeline, allocator);
  jresult.AddMember("timeline", jtimeline, allocator);

// add the pixel center location in response
  rapidjson::Value jcenter(rapidjson::kObjectType);
  jcenter.AddMember("latitude", y, allocator);
  jcenter.AddMember("longitude", x, allocator);
  jresult.AddMember("center_coordinates", jcenter, allocator);

// prepare the query part in response
  rapidjson::Value jquery(rapidjson::kObjectType);

  jquery.AddMember("coverage", cv.name.c_str(), allocator);

  rapidjson::Value jqattributes(rapidjson::kArrayType);

  tws::core::copy_string_array(queried_attributes.begin(), queried_attributes.end(), jqattributes, allocator);

  jquery.AddMember("attributes", jqattributes, allocator);

  jquery.AddMember("latitude", latitude, allocator);

  jquery.AddMember("longitude", longitude, allocator);

// form the final response
  rapidjson::Document doc;

  doc.SetObject();

  doc.AddMember("result", jresult, allocator);
  doc.AddMember("query", jquery, allocator);

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
  std::string timelines = tws::core::find_in_app_path("share/tws/config/timelines.json");

  if(timelines.empty())
    throw  tws::file_exists_error() << tws::error_description("could not locate file: 'share/tws/config/wtss_timelines.json'.");

  std::vector<std::pair<std::string, std::string> > timelines_files = tws::wtss::read_timelines_file_name(timelines);

  for(const auto& tf : timelines_files)
  {
    std::string input_file = tws::core::find_in_app_path("share/tws/config/" + tf.second);

    if(input_file.empty())
    {
      boost::format err_msg("timeline file: '%1%', not found for array '%2%'.");

      throw tws::file_exists_error() << tws::error_description((err_msg % tf.second % tf.first).str());
    }

    std::vector<std::string> str_timeline = tws::wtss::read_timeline(input_file);

    timeline t(str_timeline);

    tws::wtss::timeline_manager::instance().insert(tf.first, t);
  }
}
