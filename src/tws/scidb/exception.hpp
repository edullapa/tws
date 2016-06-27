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
  \file tws/scidb/exception.hpp

  \brief Specific exception types for SciDB module.

  \author Gilberto Ribeiro de Queiroz
  \author Eduardo Llapa Rodriguez
  \author Luiz Fernando ferreira Gomes de Assis
 */

#ifndef __TWS_SCIDB_EXCEPTION_HPP__
#define __TWS_SCIDB_EXCEPTION_HPP__

// TWS
#include "../exception.hpp"

namespace tws
{
  //! The namespace for the SciDB module of TerraLib Web Services.
  namespace scidb
  {

    //! The base type for SciDB module exceptions.
    struct exception: virtual tws::exception { };

    struct connection_error: virtual exception { };

    struct query_execution_error: virtual exception { };

  }  // end namespace scidb
}    // end namespace tws

#endif  // __TWS_SCIDB_EXCEPTION_HPP__

