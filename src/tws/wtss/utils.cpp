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
  \file tws/wtss/utils.cpp

  \brief Utility functions for WTSS service.

  \author Gilberto Ribeiro de Queiroz
 */

// TWS
#include "utils.hpp"
#include "exception.hpp"


// STL
#include <cstdio>

// Boost
#include <boost/filesystem.hpp>
#include <boost/format.hpp>

// RapidJSON
#include <rapidjson/filestream.h>

// TerraLib
//#include <terralib/srs/SpatialReferenceSystemManager.h>

std::vector<std::pair<std::string, std::string> >
tws::wtss::read_timelines_file_name(const std::string& input_file)
{
  std::vector<std::pair<std::string, std::string> > timelines;

  if(!boost::filesystem::is_regular(input_file))
  {
    boost::format err_msg("input file '%1%' doesn't exist.");
    throw tws::file_exists_error() << tws::error_description((err_msg % input_file).str());
  }

  FILE* pfile = fopen(input_file.c_str(), "r");

  if(pfile == nullptr)
  {
    boost::format err_msg("error opening input file '%1%'.");
    throw tws::file_open_error() << tws::error_description((err_msg % input_file).str());
  }

  try
  {
    rapidjson::FileStream istr(pfile);

    rapidjson::Document doc;

    doc.ParseStream<0>(istr);

    if(doc.HasParseError())
    {
      boost::format err_msg("error parsing input file '%1%': %2%.");
      throw tws::parse_error() << tws::error_description((err_msg % input_file % doc.GetParseError()).str());
    }

    if(!doc.IsObject() || doc.IsNull())
    {
      boost::format err_msg("error parsing input file '%1%': unexpected file format.");
      throw tws::parse_error() << tws::error_description((err_msg % input_file).str());
    }

    const rapidjson::Value& jtimelines = doc["timelines"];

    if(!jtimelines.IsArray())
    {
      boost::format err_msg("error parsing input file '%1%': expected a vector of array with time values.");
      throw tws::parse_error() << tws::error_description((err_msg % input_file).str());
    }

    const rapidjson::SizeType nelements = jtimelines.Size();

    for(rapidjson::SizeType i = 0; i != nelements; ++i)
    {
      const rapidjson::Value& jtimeline = jtimelines[i];

      if(jtimeline.IsNull())
        continue;

      if(!jtimeline.IsObject())
      {
        boost::format err_msg("error parsing input file '%1%': expected an object description for array timeline.");
        throw tws::parse_error() << tws::error_description((err_msg % input_file).str());
      }

      const rapidjson::Value& jarray_name = jtimeline["array"];

      if(!jarray_name.IsString() || jarray_name.IsNull())
        throw tws::parse_error() << tws::error_description("error in array timeline entry name in timelines metadata file.");

      std::string array_name = jarray_name.GetString();

      const rapidjson::Value& jarray_timeline = jtimeline["timeline"];

      if(!jarray_timeline.IsString() || jarray_timeline.IsNull())
        throw tws::parse_error() << tws::error_description("error in array timeline entry in timelines metadata file.");

      std::string timeline_file = jarray_timeline.GetString();

      timelines.push_back(std::make_pair(array_name, timeline_file));
    }
  }
  catch(...)
  {
    fclose(pfile);
    throw;
  }

  fclose(pfile);

  return timelines;
}

std::vector<std::string>
tws::wtss::read_timeline(const std::string& input_file)
{
  std::vector<std::string> timeline;

  if(!boost::filesystem::is_regular(input_file))
  {
    boost::format err_msg("input file '%1%' doesn't exist.");
    throw tws::file_exists_error() << tws::error_description((err_msg % input_file).str());
  }

  FILE* pfile = fopen(input_file.c_str(), "r");

  if(pfile == nullptr)
  {
    boost::format err_msg("error opening input file '%1%'.");
    throw tws::file_open_error() << tws::error_description((err_msg % input_file).str());
  }

  try
  {
    rapidjson::FileStream istr(pfile);

    rapidjson::Document doc;

    doc.ParseStream<0>(istr);

    if(doc.HasParseError())
    {
      boost::format err_msg("error parsing input file '%1%': %2%.");
      throw tws::parse_error() << tws::error_description((err_msg % input_file % doc.GetParseError()).str());
    }

    if(!doc.IsObject() || doc.IsNull())
    {
      boost::format err_msg("error parsing input file '%1%': unexpected file format.");
      throw tws::parse_error() << tws::error_description((err_msg % input_file).str());
    }

    const rapidjson::Value& jtimes = doc["timeline"];

    if(!jtimes.IsArray())
    {
      boost::format err_msg("error parsing input file '%1%': expected a vector of array with time values.");
      throw tws::parse_error() << tws::error_description((err_msg % input_file).str());
    }

    const rapidjson::SizeType nelements = jtimes.Size();

    for(rapidjson::SizeType i = 0; i != nelements; ++i)
    {
      const rapidjson::Value& jtime = jtimes[i];
      
      if(jtime.IsNull())
        continue;

      if(!jtime.IsString())
      {
        boost::format err_msg("error parsing input file '%1%': expected a string description for date.");
        throw tws::parse_error() << tws::error_description((err_msg % input_file).str());
      }
      
      timeline.push_back(jtime.GetString());
    }
  }
  catch(...)
  {
    fclose(pfile);
    throw;
  }

  fclose(pfile);

  return timeline;
}
/*
std::vector<tws::wtss::coverage_t>
tws::wtss::read_coverages(const std::string& input_file)
{
  if(!boost::filesystem::is_regular(input_file))
  {
    boost::format err_msg("input file '%1%' doesn't exist.");
    throw tws::file_exists_error() << tws::error_description((err_msg % input_file).str());
  }

  FILE* pfile = fopen(input_file.c_str(), "r");

  if(pfile == nullptr)
  {
    boost::format err_msg("error opening input file '%1%'.");
    throw tws::file_open_error() << tws::error_description((err_msg % input_file).str());
  }

  std::vector<coverage_t> coverages;

  try
  {
    rapidjson::FileStream istr(pfile);

    rapidjson::Document doc;

    doc.ParseStream<0>(istr);

    if(doc.HasParseError())
    {
      boost::format err_msg("error parsing input file '%1%': %2%.");
      throw tws::parse_error() << tws::error_description((err_msg % input_file % doc.GetParseError()).str());
    }

    if(!doc.IsObject() || doc.IsNull())
    {
      boost::format err_msg("error parsing input file '%1%': unexpected file format.");
      throw tws::parse_error() << tws::error_description((err_msg % input_file).str());
    }

    const rapidjson::Value& jcoverages = doc["coverages"];

    if(!jcoverages.IsArray())
    {
      boost::format err_msg("error parsing input file '%1%': expected a vector of coverages.");
      throw tws::parse_error() << tws::error_description((err_msg % input_file).str());
    }

    const rapidjson::SizeType nelements = jcoverages.Size();

    coverages.reserve(nelements);

    for(rapidjson::SizeType i = 0; i != nelements; ++i)
    {
      const rapidjson::Value& jcoverage = jcoverages[i];

      if(jcoverage.IsNull())
        continue;

      if(!jcoverage.IsObject())
      {
        boost::format err_msg("error parsing input file '%1%': expected an object description for coverage.");
        throw tws::parse_error() << tws::error_description((err_msg % input_file).str());
      }

      coverage_t coverage = read_coverage(jcoverage);

      coverages.push_back(coverage);
    }
  }
  catch(...)
  {
    fclose(pfile);
    throw;
  }

  fclose(pfile);

  return coverages;
}

tws::wtss::coverage_t
tws::wtss::read_coverage(const rapidjson::Value& jcoverage)
{
  if(!jcoverage.IsObject())
    throw tws::parse_error() << tws::error_description("error parsing views input file: expected an object description for coverage.");

  coverage_t coverage;

  const rapidjson::Value& jcoverage_name = jcoverage["name"];

  if(!jcoverage_name.IsString() || jcoverage_name.IsNull())
    throw tws::parse_error() << tws::error_description("error in coverage entry name in coverages metadata.");

  coverage.name = jcoverage_name.GetString();

  if(coverage.name.empty())
    throw tws::parse_error() << tws::error_description("coverage name can not be empty in coverages metadata.");

  const rapidjson::Value& jcoverage_dsc = jcoverage["description"];

  if(jcoverage_dsc.IsString() && !jcoverage_dsc.IsNull())
    coverage.description = jcoverage_dsc.GetString();

  const rapidjson::Value& jcoverage_detail = jcoverage["detail"];

  if(jcoverage_detail.IsString() && !jcoverage_detail.IsNull())
    coverage.detail = jcoverage_detail.GetString();

  const rapidjson::Value& jcoverage_attributes = jcoverage["attributes"];

  coverage.attributes = read_coverage_attributes(jcoverage_attributes);

  const rapidjson::Value& jcoverage_dimensions = jcoverage["dimensions"];

  coverage.dimensions = tws::metadata::read_dimensions(jcoverage_dimensions);

  const rapidjson::Value& jcoverage_extent = jcoverage["geo_extent"];

  coverage.geo_extent = tws::metadata::read_geo_extent(jcoverage_extent);

  const rapidjson::Value& jcoverage_from = jcoverage["from"];

  if(jcoverage_from.IsString() && !jcoverage_from.IsNull())
    coverage.from_clause = jcoverage_from.GetString();

  const rapidjson::Value& jcoverage_where = jcoverage["where"];

  if(jcoverage_where.IsString() && !jcoverage_where.IsNull())
    coverage.where_clause = jcoverage_where.GetString();

  return coverage;
}

std::vector<tws::wtss::coverage_attribute_t>
tws::wtss::read_coverage_attributes(const rapidjson::Value& jattributes)
{
  if(!jattributes.IsArray() || jattributes.IsNull())
    throw tws::parse_error() << tws::error_description("error parsing coverage attributes in metadata.");

  std::vector<coverage_attribute_t> attributes;

  const rapidjson::SizeType nelements = jattributes.Size();

  for(rapidjson::SizeType i = 0; i != nelements; ++i)
  {
    const rapidjson::Value& jattribute = jattributes[i];

    if(jattribute.IsNull())
      continue;

    attributes.push_back(read_coverage_attribute(jattribute));
  }

  return attributes;
}

tws::wtss::coverage_attribute_t
tws::wtss::read_coverage_attribute(const rapidjson::Value& jattribute)
{
  if(!jattribute.IsObject() || jattribute.IsNull())
    throw tws::parse_error() << tws::error_description("error parsing coverage attribute in metadata.");

  coverage_attribute_t cv_attr;

  tws::metadata::array_attribute_t& array_att = cv_attr;

  array_att = tws::metadata::read_array_attribute(jattribute);

  const rapidjson::Value& jexp = jattribute["expression"];

  if(!jexp.IsString() || jexp.IsNull())
    throw tws::parse_error() << tws::error_description("error in attribute expression entry in metadata.");

  cv_attr.expression = jexp.GetString();

  return cv_attr;
}
*/
void
tws::wtss::write(const tws::geoarray::geoarray_t& cv,
                 rapidjson::Value& jcv,
                 rapidjson::Document::AllocatorType& allocator)
{
  if(!jcv.IsObject())
    throw tws::invalid_argument_error() << tws::error_description("JSON value must be a JSON object!");

  rapidjson::Value jcv_name(cv.name.c_str(), cv.name.length(), allocator);
  jcv.AddMember("name", jcv_name, allocator);

  rapidjson::Value jcv_description(cv.description.c_str(), cv.description.length(), allocator);
  jcv.AddMember("description", jcv_description, allocator);

  rapidjson::Value jcv_detail(cv.detail.c_str(), cv.detail.length(), allocator);
  jcv.AddMember("detail", jcv_detail, allocator);

  rapidjson::Value jcv_dimensions(rapidjson::kArrayType);
  write(cv.dimensions, jcv_dimensions, allocator);
  jcv.AddMember("dimensions", jcv_dimensions, allocator);

  rapidjson::Value jcv_attributes(rapidjson::kArrayType);
  write(cv.attributes, jcv_attributes, allocator);
  jcv.AddMember("attributes", jcv_attributes, allocator);

  rapidjson::Value jcv_geo_ext(rapidjson::kObjectType);
  write(cv.geo_extent, jcv_geo_ext, allocator);
  jcv.AddMember("geo_extent", jcv_geo_ext, allocator);
}

void
tws::wtss::write(const std::vector<tws::geoarray::attribute_t>& cv_attrs,
                 rapidjson::Value& jcv_attrs,
                 rapidjson::Document::AllocatorType& allocator)
{
  if(!jcv_attrs.IsArray())
    throw tws::invalid_argument_error() << tws::error_description("JSON value must be a JSON array!");

  for(const auto& cv_attr : cv_attrs)
  {
    rapidjson::Value jcv_attr(rapidjson::kObjectType);

    write(cv_attr, jcv_attr, allocator);

    jcv_attrs.PushBack(jcv_attr, allocator);
  }
}

void
tws::wtss::write(const tws::geoarray::attribute_t& cv_attr,
                 rapidjson::Value& jcv_attr,
                 rapidjson::Document::AllocatorType& allocator)
{
  if(!jcv_attr.IsObject())
    throw tws::invalid_argument_error() << tws::error_description("JSON value must be a JSON object!");

  rapidjson::Value jcv_attr_name(cv_attr.name.c_str(), cv_attr.name.length(), allocator);
  jcv_attr.AddMember("name", jcv_attr_name, allocator);

  rapidjson::Value jcv_attr_dsc(cv_attr.description.c_str(), cv_attr.description.length(), allocator);
  jcv_attr.AddMember("description", jcv_attr_dsc, allocator);

  std::string attr_datatype = tws::geoarray::datatype_t::to_string(cv_attr.datatype);
  rapidjson::Value jcv_attr_datatype(attr_datatype.c_str(), attr_datatype.length(), allocator);
  jcv_attr.AddMember("datatype", jcv_attr_datatype, allocator);

  rapidjson::Value jcv_attr_range(rapidjson::kObjectType);
  jcv_attr_range.AddMember("min", cv_attr.valid_range.min_val, allocator);
  jcv_attr_range.AddMember("max", cv_attr.valid_range.max_val, allocator);
  jcv_attr.AddMember("valid_range", jcv_attr_range, allocator);

  jcv_attr.AddMember("scale_factor", cv_attr.scale_factor, allocator);

  jcv_attr.AddMember("missing_value", cv_attr.missing_value, allocator);
}

void
tws::wtss::write(const tws::geoarray::geo_extent_t& geo_ext,
                 rapidjson::Value& jgeo_ext,
                 rapidjson::Document::AllocatorType& allocator)
{
  if(!jgeo_ext.IsObject())
    throw tws::invalid_argument_error() << tws::error_description("JSON value must be a JSON object!");

  rapidjson::Value jspatial_extent(rapidjson::kObjectType);
  jspatial_extent.AddMember("xmin", geo_ext.spatial.extent.xmin, allocator);
  jspatial_extent.AddMember("xmax", geo_ext.spatial.extent.xmax, allocator);
  jspatial_extent.AddMember("ymin", geo_ext.spatial.extent.ymin, allocator);
  jspatial_extent.AddMember("ymax", geo_ext.spatial.extent.ymax, allocator);
  jgeo_ext.AddMember("spatial", jspatial_extent, allocator);

  rapidjson::Value jtemp_extent(rapidjson::kObjectType);

  rapidjson::Value jstart(geo_ext.temporal.time_interval.start.c_str(), geo_ext.temporal.time_interval.start.length(), allocator);
  jtemp_extent.AddMember("start", jstart, allocator);

  rapidjson::Value jend(geo_ext.temporal.time_interval.end.c_str(), geo_ext.temporal.time_interval.end.length(), allocator);
  jtemp_extent.AddMember("end", jend, allocator);

  jtemp_extent.AddMember("resolution", geo_ext.temporal.resolution, allocator);

  std::string str_unit = tws::geoarray::unit_t::to_string(geo_ext.temporal.unit);

  rapidjson::Value junit(str_unit.c_str(), str_unit.length(), allocator);
  jtemp_extent.AddMember("unit", junit, allocator);

  jgeo_ext.AddMember("temporal", jtemp_extent, allocator);
}

void
tws::wtss:: write(const std::vector<tws::geoarray::dimension_t>& cv_dimensions,
                  rapidjson::Value& jcv_dimensions,
                  rapidjson::Document::AllocatorType& allocator)
{
  if(!jcv_dimensions.IsArray())
    throw tws::invalid_argument_error() << tws::error_description("JSON value must be a JSON array!");

  for(const auto& cv_dim : cv_dimensions)
  {
    rapidjson::Value jcv_dim(rapidjson::kObjectType);

    write(cv_dim, jcv_dim, allocator);

    jcv_dimensions.PushBack(jcv_dim, allocator);
  }
}

void
tws::wtss::write(const tws::geoarray::dimension_t& dim,
                 rapidjson::Value& jdim,
                 rapidjson::Document::AllocatorType& allocator)
{
  if(!jdim.IsObject())
    throw tws::invalid_argument_error() << tws::error_description("JSON value must be a JSON object!");

  rapidjson::Value jcv_dim_name(dim.name.c_str(), dim.name.length(), allocator);
  jdim.AddMember("name", jcv_dim_name, allocator);

  rapidjson::Value jcv_dim_description(dim.description.c_str(), dim.description.length(), allocator);
  jdim.AddMember("description", jcv_dim_description, allocator);

  jdim.AddMember("min_idx", dim.min_idx, allocator);

  jdim.AddMember("max_idx", dim.min_idx, allocator);

  //jdim.AddMember("pos", dim.pos, allocator);
}

