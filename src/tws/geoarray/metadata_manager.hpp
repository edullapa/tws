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
  \file tws/metadata/array_metadata_manager.hpp

  \brief A singleton for managing metadata about arrays.

  \author Gilberto Ribeiro de Queiroz
 */

#ifndef __TWS_GEOARRAY_METADATA_MANAGER_HPP__
#define __TWS_GEOARRAY_METADATA_MANAGER_HPP__

// TWS
#include "metadata.hpp"

// STL
#include <vector>

// Boost
#include <boost/noncopyable.hpp>

namespace tws
{
  namespace geoarray
  {

    //! A singleton for managing metadata about arrays.
    class metadata_manager : public boost::noncopyable
    {
      public:

        void insert(const metadata_t& am);

        std::vector<std::string> list_arrays() const;

        const metadata_t& get(const std::string& array_name) const;

        static metadata_manager& instance();

      private:

        metadata_manager();

        ~metadata_manager();

      private:

        struct impl;

        impl* pimpl_;
    };

  }  // end namespace geoarray
}    // end namespace tws

#endif  // __TWS_GEOARRAY_METADATA_MANAGER_HPP__
