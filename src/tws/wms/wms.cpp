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
  \file tws/wms/wms.cpp

  \brief Web Map Service.

  \author Roger Victor
 */

// TWS
#include "wms.hpp"
#include "data_types.hpp"
#include "wms_manager.hpp"
#include "../core/http_request.hpp"
#include "../core/http_response.hpp"
#include "../core/service_operations_manager.hpp"
#include "../core/utils.hpp"
#include "../geoarray/geoarray_manager.hpp"

// STL
#include <algorithm>
#include <memory>

// Boost
#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/format.hpp>
#include <boost/lexical_cast.hpp>

// SciDB
//#include <SciDBAPI.h>

// RapidXml
#include <rapidxml/rapidxml.hpp>
#include <rapidxml/rapidxml_print.hpp>

void
tws::wms::get_capabilities_functor::operator()(const tws::core::http_request& request,
                                               tws::core::http_response& response)
{
  capabilities_t capabilities = tws::wms::wms_manager::instance().capabilities();
  
  try
  {
    // create WMS_Capabilities document
    rapidxml::xml_document<> doc;

    // xml declaration
    rapidxml::xml_node<>* decl = doc.allocate_node(rapidxml::node_declaration);
    decl->append_attribute(doc.allocate_attribute("version", "1.0"));
    decl->append_attribute(doc.allocate_attribute("encoding", "utf-8"));
    doc.append_node(decl);
    
    // root node
    rapidxml::xml_node<>* root = doc.allocate_node(rapidxml::node_element, "WMS_Capabilities");
    
    rapidxml::xml_attribute<>* attr = doc.allocate_attribute("version", "1.3.0");
    root->append_attribute(attr);

    attr = doc.allocate_attribute("xmlns", "http://www.opengis.net/wms");
    root->append_attribute(attr);
    
    attr = doc.allocate_attribute("xmlns:xlink", "http://www.w3.org/1999/xlink");
    root->append_attribute(attr);

    attr = doc.allocate_attribute("xmlns:xsi", "http://www.w3.org/2001/XMLSchema-instance");
    root->append_attribute(attr);

    attr = doc.allocate_attribute("xsi:schemaLocation", "http://www.opengis.net/wms http://schemas.opengis.net/wms/1.3.0/capabilities_1_3_0.xsd");
    root->append_attribute(attr);

    {
      // Service node (service metadata)
      rapidxml::xml_node<>* service_node = doc.allocate_node(rapidxml::node_element, "Service");

      {
        // Temp code. This metadata should be get from json file
        rapidxml::xml_node<>* node = doc.allocate_node(rapidxml::node_element, "Name", capabilities.service.name.c_str());

        service_node->append_node(node);
        
        node = doc.allocate_node(rapidxml::node_element, "Title", capabilities.service.title.c_str());

        service_node->append_node(node);

        node = doc.allocate_node(rapidxml::node_element, "Abstract", capabilities.service.abstract.c_str());

        service_node->append_node(node);

        // KeywordList node
        rapidxml::xml_node<>* keyword_list_node = doc.allocate_node(rapidxml::node_element, "KeywordList");

        for(auto keyword: capabilities.service.keyword_list)
        {
          keyword_list_node->append_node(doc.allocate_node(rapidxml::node_element, "Keyword", keyword.vocabulary.c_str()));
        }

        service_node->append_node(keyword_list_node);

        node = doc.allocate_node(rapidxml::node_element, "OnlineResource");

        rapidxml::xml_attribute<>* attr = doc.allocate_attribute("xmlns:xlink", "http://www.w3.org/1999/xlink");
        node->append_attribute(attr);

        attr = doc.allocate_attribute("xlink:type", capabilities.service.online_resource.xlink_type.c_str());
        node->append_attribute(attr);

        attr = doc.allocate_attribute("xlink:href", capabilities.service.online_resource.xlink_href.c_str());
        node->append_attribute(attr);

        service_node->append_node(node);

        // Contact Information node
        rapidxml::xml_node<>* contact_information_node = doc.allocate_node(rapidxml::node_element, "ContactInformation");

        {
          // Temp code. This metadata should be get from json file
          rapidxml::xml_node<>* contact_person_node = doc.allocate_node(rapidxml::node_element, "ContactPersonPrimary");

          rapidxml::xml_node<>* node = doc.allocate_node(rapidxml::node_element, "ContactPerson", capabilities.service.contact_information.contact_person_primary.contact_person.c_str());

          contact_person_node->append_node(node);

          node = doc.allocate_node(rapidxml::node_element, "ContactOrganization", capabilities.service.contact_information.contact_person_primary.contact_organization.c_str());

          contact_person_node->append_node(node);

          contact_information_node->append_node(contact_person_node);

          node = doc.allocate_node(rapidxml::node_element, "ContactPosition", capabilities.service.contact_information.contact_position.c_str());

          contact_information_node->append_node(node);

          rapidxml::xml_node<>* contact_address_node = doc.allocate_node(rapidxml::node_element, "ContactAddress");

          node = doc.allocate_node(rapidxml::node_element, "AddressType", capabilities.service.contact_information.contact_address.address_type.c_str());

          contact_address_node->append_node(node);

          node = doc.allocate_node(rapidxml::node_element, "Address", capabilities.service.contact_information.contact_address.address.c_str());

          contact_address_node->append_node(node);

          node = doc.allocate_node(rapidxml::node_element, "City", capabilities.service.contact_information.contact_address.city.c_str());

          contact_address_node->append_node(node);

          node = doc.allocate_node(rapidxml::node_element, "StateOrProvince", capabilities.service.contact_information.contact_address.state_or_province.c_str());

          contact_address_node->append_node(node);

          node = doc.allocate_node(rapidxml::node_element, "PostCode", capabilities.service.contact_information.contact_address.post_code.c_str());

          contact_address_node->append_node(node);

          node = doc.allocate_node(rapidxml::node_element, "Country", capabilities.service.contact_information.contact_address.country.c_str());

          contact_address_node->append_node(node);

          contact_information_node->append_node(contact_address_node);

          node = doc.allocate_node(rapidxml::node_element, "ContactVoiceTelephone", capabilities.service.contact_information.contact_voice_telephone.c_str());

          contact_information_node->append_node(node);

          node = doc.allocate_node(rapidxml::node_element, "ContactElectronicMailAddress", capabilities.service.contact_information.contact_electronic_mail_address.c_str());

          contact_information_node->append_node(node);
        }

        service_node->append_node(contact_information_node);

        node = doc.allocate_node(rapidxml::node_element, "Fees", capabilities.service.fees.c_str());

        service_node->append_node(node);

        node = doc.allocate_node(rapidxml::node_element, "AccessConstraints", capabilities.service.access_constraints.c_str());

        service_node->append_node(node);

        node = doc.allocate_node(rapidxml::node_element, "LayerLimit", std::to_string(capabilities.service.layer_limit).c_str());

        service_node->append_node(node);

        node = doc.allocate_node(rapidxml::node_element, "MaxWidth", std::to_string(capabilities.service.max_width).c_str());

        service_node->append_node(node);

        node = doc.allocate_node(rapidxml::node_element, "MaxHeight", std::to_string(capabilities.service.max_height).c_str());

        service_node->append_node(node);
      }

      // Capability node
      rapidxml::xml_node<>* capability_node = doc.allocate_node(rapidxml::node_element, "Capability");

      {
        // Request node
        rapidxml::xml_node<>* request_node = doc.allocate_node(rapidxml::node_element, "Request");

        {
          // GetCapabilities node
          rapidxml::xml_node<>* get_capabilities_node = doc.allocate_node(rapidxml::node_element, "GetCapabilities");

          {
            // Temp code. This metadata should be get from json file
            for(auto format: capabilities.capability.request.get_capabilities.format)
            {
              get_capabilities_node->append_node(doc.allocate_node(rapidxml::node_element, "Format", format.c_str()));
            }

            rapidxml::xml_node<>* dcp_type_node = doc.allocate_node(rapidxml::node_element, "DCPType");

            rapidxml::xml_node<>* http_node = doc.allocate_node(rapidxml::node_element, "HTTP");

            rapidxml::xml_node<>* get_node = doc.allocate_node(rapidxml::node_element, "Get");
            {
              rapidxml::xml_node<>* node = doc.allocate_node(rapidxml::node_element, "OnlineResource");

              rapidxml::xml_attribute<>* attr = doc.allocate_attribute("xlink:type", capabilities.capability.request.get_capabilities.dcp_type.http.get.xlink_type.c_str());
              node->append_attribute(attr);

              attr = doc.allocate_attribute("xlink:href", capabilities.capability.request.get_capabilities.dcp_type.http.get.xlink_href.c_str());
              node->append_attribute(attr);

              get_node->append_node(node);
            }

            http_node->append_node(get_node);

            rapidxml::xml_node<>* post_node = doc.allocate_node(rapidxml::node_element, "Post");
            {
              rapidxml::xml_node<>* node = doc.allocate_node(rapidxml::node_element, "OnlineResource");

              rapidxml::xml_attribute<>* attr = doc.allocate_attribute("xlink:type", capabilities.capability.request.get_capabilities.dcp_type.http.post.xlink_type.c_str());
              node->append_attribute(attr);

              attr = doc.allocate_attribute("xlink:href", capabilities.capability.request.get_capabilities.dcp_type.http.post.xlink_href.c_str());
              node->append_attribute(attr);

              post_node->append_node(node);
            }

            http_node->append_node(post_node);

            dcp_type_node->append_node(http_node);

            get_capabilities_node->append_node(dcp_type_node);
          }

          request_node->append_node(get_capabilities_node);

          // GetMap node
          rapidxml::xml_node<>* get_map_node = doc.allocate_node(rapidxml::node_element, "GetMap");

          {
            // Temp code. This metadata should be get from json file
            for(auto format: capabilities.capability.request.get_map.format)
            {
              get_map_node->append_node(doc.allocate_node(rapidxml::node_element, "Format", format.c_str()));
            }

            rapidxml::xml_node<>* dcp_type_node = doc.allocate_node(rapidxml::node_element, "DCPType");

            rapidxml::xml_node<>* http_node = doc.allocate_node(rapidxml::node_element, "HTTP");

            rapidxml::xml_node<>* get_node = doc.allocate_node(rapidxml::node_element, "Get");

            rapidxml::xml_node<>* node = doc.allocate_node(rapidxml::node_element, "OnlineResource");

            rapidxml::xml_attribute<>* attr = doc.allocate_attribute("xlink:type", capabilities.capability.request.get_map.dcp_type.http.get.xlink_type.c_str());
            node->append_attribute(attr);

            attr = doc.allocate_attribute("xlink:href", capabilities.capability.request.get_map.dcp_type.http.get.xlink_href.c_str());
            node->append_attribute(attr);

            get_node->append_node(node);

            http_node->append_node(get_node);

            dcp_type_node->append_node(http_node);

            get_map_node->append_node(dcp_type_node);
          }

          request_node->append_node(get_map_node);

          // GetFeatureInfo node
          rapidxml::xml_node<>* get_feature_info_node = doc.allocate_node(rapidxml::node_element, "GetFeatureInfo");

          {
            // Temp code. This metadata should be get from json file
            for(auto format: capabilities.capability.request.get_feature_info.format)
            {
              get_feature_info_node->append_node(doc.allocate_node(rapidxml::node_element, "Format", format.c_str()));
            }

            rapidxml::xml_node<>* dcp_type_node = doc.allocate_node(rapidxml::node_element, "DCPType");

            rapidxml::xml_node<>* http_node = doc.allocate_node(rapidxml::node_element, "HTTP");

            rapidxml::xml_node<>* get_node = doc.allocate_node(rapidxml::node_element, "Get");

            rapidxml::xml_node<>* node = doc.allocate_node(rapidxml::node_element, "OnlineResource");

            rapidxml::xml_attribute<>* attr = doc.allocate_attribute("xlink:type", capabilities.capability.request.get_feature_info.dcp_type.http.get.xlink_type.c_str());
            node->append_attribute(attr);

            attr = doc.allocate_attribute("xlink:href", capabilities.capability.request.get_feature_info.dcp_type.http.get.xlink_href.c_str());
            node->append_attribute(attr);

            get_node->append_node(node);

            http_node->append_node(get_node);

            dcp_type_node->append_node(http_node);

            get_feature_info_node->append_node(dcp_type_node);
          }

          request_node->append_node(get_feature_info_node);
        }

        capability_node->append_node(request_node);

        // Exception node
        rapidxml::xml_node<>* exception_node = doc.allocate_node(rapidxml::node_element, "Exception");

        {
          // Temp code. This metadata should be get from json file
          for(auto format: capabilities.capability.exception_formats)
          {
            exception_node->append_node(doc.allocate_node(rapidxml::node_element, "Format", format.c_str()));
          }
        }

        capability_node->append_node(exception_node);

        // Layer node
        rapidxml::xml_node<>* layer_node = doc.allocate_node(rapidxml::node_element, "Layer");

        {
          // Temp code. This metadata should be get from json file
          rapidxml::xml_node<>* node = doc.allocate_node(rapidxml::node_element, "Name", capabilities.capability.layer.name.c_str());

          layer_node->append_node(node);

          /*

          node = doc.allocate_node(rapidxml::node_element, "Title", capabilities.capability.layer.title.c_str());

          layer_node->append_node(node);

          node = doc.allocate_node(rapidxml::node_element, "Abstract", capabilities.capability.layer.abstract.c_str());

          layer_node->append_node(node);

          // KeywordList node
          rapidxml::xml_node<>* keyword_list_node = doc.allocate_node(rapidxml::node_element, "KeywordList");

          for(auto keyword: capabilities.capability.layer.keyword_list)
          {
            keyword_list_node->append_node(doc.allocate_node(rapidxml::node_element, "Keyword", keyword.vocabulary.c_str()));
          }

          layer_node->append_node(keyword_list_node);

          // CRS node
          for(auto crs: capabilities.capability.layer.crs)
          {
            layer_node->append_node(doc.allocate_node(rapidxml::node_element, "CRS", crs.c_str()));
          }

          // EX_GeographicBoundingBox node
          rapidxml::xml_node<>* ex_geographic_bounding_box_node = doc.allocate_node(rapidxml::node_element, "EX_GeographicBoundingBox");

          ex_geographic_bounding_box_node->append_node(doc.allocate_node(rapidxml::node_element, "westBoundLongitude", std::to_string(capabilities.capability.layer.ex_geographic_bounding_box.west_bound_longitude).c_str()));

          ex_geographic_bounding_box_node->append_node(doc.allocate_node(rapidxml::node_element, "eastBoundLongitude", std::to_string(capabilities.capability.layer.ex_geographic_bounding_box.east_bound_longitude).c_str()));

          ex_geographic_bounding_box_node->append_node(doc.allocate_node(rapidxml::node_element, "southBoundLatitude", std::to_string(capabilities.capability.layer.ex_geographic_bounding_box.south_bound_latitude).c_str()));

          ex_geographic_bounding_box_node->append_node(doc.allocate_node(rapidxml::node_element, "northBoundLatitude", std::to_string(capabilities.capability.layer.ex_geographic_bounding_box.north_bound_latitude).c_str()));

          layer_node->append_node(ex_geographic_bounding_box_node);

          // BoundingBox node
          for(auto bounding_box: capabilities.capability.layer.bounding_box)
          {
            rapidxml::xml_node<>* bounding_box_node = doc.allocate_node(rapidxml::node_element, "BoundingBox");

            rapidxml::xml_attribute<>* attr = doc.allocate_attribute("CRS", bounding_box.crs.c_str());
            bounding_box_node->append_attribute(attr);

            attr = doc.allocate_attribute("minx", std::to_string(bounding_box.min_x).c_str());
            bounding_box_node->append_attribute(attr);

            attr = doc.allocate_attribute("miny", std::to_string(bounding_box.min_y).c_str());
            bounding_box_node->append_attribute(attr);

            attr = doc.allocate_attribute("maxx", std::to_string(bounding_box.max_x).c_str());
            bounding_box_node->append_attribute(attr);

            attr = doc.allocate_attribute("maxy", std::to_string(bounding_box.max_y).c_str());
            bounding_box_node->append_attribute(attr);

            attr = doc.allocate_attribute("resx", std::to_string(bounding_box.res_x).c_str());
            bounding_box_node->append_attribute(attr);

            attr = doc.allocate_attribute("resy", std::to_string(bounding_box.res_y).c_str());
            bounding_box_node->append_attribute(attr);

            layer_node->append_node(bounding_box_node);
          }

          // Dimension node
          for(auto dimension: capabilities.capability.layer.dimension)
          {
            rapidxml::xml_node<>* dimension_node = doc.allocate_node(rapidxml::node_element, "Dimension", dimension.value.c_str());

            rapidxml::xml_attribute<>* attr = doc.allocate_attribute("name", dimension.name.c_str());
            dimension_node->append_attribute(attr);

            attr = doc.allocate_attribute("units", dimension.units.c_str());
            dimension_node->append_attribute(attr);

            attr = doc.allocate_attribute("unitSymbol", dimension.unit_symbol.c_str());
            dimension_node->append_attribute(attr);

            attr = doc.allocate_attribute("default", dimension.default_dim.c_str());
            dimension_node->append_attribute(attr);

            attr = doc.allocate_attribute("multipleValues", std::to_string(dimension.multiple_values).c_str());
            dimension_node->append_attribute(attr);

            attr = doc.allocate_attribute("nearestValue", std::to_string(dimension.nearest_value).c_str());
            dimension_node->append_attribute(attr);

            attr = doc.allocate_attribute("current", std::to_string(dimension.current).c_str());
            dimension_node->append_attribute(attr);

            layer_node->append_node(dimension_node);
          }

          // Attribution node
          rapidxml::xml_node<>* attribution_node = doc.allocate_node(rapidxml::node_element, "Attribution");

          {
            attribution_node->append_node(doc.allocate_node(rapidxml::node_element, "Title", capabilities.capability.layer.attribution.title.c_str()));

            rapidxml::xml_node<>* node = doc.allocate_node(rapidxml::node_element, "OnlineResource");

            rapidxml::xml_attribute<>* attr = doc.allocate_attribute("xlink:type", capabilities.capability.layer.attribution.online_resource.xlink_type.c_str());
            node->append_attribute(attr);

            attr = doc.allocate_attribute("xlink:href", capabilities.capability.layer.attribution.online_resource.xlink_href.c_str());
            node->append_attribute(attr);

            attribution_node->append_node(node);

            // LogoURL node
            rapidxml::xml_node<>* logo_url_node = doc.allocate_node(rapidxml::node_element, "LogoURL");

            {
              logo_url_node->append_node(doc.allocate_node(rapidxml::node_element, "Format", capabilities.capability.layer.attribution.logo_url.format.c_str()));

              rapidxml::xml_node<>* node = doc.allocate_node(rapidxml::node_element, "OnlineResource");

              rapidxml::xml_attribute<>* attr = doc.allocate_attribute("xlink:type", capabilities.capability.layer.attribution.logo_url.online_resource.xlink_type.c_str());
              node->append_attribute(attr);

              attr = doc.allocate_attribute("xlink:href", capabilities.capability.layer.attribution.logo_url.online_resource.xlink_href.c_str());
              node->append_attribute(attr);

              logo_url_node->append_node(node);

              attr = doc.allocate_attribute("width", std::to_string(capabilities.capability.layer.attribution.logo_url.width).c_str());
              logo_url_node->append_attribute(attr);

              attr = doc.allocate_attribute("height", std::to_string(capabilities.capability.layer.attribution.logo_url.height).c_str());
              logo_url_node->append_attribute(attr);
            }

            attribution_node->append_node(logo_url_node);
          }

          layer_node->append_node(attribution_node);

          // AuthorityURL node
          for(auto authority_url: capabilities.capability.layer.authority_url)
          {
            rapidxml::xml_node<>* authority_url_node = doc.allocate_node(rapidxml::node_element, "AuthorityURL");

            rapidxml::xml_node<>* node = doc.allocate_node(rapidxml::node_element, "OnlineResource");

            rapidxml::xml_attribute<>* attr = doc.allocate_attribute("xlink:type", authority_url.online_resource.xlink_type.c_str());
            node->append_attribute(attr);

            attr = doc.allocate_attribute("xlink:href", authority_url.online_resource.xlink_href.c_str());
            node->append_attribute(attr);

            authority_url_node->append_node(node);

            attr = doc.allocate_attribute("name", authority_url.name.c_str());
            authority_url_node->append_attribute(attr);

            layer_node->append_node(authority_url_node);
          }

          // Identifier node
          for(auto identifier: capabilities.capability.layer.identifier)
          {
            rapidxml::xml_node<>* identifier_node = doc.allocate_node(rapidxml::node_element, "Identifier", identifier.value.c_str());

            rapidxml::xml_attribute<>* attr = doc.allocate_attribute("authority", identifier.authority.c_str());
            identifier_node->append_attribute(attr);

            layer_node->append_node(identifier_node);
          }

          // MetadataURL node
          for(auto metadata_url: capabilities.capability.layer.metadata_url)
          {
            rapidxml::xml_node<>* metadata_url_node = doc.allocate_node(rapidxml::node_element, "MetadataURL");

            metadata_url_node->append_node(doc.allocate_node(rapidxml::node_element, "Format", metadata_url.format.c_str()));

            rapidxml::xml_node<>* node = doc.allocate_node(rapidxml::node_element, "OnlineResource");

            rapidxml::xml_attribute<>* attr = doc.allocate_attribute("xlink:type", metadata_url.online_resource.xlink_type.c_str());
            node->append_attribute(attr);

            attr = doc.allocate_attribute("xlink:href", metadata_url.online_resource.xlink_href.c_str());
            node->append_attribute(attr);

            metadata_url_node->append_node(node);

            attr = doc.allocate_attribute("type", metadata_url.type.c_str());
            metadata_url_node->append_attribute(attr);

            layer_node->append_node(metadata_url_node);
          }

          // DataURL node
          for(auto data_url: capabilities.capability.layer.data_url)
          {
            rapidxml::xml_node<>* data_url_node = doc.allocate_node(rapidxml::node_element, "DataURL");

            data_url_node->append_node(doc.allocate_node(rapidxml::node_element, "Format", data_url.format.c_str()));

            rapidxml::xml_node<>* node = doc.allocate_node(rapidxml::node_element, "OnlineResource");

            rapidxml::xml_attribute<>* attr = doc.allocate_attribute("xlink:type", data_url.online_resource.xlink_type.c_str());
            node->append_attribute(attr);

            attr = doc.allocate_attribute("xlink:href", data_url.online_resource.xlink_href.c_str());
            node->append_attribute(attr);

            data_url_node->append_node(node);

            layer_node->append_node(data_url_node);
          }

          // FeatureListURL node
          for(auto feature_list_url: capabilities.capability.layer.feature_list_url)
          {
            rapidxml::xml_node<>* feature_list_url_node = doc.allocate_node(rapidxml::node_element, "FeatureListURL");

            feature_list_url_node->append_node(doc.allocate_node(rapidxml::node_element, "Format", feature_list_url.format.c_str()));

            rapidxml::xml_node<>* node = doc.allocate_node(rapidxml::node_element, "OnlineResource");

            rapidxml::xml_attribute<>* attr = doc.allocate_attribute("xlink:type", feature_list_url.online_resource.xlink_type.c_str());
            node->append_attribute(attr);

            attr = doc.allocate_attribute("xlink:href", feature_list_url.online_resource.xlink_href.c_str());
            node->append_attribute(attr);

            feature_list_url_node->append_node(node);

            layer_node->append_node(feature_list_url_node);
          }

          // Style node
          for(auto style: capabilities.capability.layer.style)
          {
            rapidxml::xml_node<>* style_node = doc.allocate_node(rapidxml::node_element, "Style");

            rapidxml::xml_node<>* node = doc.allocate_node(rapidxml::node_element, "Name", style.name.c_str());

            style_node->append_node(node);

            node = doc.allocate_node(rapidxml::node_element, "Title", style.title.c_str());

            style_node->append_node(node);

            node = doc.allocate_node(rapidxml::node_element, "Abstract", style.abstract.c_str());

            style_node->append_node(node);

            // LegendURL node
            for(auto legend_url: style.legend_url)
            {
              rapidxml::xml_node<>* legend_url_node = doc.allocate_node(rapidxml::node_element, "LegendURL");

              legend_url_node->append_node(doc.allocate_node(rapidxml::node_element, "Format", legend_url.format.c_str()));

              rapidxml::xml_node<>* node = doc.allocate_node(rapidxml::node_element, "OnlineResource");

              rapidxml::xml_attribute<>* attr = doc.allocate_attribute("xlink:type", legend_url.online_resource.xlink_type.c_str());
              node->append_attribute(attr);

              attr = doc.allocate_attribute("xlink:href", legend_url.online_resource.xlink_href.c_str());
              node->append_attribute(attr);

              legend_url_node->append_node(node);

              attr = doc.allocate_attribute("width", std::to_string(legend_url.width).c_str());
              legend_url_node->append_attribute(attr);

              attr = doc.allocate_attribute("height", std::to_string(legend_url.height).c_str());
              legend_url_node->append_attribute(attr);

              style_node->append_node(legend_url_node);
            }

            // StyleSheetURL node
            {
              rapidxml::xml_node<>* style_sheet_url_node = doc.allocate_node(rapidxml::node_element, "StyleSheetURL");

              style_sheet_url_node->append_node(doc.allocate_node(rapidxml::node_element, "Format", style.style_sheet_url.format.c_str()));

              rapidxml::xml_node<>* node = doc.allocate_node(rapidxml::node_element, "OnlineResource");

              rapidxml::xml_attribute<>* attr = doc.allocate_attribute("xlink:type", style.style_sheet_url.online_resource.xlink_type.c_str());
              node->append_attribute(attr);

              attr = doc.allocate_attribute("xlink:href", style.style_sheet_url.online_resource.xlink_href.c_str());
              node->append_attribute(attr);

              style_sheet_url_node->append_node(node);

              style_node->append_node(style_sheet_url_node);
            }

            // StyleURL node
            {
              rapidxml::xml_node<>* style_url_node = doc.allocate_node(rapidxml::node_element, "StyleURL");

              style_url_node->append_node(doc.allocate_node(rapidxml::node_element, "Format", style.style_url.format.c_str()));

              rapidxml::xml_node<>* node = doc.allocate_node(rapidxml::node_element, "OnlineResource");

              rapidxml::xml_attribute<>* attr = doc.allocate_attribute("xlink:type", style.style_url.online_resource.xlink_type.c_str());
              node->append_attribute(attr);

              attr = doc.allocate_attribute("xlink:href", style.style_url.online_resource.xlink_href.c_str());
              node->append_attribute(attr);

              style_url_node->append_node(node);

              style_node->append_node(style_url_node);
            }

            layer_node->append_node(style_node);
          }

          node = doc.allocate_node(rapidxml::node_element, "MinScaleDenominator", std::to_string(capabilities.capability.layer.min_scale_denominator).c_str());

          layer_node->append_node(node);

          node = doc.allocate_node(rapidxml::node_element, "MaxScaleDenominator", std::to_string(capabilities.capability.layer.max_scale_denominator).c_str());

          layer_node->append_node(node);

          rapidxml::xml_attribute<>* attr = doc.allocate_attribute("queryable", std::to_string(capabilities.capability.layer.queryable).c_str());
          layer_node->append_attribute(attr);

          attr = doc.allocate_attribute("cascaded", std::to_string(capabilities.capability.layer.cascaded).c_str());
          layer_node->append_attribute(attr);

          attr = doc.allocate_attribute("opaque", std::to_string(capabilities.capability.layer.opaque).c_str());
          layer_node->append_attribute(attr);

          attr = doc.allocate_attribute("noSubsets", std::to_string(capabilities.capability.layer.no_subsets).c_str());
          layer_node->append_attribute(attr);

          attr = doc.allocate_attribute("fixedWidth", std::to_string(capabilities.capability.layer.fixed_width).c_str());
          layer_node->append_attribute(attr);

          attr = doc.allocate_attribute("fixedHeight", std::to_string(capabilities.capability.layer.fixed_height).c_str());
          layer_node->append_attribute(attr);

          */

          // subLayers node
          for(auto layer: capabilities.capability.layer.layers)
          {
            rapidxml::xml_node<>* sub_layer_node = doc.allocate_node(rapidxml::node_element, "Layer");

            sub_layer_node->append_node(doc.allocate_node(rapidxml::node_element, "Name", layer.name.c_str()));

            layer_node->append_node(sub_layer_node);
          }
        }

        capability_node->append_node(layer_node);
      }

      root->append_node(service_node);

      root->append_node(capability_node);
    }

    doc.append_node(root);

    // output result
    std::string str_buff;

    rapidxml::print(std::back_inserter(str_buff), doc, 0);

    const char* p_str_buff = str_buff.c_str();

    response.add_header("Content-Type", "application/xml");
    response.add_header("Access-Control-Allow-Origin", "*");
    response.set_content(p_str_buff, str_buff.size());
  }
  catch(...)
  {

  }
}

void
tws::wms::get_map_functor::operator()(const tws::core::http_request& request,
                                      tws::core::http_response& response)
{

}

void
tws::wms::get_feature_info_functor::operator()(const tws::core::http_request& request,
                                           tws::core::http_response& response)
{

}

void
tws::wms::register_operations()
{
  tws::core::service_metadata service;

  service.name = "wms";

// 1st WMS operation: GetCapabilities
  {
    tws::core::service_operation s_op;

    s_op.name = "GetCapabilities";
    s_op.description = "List the metadata, describing the layers avaliable for vizualization.";
    s_op.handler = get_capabilities_functor();

    service.operations.push_back(s_op);
  }

// 2nd WMS operation: GetMap
  {
    tws::core::service_operation s_op;

    s_op.name = "GetMap";
    s_op.description = "Request the server to render a map giving a list of layers.";
    s_op.handler = get_map_functor();

    service.operations.push_back(s_op);
  }

// 3rd WMS operation: GetFeatureInfo
  {
    tws::core::service_operation s_op;

    s_op.name = "GetFeatureInfo";
    s_op.description = "Retrieve information about an element of a particular layer.";
    s_op.handler = get_feature_info_functor();

    service.operations.push_back(s_op);
  }

  tws::core::service_operations_manager::instance().insert(service);
}

void
tws::wms::initialize_operations()
{

}
