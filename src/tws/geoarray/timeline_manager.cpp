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
  \file tws/geoarray/timeline_manager.cpp

  \brief A singleton for managing the timeline of arrays.

  \author Gilberto Ribeiro de Queiroz
 */

// TWS
#include "timeline_manager.hpp"
#include "../core/utils.hpp"
#include "exception.hpp"
#include "geoarray_manager.hpp"
#include "timeline.hpp"
#include "utils.hpp"

// STL
#include <map>

// Boost
#include <boost/format.hpp>

struct tws::geoarray::timeline_manager::impl
{
  std::map<std::string, timeline > timelines;
};

void
tws::geoarray::timeline_manager::insert(const std::string& geoarray_name,
                                        const timeline& t)
{
  if(pimpl_->timelines.find(geoarray_name) != pimpl_->timelines.end())
  {
    boost::format err_msg("a timeline for the array named '%1%' is already registered.");

    throw tws::item_already_exists_error() << tws::error_description((err_msg % geoarray_name).str());
  }

  pimpl_->timelines[geoarray_name] = t;
}

const tws::geoarray::timeline&
tws::geoarray::timeline_manager::get(const std::string& geoarray_name) const
{
  std::map<std::string, timeline >::iterator it = pimpl_->timelines.find(geoarray_name);

  if(it == pimpl_->timelines.end())
  {
    boost::format err_msg("could not find a timeline for array named: %1%.");
    
    throw tws::item_not_found_error() << tws::error_description((err_msg % geoarray_name).str());
  }

  return it->second;
}

tws::geoarray::timeline_manager&
tws::geoarray::timeline_manager::instance()
{
  static timeline_manager inst;

  return inst;
}

tws::geoarray::timeline_manager::timeline_manager()
  : pimpl_(nullptr)
{
  pimpl_ = new impl;

  geoarray_manager& gmanager = geoarray_manager::instance();

  std::string timelines = tws::core::find_in_app_path("share/tws/config/timelines.json");

  if(timelines.empty())
    throw  tws::file_exists_error() << tws::error_description("could not locate file: 'share/tws/config/wtss_timelines.json'.");

  std::vector<std::pair<std::string, std::string> > timelines_files = read_timelines_file_name(timelines);

  for(const auto& tf : timelines_files)
  {
    std::string input_file = tws::core::find_in_app_path("share/tws/config/" + tf.second);

    if(input_file.empty())
    {
      boost::format err_msg("timeline file: '%1%', not found for array '%2%'.");

      throw tws::file_exists_error() << tws::error_description((err_msg % tf.second % tf.first).str());
    }

    std::vector<std::string> str_timeline = read_timeline(input_file);

    const geoarray_t& garray = gmanager.get(tf.first);

    assert(garray.dimensions.size() == 3);

    timeline t(str_timeline, garray.dimensions[2]);

    insert(tf.first, t);
  }
}

tws::geoarray::timeline_manager::~timeline_manager()
{
  delete pimpl_;
}

