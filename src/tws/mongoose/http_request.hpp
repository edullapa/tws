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
  \file tws/mongoose/http_request.hpp

  \brief HTTP request implementation for Mongoose.

  \author Gilberto Ribeiro de Queiroz
 */

#ifndef __TWS_MONGOOSE_HTTP_REQUEST_HPP__
#define __TWS_MONGOOSE_HTTP_REQUEST_HPP__

// TWS
#include "../core/http_request.hpp"

// Forward declaration
extern "C" { struct mg_connection; }

namespace tws
{
  namespace mongoose
  {

    //! HTTP request implementation for Mongoose
    class http_request : public tws::core::http_request
    {
      public:

        http_request(mg_connection* conn);

        ~http_request();

        const char* method() const;

        const char* version() const;

        const char* base_uri() const;

        const char* query_string() const;

        const char* content() const;

        std::size_t content_length() const;
  
        const char* get_var(const char* value) const;

      private:

        mg_connection* conn_;
    };

  }   // end namespace mongoose
}     // end namespace tws

#endif  // __TWS_MONGOOSE_HTTP_REQUEST_HPP__
