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
  \file tws/wtss/utils.hpp

  \brief Utility functions for WTSS service.

  \author Gilberto Ribeiro de Queiroz
 */

#ifndef __TWS_WTSS_UTILS_HPP__
#define __TWS_WTSS_UTILS_HPP__

// TWS
//#include "config.hpp"
#include "../geoarray/data_types.hpp"

// RapidJSON
#include <rapidjson/document.h>

namespace tws
{
  namespace wtss
  {

    //! Read the list of timelines files
    /*!
      \exception tws::file_exists_error If the file doesn't exists.
      \exception tws::file_open_error   If the file can not be opened.
      \exception tws::parser_error      If the file has any syntax error.
     */
    //std::vector<std::pair<std::string, std::string> >
    //read_timelines_file_name(const std::string& input_file);

    //! Read the timeline from informed file.
    /*!
      \exception tws::file_exists_error If the file doesn't exists.
      \exception tws::file_open_error   If the file can not be opened.
      \exception tws::parser_error      If the file has any syntax error.
     */
    //std::vector<std::string> read_timeline(const std::string& input_file);

    //! Read the list of coverages from informed file.
    /*!
      \exception tws::file_exists_error If the file doesn't exists.
      \exception tws::file_open_error   If the file can not be opened.
      \exception tws::parser_error      If the file has any syntax error.
     */
    //std::vector<coverage_t> read_coverages(const std::string& input_file);

    //coverage_t read_coverage(const rapidjson::Value& jcoverage);

    //std::vector<coverage_attribute_t> read_coverage_attributes(const rapidjson::Value& jattributes);

    //coverage_attribute_t read_coverage_attribute(const rapidjson::Value& jattribute);

    void write(const tws::geoarray::geoarray_t& cv,
               rapidjson::Value& jcv,
               rapidjson::Document::AllocatorType& allocator);

    void write(const std::vector<tws::geoarray::attribute_t>& cv_attrs,
               rapidjson::Value& jcv_attrs,
               rapidjson::Document::AllocatorType& allocator);

    void write(const tws::geoarray::attribute_t& cv_attr,
               rapidjson::Value& jcv_attr,
               rapidjson::Document::AllocatorType& allocator);

    void write(const tws::geoarray::geo_extent_t& geo_ext,
               rapidjson::Value& jgeo_ext,
               rapidjson::Document::AllocatorType& allocator);

    void write(const std::vector<tws::geoarray::dimension_t>& cv_dimensions,
               rapidjson::Value& jcv_dimensions,
               rapidjson::Document::AllocatorType& allocator);

    void write(const tws::geoarray::dimension_t& dim,
               rapidjson::Value& jdim,
               rapidjson::Document::AllocatorType& allocator);

  } // end namespace wtss
}   // end namespace tws

#endif  // __TWS_WTSS_UTILS_HPP__
