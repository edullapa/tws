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
#include "metadata.hpp"

// STL
#include <map>
#include <string>
#include <vector>

// RapidJSON
#include <rapidjson/document.h>

namespace tws
{
  namespace geoarray
  {

    void load_metadata(std::map<std::string, metadata_t>& arrays);
    
    void load_metadata(std::map<std::string, metadata_t>& arrays,
                       const std::string& input_file);

    metadata_t read_array_metadata(const rapidjson::Value& jmetadata);

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

  }   // end namespace geoarray
}     // end namespace tws

#endif  // __TWS_METADATA_UTILS_HPP__
