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

  \author Gilberto Ribeiro de Queiroz <gribeiro@dpi.inpe.br>

  \date 2014-2014

  \copyright GNU General Public License version 3
 */

#ifndef __TWS_METADATA_UTILS_HPP__
#define __TWS_METADATA_UTILS_HPP__

// TWS
#include "data_types.hpp"

// STL
#include <cassert>
#include <map>
#include <string>
#include <vector>

// RapidJSON
#include <rapidjson/document.h>

namespace tws
{
  namespace geoarray
  {

    void load_geoarrays(std::map<std::string, geoarray_t>& arrays);
    
    void load_geoarrays(std::map<std::string, geoarray_t>& arrays,
                        const std::string& input_file);

    geoarray_t read_array_metadata(const rapidjson::Value& jmetadata);

    std::vector<attribute_t> read_array_attributes(const rapidjson::Value& jattributes);

    std::vector<dimension_t> read_dimensions(const rapidjson::Value& jdims);

    spatial_resolution_t read_spatial_resolution(const rapidjson::Value& jresolution);

    numeric_range_t read_numeric_range(const rapidjson::Value& jrange);

    time_interval_t read_time_interval(const rapidjson::Value& jinterval);

    extent_t read_extent(const rapidjson::Value& jextent);

    dimension_t read_dimension(const rapidjson::Value& jdim);

    spatial_extent_t read_spatial_extent(const rapidjson::Value& jspatial_extent);

    temporal_extent_t read_temporal_extent(const rapidjson::Value& jtemporal_extent);

    geo_extent_t read_geo_extent(const rapidjson::Value& jgeo_extent);

    attribute_t read_array_attribute(const rapidjson::Value& jattribute);

    void write(const std::vector<dimension_t>& dims,
               rapidjson::Value& jdims,
               rapidjson::Document::AllocatorType& allocator);

    void write(const dimension_t& dim,
               rapidjson::Value& jdim,
               rapidjson::Document::AllocatorType& allocator);

    //! Read the list of timelines files
    /*!
      \exception tws::file_exists_error If the file doesn't exists.
      \exception tws::file_open_error   If the file can not be opened.
      \exception tws::parser_error      If the file has any syntax error.
     */
    std::vector<std::pair<std::string, std::string> >
    read_timelines_file_name(const std::string& input_file);

    //! Read the timeline from informed file.
    /*!
      \exception tws::file_exists_error If the file doesn't exists.
      \exception tws::file_open_error   If the file can not be opened.
      \exception tws::parser_error      If the file has any syntax error.
     */
    std::vector<std::string> read_timeline(const std::string& input_file);

    inline bool intersects(const double& x, const double& y, const extent_t& e)
    {
      if(x < e.xmin)
        return false;

      if(x > e.xmax)
        return false;

      if(y < e.ymin)
        return false;

      if(y > e.ymax)
        return false;

      return true;
    }

    inline bool is_in_range(int64_t pos, const dimension_t& d)
    {
      return (pos >= d.min_idx && pos <= d.max_idx);
    }

    inline bool is_in_spatial_range(int64_t col, int64_t row, const std::vector<dimension_t>& dimensions)
    {
      assert(dimensions.size() >= 2);

      return is_in_range(col, dimensions[0]) && is_in_range(row, dimensions[1]);
    }

  }   // end namespace geoarray
}     // end namespace tws

#endif  // __TWS_METADATA_UTILS_HPP__
