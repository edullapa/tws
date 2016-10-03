/*
  Copyright (C) 2014 National Institute For Space Research (INPE) - Brazil.

  This file is part of the TerraLib GeoWeb Services.

  TerraLib GeoWeb Services is free software: you can redistribute it and/or
  modify
  it under the terms of the GNU General Public License version 3 as
  published by the Free Software Foundation.

  TerraLib GeoWeb Services is distributed  "AS-IS" in the hope that it will be
  useful,
  but WITHOUT ANY WARRANTY OF ANY KIND; without even the implied warranty
  of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
  GNU Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public License along
  with TerraLib Web Services. See COPYING. If not, see
  <http://www.gnu.org/licenses/lgpl-3.0.html>.
 */

/*!
  \file tws/mongoose/http_response.hpp

  \brief HTTP response implementaton for Moongose.

  \author Gilberto Ribeiro de Queiroz
 */

#ifndef __TWS_MONGOOSE_HTTP_RESPONSE_HPP__
#define __TWS_MONGOOSE_HTTP_RESPONSE_HPP__

// SciDB-WS
#include "../core/http_response.hpp"

// STL
#include <string>

// Forward declaration
extern "C" { struct mg_connection; }

namespace tws
{
  namespace mongoose
  {
    //! HTTP response implementaton for Moongose.
    class http_response : public tws::core::http_response
    {
     public:
      http_response(mg_connection* conn);

      ~http_response();
      void add_header(const char* key, const char* value);
      void set_content(const char* value, const std::size_t size);

     private:
      mg_connection* conn_;
      std::string headers_;
    };

  }  // end namespace mongoose
}  // end namespace tws

#endif  // __TWS_MONGOOSE_HTTP_RESPONSE_HPP__
