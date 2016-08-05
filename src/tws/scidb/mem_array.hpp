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
  \file tws/scidb/mem_array.hpp

  \brief A template class for 2D arrays.

  \author Gilberto Ribeiro de Queiroz
 */

#ifndef __TWS_SCIDB_MEM_ARRAY_HPP__
#define  __TWS_SCIDB_MEM_ARRAY_HPP__

// TWS
#include "cell_iterator.hpp"
#include "exception.hpp"

// SciDB
#include <SciDBAPI.h>

// STL
#include <vector>

namespace tws
{
  namespace scidb
  {
    template<typename T>
    struct assert_false : std::false_type
    {
    };

    template<typename T>
    struct get_value_selector
    {
      static T get(tws::scidb::cell_iterator& cit, std::size_t pos)
      {
         static_assert(assert_false<T>::value , "Erro in get_value_selector template specialization!");
      }
    };

    template<>
    struct get_value_selector<uint8_t>
    {
      static uint8_t get(tws::scidb::cell_iterator& cit, std::size_t pos)
      {
         return cit.get_uint8(pos);
      }
    };

//    template<>
//    struct get_value_selector<uint16_t>
//    {
//      static uint16_t get(tws::scidb::cell_iterator& cit, std::size_t pos)
//      {
//         return cit.get_uint16(pos);
//      }
//    };

    template<class T> struct array2d
    {
      int64_t first_col_idx;
      int64_t last_col_idx;
      int64_t first_row_idx;
      int64_t last_row_idx;
      std::size_t width;
      std::size_t height;
      std::size_t size;
      std::size_t num_attributes;
      std::vector<T*> attribute_data;

      array2d(int64_t first_col, int64_t last_col,
              int64_t first_row, int64_t last_row,
              std::size_t nattrs)
        : first_col_idx(first_col), last_col_idx(last_col),
          first_row_idx(first_row), last_row_idx(last_row),
          width(last_col - first_col + 1),
          height(last_row - first_row + 1),
          size(width * height),
          num_attributes(nattrs)
      {
        for(std::size_t i = 0; i != num_attributes; ++i)
          attribute_data.push_back(new T[size]);
      }

      ~array2d()
      {
        for(std::size_t i = 0; i != num_attributes; ++i)
          delete [] (attribute_data[i]);
      }

      void fill(tws::scidb::cell_iterator& cit)
      {
        while(!cit.end())
        {
          const ::scidb::Coordinates& coords = cit.get_position();

          int col = coords[0] - first_col_idx;
          int row = coords[1] - first_row_idx;

          for(std::size_t i = 0; i != num_attributes; ++i)
          {
            T v = get_value_selector<T>::get(cit, i);

            std::size_t pos = width * row + col;

            attribute_data[i][pos] = v;
          }

          ++cit;
        }
      }

    };

  }  // end namespace scidb
}    // end namespace tws

#endif  //  __TWS_SCIDB_MEM_ARRAY_HPP__

