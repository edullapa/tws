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
  \file tws/geoarray/timeline_manager.hpp

  \brief A singleton for managing the timeline of geo-arrays.

  \author Gilberto Ribeiro de Queiroz
 */

#ifndef __TWS_GEOARRAY_TIMELINE_MANAGER_HPP__
#define __TWS_GEOARRAY_TIMELINE_MANAGER_HPP__

// STL
#include <string>
#include <vector>

// Boost
#include <boost/function.hpp>
#include <boost/noncopyable.hpp>

namespace tws
{
  namespace geoarray
  {
    class timeline;

    //! A singleton for managing the timeline of arrays.
    class timeline_manager : public boost::noncopyable
    {
      public:

        //! Register a timeline for an array.
        /*!
          \exception tws::item_already_exists_error If another timeline is already registered for the same array.
         */
        void insert(const std::string& geoarray_name,
                    const timeline& t);

        //! Find a timeline for a given array.
        /*!
          \exception tws::item_not_found_error If a timeline for the given array is not found.
         */
        const timeline& get(const std::string& geoarray_name) const;

        //! Access the singleton.
        static timeline_manager& instance();

      private:

        timeline_manager();

        ~timeline_manager();

      private:

        struct impl;

        impl* pimpl_;
    };

  } // end namespace geoarray
}   // end namespace tws

#endif  // __TWS_GEOARRAY_TIMELINE_MANAGER_HPP__

