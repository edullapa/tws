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
  \file tws/scidb/pool_connection.hpp

  \brief A class for representing a database connection from a given pool.

  \author Gilberto Ribeiro de Queiroz
  \author Eduardo Llapa Rodriguez
  \author Luiz Fernando ferreira Gomes de Assis
 */

#ifndef __TWS_SCIDB_POOL_CONNECTION_HPP__
#define __TWS_SCIDB_POOL_CONNECTION_HPP__

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

    //! A class for representing a database connection from a given pool.
    class pool_connection : public boost::noncopyable
    {
      public:

        //! Constructor.
        pool_connection(const std::string& uuid,
                        const std::string& instance_name,
                        const std::string& coordinator_address = "localhost",
                        const uint16_t coordinator_port = 1239);

        //! Destructor.
        ~pool_connection();

        //! Returns the connection handle.
        void* handle() const;

        //! Returns the SciDB Instance UUID.
        const std::string& id() const;

        //! Set the connection unique identifier.
        void set_id(const std::string& uuid);

        //! Returns the SciDB Instance name given by the user.
        const std::string& instance_name() const;

        //! Returns the address or host name of the coordinator server.
        const std::string& coordinator_address() const;

        //! Returns the port used by the coordinator server.
        uint16_t coordinator_port() const;

        //! Try to stablish the connection to the database instance.
        /*!
          \exception connection_error It throws an exception if the connection can not be established.
         */
        void open();

        //! Close the connection with the database instance.
        /*!
          \exception connection_error It throws an exception if the connection can not be closed.
         */
        void close();

        //! Returns true if the connection is opened.
        bool is_open() const;

        //! Returns true if the connection is closed.
        bool is_closed() const;

        //! Executes a given query using AFL or AQL.
        /*!
          \exception query_execution_error Throws an exception if query can not be executed or if an error occurs.
         */
        boost::shared_ptr< ::scidb::QueryResult >
        execute(const std::string& query_str, const bool afl = true);

        void completed(::scidb::QueryID id);

      private:

        std::string uuid_;                  //!< UUID.
        std::string instance_name_;         //!< name given by the user to identify the SciDB Instance.
        std::string coordinator_address_;   //!< address or host name of the coordinator server.
        void* handle_;                      //!< connection handle.
        uint16_t coordinator_port_;         //!< port used by the coordinator server.
    };

  }  // end namespace scidb
}    // end namespace tws

#endif  // __TWS_SCIDB_POOL_CONNECTION_HPP__

