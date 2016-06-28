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
  \file tws/geoarray/timeline.cpp

  \brief A class for handling the timeline of a geo-array.

  \author Gilberto Ribeiro de Queiroz
 */

// TWS
#include "timeline.hpp"
#include "exception.hpp"
#include "utils.hpp"

// STL
#include <algorithm>
#include <iterator>

// Boost
#include <boost/format.hpp>

tws::geoarray::timeline::timeline(const std::vector<std::string>& tp, const dimension_t& dim)
  : time_points_(tp),
    dim_(dim)
{
  std::insert_iterator< std::map<std::string, std::size_t> > iit(time_point_idx_, time_point_idx_.begin());

  std::size_t pos = 0;

  std::transform(std::begin(time_points_), std::end(time_points_),
                 iit,
                 [&pos](const std::string& time_point) { return std::make_pair(time_point, pos++); } );
}

const std::string&
tws::geoarray::timeline::get(std::size_t pos) const
{
  if(pos >= time_points_.size())
  {
    boost::format err_msg("timeline position '%1%' is out of bounds [0, %2%).");
    
    throw tws::outof_bounds_error() << tws::error_description((err_msg % pos % time_points_.size()).str());
  }

  return time_points_[pos];
}

std::size_t
tws::geoarray::timeline::index(const std::string& time_point) const
{
  return pos(time_point) + dim_.min_idx;
}

std::size_t
tws::geoarray::timeline::pos(const std::string& time_point) const
{
  std::map<std::string, std::size_t>::const_iterator it = time_point_idx_.find(time_point);

  if(it == time_point_idx_.end())
  {
    boost::format err_msg("could not find a time point '%1%'.");

    throw tws::outof_bounds_error() << tws::error_description((err_msg % time_point).str());
  }

  return it->second;
}

std::size_t
tws::geoarray::timeline::pos(std::size_t time_idx) const
{
  if(!is_in_range(time_idx, dim_))
  {
    boost::format err_msg("invalid time index: '%1%'.");

    throw tws::outof_bounds_error() << tws::error_description((err_msg % time_idx).str());
  }

  return time_idx - dim_.min_idx;
}

const std::vector<std::string>&
tws::geoarray::timeline::time_points() const
{
  return time_points_;
}
