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
    
    //! ...
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

  }  // end namespace wms
}    // end namespace tws

#endif  // __TWS_WMS_DATA_TYPES_HPP__
