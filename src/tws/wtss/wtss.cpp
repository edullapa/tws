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
#include "utils.hpp"


// STL
#include <algorithm>
#include <memory>

// Boost
#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>
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
//
//  const char* qstring = request.query_string();
//
//  if(qstring == nullptr)
//    throw tws::core::http_request_error() << tws::error_description("time_series operation requires the following parameters: \"coverage\", \"attributes\", \"latitude\", \"longitude\", \"start\", \"end\".");
//
//  tws::core::query_string_t qstr = tws::core::expand(qstring);
//
//// which coverage?
//  tws::core::query_string_t::const_iterator it = qstr.find("coverage");
//  tws::core::query_string_t::const_iterator it_end = qstr.end();
//
//  if(it == it_end)
//    throw tws::core::http_request_error() << tws::error_description("check time_series operation: \"coverage\" parameter is missing!");
//
//  const std::string coverage_name = it->second;
//
//// which attributes
//  it = qstr.find("attributes");
//
//  if(it == it_end)
//  {
//    boost::format err_ms("check time_series operation: \"attributes\" parameter is missing!");
//
//    throw tws::core::http_request_error() << tws::error_description((err_ms % coverage_name).str());
//  }
//
//  std::vector<std::string> queried_attributes;
//
//  boost::split(queried_attributes, it->second, boost::is_any_of(","));
//
//  if(queried_attributes.empty())
//  {
//    boost::format err_ms("check time_series operation for coverage '%1%': please, inform at least one attribute.");
//
//    throw tws::core::http_request_error() << tws::error_description((err_ms % coverage_name).str());
//  }
//
//// extract longitude
//  it = qstr.find("longitude");
//
//  if(it == it_end || it->second.empty())
//    throw tws::core::http_request_error() << tws::error_description("check time_series operation: \"longitude\" parameter is missing!");
//
//  const double longitude = boost::lexical_cast<double>(it->second);
//
//// extract latitude
//  it = qstr.find("latitude");
//
//  if(it == it_end || it->second.empty())
//    throw tws::core::http_request_error() << tws::error_description("check time_series operation: \"latitude\" parameter is missing!");
//
//  const double latitude = boost::lexical_cast<double>(it->second);
//
//// extract start and end times if any
//  it = qstr.find("start");
//
//  const std::string start_time = (it != it_end) ? it->second : std::string("");
//
//  it = qstr.find("end");
//
//  const std::string end_time = (it != it_end) ? it->second : std::string("");
//
//// get coverage definition
//  const coverage_t& coverage = coverage_manager::instance().get(coverage_name);
//
//// prepare SRS conversor that allows to go from lat/long to array projection system and then come back to lat/long
//  te::srs::Converter srs_conv(4326, coverage.geo_extent.spatial.crs_code);
//
//  double x = longitude;
//  double y = latitude;
//
//  srs_conv.convert(longitude, latitude, x, y); // degrees to radians
//
//// compute pixel location from input Lat/Long WGS84 coordinate
//  te::rst::Grid array_grid(coverage.dimensions[0].max_idx - coverage.dimensions[0].min_idx + 1,
//                           coverage.dimensions[1].max_idx - coverage.dimensions[1].min_idx + 1,
//                           coverage.geo_extent.spatial.resolution.x, coverage.geo_extent.spatial.resolution.y,
//                           new te::gm::Envelope(coverage.geo_extent.spatial.extent.xmin, coverage.geo_extent.spatial.extent.ymin,
//                                                coverage.geo_extent.spatial.extent.xmax, coverage.geo_extent.spatial.extent.ymax),
//                           coverage.geo_extent.spatial.crs_code);
//
//  double dpixel_col = 0.0;
//  double dpixel_row = 0.0;
//
//  array_grid.geoToGrid(x, y, dpixel_col, dpixel_row);
//
//  int64_t pixel_col = static_cast<int64_t>(dpixel_col);
//  int64_t pixel_row = static_cast<int64_t>(dpixel_row);
//
//// then compute the location of the center of the pixel
//  array_grid.gridToGeo(pixel_col, pixel_row, x, y);
//
//// get back from sinu to lat/long
//  srs_conv.invert(x, y, x, y);
//
//// find array timeline strategy and retrieve its timeline
//  const std::vector<std::string>& timeline = timeline_manager::instance().get(coverage.name);
//
//  std::size_t ntime_pts = timeline.size();
//
//  if(ntime_pts == 0)
//  {
//    boost::format err_msg("could not retrieve timeline for coverage: %1%");
//
//    throw tws::core::http_request_error() << tws::error_description((err_msg % coverage_name).str());
//  }
//
//// locate start and end time
//  std::size_t start_time_pos = 0;
//
//  if(!start_time.empty())
//  {
//    std::vector<std::string>::const_iterator it_timeline =  std::lower_bound(timeline.begin(), timeline.end(), start_time);
//
//    if(it_timeline != timeline.end())
//      start_time_pos = it_timeline - timeline.begin();
//  }
//
//  std::size_t end_time_pos = ntime_pts - 1;
//
//  if(!end_time.empty())
//  {
//    std::vector<std::string>::const_iterator it_timeline =  std::lower_bound(timeline.begin(), timeline.end(), end_time);
//
//    if(it_timeline != timeline.end())
//      end_time_pos = it_timeline - timeline.begin();
//  }
//
//  ntime_pts = end_time_pos - start_time_pos + 1;
//
////// prepare expressions for SELECT clause
////  std::string attrs;
////
////  for(std::size_t i = 0; i != queried_attributes.size(); ++i)
////  {
////    std::string attr_exp = "";
////
////    for(std::size_t j = 0; j != coverage.attributes.size(); ++j)
////    {
////      if(queried_attributes[i] == coverage.attributes[j].name)
////      {
////        attr_exp = coverage.attributes[j].expression;
////        break;
////      }
////    }
////
////    if(attr_exp.empty())
////    {
////      boost::format err_msg("invalid coverage attribute: %1%");
////
////      throw tws::core::http_request_error() << tws::error_description((err_msg % queried_attributes[i]).str());
////    }
////
////    if(attrs.empty())
////      attrs = attr_exp;
////    else
////      attrs += "," + attr_exp;
////  }
////
////// prepare query
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
//
//// iterate through each attribute and add it to the result
//  rapidjson::Document::AllocatorType allocator;
//
//  rapidjson::Value jattributes(rapidjson::kArrayType);
//
//  for(std::size_t i = 0; i != nattributes; ++i)
//  {
//// find attribute expression
//    std::string attr_exp = "";
//
//    for(std::size_t j = 0; j != coverage.attributes.size(); ++j)
//    {
//      if(queried_attributes[i] == coverage.attributes[j].name)
//      {
//        attr_exp = coverage.attributes[j].expression;
//        break;
//      }
//    }
//
//    if(attr_exp.empty())
//    {
//      boost::format err_msg("invalid coverage attribute: %1%");
//
//      throw tws::core::http_request_error() << tws::error_description((err_msg % queried_attributes[i]).str());
//    }
//
//// prepare query for attribute 'i'
//    std::string str_q_aql("SELECT " + attr_exp + " FROM " + coverage.from_clause);
//
//    if(!coverage.where_clause.empty())
//      str_q_aql += " WHERE " + coverage.where_clause;
//
//    boost::format q_aql(str_q_aql);
//    q_aql.bind_arg(1, pixel_col);
//    q_aql.bind_arg(2, pixel_row);
//    q_aql.bind_arg(3, start_time_pos);
//    q_aql.bind_arg(4, pixel_col);
//    q_aql.bind_arg(5, pixel_row);
//    q_aql.bind_arg(6, end_time_pos);
//
//    boost::shared_ptr< ::scidb::QueryResult > qresult = conn->execute(q_aql.str(), false);
//
//    if((qresult.get() == nullptr) || (qresult->array.get() == nullptr))
//      throw tws::core::http_request_error() << tws::error_description("no query result returned after querying database.");
//
//    const ::scidb::ArrayDesc& array_desc = qresult->array->getArrayDesc();
//
//    const ::scidb::Attributes& array_attributes = array_desc.getAttributes(true);
//
//    const ::scidb::AttributeDesc& attr = array_attributes[0];
//
//    std::vector<double> values;
//
//    values.reserve(ntime_pts);
//
//    ::scidb::ConstItemIterator cell_attr_it(*(qresult->array), 0, ::scidb::ConstChunkIterator::NO_EMPTY_CHECK);
//
//    tws::scidb::fill(values, cell_attr_it, attr.getType());
//
//
//
//
//
//    //const ::scidb::AttributeDesc& attr = array_attributes[i];
//
//    //::scidb::AttributeID att_id = attr.getId();
//
////    boost::shared_ptr< ::scidb::ConstArrayIterator > array_it = qresult->array->getConstIterator(0/*att_id*/);
////
////    while(!array_it->end())
////    {
////      const ::scidb::ConstChunk& chunk = array_it->getChunk();
////
////      boost::shared_ptr< ::scidb::ConstChunkIterator > chunk_it = chunk.getConstIterator();
////
////      while(!chunk_it->end())
////      {
////        ::scidb::Value& v = chunk_it->getItem();
////
////        v.getInt16();
////        ++(*chunk_it);
////      }
////
////      ++(*array_it);
////    }
//
//    //if(values.size() != ntime_pts)
//    //{
//    //  boost::format err_msg("check values for array coverage: %1%, because the number of time points and values are different.");
//
//    //  throw tws::core::http_request_error() << tws::error_description((err_msg % coverage_name).str());
//    //}
//
//    rapidjson::Value jattribute(rapidjson::kObjectType);
//
//    jattribute.AddMember("attribute", queried_attributes[i].c_str(), allocator);
//
//    rapidjson::Value jvalues(rapidjson::kArrayType);
//
//    tws::json::copy_numeric_array(values.begin(), values.end(), jvalues, allocator);
//
//    jattribute.AddMember("values", jvalues, allocator);
//
//    jattributes.PushBack(jattribute, allocator);
//
//  }
//
//// prepare result part in response
//  rapidjson::Value jresult(rapidjson::kObjectType);
//
//  jresult.AddMember("attributes", jattributes, allocator);
//
//// add timeline in the response
//  rapidjson::Value jtimeline(rapidjson::kArrayType);
//  tws::json::copy_string_array(timeline.begin() + start_time_pos, timeline.begin() + end_time_pos + 1, jtimeline, allocator);
//  jresult.AddMember("timeline", jtimeline, allocator);
//
//// add the pixel center location in response
//  rapidjson::Value jcenter(rapidjson::kObjectType);
//  jcenter.AddMember("latitude", y, allocator);
//  jcenter.AddMember("longitude", x, allocator);
//  jresult.AddMember("center_coordinates", jcenter, allocator);
//
//// prepare the query part in response
//  rapidjson::Value jquery(rapidjson::kObjectType);
//
//  jquery.AddMember("coverage", coverage_name.c_str(), allocator);
//
//  rapidjson::Value jqattributes(rapidjson::kArrayType);
//
//  tws::json::copy_string_array(queried_attributes.begin(), queried_attributes.end(), jqattributes, allocator);
//
//  jquery.AddMember("attributes", jqattributes, allocator);
//
//  jquery.AddMember("latitude", latitude, allocator);
//
//  jquery.AddMember("longitude", longitude, allocator);
//
//// form the final response
//  rapidjson::Document doc;
//
//  doc.SetObject();
//
//  doc.AddMember("result", jresult, allocator);
//  doc.AddMember("query", jquery, allocator);
//
//// send response
//  rapidjson::StringBuffer str_buff;
//
//  rapidjson::Writer<rapidjson::StringBuffer> writer(str_buff);
//
//  doc.Accept(writer);
//
//  const char* p_str_buff = str_buff.GetString();
//
//  //response.add_header("Content-Length", boost::lexical_cast<std::string>(content.size()).c_str());
//  response.add_header("Content-Type", "application/json");
//  response.add_header("Access-Control-Allow-Origin", "*");
//  response.set_content(p_str_buff, str_buff.Size());
//
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

}

