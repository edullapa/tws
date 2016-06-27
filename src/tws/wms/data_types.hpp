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
#include "../geoarray/data_types.hpp"

// STL
#include <string>
#include <vector>

namespace tws
{
  namespace wms
  {
    //! A datatype for representing a single keyword.
    struct keyword_t
    {
      std::string vocabulary;
    };
    
    //! An OnlineResource typically represent an HTTP URL.
    /*!
      The URL is placed in the xlink:href attribute, and the value "simple" is
      placed in the xlink:type attribute.
     */
    struct online_resource_t
    {
      std::string xlink_type;
      std::string xlink_href;
    };
    
    //! URL for HTTP request methods.
    struct http_t
    {
      online_resource_t get;
      online_resource_t post;
    };
    
    //! Distributed Computing Platforms.
    struct dcp_type_t
    {
      http_t http;
    };
    
    //! A rectangle with longitude and latitude in decimal degrees.
    struct ex_geographic_bounding_box_t
    {
      double west_bound_longitude;
      double east_bound_longitude;
      double south_bound_latitude;
      double north_bound_latitude;
    };
    
    //! Indicates the limits of a bounding box in a specific CRS.
    /*!
      Units will be assumed as specified by the coordinate reference system.
     */
    struct bounding_box_t
    {
      std::string crs;
      double min_x;
      double min_y;
      double max_x;
      double max_y;
      double res_x;
      double res_y;
    };
    
    //! Used to indicate what values along a dimension are valid.
    struct dimension_t
    {
      std::string value;
      std::string name;
      std::string units;
      std::string unit_symbol;
      std::string default_dim;
      bool multiple_values;
      bool nearest_value;
      bool current;
    };
    

    struct logo_url_t
    {
      std::string format;
      online_resource_t online_resource;
      uint32_t width;
      uint32_t height;
    };
    
    //! Indicates the provider of a Layer or collection of Layers.
    struct attribution_t
    {
      std::string title;
      online_resource_t online_resource;
      logo_url_t logo_url;
    };
    
    //! Used to offer detailed, standardized metadata about the data underneath a particular layer.
    struct metadata_url_t
    {
      std::string format;
      online_resource_t online_resource;
      std::string type;
    };
    
    //! Used to list ID numbers or labels defined by a particular Authority.
    struct authority_url_t
    {
      online_resource_t online_resource;
      std::string name;
    };
    
    struct identifier_t
    {
      std::string value;
      std::string authority;
    };
    
    //! Used to offer a link to the underlying data represented by a particular layer.
    struct data_url_t
    {
      std::string format;
      online_resource_t online_resource;
    };
    
    //! Used to point to a list of the features represented in a Layer.
    struct feature_list_url_t
    {
      std::string format;
      online_resource_t online_resource;
    };
    
    //! Used to provide an image of a legend relevant to each Style of a Layer.
    struct legend_url_t
    {
      std::string format;
      online_resource_t online_resource;
      uint32_t width;
      uint32_t height;
    };
    
    //! Provides symbology information for each Style of a Layer.
    struct style_sheet_url_t
    {
      std::string format;
      online_resource_t online_resource;
    };
    
    //! Used to offer more information about the data or symbology underlying a particular Style.
    struct style_url_t
    {
      std::string format;
      online_resource_t online_resource;
    };
    
    //! Lists the name by which a style is requested and a human-readable title for pick lists.
    /*!
     Optionally provides a human-readable description, and optionally gives a style URL.
     */
    struct style_t
    {
      std::string name;
      std::string title;
      std::string abstract;
      std::vector<legend_url_t> legend_url;
      style_sheet_url_t style_sheet_url;
      style_url_t style_url;
      std::string style_type;
      std::vector<std::string> colors;
    };
   
    //! Nested list of zero or more map Layers offered by this server.
    struct layer_t
    {
      std::string name;         //!< Same name as main geo-array name.
      std::string title;
      std::string abstract;
      std::vector<keyword_t> keyword_list;
      std::vector<std::string> crs;
      ex_geographic_bounding_box_t ex_geographic_bounding_box;
      std::vector<bounding_box_t> bounding_box;
      std::vector<dimension_t> dimension;
      attribution_t attribution;
      std::vector<authority_url_t> authority_url;
      std::vector<identifier_t> identifier;
      std::vector<metadata_url_t> metadata_url;
      std::vector<data_url_t> data_url;
      std::vector<feature_list_url_t> feature_list_url;
      std::vector<style_t> styles;
      double min_scale_denominator;
      double max_scale_denominator;
      std::vector<layer_t> layers;
      bool queryable;
      uint32_t cascaded;
      bool opaque;
      bool no_subsets;
      uint32_t fixed_width;
      uint32_t fixed_height;
      std::vector<tws::geoarray::geoarray_t> overviews;
    };
    
    //! Output format and URI of an operation.
    struct operation_t
    {
      std::vector<std::string> format;
      dcp_type_t dcp_type;
    };
    
    //! Available WMS Operations.
    struct request_t
    {
      operation_t get_capabilities;
      operation_t get_map;
      operation_t get_feature_info;
    };
    
    //! Lists of available layers, available request types, available exception formats.
    struct capability_t
    {
      request_t request;
      std::vector<std::string> exception_formats;
      layer_t layer;
    };
    
    struct contact_address_t
    {
      std::string address_type;
      std::string address;
      std::string city;
      std::string state_or_province;
      std::string post_code;
      std::string country;
    };
    
    struct contact_person_primary_t
    {
      std::string contact_person;
      std::string contact_organization;
    };
    
    //! Information about a contact person for the service.
    struct contact_information_t
    {
      contact_person_primary_t contact_person_primary;
      std::string contact_position;
      contact_address_t contact_address;
      std::string contact_voice_telephone;
      std::string contact_electronic_mail_address;
    };
    
    //! Base datatype for representing the service metadata.
    struct service_t
    {
      std::string name;
      std::string title;
      std::string abstract;
      std::vector<keyword_t> keyword_list;
      online_resource_t online_resource;
      contact_information_t contact_information;
      std::string fees;
      std::string access_constraints;
      uint32_t layer_limit;
      uint32_t max_width;
      uint32_t max_height;
    };
    
    //! Base datatype for WMS_Capabilities document.
    struct capabilities_t
    {
      service_t service;
      capability_t capability;
      std::string version;
    };
    
//    //! An overview of a given geo-array.
//    struct overview_t
//    {
//      std::string geo_array_name;                          //!< the original array name.
//      std::string o_geo_array_name;                        //!< the name of an array that materizalies the overview.
//      std::vector<tws::geoarray::attribute_t> attributes;  //!< the list of attributes in the overview array.
//      std::vector<tws::geoarray::dimension_t> dimensions;  //!< the dimension information of the overview array.
//      tws::geoarray::geo_extent_t geo_extent;              //!< spatial and temporal resolution and extent of the new array.
//    };

  }  // end namespace wms
}    // end namespace tws

#endif  // __TWS_WMS_DATA_TYPES_HPP__
