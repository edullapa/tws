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
  \file tws/mongoose/http_request.cpp

  \brief HTTP request implementation for Mongoose.

  \author Gilberto Ribeiro de Queiroz
 */

// TWS
#include "http_request.hpp"

// STL
#include <cassert>
#include <string>

// Mongoose
#include "mongoose.h"

tws::mongoose::http_request::http_request(http_message* msg)
  : msg_(msg)
{
  assert(msg_);
}

tws::mongoose::http_request::~http_request()
{
}

const char*
tws::mongoose::http_request::method() const
{
  return nullptr;
}

const char*
tws::mongoose::http_request::version() const
{
  return nullptr;
}

const char*
tws::mongoose::http_request::base_uri() const
{
  return nullptr;
}

const char*
tws::mongoose::http_request::query_string() const
{
  return std::string( msg_->query_string.p, msg_->query_string.len).c_str();
}

const char*
tws::mongoose::http_request::content() const
{
  return nullptr;
}

std::size_t
tws::mongoose::http_request::content_length() const
{
  return 0;
}

const char*
tws::mongoose::http_request::get_var(const char* value) const
{
  return nullptr;
}
