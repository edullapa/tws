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
  \file tws/metadata/array_metadata.hpp

  \brief A class for describing the geospatial metadata of a SciDB array.

  \author Gilberto Ribeiro de Queiroz <gribeiro@dpi.inpe.br>

  \date 2014-2014

  \copyright GNU General Public License version 3
 */

#ifndef __TWS_GEOARRAY_METADATA_HPP__
#define __TWS_GEOARRAY_METADATA_HPP__

// TWS
#include "data_types.hpp"

// STL
#include <string>
#include <vector>

namespace tws
{
  namespace geoarray
  {
    //! Base metadata of an array attribute.
    struct attribute_t
    {
      std::string name;
      std::string description;
      numeric_range_t valid_range;
      double scale_factor;
      double missing_value;
      int datatype;
    };

    //! Base metadata of an array.
    struct metadata_t
    {
      std::string name;
      std::string description;
      std::string detail;
      std::vector<attribute_t> attributes;
      std::vector<dimension_t> dimensions;
      geo_extent_t geo_extent;
    };

  }  // end namespace geoarray
}    // end namespace tws

#endif  // __TWS_GEOARRAY_METADATA_HPP__
