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
#include "geoarray_manager.hpp"
#include "data_types.hpp"
#include "exception.hpp"
#include "utils.hpp"

// STL
#include <iterator>
#include <map>
#include <utility>

// Boost
#include <boost/format.hpp>

struct tws::geoarray::geoarray_manager::impl
{
  std::map<std::string, geoarray_t> arrays;
};

void
tws::geoarray::geoarray_manager::insert(const geoarray_t& a)
{
  std::map<std::string, geoarray_t>::const_iterator it = pimpl_->arrays.find(a.name);

  if(it != std::end(pimpl_->arrays))
  {
    boost::format err_msg("geo-array '%1%' already registered.");
    
    throw tws::item_already_exists_error() << tws::error_description((err_msg % a.name).str());
  }

  pimpl_->arrays.insert(std::make_pair(a.name, a));
}

std::vector<std::string>
tws::geoarray::geoarray_manager::list_arrays() const
{
  std::vector<std::string> arrays;

  for(const auto& v : pimpl_->arrays)
  {
    arrays.push_back(v.first);
  }
  
  return arrays;
}

const tws::geoarray::geoarray_t&
tws::geoarray::geoarray_manager::get(const std::string& array_name) const
{
  std::map<std::string, geoarray_t>::const_iterator it = pimpl_->arrays.find(array_name);

  if(it == std::end(pimpl_->arrays))
  {
    boost::format err_msg("could not find metadata for array: %1%");

    throw tws::item_not_found_error() << tws::error_description((err_msg % array_name).str());
  }

  return it->second;
}

tws::geoarray::geoarray_manager&
tws::geoarray::geoarray_manager::instance()
{
  static geoarray_manager inst;

  return inst;
}

tws::geoarray::geoarray_manager::geoarray_manager()
  : pimpl_(nullptr)
{
  pimpl_ = new impl;
  
  load_geoarrays(pimpl_->arrays);
}

tws::geoarray::geoarray_manager::~geoarray_manager()
{
  delete pimpl_;
}
