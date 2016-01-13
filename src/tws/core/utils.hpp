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
  \file tws/core/Utils.hpp

  \brief Utility functions.

  \author Gilberto Ribeiro de Queiroz
 */

#ifndef __TWS_CORE_UTILS_HPP__
#define __TWS_CORE_UTILS_HPP__

// TWS
#include "Config.hpp"
#include "Exception.hpp"

// STL
#include <string>

// Boost
#include <boost/tokenizer.hpp>

namespace tws
{
  namespace core
  {
    //! Query string.
    typedef std::map<std::string, std::string> query_string_t;

    //! Split a query string into its components (the query string must be hex encoded).
    /*!
      \exception tws::parser_error If a parser error occurs during decomposition of query string.
     */
    inline query_string_t expand(const std::string& query_str)
    {
      query_string_t result;

      boost::char_separator<char> kvp_sep("&");

      boost::tokenizer<boost::char_separator<char> > key_value_pairs(query_str, kvp_sep);

      boost::tokenizer<boost::char_separator<char> >::iterator ikv_pair = key_value_pairs.begin();

      while(ikv_pair != key_value_pairs.end())
      {
        std::string kv = *ikv_pair;

        boost::char_separator<char> kv_sep("=");

        boost::tokenizer<boost::char_separator<char> > key_value_pair(kv, kv_sep);

        boost::tokenizer<boost::char_separator<char> >::iterator ikv = key_value_pair.begin();

        if(ikv == key_value_pair.end())
          throw tws::parser_error() << tws::error_description("invalid key-value pair format in query string!");

        std::string k = *ikv;

        ++ikv;

        if(ikv == key_value_pair.end())
          throw tws::parser_error() << error_description("invalid key-value pair format in query string!");

        std::string v = *ikv;

        //std::string decoded_k;
        //std::string decoded_v;

        //boost::network::uri::decode(k.begin(), k.end(), std::back_inserter(decoded_k));
        //boost::network::uri::decode(v.begin(), v.end(), std::back_inserter(decoded_v));

        //result[decoded_k] = decoded_v;
        result[k] = v;

        ++ikv_pair;
      }

      return result;
    }

    //! This routine must be called once at application startup in order to initialize TWS.
    /*!
      \exception tws::exception It may throw exceptions.

      \note Not thread-safe.
     */
    void init_terralib_web_services();

    //! This routine must be called once at application end in order to correctly make a cleanup of TWS.
    /*!
      \exception tws::exception It may throw exceptions.

      \note Not thread-safe.
     */
    void shutdown_terralib_web_services();

    //! Returns the path relative to a directory or file in the context of the application.
    /*!
      If the path is not found it returns an empty string.
     */
    std::string find_in_app_path(const std::string& p);

  }   // end namespace core
}     // end namespace tws

#endif  // __TWS_CORE_UTILS_HPP__
