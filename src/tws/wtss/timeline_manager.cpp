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
  \file tws/wtss/timeline_manager.cpp

  \brief A singleton for managing the timeline of arrays.

  \author Gilberto Ribeiro de Queiroz
 */

// TWS
#include "timeline_manager.hpp"
#include "exception.hpp"
#include "timeline.hpp"

// STL
#include <map>

// Boost
#include <boost/format.hpp>

struct tws::wtss::timeline_manager::impl
{
  std::map<std::string, timeline > timelines;
};

void
tws::wtss::timeline_manager::insert(const std::string& geoarray_name,
                                    const timeline& t)
{
  if(pimpl_->timelines.find(geoarray_name) != pimpl_->timelines.end())
  {
    boost::format err_msg("a timeline for the array named '%1%' is already registered.");

    throw tws::item_already_exists_error() << tws::error_description((err_msg % geoarray_name).str());
  }

  pimpl_->timelines[geoarray_name] = t;
}

const tws::wtss::timeline&
tws::wtss::timeline_manager::get(const std::string& geoarray_name) const
{
  std::map<std::string, timeline >::iterator it = pimpl_->timelines.find(geoarray_name);

  if(it == pimpl_->timelines.end())
  {
    boost::format err_msg("could not find a timeline for array named: %1%.");
    
    throw tws::item_not_found_error() << tws::error_description((err_msg % geoarray_name).str());
  }

  return it->second;
}

tws::wtss::timeline_manager&
tws::wtss::timeline_manager::instance()
{
  static timeline_manager inst;

  return inst;
}

tws::wtss::timeline_manager::timeline_manager()
  : pimpl_(nullptr)
{
  pimpl_ = new impl;
}

tws::wtss::timeline_manager::~timeline_manager()
{
  delete pimpl_;
}

