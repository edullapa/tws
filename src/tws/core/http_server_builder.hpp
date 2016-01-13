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
  \file tws/core/http_server_builder.hpp

  \brief A singleton for registering available HTTP server implementation builders.

  \author Gilberto Ribeiro de Queiroz
 */

#ifndef __TWS_CORE_HTTP_SERVER_BUILDER_HPP__
#define __TWS_CORE_HTTP_SERVER_BUILDER_HPP__

// TWS
#include "config.hpp"

// STL
#include <memory>
#include <string>

// Boost
#include <boost/noncopyable.hpp>
#include <boost/function.hpp>

namespace tws
{
  namespace core
  {

// Forward declaration
    class http_server;

    //! The type of builder functions and functors.
    typedef boost::function0< std::unique_ptr<http_server> > http_server_builder_functor_t;

    //! A singleton for registering available HTTP server implementations.
    class http_server_builder : public boost::noncopyable
    {
      public:

        //! Make a new web server of informed implementation type.
        /*!
          \exception invalid_http_server_build_error It may throws an exception if a builder is not found or it ay not create the web server.
         */
        std::unique_ptr<http_server> build(const std::string& builder_id);

        //! Register a new builder of http_server.
        /*!
          \exception invalid_http_server_builder_error It may throws an exception if the builder can not be registered.

          \note Not thread-safe.
         */
        void insert(const std::string& builder_id,
                    http_server_builder_functor_t builder_fnct);

        //! unregister the http server builder identified by the given id.
        /*!
          \exception tws::item_not_found_error If a builder with the given id is not found.
         */
        void remove(const std::string& builder_id);

        //! Access the singleton.
        static http_server_builder& instance();

      private:

        http_server_builder();

        ~http_server_builder();

      private:

        struct impl;
      
        impl* pimpl_;
    };

  }   // end namespace core
}     // end namespace tws

#endif  // __TWS_CORE_HTTP_SERVER_BUILDER_HPP__
