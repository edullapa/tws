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
  \file tws/core/http_request.hpp

  \brief Base class that models an HTTP request.

  \author Gilberto Ribeiro de Queiroz
 */

#ifndef __TWS_CORE_HTTP_REQUEST_HPP__
#define __TWS_CORE_HTTP_REQUEST_HPP__

// TWS
#include "config.hpp"

// Boost
#include <boost/noncopyable.hpp>

namespace tws
{
  namespace core
  {

    //! Base class that models an HTTP request.
    class http_request : public boost::noncopyable
    {
      public:

        //! Constructor.
        http_request() { }

        //! Virtual destructor.
        virtual ~http_request() { }

        //! The request method: GET, POST, HEAD, PUT or DELETE.
        virtual const char* method() const = 0;

        //! Requested HTTP version: 1.0 or 1.1.
        virtual const char* version() const = 0;

        //! The base URI (already decoded) without the query string part.
        virtual const char* base_uri() const = 0;

        //! The query string part in the URI (the URI part after '?' not including '?'). May be NULL.
        virtual const char* query_string() const = 0;

        //! The request content.
        virtual const char* content() const = 0;

        //! The request content length.
        virtual std::size_t content_length() const = 0;

        //! Get a specific variable inside the request.
        virtual const char* get_var(const char* value) const = 0;
    };

  }   // end namespace core
}     // end namespace tws

#endif  // __TWS_CORE_HTTP_REQUEST_HPP__
