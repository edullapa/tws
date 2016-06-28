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
  \file tws/geoarray/timeline.hpp

  \brief A class for handling the timeline of a geo-array.

  \author Gilberto Ribeiro de Queiroz
 */

#ifndef __TWS_GEOARRAY_TIMELINE_HPP__
#define __TWS_GEOARRAY_TIMELINE_HPP__

// TWS
#include "data_types.hpp"

// STL
#include <map>
#include <string>
#include <vector>

namespace tws
{
  namespace geoarray
  {

    //!A class for handling the timeline of a geo-array.
    class timeline
    {
      public:

        timeline() {}

        timeline(const std::vector<std::string>& tp, const dimension_t& dim);

        ~timeline() {}

        /*!
          \exception tws::outof_bounds_error If pos is invalid.
         */
        const std::string& get(std::size_t pos) const;

        /*!
          \brief Return the time index in the temporal dimension.

          \exception tws::outof_bounds_error If time point doesn't exist.
         */
        std::size_t index(const std::string& time_point) const;

        /*!
          \brief Return the time position in the timeline

          \exception tws::outof_bounds_error If time point doesn't exist.
         */
        std::size_t pos(const std::string& time_point) const;

        /*!
          \brief Return the time position in the timeline for a given time index.

          \exception tws::outof_bounds_error If time point doesn't exist.
         */
        std::size_t pos(std::size_t time_idx) const;

        const std::vector<std::string>& time_points() const;

      private:

        std::vector<std::string> time_points_;
        std::map<std::string, std::size_t> time_point_idx_;
        dimension_t dim_;
    };

  } // end namespace geoarray
}   // end namespace tws

#endif  // __TWS_GEOARRAY_TIMELINE_HPP__

