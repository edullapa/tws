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
  \file tws/wms/data_types.hpp

  \brief A class for describing the WMS metadata.

  \author Roger Victor
 */

#ifndef __TWS_WMS_DATA_TYPES_HPP__
#define __TWS_WMS_DATA_TYPES_HPP__

// TWS
#include "config.hpp"

// STL
#include <string>

namespace tws
{
  namespace wms
  {
    //! A keyword related to the metadata.
    struct keyword_t
    {
      std::string keyword;
      std::string vocabulary;
    };
    
    //! An OnlineResource is typically an HTTP URL. The URL is placed in the xlink:href attribute, and the value "simple" is placed in the xlink:type attribute.
    struct online_resource_t
    {
      std::string xlink_type;
      std::string xlink_href;
    };
    
    //! ...
    struct get_t
    {
      online_resource_t online_resource;
    };
    
    //! ...
    struct post_t
    {
      online_resource_t online_resource;
    };
    
    //! ...
    struct http_t
    {
      get_t get;
      post_t post;
    };
    
    //! The DCP type of the request.
    struct dcp_type_t
    {
      http_t http;
    };
    
    //! ...
    struct ex_geographic_bounding_box_t
    {
      longitude_t west_bound_longitude
      longitude_t east_bound_longitude
      latitude_t south_bound_latitude
      latitude_t north_bound_latitude
    };
    
    //! ...
    struct bounding_box_t
    {
      std::string crs;
      double xmin;
      double ymin;
      double xmax;
      double ymax;
      double xres;
      double yres;
    };
    
    //! ...
    struct dimension_t
    {
      std::string name;
      std::string units;
      std::string unit_symbol;
      std::string default;
      bool multiple_values;
      bool nearest_value;
      bool current;
    };
    
    //! ...
    struct attribution_t
    {
      std::string title;
      online_resource_t online_resource;
      logo_url_t logo_url;
    };
    
    //! ...
    struct logo_url_t
    {
      std::string format;
      online_resource_t online_resource;
      uint32_t width;
      uint32_t height;
    };
    
    //! ...
    struct metadata_url_t
    {
      std::string format;
      online_resource_t online_resource;
      std::string type;
    };
    
    //! ...
    struct authority_url_t
    {
      online_resource_t online_resource;
      std::string name;
    };
    
    //! ...
    struct identifier_t
    {
      std::string name;
    };
    
    //! ...
    struct data_url_t
    {
      std::string format;
      online_resource_t online_resource;
    };
    
    //! ...
    struct feature_list_url_t
    {
      std::string format;
      online_resource_t online_resource;
    };
    
    //! ...
    struct style_t
    {
      std::string name;
      std::string title;
      std::string abstract;
      legend_url_t legend_url;
      style_sheet_url_t style_sheet_url;
      style_url_t style_url;
    };
    
    //! ...
    struct legend_url_t
    {
      std::string format;
      online_resource_t online_resource;
      uint32_t width;
      uint32_t height;
    };
    
    //! ...
    struct style_sheet_url_t
    {
      std::string format;
      online_resource_t online_resource;
    };
    
    //! ...
    struct style_url_t
    {
      std::string format;
      online_resource_t online_resource;
    };
    
    //! ...
    struct longitude_t
    {
      double valeu;
      double min_val;
      double max_val;
    };
    
    //! ...
    struct latitude_t
    {
      double valeu;
      double min_val;
      double max_val;
    };

  }  // end namespace wms
}    // end namespace tws

#endif  // __TWS_WMS_DATA_TYPES_HPP__
