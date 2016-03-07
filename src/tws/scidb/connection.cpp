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
  \file tws/scidb/connection.cpp

  \brief A class for representing database connections retrieved from connection pool.

  \author Gilberto Ribeiro de Queiroz
  \author Eduardo Llapa Rodriguez
  \author Luiz Fernando ferreira Gomes de Assis
 */

// TWS
#include "connection.hpp"
#include "connection_pool.hpp"
#include "exception.hpp"
#include "pool_connection.hpp"

tws::scidb::connection::~connection()
{
  connection_pool::instance().release(conn_);
}

boost::shared_ptr<scidb::QueryResult>
tws::scidb::connection::execute(const std::string& query_str, const bool afl)
{
  return conn_->execute(query_str, afl);
}

void
tws::scidb::connection::completed(::scidb::QueryID id)
{
  conn_->completed(id);
}

tws::scidb::connection::connection(pool_connection* conn_impl)
  : conn_(conn_impl)
{
}
