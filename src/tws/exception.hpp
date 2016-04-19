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
  \file tws/exception.hpp

  \brief Base exception class exceptions in TerraLib Web Services.

  \author Gilberto Ribeiro de Queiroz
 */

#ifndef __TWS_EXCEPTION_HPP__
#define __TWS_EXCEPTION_HPP__

// STL
#include <stdexcept>
#include <string>

// Boost
#include <boost/exception/all.hpp>

//! TerraLib Web Services namespace.
namespace tws
{

  //! Base exception class for SciDB-WS.
  struct exception: virtual std::exception, virtual boost::exception { };

  //! An exception indicating that a given item was not found in a collection.
  struct item_not_found_error: virtual exception { };

  //! An exception indicating that a given item already exists in a collection.
  struct item_already_exists_error: virtual exception { };

  //! Type for handling missing parameter value in service request.
  struct missing_argument_error: virtual exception { };

  //! Type for handling invalid parameter value in service request.
  struct invalid_argument_error: virtual exception { };

  //! An exception indicating that an error in the plugin system.
  struct plugin_error: virtual exception { };
  
  //! An exception indicating that a given file was not found.
  struct file_exists_error: virtual exception { };

  //! An exception indicating that a given file could not be opened.
  struct file_open_error: virtual exception { };

  //! An exception indicating an error during parse.
  struct parse_error: virtual tws::exception { };

  //! An exception indicating a data type conversion error.
  struct conversion_error: virtual tws::exception { };
  
  //! An exception indicating that something is out-of-bounds.
  struct outof_bounds_error: virtual exception { };

  //! The base type for error report messages.
  typedef boost::error_info<struct tag_error_description, std::string> error_description;

}   // end namespace tws

#endif // __TWS_EXCEPTION_HPP__
