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
  \file tws/metadata/utils.hpp

  \brief Utility functions for the Metadata module.

  \author Gilberto Ribeiro de Queiroz
 */

// TWS
#include "utils.hpp"
#include "../core/utils.hpp"
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

void
tws::geoarray::load_geoarrays(std::map<std::string, geoarray_t>& arrays)
{
  std::string input_file = tws::core::find_in_app_path("share/tws/config/geo_arrays.json");

  if(input_file.empty())
    throw  tws::file_exists_error() << tws::error_description("could not locate array metadata file: 'share/tws/config/arrays_metadata.json'.");

  load_geoarrays(arrays, input_file);
}

void
tws::geoarray::load_geoarrays(std::map<std::string, geoarray_t>& arrays,
                              const std::string& input_file)
{
  arrays.clear();

  try
  {
    rapidjson::Document* doc = tws::core::open_json_file(input_file);

    const rapidjson::Value& jarrays = (*doc)["arrays"];

    if(!jarrays.IsArray())
    {
      boost::format err_msg("error parsing input file '%1%': expected a vector of array metadata.");

      throw tws::parser_error() << tws::error_description((err_msg % input_file).str());
    }

    const rapidjson::SizeType nelements = jarrays.Size();

    for(rapidjson::SizeType i = 0; i != nelements; ++i)
    {
      const rapidjson::Value& jarray = jarrays[i];

      if(jarray.IsNull())
        continue;

      geoarray_t g_array = read_array_metadata(jarray);

      arrays.insert(std::make_pair(g_array.name, g_array));
    }
  }
  catch(...)
  {
    throw;
  }
  
}

tws::geoarray::geoarray_t
tws::geoarray::read_array_metadata(const rapidjson::Value& jmetadata)
{
  if(!jmetadata.IsObject())
    throw tws::parser_error() << tws::error_description("error parsing array metadata.");

  geoarray_t ameta;

  const rapidjson::Value& jarray_name = jmetadata["name"];

  if(!jarray_name.IsString() || jarray_name.IsNull())
    throw tws::parser_error() << tws::error_description("error in array entry name in metadata.");

  ameta.name = jarray_name.GetString();

  if(ameta.name.empty())
    throw tws::parser_error() << tws::error_description("array name can not be empty in metadata.");

  const rapidjson::Value& jarray_dsc = jmetadata["description"];

  if(jarray_dsc.IsString() && !jarray_dsc.IsNull())
    ameta.description = jarray_dsc.GetString();
  
  const rapidjson::Value& jarray_detail = jmetadata["detail"];
  
  if(jarray_detail.IsString() && !jarray_detail.IsNull())
    ameta.detail = jarray_detail.GetString();

  const rapidjson::Value& jarray_dimensions = jmetadata["dimensions"];

  ameta.dimensions = read_dimensions(jarray_dimensions);

  const rapidjson::Value& jarray_attributes = jmetadata["attributes"];

  ameta.attributes = read_array_attributes(jarray_attributes);

  const rapidjson::Value& jarray_geo_extent = jmetadata["geo_extent"];

  ameta.geo_extent = read_geo_extent(jarray_geo_extent);

  return ameta;
}

std::vector<tws::geoarray::attribute_t>
tws::geoarray::read_array_attributes(const rapidjson::Value& jattributes)
{
  if(!jattributes.IsArray() || jattributes.IsNull())
    throw tws::parser_error() << tws::error_description("error parsing array attributes in metadata.");

  std::vector<attribute_t> attributes;

  const rapidjson::SizeType nelements = jattributes.Size();

  for(rapidjson::SizeType i = 0; i != nelements; ++i)
  {
    const rapidjson::Value& jattribute = jattributes[i];

    if(jattribute.IsNull())
      continue;

    attributes.push_back(read_array_attribute(jattribute));
  }

  return attributes;
}

std::vector<tws::geoarray::dimension_t>
tws::geoarray::read_dimensions(const rapidjson::Value& jdims)
{
  if(!jdims.IsArray() || jdims.IsNull())
    throw tws::parser_error() << tws::error_description("error parsing array dimensions in metadata.");

  std::vector<dimension_t> dims;

  const rapidjson::SizeType nelements = jdims.Size();

  for(rapidjson::SizeType i = 0; i != nelements; ++i)
  {
    const rapidjson::Value& jdim = jdims[i];

    if(jdim.IsNull())
      continue;

    dims.push_back(read_dimension(jdim));
  }

  return dims;
}

tws::geoarray::spatial_resolution_t
tws::geoarray::read_spatial_resolution(const rapidjson::Value& jresolution)
{
  if(!jresolution.IsObject() || jresolution.IsNull())
    throw tws::parser_error() << tws::error_description("error parsing spatial resolution in metadata.");

  spatial_resolution_t resolution;

  const rapidjson::Value& jx = jresolution["x"];

  if(!jx.IsNumber() || jx.IsNull())
    throw tws::parser_error() << tws::error_description("error in spatial resolution 'x' entry in metadata.");

  resolution.x = jx.GetDouble();

  const rapidjson::Value& jy = jresolution["y"];

  if(!jy.IsNumber() || jy.IsNull())
    throw tws::parser_error() << tws::error_description("error in spatial resolution 'y' entry in metadata.");

  resolution.y = jy.GetDouble();

  return resolution;
}

tws::geoarray::numeric_range_t
tws::geoarray::read_numeric_range(const rapidjson::Value& jrange)
{
  if(!jrange.IsObject() || jrange.IsNull())
    throw tws::parser_error() << tws::error_description("error parsing attribute range in metadata.");

  numeric_range_t r;

  const rapidjson::Value& jmin_val = jrange["min"];

  if(!jmin_val.IsNumber() || jmin_val.IsNull())
    throw tws::parser_error() << tws::error_description("error in attribute range entry in metadata.");

  r.min_val = jmin_val.GetDouble();

  const rapidjson::Value& jmax_val = jrange["max"];

  if(!jmax_val.IsNumber() || jmax_val.IsNull())
    throw tws::parser_error() << tws::error_description("error in attribute range entry in metadata.");

  r.max_val = jmax_val.GetDouble();

  return r;
}

tws::geoarray::time_interval_t
tws::geoarray::read_time_interval(const rapidjson::Value& jinterval)
{
  if(!jinterval.IsObject() || jinterval.IsNull())
    throw tws::parser_error() << tws::error_description("error parsing temporal interval in metadata.");

  time_interval_t ti;

  const rapidjson::Value& jstart_val = jinterval["start"];

  if(!jstart_val.IsString() || jstart_val.IsNull())
    throw tws::parser_error() << tws::error_description("error parsing temporal interval in metadata.");

  ti.start = jstart_val.GetString();

  const rapidjson::Value& jend_val = jinterval["end"];

  if(!jend_val.IsString() || jend_val.IsNull())
    throw tws::parser_error() << tws::error_description("error parsing temporal interval in metadata.");

  ti.end = jend_val.GetString();

  return ti;
}

tws::geoarray::extent_t
tws::geoarray::read_extent(const rapidjson::Value& jextent)
{
  if(!jextent.IsObject() || jextent.IsNull())
    throw tws::parser_error() << tws::error_description("error parsing spatial extent in metadata.");

  extent_t extent;

  const rapidjson::Value& jxmin = jextent["xmin"];

  if(!jxmin.IsNumber() || jxmin.IsNull())
    throw tws::parser_error() << tws::error_description("error in spatial extent 'xmin' entry in metadata.");

  extent.xmin = jxmin.GetDouble();

  const rapidjson::Value& jymin = jextent["ymin"];

  if(!jymin.IsNumber() || jymin.IsNull())
    throw tws::parser_error() << tws::error_description("error in spatial extent 'ymin' entry in metadata.");

  extent.ymin = jymin.GetDouble();

  const rapidjson::Value& jxmax = jextent["xmax"];

  if(!jxmax.IsNumber() || jxmax.IsNull())
    throw tws::parser_error() << tws::error_description("error in spatial extent 'xmax' entry in metadata.");

  extent.xmax = jxmax.GetDouble();

  const rapidjson::Value& jymax = jextent["ymax"];

  if(!jymax.IsNumber() || jymax.IsNull())
    throw tws::parser_error() << tws::error_description("error in spatial extent 'ymax' entry in metadata.");

  extent.ymax = jymax.GetDouble();

  return extent;
}

tws::geoarray::dimension_t
tws::geoarray::read_dimension(const rapidjson::Value& jdim)
{
  if(!jdim.IsObject() || jdim.IsNull())
    throw tws::parser_error() << tws::error_description("error parsing array dimension in metadata.");

  dimension_t dim;

  const rapidjson::Value& jname = jdim["name"];

  if(!jname.IsString() || jname.IsNull())
    throw tws::parser_error() << tws::error_description("error in dimension name entry in metadata.");

  dim.name = jname.GetString();

  const rapidjson::Value& jdescription = jdim["description"];

  if(jdescription.IsString() || !jdescription.IsNull())
    dim.description = jdescription.GetString();

  const rapidjson::Value& jdim_min = jdim["min_idx"];

  if(!jdim_min.IsNumber() || jdim_min.IsNull())
    throw tws::parser_error() << tws::error_description("error in dimension min_idx entry in metadata.");

  dim.min_idx = jdim_min.GetInt64();

  const rapidjson::Value& jdim_max = jdim["max_idx"];

  if(!jdim_max.IsNumber() || jdim_max.IsNull())
    throw tws::parser_error() << tws::error_description("error in dimension max_idx entry in metadata.");

  dim.max_idx = jdim_max.GetInt64();

  const rapidjson::Value& jdim_pos = jdim["pos"];

  if(!jdim_pos.IsNumber() || jdim_pos.IsNull())
    throw tws::parser_error() << tws::error_description("error in dimension position entry in metadata.");

  dim.pos = jdim_pos.GetUint();

  return dim;
}

tws::geoarray::spatial_extent_t
tws::geoarray::read_spatial_extent(const rapidjson::Value& jspatial_extent)
{
  if(!jspatial_extent.IsObject() || jspatial_extent.IsNull())
    throw tws::parser_error() << tws::error_description("error parsing spatial extent in metadata.");

  spatial_extent_t sp_extent;

  const rapidjson::Value& jextent = jspatial_extent["extent"];

  sp_extent.extent = read_extent(jextent);

  const rapidjson::Value& jres = jspatial_extent["resolution"];

  sp_extent.resolution = read_spatial_resolution(jres);

  //const rapidjson::Value& jcrs = jspatial_extent["crs"];

  //if(!jcrs.IsString() || jcrs.IsNull())
    //throw tws::parser_error() << tws::error_description("error in CRS in metadata.");

  //std::string crs = jcrs.GetString();

  //std::pair<std::string,unsigned int> crs_id = te::srs::SpatialReferenceSystemManager::getInstance().getIdFromP4Txt(crs);

  const rapidjson::Value& jsrid = jspatial_extent["srid"];

  if(!jsrid.IsInt() || jsrid.IsNull())
    throw tws::parser_error() << tws::error_description("error reading geoarray srid.");

  sp_extent.crs_code = jsrid.GetUint();

  return sp_extent;
}

tws::geoarray::temporal_extent_t
tws::geoarray::read_temporal_extent(const rapidjson::Value& jtemporal_extent)
{
  if(!jtemporal_extent.IsObject() || jtemporal_extent.IsNull())
    throw tws::parser_error() << tws::error_description("error parsing temporal extent in metadata.");

  temporal_extent_t t_ext;

  const rapidjson::Value& jti = jtemporal_extent["interval"];

  t_ext.time_interval = read_time_interval(jti);

  const rapidjson::Value& jres = jtemporal_extent["resolution"];

  t_ext.resolution = jres.GetUint64();

  const rapidjson::Value& junit = jtemporal_extent["unit"];

  const std::string sunit = junit.GetString();

  t_ext.unit = unit_t::from_string(sunit);

  return t_ext;
}

tws::geoarray::geo_extent_t
tws::geoarray::read_geo_extent(const rapidjson::Value& jgeo_extent)
{
  if(!jgeo_extent.IsObject() || jgeo_extent.IsNull())
    throw tws::parser_error() << tws::error_description("error parsing geographical extent in metadata.");

  geo_extent_t geo_extent;

  const rapidjson::Value& jspatial = jgeo_extent["spatial"];

  geo_extent.spatial = read_spatial_extent(jspatial);

  const rapidjson::Value& jtemporal = jgeo_extent["temporal"];

  geo_extent.temporal = read_temporal_extent(jtemporal);

  return geo_extent;
}

tws::geoarray::attribute_t
tws::geoarray::read_array_attribute(const rapidjson::Value& jattribute)
{
  if(!jattribute.IsObject() || jattribute.IsNull())
    throw tws::parser_error() << tws::error_description("error parsing array attribute in metadata.");

  attribute_t attr;

  const rapidjson::Value& jname = jattribute["name"];

  if(!jname.IsString() || jname.IsNull())
    throw tws::parser_error() << tws::error_description("error in attribute name entry in metadata.");

  attr.name = jname.GetString();

  const rapidjson::Value& jdescription = jattribute["description"];

  if(jdescription.IsString() || !jdescription.IsNull())
    attr.description = jdescription.GetString();

  const rapidjson::Value& jdatatype = jattribute["datatype"];

  if(!jdatatype.IsString() || jdatatype.IsNull())
    throw tws::parser_error() << tws::error_description("error in attribute datatype entry in metadata.");

  attr.datatype = datatype_t::from_string(jdatatype.GetString());

  const rapidjson::Value& jvalid_range = jattribute["valid_range"];

  attr.valid_range = read_numeric_range(jvalid_range);

  const rapidjson::Value& jscale_factor = jattribute["scale_factor"];

  if(!jscale_factor.IsNumber() || jscale_factor.IsNull())
    throw tws::parser_error() << tws::error_description("error in attribute scale_factor entry in metadata.");

  attr.scale_factor = jscale_factor.GetDouble();

  const rapidjson::Value& jmissing_value = jattribute["missing_value"];

  if(!jmissing_value.IsNumber() || jmissing_value.IsNull())
    throw tws::parser_error() << tws::error_description("error in attribute missing_value entry in metadata.");

  attr.missing_value = jmissing_value.GetDouble();

  return attr;
}

void
tws::geoarray::write(const std::vector<dimension_t>& dims,
                     rapidjson::Value& jdims,
                     rapidjson::Document::AllocatorType& allocator)
{
  if(!jdims.IsArray())
    throw tws::conversion_error() << tws::error_description("dimensions metadata must be a JSON array!");

  const std::size_t ndims = dims.size();

  for(std::size_t i = 0; i != ndims; ++i)
  {
    const dimension_t& dim = dims[i];

    rapidjson::Value jdim(rapidjson::kObjectType);

    write(dim, jdim, allocator);

    jdims.PushBack(jdim, allocator);
  }
}

void
tws::geoarray::write(const dimension_t& dim,
                     rapidjson::Value& jdim,
                     rapidjson::Document::AllocatorType& allocator)
{
  if(!jdim.IsObject())
    throw tws::conversion_error() << tws::error_description("dimension metadata must be a JSON object!");

  rapidjson::Value jdim_name(dim.name.c_str(), dim.name.length(), allocator);
  jdim.AddMember("name", jdim_name, allocator);

  rapidjson::Value jdim_dsc(dim.description.c_str(), dim.description.length(), allocator);
  jdim.AddMember("description", jdim_dsc, allocator);

  jdim.AddMember("min_idx", dim.min_idx, allocator);
  jdim.AddMember("max_idx", dim.max_idx, allocator);
}
