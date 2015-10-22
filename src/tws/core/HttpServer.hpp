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
  \file tws/core/http_server.hpp

  \brief Base class for HTTP servers.

  \author Gilberto Ribeiro de Queiroz
 */

#ifndef __TWS_CORE_HTTP_SERVER_HPP__
#define __TWS_CORE_HTTP_SERVER_HPP__

// TWS
#include "config.hpp"

// Boost
#include <boost/noncopyable.hpp>

namespace tws
{
  namespace core
  {

    //! Base class for HTTP servers.
    class http_server : public boost::noncopyable
    {
      public:

        //! Constructor.
        http_server() { }

        //! Virtual destructor.
        virtual ~http_server() { }

        //! Start the web server.
        /*!
          \excpetion tws::exception It may throws an exception.

          \note Not thread-safe.
         */
        virtual void start() = 0;

        //! Stop the web server.
        /*!
          This method may be called by a different thread.
         
          \excpetion tws::exception It may throws an exception.

          \note Thread-safe.
         */
        virtual void stop() = 0;
    };

  }   // end namespace core
}     // end namespace tws

#endif  // __TWS_CORE_HTTP_SERVER_HPP__
