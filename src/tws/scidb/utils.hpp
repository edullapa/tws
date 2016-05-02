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
  \file tws/scidb/utils.hpp

  \brief Utility functions.

  \author Gilberto Ribeiro de Queiroz
 */

#ifndef __TWS_SCIDB_UTILS_HPP__
#define __TWS_SCIDB_UTILS_HPP__

// TWS
#include "config.hpp"

// SciDB
#include <SciDBAPI.h>

namespace tws
{
  namespace scidb
  {

    void fill(std::vector<double>& values, ::scidb::ConstArrayIterator* it, const ::scidb::TypeId& id);

//    void fill_int8(std::vector<double>& values, ::scidb::ConstItemIterator& it);
//
//    void fill_uint8(std::vector<double>& values, ::scidb::ConstItemIterator& it);
//
//    void fill_int16(std::vector<double>& values, ::scidb::ConstItemIterator& it);
//
//    void fill_uint16(std::vector<double>& values, ::scidb::ConstItemIterator& it);
//
//    void fill_int32(std::vector<double>& values, ::scidb::ConstItemIterator& it);
//
//    void fill_uint32(std::vector<double>& values, ::scidb::ConstItemIterator& it);
//
//    void fill_int64(std::vector<double>& values, ::scidb::ConstItemIterator& it);
//
//    void fill_uint64(std::vector<double>& values, ::scidb::ConstItemIterator& it);
//
//    void fill_float(std::vector<double>& values, ::scidb::ConstItemIterator& it);
//
//    void fill_double(std::vector<double>& values, ::scidb::ConstItemIterator& it);

  }   // end namespace scidb
}     // end namespace tws

#endif  // __TWS_SCIDB_UTILS_HPP__
