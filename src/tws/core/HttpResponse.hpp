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
  \file tws/core/HttpResponse.hpp

  \brief Base class that models an HTTP response.

  \author Gilberto Ribeiro de Queiroz
 */

#ifndef __TWS_CORE_HTTPRESPONSE_HPP__
#define __TWS_CORE_HTTPRESPONSE_HPP__

// TWS
#include "config.hpp"

// Boost
#include <boost/noncopyable.hpp>

namespace tws
{
  namespace core
  {

    //! Base class that models an HTTP response.
    class http_response : public boost::noncopyable
    {
      public:

        //! Constructor.
        http_response() { }

        //! Virtual destructor.
        virtual ~http_response() { }

        //! Add a header to the response.
        virtual void add_header(const char* key, const char* value) = 0;

        //! Set the content in the response.
        virtual void set_content(const char* value, const std::size_t size) = 0;
    };

  }   // end namespace core
}     // end namespace tws

#endif  // __TWS_CORE_HTTPRESPONSE_HPP__
