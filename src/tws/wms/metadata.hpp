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
  \file tws/wms/metadata.hpp

  \brief A class for describing the WMS metadata.

  \author Roger Victor
 */

#ifndef __TWS_WMS_METADATA_HPP__
#define __TWS_WMS_METADATA_HPP__

// TWS
#include "data_types.hpp"

// STL
#include <string>
#include <vector>

namespace tws
{
  namespace wms
  {
    //! Base metadata of a layer.
    struct layer_metadata_t
    {
      std::string title;
      std::string crs;
    };
    
    //! Base metadata of a exception.
    struct exception_metadata_t
    {
      std::string format;
    };

    //! Base metadata of a request.
    struct request_metadata_t
    {
      std::string format;
      dcp_type_t dcp_type;
    };
    
    //! Base metadata of a capability.
    struct capability_metadata_t
    {
      std::vector<request_metadata_t> request;
      exception_metadata_t exception;
      std::vector<layer_metadata_t> layer;
    };
    
    //! Base metadata of the service contact information.
    struct contact_metadata_t
    {
      std::string contact_person;
      std::string contact_organization;
      std::string contact_position;
      std::string contact_address;
      std::string contact_voice_telephone;
      std::string contact_electronic_mail_address;
    };
    
    //! Base metadata of the service information.
    struct service_metadata_t
    {
      std::string name;
      std::string title;
      std::string abstract;
      std::vector<keyword_t> keyword_list;
      online_resource_t online_resource;
      contact_metadata_t contact_information;
      std::string fees;
      std::string access_constraints;
      std::string layer_limit;
      std::string max_width;
      std::string max_height;
    };

  }  // end namespace wms
}    // end namespace tws

#endif  // __TWS_WMS_METADATA_HPP__
