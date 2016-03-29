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
  \file tws/wms/wms_manager.hpp

  \brief A singleton for controlling the structs containing wms configuration.

  \author Roger Victor
 */

#ifndef __TWS_WMS_WMS_MANAGER_HPP__
#define __TWS_WMS_WMS_MANAGER_HPP__

// TWS
#include "config.hpp"
#include "data_types.hpp"

// STL
#include <string>
#include <vector>

namespace tws
{
  namespace wms
  {
// forward declarations
    struct capabilities_t;
    struct service_t;

    class wms_manager
    {
      public:

        //! Singleton method
        static wms_manager& instance();

        /*!
          \brief It makes a capabilities object with wms metadata
          \return A tws::wms::capabilities_t struct containing wms configuration
        */
        capabilities_t capabilities();

      private:

// singleton is accesible through class member function: instance()
        //! Constructor
        wms_manager();

        //! Destructor
        ~wms_manager();

// Pimpl idiom
        struct impl;

        impl* pimpl_;
    };
  }
}

#endif  // __TWS_WMS_WMS_MANAGER_HPP__
