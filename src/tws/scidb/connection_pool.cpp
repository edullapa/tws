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
  \file tws/scidb/connection_pool.cpp

  \brief A connection pool for SciDB connections.

  \author Gilberto Ribeiro de Queiroz
  \author Eduardo Llapa Rodriguez
  \author Luiz Fernando ferreira Gomes de Assis
 */

// TWS
#include "connection_pool.hpp"
#include "connection.hpp"
#include "pool_connection.hpp"

// STL
#include <list>

// Boost
#include <boost/thread/locks.hpp>
#include <boost/thread/mutex.hpp>

struct tws::scidb::connection_pool::impl
{
  std::list<pool_connection*> connections;
  boost::mutex mtx;
};

std::unique_ptr<tws::scidb::connection>
tws::scidb::connection_pool::get()
{
  boost::lock_guard<boost::mutex> lock(pimpl_->mtx);

  if(pimpl_->connections.empty())
  {
    std::unique_ptr<pool_connection> pconn(new pool_connection("id", "local-server", "localhost", 1239));

    pconn->open();

    return std::unique_ptr<tws::scidb::connection>(new connection(pconn.release()));
  }

  pool_connection* conn = pimpl_->connections.front();

  pimpl_->connections.pop_front();

  return std::unique_ptr<tws::scidb::connection>(new connection(conn));
}

tws::scidb::connection_pool&
tws::scidb::connection_pool::instance()
{
  static connection_pool inst;

  return inst;
}

void
tws::scidb::connection_pool::release(pool_connection* conn)
{
  boost::lock_guard<boost::mutex> lock(pimpl_->mtx);

  pimpl_->connections.push_back(conn);
}

tws::scidb::connection_pool::connection_pool()
  : pimpl_(nullptr)
{
  pimpl_ = new impl;
}

tws::scidb::connection_pool::~connection_pool()
{
  delete pimpl_;
}

