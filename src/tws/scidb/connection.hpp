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
  \file tws/scidb/connection.hpp

  \brief A class for representing database connections retrieved from connection pool.

  \author Gilberto Ribeiro de Queiroz
  \author Eduardo Llapa Rodriguez
  \author Luiz Fernando ferreira Gomes de Assis
 */

#ifndef __TWS_SCIDB_CONNECTION_HPP__
#define __TWS_SCIDB_CONNECTION_HPP__

// TWS
#include "config.hpp"

// STL
#include <cstdint>
#include <string>

// Boost
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>

// SciDB
#include <SciDBAPI.h>

namespace tws
{
  namespace scidb
  {
    // Forward declaration
    class pool_connection;

    //! A class for representing database connections retrieved from connection pool.
    class connection : public boost::noncopyable
    {
      public:

        //! Destructor.
        ~connection();

        //! Executes a given query using AFL or AQL.
        /*!
          \exception query_execution_error Throws an exception if query can not be executed or if an error occurs.
         */
        boost::shared_ptr< ::scidb::QueryResult >
        execute(const std::string& query_str, const bool afl = true);

        void completed(::scidb::QueryID id);

      private:

        //! Constructor.
        connection(pool_connection* conn_impl);

      private:

        pool_connection* conn_;

      friend class connection_pool;
    };

  }  // end namespace scidb
}    // end namespace tws

#endif  // __TWS_SCIDB_CONNECTION_HPP__
