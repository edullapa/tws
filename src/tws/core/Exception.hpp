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
  \file tws/core/Exception.hpp

  \brief Specific exception types for Core Runtime module.

  \author Gilberto Ribeiro de Queiroz
 */

#ifndef __TWS_CORE_EXCEPTION_HPP__
#define __TWS_CORE_EXCEPTION_HPP__

// TWS
#include "../Exception.hpp"

namespace tws
{
  //! The namespace for the Core Runtime module of TerraLib Web Services.
  namespace core
  {

    //! The base type for exceptions from the core runtime module.
    struct exception: virtual tws::exception { };

    //! An exception indicating an error on service registering or when searching for a missing one.
    struct invalid_service_error: virtual exception { };

    //! An exception indicating that a service operation could not be registered or it could not be found in the list of valid operations.
    struct invalid_service_operation_error: virtual exception { };
    
    //! An exception indicating an error on http builder registering.
    struct invalid_http_server_builder_error: virtual exception { };
    
    //! An exception indicating an error when building a given type of web server.
    struct invalid_http_server_build_error: virtual exception { };
    
    //! An exception indicating an error on the request.
    struct http_request_error: virtual exception { };

  }  // end namespace core
}    // end namespace tws

#endif  // __TWS_CORE_EXCEPTION_HPP__
