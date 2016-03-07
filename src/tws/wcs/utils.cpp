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
  \file tws/wcs/utils.cpp

  \brief Utils for WCS module.

  \author Raphael Willian da Costa
 */

// TWS
#include "utils.hpp"
#include "../core/utils.hpp"
#include "../core/http_request.hpp"
#include "../core/http_response.hpp"


rapidxml::xml_node<char>* tws::wcs::initialize_wcs_xml_namespaces(rapidxml::xml_document<char>& doc)
{
  rapidxml::xml_node<> *root = doc.allocate_node(rapidxml::node_element, "wcs:CoverageDescriptions");

  rapidxml::xml_attribute<>* attr = doc.allocate_attribute("xmlns:wcs", "http://www.opengis.net/wcs/2.0");
  root->append_attribute(attr);

  attr = doc.allocate_attribute("xmlns:ows", "http://www.opengis.net/ows/2.0");
  root->append_attribute(attr);

  attr = doc.allocate_attribute("xmlns:gml", "http://www.opengis.net/gml/3.2");
  root->append_attribute(attr);

  attr = doc.allocate_attribute("xmlns:gmlcov", "http://www.opengis.net/gmlcov/1.0");
  root->append_attribute(attr);

  attr = doc.allocate_attribute("xmlns:xlink", "http://www.w3.org/1999/xlink");
  root->append_attribute(attr);

  attr = doc.allocate_attribute("xmlns:xsi", "http://www.w3.org/2001/XMLSchema-instance");
  root->append_attribute(attr);

  attr = doc.allocate_attribute("xmlns:swe", "http://www.opengis.net/swe/2.0");
  root->append_attribute(attr);

  attr = doc.allocate_attribute("version", "2.0.1");
  root->append_attribute(attr);

  attr = doc.allocate_attribute("xsi:schemaLocation", "http://www.opengis.net/wcs/2.0 http://schemas.opengis.net/wcs/2.0/wcsGetCapabilities.xsd");
  root->append_attribute(attr);

  return root;
}

bool tws::wcs::validate_request_parameters(const tws::core::http_request &request, const tws::core::http_response &response, const std::string &operation)
{
  const char* query_string = request.query_string();
  if (query_string == nullptr)
  {
    // TODO: wcs xml exception
    throw tws::missing_argument_error() << tws::error_description("Arguments missing. OGC services requires at least three arguments: \"service\", \"version\" e \"request\"");
  }

  tws::core::query_string_t arguments = tws::core::expand(query_string);

  // Service Validation
  const auto& coverage_id = arguments.find("service");
  if (coverage_id == arguments.end())
    throw tws::missing_argument_error() << tws::error_description("Missing \"coverageID\" argument");

  if (coverage_id->second != "WCS")
    throw tws::invalid_argument_error() << tws::error_description("Invalid parameter \"service\" value. It should be \"WCS\"");

  // Version Validation
  const auto& version = arguments.find("version");
  if (version == arguments.end())
    throw tws::missing_argument_error() << tws::error_description("Missing \"version\" argument");

  if (version->second != "2.0.1" && version->second != "2.0")
    throw tws::invalid_argument_error() << tws::error_description("Invalid parameter \"version\" value. It should be \"2.0\"");

  // Request Validation
  const auto& request_param = arguments.find("request");
  if (request_param == arguments.end())
    throw tws::missing_argument_error() << tws::error_description("Missing \"request\" argument");

  if (request_param->second != operation)
    throw tws::invalid_argument_error() << tws::error_description("Invalid parameter \"request\" value.");

  return false;
}
