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
  \file tws/scidb/utils.cpp

  \brief Utility functions.

  \author Gilberto Ribeiro de Queiroz
 */

// TWS
#include "exception.hpp"
#include "utils.hpp"

#define TWS_FILL_VECTOR(values, array_it, get_name) \
  while(!array_it->end()) \
  { \
    const ::scidb::ConstChunk& chunk = array_it->getChunk(); \
  \
    std::shared_ptr< ::scidb::ConstChunkIterator > chunk_it = chunk.getConstIterator(); \
  \
    while(!chunk_it->end()) \
    { \
      const ::scidb::Value& v = chunk_it->getItem(); \
  \
      values.push_back(v.get_name()); \
  \
      ++(*chunk_it); \
    } \
  \
    ++(*array_it); \
 \
  }

inline void
tws_scidb_fill_int8(std::vector<double>& values, ::scidb::ConstArrayIterator* it)
{
  TWS_FILL_VECTOR(values, it, getInt8)
}

inline void
tws_scidb_fill_uint8(std::vector<double>& values, ::scidb::ConstArrayIterator* it)
{
  TWS_FILL_VECTOR(values, it, getUint8)
}

inline void
tws_scidb_fill_int16(std::vector<double>& values, ::scidb::ConstArrayIterator* it)
{
  TWS_FILL_VECTOR(values, it, getInt16)
}

inline void
tws_scidb_fill_uint16(std::vector<double>& values, ::scidb::ConstArrayIterator* it)
{
  TWS_FILL_VECTOR(values, it, getUint16)
}

inline void
tws_scidb_fill_int32(std::vector<double>& values, ::scidb::ConstArrayIterator* it)
{
  TWS_FILL_VECTOR(values, it, getInt32)
}

inline void
tws_scidb_fill_uint32(std::vector<double>& values, ::scidb::ConstArrayIterator* it)
{
  TWS_FILL_VECTOR(values, it, getUint32)
}

inline void
tws_scidb_fill_int64(std::vector<double>& values, ::scidb::ConstArrayIterator* it)
{
  TWS_FILL_VECTOR(values, it, getInt64)
}

inline void
tws_scidb_fill_uint64(std::vector<double>& values, ::scidb::ConstArrayIterator* it)
{
  TWS_FILL_VECTOR(values, it, getUint64)
}

inline void
tws_scidb_fill_float(std::vector<double>& values, ::scidb::ConstArrayIterator* it)
{
  TWS_FILL_VECTOR(values, it, getFloat)
}

inline void
tws_scidb_fill_double(std::vector<double>& values, ::scidb::ConstArrayIterator* it)
{
  TWS_FILL_VECTOR(values, it, getDouble)
}

void
tws::scidb::fill(std::vector<double>& values, ::scidb::ConstArrayIterator* it, const ::scidb::TypeId& id)
{
  if(id == ::scidb::TID_INT8)
    tws_scidb_fill_int8(values, it);
  else if(id == ::scidb::TID_UINT8)
    tws_scidb_fill_uint8(values, it);
  else if(id == ::scidb::TID_INT16)
    tws_scidb_fill_int16(values, it);
  else if(id == ::scidb::TID_UINT16)
    tws_scidb_fill_uint16(values, it);
  else if(id == ::scidb::TID_INT32)
    tws_scidb_fill_int32(values, it);
  else if(id == ::scidb::TID_UINT32)
    tws_scidb_fill_uint32(values, it);
  else if(id == ::scidb::TID_INT64)
      tws_scidb_fill_int64(values, it);
    else if(id == ::scidb::TID_UINT64)
      tws_scidb_fill_uint64(values, it);
  else if(id == ::scidb::TID_FLOAT)
    tws_scidb_fill_float(values, it);
  else if(id == ::scidb::TID_DOUBLE)
    tws_scidb_fill_double(values, it);
  else
    throw tws::conversion_error() << tws::error_description("Could not fill values vector with iterator items: data type not supported.");
}
