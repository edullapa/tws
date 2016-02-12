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
  \file tws/geoarray/geoarray_manager.hpp

  \brief A singleton for managing the registered geo-arrays.

  \author Gilberto Ribeiro de Queiroz
 */

#ifndef __TWS_GEOARRAY_GEOARRAY_MANAGER_HPP__
#define __TWS_GEOARRAY_GEOARRAY_MANAGER_HPP__

// TWS
#include "config.hpp"

// STL
#include <vector>

// Boost
#include <boost/noncopyable.hpp>

namespace tws
{
  namespace geoarray
  {
  
    //! Forward declaration
    struct geoarray_t;

    //! A singleton for managing geo-arrays.
    class geoarray_manager : public boost::noncopyable
    {
      public:

        void insert(const geoarray_t& a);

        std::vector<std::string> list_arrays() const;

        const geoarray_t& get(const std::string& array_name) const;

        static geoarray_manager& instance();

      private:

        geoarray_manager();

        ~geoarray_manager();

      private:

        struct impl;

        impl* pimpl_;
    };

  }  // end namespace geoarray
}    // end namespace tws

#endif  // __TWS_GEOARRAY_GEOARRAY_MANAGER_HPP__
