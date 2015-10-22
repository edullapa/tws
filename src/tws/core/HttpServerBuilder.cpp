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
  \file tws/core/HttpServerBuilder.cpp

  \brief A singleton for registering available HTTP server implementations..

  \author Gilberto Ribeiro de Queiroz
 */

// TWS
#include "Exception.hpp"
#include "HttpServer.hpp"
#include "HttpServerBuilder.hpp"

// STL
#include <cassert>
#include <map>

// Boost
#include <boost/format.hpp>

struct tws::core::http_server_builder::impl
{
  std::map<std::string, http_server_builder_functor_t> builders;
};

std::unique_ptr<tws::core::http_server>
tws::core::http_server_builder::build(const std::string& builder_id)
{
  std::map<std::string, http_server_builder_functor_t>::const_iterator it = pimpl_->builders.find(builder_id);
  
  if(it == pimpl_->builders.end())
  {
    boost::format err_msg("could not find http server: %1%.");

    throw invalid_http_server_build_error() << tws::error_description((err_msg % builder_id).str());
  }
  
  std::unique_ptr<tws::core::http_server> retval = it->second();

  return std::move(retval);
}

void
tws::core::http_server_builder::insert(const std::string& builder_id,
                                       http_server_builder_functor_t builder_fnct)
{
  std::map<std::string, http_server_builder_functor_t>::iterator it = pimpl_->builders.find(builder_id);
  
  if(it != pimpl_->builders.end())
  {
    boost::format err_msg("there is already a web server builder registered with the informed id: %1%.");
    
    throw invalid_http_server_builder_error() << tws::error_description((err_msg % builder_id).str());
  }
  
  pimpl_->builders[builder_id] = builder_fnct;
}

void
tws::core::http_server_builder::remove(const std::string& builder_id)
{
  std::map<std::string, http_server_builder_functor_t>::iterator it = pimpl_->builders.find(builder_id);
  
  if(it == pimpl_->builders.end())
  {
    boost::format err_msg("could not find web server builder: %1%.");
    
    throw tws::item_not_found_error() << tws::error_description((err_msg % builder_id).str());
  }
  
  pimpl_->builders.erase(it);
}

tws::core::http_server_builder&
tws::core::http_server_builder::instance()
{
  static http_server_builder sbuilder;

  return sbuilder;
}

tws::core::http_server_builder::http_server_builder()
  : pimpl_(nullptr)
{
  pimpl_ = new impl;
}

tws::core::http_server_builder::~http_server_builder()
{
  delete pimpl_;
}
