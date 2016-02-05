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
  \file tws/wcs/exception.hpp

  \brief Specific exception types for the WCS model.

  \author Raphael Willian da Costa
 */

#ifndef __TWS_WCS_EXCEPTION_HPP__
#define __TWS_WCS_EXCEPTION_HPP__

// TWS
#include "../exception.hpp"

namespace tws
{
  namespace wcs
  {

    //! The base type for the WCS exceptions.
    struct exception: virtual tws::exception { };

    //! Type for handling missing parameter value in service request.
    struct missing_parameter_value: virtual exception { };

    //! Type for handling invalid parameter value in service request.
    struct invalid_parameter_value: virtual exception { };


  }  // end namespace geoarray
}    // end namespace tws

#endif  // __TWS_WCS_EXCEPTION_HPP__
