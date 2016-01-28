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
  \file tws/geoarray/metadata_manager.cpp

  \brief A singleton for managing metadata about arrays.

  \author Gilberto Ribeiro de Queiroz
 */

// TWS
#include "metadata_manager.hpp"
#include "exception.hpp"
#include "utils.hpp"

// STL
#include <map>

// Boost
#include <boost/foreach.hpp>
#include <boost/format.hpp>

struct tws::geoarray::metadata_manager::impl
{
  std::map<std::string, metadata_t> arrays;
};

void
tws::geoarray::metadata_manager::insert(const metadata_t& am)
{
  std::map<std::string, metadata_t>::const_iterator it = pimpl_->arrays.find(am.name);

  if(it != pimpl_->arrays.end())
  {
    boost::format err_msg("metadata for array '%1%' already registered.");
    
    throw tws::item_already_exists_error() << tws::error_description((err_msg % am.name).str());
  }

  pimpl_->arrays[am.name] = am;
}

std::vector<std::string>
tws::geoarray::metadata_manager::list_arrays() const
{
  std::vector<std::string> arrays;

  typedef std::map<std::string, metadata_t> array_map_t;
  
  BOOST_FOREACH(const array_map_t::value_type& v, pimpl_->arrays)
  {
    arrays.push_back(v.first);
  }
  
  return arrays;
}

const tws::geoarray::metadata_t&
tws::geoarray::metadata_manager::get(const std::string& array_name) const
{
  std::map<std::string, metadata_t>::const_iterator it = pimpl_->arrays.find(array_name);

  if(it == pimpl_->arrays.end())
  {
    boost::format err_msg("could not find metadata for array: %1%");

    throw tws::item_not_found_error() << tws::error_description((err_msg % array_name).str());
  }

  return it->second;
}

tws::geoarray::metadata_manager&
tws::geoarray::metadata_manager::instance()
{
  static metadata_manager inst;

  return inst;
}

tws::geoarray::metadata_manager::metadata_manager()
  : pimpl_(nullptr)
{
  pimpl_ = new impl;
  
  load_metadata(pimpl_->arrays);
}

tws::geoarray::metadata_manager::~metadata_manager()
{
  delete pimpl_;
}
