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
  \file tws/wtss/timeline.hpp

  \brief A class for handling the timeline of a geo-array.

  \author Gilberto Ribeiro de Queiroz
 */

#ifndef __TWS_WTSS_TIMELINE_HPP__
#define __TWS_WTSS_TIMELINE_HPP__

// STL
#include <map>
#include <string>
#include <vector>

namespace tws
{
  namespace wtss
  {

    //!A class for handling the timeline of a geo-array.
    class timeline
    {
      public:

        timeline() = default;

        timeline(const std::vector<std::string>& time_points);

        ~timeline() = default;

        /*!
          \exception tws::outof_bounds_error If pos is invalid.
         */
        const std::string& get(std::size_t pos) const;

        /*!
          \exception tws::item_not_found_error If time point doesn't exist.
         */
        std::size_t index(const std::string& time_point) const;


      private:

        std::vector<std::string> time_points_;
        std::map<std::string, std::size_t> time_point_idx_;
    };

  } // end namespace wtss
}   // end namespace tws

#endif  // __TWS_WTSS_TIMELINE_HPP__

