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
  \file tws/scidb/pool_connection.cpp

  \brief A class for representing a database connection from a given pool.

  \author Gilberto Ribeiro de Queiroz
  \author Eduardo Llapa Rodriguez
  \author Luiz Fernando ferreira Gomes de Assis
 */

// TWS
#include "pool_connection.hpp"
#include "exception.hpp"

tws::scidb::pool_connection::pool_connection(const std::string& uuid,
                                             const std::string& instance_name,
                                             const std::string& coordinator_address,
                                             const uint16_t coordinator_port)
  : uuid_(uuid),
    instance_name_(instance_name),
    coordinator_address_(coordinator_address),
    handle_(nullptr),
    coordinator_port_(coordinator_port)
{
  if(coordinator_address.empty())
    throw tws::invalid_argument_error() << tws::error_description("invalid coordinator server address.");
  
  if(coordinator_port_ == 0)
    throw tws::invalid_argument_error() << tws::error_description("invalid coordinator server port number.");
}

tws::scidb::pool_connection::~pool_connection()
{
  close();
}

void*
tws::scidb::pool_connection::handle() const
{
  return handle_;
}

const std::string&
tws::scidb::pool_connection::id() const
{
  return uuid_;
}

void
tws::scidb::pool_connection::set_id(const std::string& uuid)
{
  uuid_ = uuid;
}

const std::string&
tws::scidb::pool_connection::instance_name() const
{
  return instance_name_;
}

const std::string&
tws::scidb::pool_connection::coordinator_address() const
{
  return coordinator_address_;
}

uint16_t
tws::scidb::pool_connection::coordinator_port() const
{
  return coordinator_port_;
}

void
tws::scidb::pool_connection::open()
{
  close();
  
  const ::scidb::SciDB& db_api = ::scidb::getSciDB();
  
  try
  {
    handle_ = db_api.connect(coordinator_address_, coordinator_port_);

    if(handle_ == nullptr)
      throw connection_error() << tws::error_description("could not connect to the coordinator server.");
  }
  catch(const ::scidb::Exception& e)
  {
    throw connection_error() << tws::error_description(std::string(e.what()));
  }
  catch(...)
  {
    throw connection_error() << tws::error_description("could not connect to the coordinator server.");
  }
}

void
tws::scidb::pool_connection::close()
{
  if(handle_)
  {
    const ::scidb::SciDB& db_api = ::scidb::getSciDB();
    
    try
    {
      db_api.disconnect(handle_);
    }
    catch(const ::scidb::Exception& e)
    {
      throw connection_error() << tws::error_description(std::string(e.what()));
    }
    catch(...)
    {
      throw connection_error() << tws::error_description("could not close connection with coordinator server.");
    }
    
    handle_ = nullptr;
  }
}

bool
tws::scidb::pool_connection::is_open() const
{
  return handle_ != nullptr;
}

bool
tws::scidb::pool_connection::is_closed() const
{
  return handle_ == nullptr;
}

boost::shared_ptr<scidb::QueryResult>
tws::scidb::pool_connection::execute(const std::string& query_str, const bool afl)
{
  boost::shared_ptr< ::scidb::QueryResult > qresult(new ::scidb::QueryResult);

  const ::scidb::SciDB& db_api = ::scidb::getSciDB();

  try
  {
    db_api.executeQuery(query_str, afl, *qresult, handle_);
  }
  catch(const ::scidb::Exception& e)
  {
    throw query_execution_error() << tws::error_description(e.what());
  }
  catch(...)
  {
    throw query_execution_error() << tws::error_description("could not execute query.");
  }

  return qresult;
}

void
tws::scidb::pool_connection::completed(::scidb::QueryID id)
{
  const ::scidb::SciDB& db_api = ::scidb::getSciDB();

  db_api.completeQuery(id, handle_);
}
