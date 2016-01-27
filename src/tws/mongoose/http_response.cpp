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
  \file tws/mongoose/http_response.cpp

  \brief HTTP response implementaton for Moongose.

  \author Gilberto Ribeiro de Queiroz
 */

// TWS
#include "http_response.hpp"

// STL
#include <cassert>

// Mongoose
#include "mongoose.h"

tws::mongoose::http_response::http_response(mg_connection* conn)
  : conn_(conn)
{
  assert(conn_);
}

tws::mongoose::http_response::~http_response()
{
}

void
tws::mongoose::http_response::add_header(const char* key,
                                         const char* value)
{
  mg_send_header(conn_, key, value);
}

void
tws::mongoose::http_response::set_content(const char* value,
                                          const std::size_t size)
{
  mg_send_data(conn_, value, size);
}
