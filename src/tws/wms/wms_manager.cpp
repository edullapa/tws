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
  \file tws/wms/wms_manager.cpp

  \brief A singleton for controlling the structs containing wms configuration.

  \author Roger Victor
 */

// TWS
#include "wms_manager.hpp"
#include "../core/utils.hpp"
#include "data_types.hpp"

// Rapidjson
#include <rapidjson/document.h>

// STL
#include <memory>


struct tws::wms::wms_manager::impl
{
  impl()
    : json_file(nullptr)
  {

  }

  ~impl()
  {
  }

  std::unique_ptr<rapidjson::Document> json_file;
  //capabilities_t capabilities;
};

tws::wms::wms_manager::wms_manager()
  : pimpl_(nullptr)
{
  pimpl_ = new impl;

  std::string wms_file = tws::core::find_in_app_path("share/tws/config/wms.json");
  pimpl_->json_file.reset(tws::core::open_json_file(wms_file));
}

tws::wms::wms_manager::~wms_manager()
{
  delete pimpl_;
}

tws::wms::wms_manager& tws::wms::wms_manager::instance()
{
  static wms_manager instance;

  return instance;
}

tws::wms::capabilities_t tws::wms::wms_manager::capabilities()
{
  capabilities_t capabilities;
  capabilities.service = service();
  capabilities.capability = capability();

  return capabilities;
}

tws::wms::service_t tws::wms::wms_manager::service()
{
  try
  {
    const rapidjson::Value& wms_capabilities_object = (*pimpl_->json_file)["wms_capabilities"];
    if (!wms_capabilities_object.IsObject())
    {
      throw tws::parser_error() << tws::error_description("Could not find capabilities in wms config file");
    }

    service_t service;
    const rapidjson::Value& service_object = wms_capabilities_object["service"];
    if (!service_object.IsObject())
    {
      throw tws::parser_error() << tws::error_description("Could not find service in wms config file");
    }

    const rapidjson::Value& name = service_object["name"];
    if (!name.IsString())
    {
      throw tws::parser_error() << tws::error_description("Could not find service name in wms config file");
    }
    service.name = name.GetString();

    const rapidjson::Value& title = service_object["title"];
    if (!title.IsString())
    {
      throw tws::parser_error() << tws::error_description("Could not find service title in wms config file");
    }
    service.title = title.GetString();

    const rapidjson::Value& abstract = service_object["abstract"];
    if (!abstract.IsString())
    {
      throw tws::parser_error() << tws::error_description("Could not find service abstract in wms config file");
    }
    service.abstract = abstract.GetString();

    const rapidjson::Value& fees = service_object["fees"];
    if (!fees.IsString())
    {
      throw tws::parser_error() << tws::error_description("Could not find service fees in wms config file");
    }
    service.fees = fees.GetString();

    const rapidjson::Value& access_constraints = service_object["access_constraints"];
    if (!access_constraints.IsString())
    {
      throw tws::parser_error() << tws::error_description("Could not find service access constraints in wms config file");
    }
    service.access_constraints = access_constraints.GetString();

    const rapidjson::Value& layer_limit = service_object["layer_limit"];
    if (!layer_limit.IsInt())
    {
      throw tws::parser_error() << tws::error_description("Could not find service layer limit in wms config file");
    }
    service.layer_limit = layer_limit.GetInt();

    const rapidjson::Value& max_width = service_object["max_width"];
    if (!max_width.IsInt())
    {
      throw tws::parser_error() << tws::error_description("Could not find service max width in wms config file");
    }
    service.max_width = max_width.GetInt();

    const rapidjson::Value& max_height = service_object["max_height"];
    if (!max_height.IsInt())
    {
      throw tws::parser_error() << tws::error_description("Could not find service max height in wms config file");
    }
    service.max_height = max_height.GetInt();

    const rapidjson::Value& keyword_list = service_object["keyword_list"];
    if (!keyword_list.IsArray())
    {
      throw tws::parser_error() << tws::error_description("Could not find service keyword list in wms config file");
    }
    keyword_t keyword;
    for (unsigned int i = 0; i < keyword_list.Size(); ++i)
    {
      keyword.vocabulary = keyword_list[i].GetString();
      service.keyword_list.push_back(keyword);
    }

    online_resource_t online_resource;
    const rapidjson::Value& online_resource_object = service_object["online_resource"];
    if (!online_resource_object.IsObject())
    {
      throw tws::parser_error() << tws::error_description("Could not find service online resource in wms config file");
    }

    const rapidjson::Value& xlink_type = online_resource_object["xlink_type"];
    if (!xlink_type.IsString())
    {
      throw tws::parser_error() << tws::error_description("Could not find service online resource xlink type in wms config file");
    }
    online_resource.xlink_type = xlink_type.GetString();

    const rapidjson::Value& xlink_href = online_resource_object["xlink_href"];
    if (!xlink_href.IsString())
    {
      throw tws::parser_error() << tws::error_description("Could not find service online resource xlink href in wms config file");
    }
    online_resource.xlink_href = xlink_href.GetString();

    service.online_resource = online_resource;
    
    contact_information_t contact_information;
    const rapidjson::Value& contact_information_object = service_object["contact_information"];
    if (!contact_information_object.IsObject())
    {
      throw tws::parser_error() << tws::error_description("Could not find service contact information in wms config file");
    }

    contact_person_primary_t contact_person_primary;
    const rapidjson::Value& contact_person_primary_object = contact_information_object["contact_person_primary"];
    if (!contact_person_primary_object.IsObject())
    {
      throw tws::parser_error() << tws::error_description("Could not find service contact person primary in wms config file");
    }

    const rapidjson::Value& contact_person = contact_person_primary_object["contact_person"];
    if (!contact_person.IsString())
    {
      throw tws::parser_error() << tws::error_description("Could not find service contact person in wms config file");
    }
    contact_person_primary.contact_person = contact_person.GetString();

    const rapidjson::Value& contact_organization = contact_person_primary_object["contact_organization"];
    if (!contact_organization.IsString())
    {
      throw tws::parser_error() << tws::error_description("Could not find service contact organization in wms config file");
    }
    contact_person_primary.contact_organization = contact_organization.GetString();

    contact_information.contact_person_primary = contact_person_primary;

    const rapidjson::Value& contact_position = contact_information_object["contact_position"];
    if (!contact_position.IsString())
    {
      throw tws::parser_error() << tws::error_description("Could not find service contact position in wms config file");
    }
    contact_information.contact_position = contact_position.GetString();

    contact_address_t contact_address;
    const rapidjson::Value& contact_address_object = contact_information_object["contact_address"];
    if (!contact_address_object.IsObject())
    {
      throw tws::parser_error() << tws::error_description("Could not find service contact address in wms config file");
    }

    const rapidjson::Value& address_type = contact_address_object["address_type"];
    if (!address_type.IsString())
    {
      throw tws::parser_error() << tws::error_description("Could not find service contact address type in wms config file");
    }
    contact_address.address_type = address_type.GetString();

    const rapidjson::Value& address = contact_address_object["address"];
    if (!address.IsString())
    {
      throw tws::parser_error() << tws::error_description("Could not find service contact address in wms config file");
    }
    contact_address.address = address.GetString();

    const rapidjson::Value& city = contact_address_object["city"];
    if (!city.IsString())
    {
      throw tws::parser_error() << tws::error_description("Could not find service contact city in wms config file");
    }
    contact_address.city = city.GetString();

    const rapidjson::Value& state_or_province = contact_address_object["state_or_province"];
    if (!state_or_province.IsString())
    {
      throw tws::parser_error() << tws::error_description("Could not find service contact state or province in wms config file");
    }
    contact_address.state_or_province = state_or_province.GetString();

    const rapidjson::Value& post_code = contact_address_object["post_code"];
    if (!post_code.IsString())
    {
      throw tws::parser_error() << tws::error_description("Could not find service contact post code in wms config file");
    }
    contact_address.post_code = post_code.GetString();

    const rapidjson::Value& country = contact_address_object["country"];
    if (!country.IsString())
    {
      throw tws::parser_error() << tws::error_description("Could not find service contact country in wms config file");
    }
    contact_address.country = country.GetString();

    contact_information.contact_address = contact_address;

    const rapidjson::Value& contact_voice_telephone = contact_information_object["contact_voice_telephone"];
    if (!contact_voice_telephone.IsString())
    {
      throw tws::parser_error() << tws::error_description("Could not find service contact voice telephone in wms config file");
    }
    contact_information.contact_voice_telephone = contact_voice_telephone.GetString();

    const rapidjson::Value& contact_electronic_mail_address = contact_information_object["contact_electronic_mail_address"];
    if (!contact_electronic_mail_address.IsString())
    {
      throw tws::parser_error() << tws::error_description("Could not find service contact electronic mail address in wms config file");
    }
    contact_information.contact_electronic_mail_address = contact_electronic_mail_address.GetString();

    service.contact_information = contact_information;

    return service;
  }
  catch(...)
  {
    throw;
  }
}

tws::wms::capability_t tws::wms::wms_manager::capability()
{
  try
  {
    const rapidjson::Value& wms_capabilities_object = (*pimpl_->json_file)["wms_capabilities"];
    if (!wms_capabilities_object.IsObject())
    {
      throw tws::parser_error() << tws::error_description("Could not find capabilities in wms config file");
    }

    capability_t capability;
    const rapidjson::Value& capability_object = wms_capabilities_object["capability"];
    if (!capability_object.IsObject())
    {
      throw tws::parser_error() << tws::error_description("Could not find capability in wms config file");
    }

    const rapidjson::Value& exception_formats = capability_object["exception"];
    if (!exception_formats.IsArray())
    {
      throw tws::parser_error() << tws::error_description("Could not find exception formats in wms config file");
    }
    for (unsigned int i = 0; i < exception_formats.Size(); ++i)
    {
      capability.exception_formats.push_back(exception_formats[i].GetString());
    }

    capability.request = request();

    capability.layer = layer();

    return capability;
  }
  catch (...)
  {
    throw;
  }
}

tws::wms::request_t tws::wms::wms_manager::request()
{
  try
  {
    const rapidjson::Value& wms_capabilities_object = (*pimpl_->json_file)["wms_capabilities"];
    if (!wms_capabilities_object.IsObject())
    {
      throw tws::parser_error() << tws::error_description("Could not find capabilities in wms config file");
    }

    const rapidjson::Value& capability_object = wms_capabilities_object["capability"];
    if (!capability_object.IsObject())
    {
      throw tws::parser_error() << tws::error_description("Could not find capability in wms config file");
    }

    request_t request;
    const rapidjson::Value& request_object = capability_object["request"];
    if (!request_object.IsObject())
    {
      throw tws::parser_error() << tws::error_description("Could not find request in wms config file");
    }

    request.get_capabilities = get_capabilities();
    request.get_map = get_map();
    request.get_feature_info = get_feature_info();

    return request;
  }
  catch (...)
  {
    throw;
  }
}

tws::wms::operation_t tws::wms::wms_manager::get_capabilities()
{
  try
  {
    const rapidjson::Value& wms_capabilities_object = (*pimpl_->json_file)["wms_capabilities"];
    if (!wms_capabilities_object.IsObject())
    {
      throw tws::parser_error() << tws::error_description("Could not find capabilities in wms config file");
    }

    const rapidjson::Value& capability_object = wms_capabilities_object["capability"];
    if (!capability_object.IsObject())
    {
      throw tws::parser_error() << tws::error_description("Could not find capability in wms config file");
    }

    const rapidjson::Value& request_object = capability_object["request"];
    if (!request_object.IsObject())
    {
      throw tws::parser_error() << tws::error_description("Could not find request in wms config file");
    }

    operation_t get_capabilities;
    const rapidjson::Value& get_capabilities_object = request_object["get_capabilities"];
    if (!get_capabilities_object.IsObject())
    {
      throw tws::parser_error() << tws::error_description("Could not find get capabilities in wms config file");
    }

    const rapidjson::Value& format = get_capabilities_object["format"];
    if (!format.IsArray())
    {
      throw tws::parser_error() << tws::error_description("Could not find format in wms config file");
    }
    for (unsigned int i = 0; i < format.Size(); ++i)
    {
      get_capabilities.format.push_back(format[i].GetString());
    }

    dcp_type_t dcptype;
    const rapidjson::Value& dcptype_object = get_capabilities_object["dcptype"];
    if (!dcptype_object.IsObject())
    {
      throw tws::parser_error() << tws::error_description("Could not find dcp type in wms config file");
    }

    http_t http;
    const rapidjson::Value& http_object = dcptype_object["http"];
    if (!http_object.IsObject())
    {
      throw tws::parser_error() << tws::error_description("Could not find http in wms config file");
    }

    online_resource_t get;
    const rapidjson::Value& get_object = http_object["get"];
    if (!get_object.IsObject())
    {
      throw tws::parser_error() << tws::error_description("Could not find get in wms config file");
    }

    const rapidjson::Value& get_xlink_type = get_object["xlink_type"];
    if (!get_xlink_type.IsString())
    {
      throw tws::parser_error() << tws::error_description("Could not find xlink type in wms config file");
    }
    get.xlink_type = get_xlink_type.GetString();

    const rapidjson::Value& get_xlink_href = get_object["xlink_href"];
    if (!get_xlink_href.IsString())
    {
      throw tws::parser_error() << tws::error_description("Could not find xlink href in wms config file");
    }
    get.xlink_href = get_xlink_href.GetString();

    http.get = get;

    online_resource_t post;
    const rapidjson::Value& post_object = http_object["post"];
    if (!post_object.IsObject())
    {
      throw tws::parser_error() << tws::error_description("Could not find post in wms config file");
    }

    const rapidjson::Value& post_xlink_type = post_object["xlink_type"];
    if (!post_xlink_type.IsString())
    {
      throw tws::parser_error() << tws::error_description("Could not find xlink type in wms config file");
    }
    post.xlink_type = post_xlink_type.GetString();

    const rapidjson::Value& post_xlink_href = post_object["xlink_href"];
    if (!post_xlink_href.IsString())
    {
      throw tws::parser_error() << tws::error_description("Could not find xlink href in wms config file");
    }
    post.xlink_href = post_xlink_href.GetString();

    http.post = post;

    dcptype.http = http;

    get_capabilities.dcp_type = dcptype;

    return get_capabilities;
  }
  catch (...)
  {
    throw;
  }
}

tws::wms::operation_t tws::wms::wms_manager::get_map()
{
  try
  {
    const rapidjson::Value& wms_capabilities_object = (*pimpl_->json_file)["wms_capabilities"];
    if (!wms_capabilities_object.IsObject())
    {
      throw tws::parser_error() << tws::error_description("Could not find capabilities in wms config file");
    }

    const rapidjson::Value& capability_object = wms_capabilities_object["capability"];
    if (!capability_object.IsObject())
    {
      throw tws::parser_error() << tws::error_description("Could not find capability in wms config file");
    }

    const rapidjson::Value& request_object = capability_object["request"];
    if (!request_object.IsObject())
    {
      throw tws::parser_error() << tws::error_description("Could not find request in wms config file");
    }

    operation_t get_map;
    const rapidjson::Value& get_map_object = request_object["get_map"];
    if (!get_map_object.IsObject())
    {
      throw tws::parser_error() << tws::error_description("Could not find get map in wms config file");
    }

    const rapidjson::Value& format = get_map_object["format"];
    if (!format.IsArray())
    {
      throw tws::parser_error() << tws::error_description("Could not find format in wms config file");
    }
    for (unsigned int i = 0; i < format.Size(); ++i)
    {
      get_map.format.push_back(format[i].GetString());
    }

    dcp_type_t dcptype;
    const rapidjson::Value& dcptype_object = get_map_object["dcptype"];
    if (!dcptype_object.IsObject())
    {
      throw tws::parser_error() << tws::error_description("Could not find dcp type in wms config file");
    }

    http_t http;
    const rapidjson::Value& http_object = dcptype_object["http"];
    if (!http_object.IsObject())
    {
      throw tws::parser_error() << tws::error_description("Could not find http in wms config file");
    }

    online_resource_t get;
    const rapidjson::Value& get_object = http_object["get"];
    if (!get_object.IsObject())
    {
      throw tws::parser_error() << tws::error_description("Could not find get in wms config file");
    }

    const rapidjson::Value& xlink_type = get_object["xlink_type"];
    if (!xlink_type.IsString())
    {
      throw tws::parser_error() << tws::error_description("Could not find xlink type in wms config file");
    }
    get.xlink_type = xlink_type.GetString();

    const rapidjson::Value& xlink_href = get_object["xlink_href"];
    if (!xlink_href.IsString())
    {
      throw tws::parser_error() << tws::error_description("Could not find xlink href in wms config file");
    }
    get.xlink_href = xlink_href.GetString();

    http.get = get;

    dcptype.http = http;

    get_map.dcp_type = dcptype;

    return get_map;
  }
  catch (...)
  {
    throw;
  }
}

tws::wms::operation_t tws::wms::wms_manager::get_feature_info()
{
  try
  {
    const rapidjson::Value& wms_capabilities_object = (*pimpl_->json_file)["wms_capabilities"];
    if (!wms_capabilities_object.IsObject())
    {
      throw tws::parser_error() << tws::error_description("Could not find capabilities in wms config file");
    }

    const rapidjson::Value& capability_object = wms_capabilities_object["capability"];
    if (!capability_object.IsObject())
    {
      throw tws::parser_error() << tws::error_description("Could not find capability in wms config file");
    }

    const rapidjson::Value& request_object = capability_object["request"];
    if (!request_object.IsObject())
    {
      throw tws::parser_error() << tws::error_description("Could not find request in wms config file");
    }

    operation_t get_feature_info;
    const rapidjson::Value& get_feature_info_object = request_object["get_feature_info"];
    if (!get_feature_info_object.IsObject())
    {
      throw tws::parser_error() << tws::error_description("Could not find get feature info in wms config file");
    }

    const rapidjson::Value& format = get_feature_info_object["format"];
    if (!format.IsArray())
    {
      throw tws::parser_error() << tws::error_description("Could not find format in wms config file");
    }
    for (unsigned int i = 0; i < format.Size(); ++i)
    {
      get_feature_info.format.push_back(format[i].GetString());
    }

    dcp_type_t dcptype;
    const rapidjson::Value& dcptype_object = get_feature_info_object["dcptype"];
    if (!dcptype_object.IsObject())
    {
      throw tws::parser_error() << tws::error_description("Could not find dcp type in wms config file");
    }

    http_t http;
    const rapidjson::Value& http_object = dcptype_object["http"];
    if (!http_object.IsObject())
    {
      throw tws::parser_error() << tws::error_description("Could not find http in wms config file");
    }

    online_resource_t get;
    const rapidjson::Value& get_object = http_object["get"];
    if (!get_object.IsObject())
    {
      throw tws::parser_error() << tws::error_description("Could not find get in wms config file");
    }

    const rapidjson::Value& xlink_type = get_object["xlink_type"];
    if (!xlink_type.IsString())
    {
      throw tws::parser_error() << tws::error_description("Could not find xlink type in wms config file");
    }
    get.xlink_type = xlink_type.GetString();

    const rapidjson::Value& xlink_href = get_object["xlink_href"];
    if (!xlink_href.IsString())
    {
      throw tws::parser_error() << tws::error_description("Could not find xlink href in wms config file");
    }
    get.xlink_href = xlink_href.GetString();

    http.get = get;

    dcptype.http = http;

    get_feature_info.dcp_type = dcptype;

    return get_feature_info;
  }
  catch (...)
  {
    throw;
  }
}

tws::wms::layer_t tws::wms::wms_manager::layer()
{
  try
  {
    const rapidjson::Value& wms_capabilities_object = (*pimpl_->json_file)["wms_capabilities"];
    if (!wms_capabilities_object.IsObject())
    {
      throw tws::parser_error() << tws::error_description("Could not find capabilities in wms config file");
    }

    const rapidjson::Value& capability_object = wms_capabilities_object["capability"];
    if (!capability_object.IsObject())
    {
      throw tws::parser_error() << tws::error_description("Could not find capability in wms config file");
    }

    layer_t layer;
    const rapidjson::Value& layer_object = capability_object["layer"];
    if (!layer_object.IsObject())
    {
      throw tws::parser_error() << tws::error_description("Could not find layer in wms config file");
    }

    const rapidjson::Value& name = layer_object["name"];
    if (!name.IsString())
    {
      throw tws::parser_error() << tws::error_description("Could not find layer name in wms config file");
    }
    layer.name = name.GetString();

    /*

    const rapidjson::Value& title = layer_object["title"];
    if (!title.IsString())
    {
      throw tws::parser_error() << tws::error_description("Could not find layer title in wms config file");
    }
    layer.title = title.GetString();

    const rapidjson::Value& abstract = layer_object["abstract"];
    if (!abstract.IsString())
    {
      throw tws::parser_error() << tws::error_description("Could not find layer abstract in wms config file");
    }
    layer.abstract = abstract.GetString();

    const rapidjson::Value& keyword_list = layer_object["keyword_list"];
    if (!keyword_list.IsArray())
    {
      throw tws::parser_error() << tws::error_description("Could not find layer keyword list in wms config file");
    }
    keyword_t keyword;
    for (unsigned int i = 0; i < keyword_list.Size(); ++i)
    {
      keyword.vocabulary = keyword_list[i].GetString();
      layer.keyword_list.push_back(keyword);
    }

    const rapidjson::Value& crs = layer_object["crs"];
    if (!crs.IsArray())
    {
      throw tws::parser_error() << tws::error_description("Could not find layer crs in wms config file");
    }
    for (unsigned int i = 0; i < crs.Size(); ++i)
    {
      layer.crs.push_back(crs[i].GetString());
    }

    ex_geographic_bounding_box_t ex_geographic_bounding_box;
    const rapidjson::Value& ex_geographic_bounding_box_object = layer_object["ex_geographic_bounding_box"];
    if (!ex_geographic_bounding_box_object.IsObject())
    {
      throw tws::parser_error() << tws::error_description("Could not find layer EX_GeographicBoundingBox in wms config file");
    }

    const rapidjson::Value& west_bound_longitude = ex_geographic_bounding_box_object["west_bound_longitude"];
    if (!west_bound_longitude.IsDouble())
    {
      throw tws::parser_error() << tws::error_description("Could not find layer EX_GeographicBoundingBox west bound longitude in wms config file");
    }
    ex_geographic_bounding_box.west_bound_longitude = west_bound_longitude.GetDouble();

    const rapidjson::Value& east_bound_longitude = ex_geographic_bounding_box_object["east_bound_longitude"];
    if (!east_bound_longitude.IsDouble())
    {
      throw tws::parser_error() << tws::error_description("Could not find layer EX_GeographicBoundingBox east bound longitude in wms config file");
    }
    ex_geographic_bounding_box.east_bound_longitude = east_bound_longitude.GetDouble();

    const rapidjson::Value& south_bound_latitude = ex_geographic_bounding_box_object["south_bound_latitude"];
    if (!south_bound_latitude.IsDouble())
    {
      throw tws::parser_error() << tws::error_description("Could not find layer EX_GeographicBoundingBox south bound latitude in wms config file");
    }
    ex_geographic_bounding_box.south_bound_latitude = south_bound_latitude.GetDouble();

    const rapidjson::Value& north_bound_latitude = ex_geographic_bounding_box_object["north_bound_latitude"];
    if (!north_bound_latitude.IsDouble())
    {
      throw tws::parser_error() << tws::error_description("Could not find layer EX_GeographicBoundingBox north bound latitude in wms config file");
    }
    ex_geographic_bounding_box.north_bound_latitude = north_bound_latitude.GetDouble();

    layer.ex_geographic_bounding_box = ex_geographic_bounding_box;

    // bounding_box_t
    const rapidjson::Value& bounding_box_list = layer_object["bounding_box"];
    if (!bounding_box_list.IsArray())
    {
      throw tws::parser_error() << tws::error_description("Could not find layer bounding box in wms config file");
    }
    bounding_box_t bounding_box;
    for (unsigned int i = 0; i < bounding_box_list.Size(); ++i)
    {
      const rapidjson::Value& crs = bounding_box_list[i]["crs"];
      if (!crs.IsString())
      {
        throw tws::parser_error() << tws::error_description("Could not find layer bounding box crs in wms config file");
      }
      bounding_box.crs = crs.GetString();

      const rapidjson::Value& min_x = bounding_box_list[i]["min_x"];
      if (!min_x.IsDouble())
      {
        throw tws::parser_error() << tws::error_description("Could not find layer bounding box min_x in wms config file");
      }
      bounding_box.min_x = min_x.GetDouble();

      const rapidjson::Value& min_y = bounding_box_list[i]["min_y"];
      if (!min_y.IsDouble())
      {
        throw tws::parser_error() << tws::error_description("Could not find layer bounding box min_y in wms config file");
      }
      bounding_box.min_y = min_y.GetDouble();

      const rapidjson::Value& max_x = bounding_box_list[i]["max_x"];
      if (!max_x.IsDouble())
      {
        throw tws::parser_error() << tws::error_description("Could not find layer bounding box max_x in wms config file");
      }
      bounding_box.max_x = max_x.GetDouble();

      const rapidjson::Value& max_y = bounding_box_list[i]["max_y"];
      if (!max_y.IsDouble())
      {
        throw tws::parser_error() << tws::error_description("Could not find layer bounding box max_y in wms config file");
      }
      bounding_box.max_y = max_y.GetDouble();

      const rapidjson::Value& res_x = bounding_box_list[i]["res_x"];
      if (!res_x.IsDouble())
      {
        throw tws::parser_error() << tws::error_description("Could not find layer bounding box res_x in wms config file");
      }
      bounding_box.res_x = res_x.GetDouble();

      const rapidjson::Value& res_y = bounding_box_list[i]["res_y"];
      if (!res_y.IsDouble())
      {
        throw tws::parser_error() << tws::error_description("Could not find layer bounding box res_y in wms config file");
      }
      bounding_box.res_y = res_y.GetDouble();

      layer.bounding_box.push_back(bounding_box);
    }

    // dimension_t
    const rapidjson::Value& dimension_list = layer_object["dimension"];
    if (!dimension_list.IsArray())
    {
      throw tws::parser_error() << tws::error_description("Could not find layer dimension in wms config file");
    }
    dimension_t dimension;
    for (unsigned int i = 0; i < dimension_list.Size(); ++i)
    { 
      const rapidjson::Value& value = dimension_list[i]["value"];
      if (!value.IsString())
      {
        throw tws::parser_error() << tws::error_description("Could not find layer dimension value in wms config file");
      }
      dimension.value = value.GetString();

      const rapidjson::Value& name = dimension_list[i]["name"];
      if (!name.IsString())
      {
        throw tws::parser_error() << tws::error_description("Could not find layer dimension name in wms config file");
      }
      dimension.name = name.GetString();

      const rapidjson::Value& units = dimension_list[i]["units"];
      if (!units.IsString())
      {
        throw tws::parser_error() << tws::error_description("Could not find layer dimension units in wms config file");
      }
      dimension.units = units.GetString();

      const rapidjson::Value& unit_symbol = dimension_list[i]["unit_symbol"];
      if (!unit_symbol.IsString())
      {
        throw tws::parser_error() << tws::error_description("Could not find layer dimension unit symbol in wms config file");
      }
      dimension.unit_symbol = unit_symbol.GetString();

      const rapidjson::Value& default_dim = dimension_list[i]["default_dim"];
      if (!default_dim.IsString())
      {
        throw tws::parser_error() << tws::error_description("Could not find layer dimension default_dim in wms config file");
      }
      dimension.default_dim = default_dim.GetString();

      const rapidjson::Value& multiple_values = dimension_list[i]["multiple_values"];
      if (!multiple_values.IsBool())
      {
        throw tws::parser_error() << tws::error_description("Could not find layer dimension multiple values in wms config file");
      }
      dimension.multiple_values = multiple_values.GetBool();

      const rapidjson::Value& nearest_value = dimension_list[i]["nearest_value"];
      if (!nearest_value.IsBool())
      {
        throw tws::parser_error() << tws::error_description("Could not find layer dimension nearest value in wms config file");
      }
      dimension.nearest_value = nearest_value.GetBool();

      const rapidjson::Value& current = dimension_list[i]["current"];
      if (!current.IsBool())
      {
        throw tws::parser_error() << tws::error_description("Could not find layer dimension current in wms config file");
      }
      dimension.current = current.GetBool();

      layer.dimension.push_back(dimension);
    }

    attribution_t attribution;
    const rapidjson::Value& attribution_object = layer_object["attribution"];
    if (!attribution_object.IsObject())
    {
      throw tws::parser_error() << tws::error_description("Could not find layer attribution in wms config file");
    }

    const rapidjson::Value& att_title = attribution_object["title"];
    if (!att_title.IsString())
    {
      throw tws::parser_error() << tws::error_description("Could not find layer attribution title in wms config file");
    }
    attribution.title = att_title.GetString();

    online_resource_t online_resource;
    {
      const rapidjson::Value& online_resource_object = attribution_object["online_resource"];
      if (!online_resource_object.IsObject())
      {
        throw tws::parser_error() << tws::error_description("Could not find layer attribution online resource in wms config file");
      }

      const rapidjson::Value& xlink_type = online_resource_object["xlink_type"];
      if (!xlink_type.IsString())
      {
        throw tws::parser_error() << tws::error_description("Could not find layer attribution online resource xlink type in wms config file");
      }
      online_resource.xlink_type = xlink_type.GetString();

      const rapidjson::Value& xlink_href = online_resource_object["xlink_href"];
      if (!xlink_href.IsString())
      {
        throw tws::parser_error() << tws::error_description("Could not find layer attribution online resource xlink href in wms config file");
      }
      online_resource.xlink_href = xlink_href.GetString();
    }

    attribution.online_resource = online_resource;

    logo_url_t logo_url;
    {
      const rapidjson::Value& logo_url_object = attribution_object["logo_url"];
      if (!logo_url_object.IsObject())
      {
        throw tws::parser_error() << tws::error_description("Could not find layer attribution logo url in wms config file");
      }

      const rapidjson::Value& format = logo_url_object["format"];
      if (!format.IsString())
      {
        throw tws::parser_error() << tws::error_description("Could not find layer attribution logo url format in wms config file");
      }
      logo_url.format = format.GetString();

      online_resource_t online_resource;
      {
        const rapidjson::Value& online_resource_object = logo_url_object["online_resource"];
        if (!online_resource_object.IsObject())
        {
          throw tws::parser_error() << tws::error_description("Could not find layer attribution online resource in wms config file");
        }

        const rapidjson::Value& xlink_type = online_resource_object["xlink_type"];
        if (!xlink_type.IsString())
        {
          throw tws::parser_error() << tws::error_description("Could not find layer attribution online resource xlink type in wms config file");
        }
        online_resource.xlink_type = xlink_type.GetString();

        const rapidjson::Value& xlink_href = online_resource_object["xlink_href"];
        if (!xlink_href.IsString())
        {
          throw tws::parser_error() << tws::error_description("Could not find layer attribution online resource xlink href in wms config file");
        }
        online_resource.xlink_href = xlink_href.GetString();
      }

      logo_url.online_resource = online_resource;

      const rapidjson::Value& width = logo_url_object["width"];
      if (!width.IsInt())
      {
        throw tws::parser_error() << tws::error_description("Could not find layer attribution logo url width in wms config file");
      }
      logo_url.width = width.GetInt();

      const rapidjson::Value& height = logo_url_object["height"];
      if (!height.IsInt())
      {
        throw tws::parser_error() << tws::error_description("Could not find layer attribution logo url height in wms config file");
      }
      logo_url.height = height.GetInt();
    }

    attribution.logo_url = logo_url;

    layer.attribution = attribution;

    // authority_url_t
    const rapidjson::Value& authority_url_list = layer_object["authority_url"];
    if (!authority_url_list.IsArray())
    {
      throw tws::parser_error() << tws::error_description("Could not find layer authority url in wms config file");
    }
    authority_url_t authority_url;
    for (unsigned int i = 0; i < authority_url_list.Size(); ++i)
    {
      online_resource_t online_resource;
      {
        const rapidjson::Value& online_resource_object = authority_url_list[i]["online_resource"];
        if (!online_resource_object.IsObject())
        {
          throw tws::parser_error() << tws::error_description("Could not find layer authority url online resource in wms config file");
        }

        const rapidjson::Value& xlink_type = online_resource_object["xlink_type"];
        if (!xlink_type.IsString())
        {
          throw tws::parser_error() << tws::error_description("Could not find layer authority url online resource xlink type in wms config file");
        }
        online_resource.xlink_type = xlink_type.GetString();

        const rapidjson::Value& xlink_href = online_resource_object["xlink_href"];
        if (!xlink_href.IsString())
        {
          throw tws::parser_error() << tws::error_description("Could not find layer authority url online resource xlink href in wms config file");
        }
        online_resource.xlink_href = xlink_href.GetString();
      }

      authority_url.online_resource = online_resource;

      const rapidjson::Value& name = authority_url_list[i]["name"];
      if (!name.IsString())
      {
        throw tws::parser_error() << tws::error_description("Could not find layer authority url name in wms config file");
      }
      authority_url.name = name.GetString();

      layer.authority_url.push_back(authority_url);
    }

    // identifier_t
    const rapidjson::Value& identifier_list = layer_object["identifier"];
    if (!identifier_list.IsArray())
    {
      throw tws::parser_error() << tws::error_description("Could not find layer identifier in wms config file");
    }
    identifier_t identifier;
    for (unsigned int i = 0; i < identifier_list.Size(); ++i)
    {
      const rapidjson::Value& value = identifier_list[i]["value"];
      if (!value.IsString())
      {
        throw tws::parser_error() << tws::error_description("Could not find layer identifier value in wms config file");
      }
      identifier.value = value.GetString();

      const rapidjson::Value& authority = identifier_list[i]["authority"];
      if (!authority.IsString())
      {
        throw tws::parser_error() << tws::error_description("Could not find layer identifier authority in wms config file");
      }
      identifier.authority = authority.GetString();

      layer.identifier.push_back(identifier);
    }

    // metadata_url_t
    const rapidjson::Value& metadata_url_list = layer_object["metadata_url"];
    if (!metadata_url_list.IsArray())
    {
      throw tws::parser_error() << tws::error_description("Could not find layer metadata url in wms config file");
    }
    metadata_url_t metadata_url;
    for (unsigned int i = 0; i < metadata_url_list.Size(); ++i)
    {
      const rapidjson::Value& format = metadata_url_list[i]["format"];
      if (!format.IsString())
      {
        throw tws::parser_error() << tws::error_description("Could not find layer metadata url format in wms config file");
      }
      metadata_url.format = format.GetString();

      online_resource_t online_resource;
      {
        const rapidjson::Value& online_resource_object = metadata_url_list[i]["online_resource"];
        if (!online_resource_object.IsObject())
        {
          throw tws::parser_error() << tws::error_description("Could not find layer metadata url online resource in wms config file");
        }

        const rapidjson::Value& xlink_type = online_resource_object["xlink_type"];
        if (!xlink_type.IsString())
        {
          throw tws::parser_error() << tws::error_description("Could not find layer metadata url online resource xlink type in wms config file");
        }
        online_resource.xlink_type = xlink_type.GetString();

        const rapidjson::Value& xlink_href = online_resource_object["xlink_href"];
        if (!xlink_href.IsString())
        {
          throw tws::parser_error() << tws::error_description("Could not find layer metadata url online resource xlink href in wms config file");
        }
        online_resource.xlink_href = xlink_href.GetString();
      }

      metadata_url.online_resource = online_resource;

      const rapidjson::Value& type = metadata_url_list[i]["type"];
      if (!type.IsString())
      {
        throw tws::parser_error() << tws::error_description("Could not find layer metadata url type in wms config file");
      }
      metadata_url.type = type.GetString();

      layer.metadata_url.push_back(metadata_url);
    }

    // data_url_t
    const rapidjson::Value& data_url_list = layer_object["data_url"];
    if (!data_url_list.IsArray())
    {
      throw tws::parser_error() << tws::error_description("Could not find layer data url in wms config file");
    }
    data_url_t data_url;
    for (unsigned int i = 0; i < data_url_list.Size(); ++i)
    {
      const rapidjson::Value& format = data_url_list[i]["format"];
      if (!format.IsString())
      {
        throw tws::parser_error() << tws::error_description("Could not find layer data url format in wms config file");
      }
      data_url.format = format.GetString();

      online_resource_t online_resource;
      {
        const rapidjson::Value& online_resource_object = data_url_list[i]["online_resource"];
        if (!online_resource_object.IsObject())
        {
          throw tws::parser_error() << tws::error_description("Could not find layer data url online resource in wms config file");
        }

        const rapidjson::Value& xlink_type = online_resource_object["xlink_type"];
        if (!xlink_type.IsString())
        {
          throw tws::parser_error() << tws::error_description("Could not find layer data url online resource xlink type in wms config file");
        }
        online_resource.xlink_type = xlink_type.GetString();

        const rapidjson::Value& xlink_href = online_resource_object["xlink_href"];
        if (!xlink_href.IsString())
        {
          throw tws::parser_error() << tws::error_description("Could not find layer data url online resource xlink href in wms config file");
        }
        online_resource.xlink_href = xlink_href.GetString();
      }

      data_url.online_resource = online_resource;

      layer.data_url.push_back(data_url);
    }

    // feature_list_url_t
    const rapidjson::Value& feature_list_url_list = layer_object["feature_list_url"];
    if (!feature_list_url_list.IsArray())
    {
      throw tws::parser_error() << tws::error_description("Could not find layer feature list url in wms config file");
    }
    feature_list_url_t feature_list_url;
    for (unsigned int i = 0; i < feature_list_url_list.Size(); ++i)
    {
      const rapidjson::Value& format = feature_list_url_list[i]["format"];
      if (!format.IsString())
      {
        throw tws::parser_error() << tws::error_description("Could not find layer feature list url format in wms config file");
      }
      feature_list_url.format = format.GetString();

      online_resource_t online_resource;
      {
        const rapidjson::Value& online_resource_object = feature_list_url_list[i]["online_resource"];
        if (!online_resource_object.IsObject())
        {
          throw tws::parser_error() << tws::error_description("Could not find layer data url online resource in wms config file");
        }

        const rapidjson::Value& xlink_type = online_resource_object["xlink_type"];
        if (!xlink_type.IsString())
        {
          throw tws::parser_error() << tws::error_description("Could not find layer data url online resource xlink type in wms config file");
        }
        online_resource.xlink_type = xlink_type.GetString();

        const rapidjson::Value& xlink_href = online_resource_object["xlink_href"];
        if (!xlink_href.IsString())
        {
          throw tws::parser_error() << tws::error_description("Could not find layer data url online resource xlink href in wms config file");
        }
        online_resource.xlink_href = xlink_href.GetString();
      }

      feature_list_url.online_resource = online_resource;

      layer.feature_list_url.push_back(feature_list_url);
    }

    // style_t
    const rapidjson::Value& style_list = layer_object["style"];
    if (!style_list.IsArray())
    {
      throw tws::parser_error() << tws::error_description("Could not find layer style in wms config file");
    }
    style_t style;
    for (unsigned int i = 0; i < style_list.Size(); ++i)
    {
      const rapidjson::Value& name = style_list[i]["name"];
      if (!name.IsString())
      {
        throw tws::parser_error() << tws::error_description("Could not find layer style name in wms config file");
      }
      style.name = name.GetString();

      const rapidjson::Value& title = style_list[i]["title"];
      if (!title.IsString())
      {
        throw tws::parser_error() << tws::error_description("Could not find layer style title in wms config file");
      }
      style.title = title.GetString();

      const rapidjson::Value& abstract = style_list[i]["abstract"];
      if (!abstract.IsString())
      {
        throw tws::parser_error() << tws::error_description("Could not find layer style abstract in wms config file");
      }
      style.abstract= abstract.GetString();

      // legend_url_t
      const rapidjson::Value& legend_url_list = style_list[i]["legend_url"];
      if (!legend_url_list.IsArray())
      {
        throw tws::parser_error() << tws::error_description("Could not find layer style legend url in wms config file");
      }
      legend_url_t legend_url;
      for (unsigned int i = 0; i < legend_url_list.Size(); ++i)
      {
        const rapidjson::Value& format = legend_url_list[i]["format"];
        if (!format.IsString())
        {
          throw tws::parser_error() << tws::error_description("Could not find layer style legend url format in wms config file");
        }
        legend_url.format = format.GetString();

        online_resource_t online_resource;
        {
          const rapidjson::Value& online_resource_object = legend_url_list[i]["online_resource"];
          if (!online_resource_object.IsObject())
          {
            throw tws::parser_error() << tws::error_description("Could not find layer style legend url online resource in wms config file");
          }

          const rapidjson::Value& xlink_type = online_resource_object["xlink_type"];
          if (!xlink_type.IsString())
          {
            throw tws::parser_error() << tws::error_description("Could not find layer style legend url online resource xlink type in wms config file");
          }
          online_resource.xlink_type = xlink_type.GetString();

          const rapidjson::Value& xlink_href = online_resource_object["xlink_href"];
          if (!xlink_href.IsString())
          {
            throw tws::parser_error() << tws::error_description("Could not find layer style legend url online resource xlink href in wms config file");
          }
          online_resource.xlink_href = xlink_href.GetString();
        }

        legend_url.online_resource = online_resource;

        const rapidjson::Value& width = legend_url_list[i]["width"];
        if (!width.IsInt())
        {
          throw tws::parser_error() << tws::error_description("Could not find layer style legend url width in wms config file");
        }
        legend_url.width = width.GetInt();

        const rapidjson::Value& height = legend_url_list[i]["height"];
        if (!height.IsInt())
        {
          throw tws::parser_error() << tws::error_description("Could not find layer style legend url height in wms config file");
        }
        legend_url.height = height.GetInt();

        style.legend_url.push_back(legend_url);
      }

      // style_sheet_url_t
      style_sheet_url_t style_sheet_url;
      {
        const rapidjson::Value& style_sheet_url_object = style_list[i]["style_sheet_url"];
        if (!style_sheet_url_object.IsObject())
        {
          throw tws::parser_error() << tws::error_description("Could not find layer style sheet url in wms config file");
        }

        const rapidjson::Value& format = style_sheet_url_object["format"];
        if (!format.IsString())
        {
          throw tws::parser_error() << tws::error_description("Could not find layer style sheet url format in wms config file");
        }
        style_sheet_url.format = format.GetString();

        online_resource_t online_resource;
        {
          const rapidjson::Value& online_resource_object = style_sheet_url_object["online_resource"];
          if (!online_resource_object.IsObject())
          {
            throw tws::parser_error() << tws::error_description("Could not find layer style sheet url online resource in wms config file");
          }

          const rapidjson::Value& xlink_type = online_resource_object["xlink_type"];
          if (!xlink_type.IsString())
          {
            throw tws::parser_error() << tws::error_description("Could not find layer style sheet url online resource xlink type in wms config file");
          }
          online_resource.xlink_type = xlink_type.GetString();

          const rapidjson::Value& xlink_href = online_resource_object["xlink_href"];
          if (!xlink_href.IsString())
          {
            throw tws::parser_error() << tws::error_description("Could not find layer style sheet url online resource xlink href in wms config file");
          }
          online_resource.xlink_href = xlink_href.GetString();
        }

        style_sheet_url.online_resource = online_resource;
      }

      style.style_sheet_url = style_sheet_url;

      // style_url_t
      style_url_t style_url;
      {
        const rapidjson::Value& style_url_object = style_list[i]["style_url"];
        if (!style_url_object.IsObject())
        {
          throw tws::parser_error() << tws::error_description("Could not find layer style url in wms config file");
        }

        const rapidjson::Value& format = style_url_object["format"];
        if (!format.IsString())
        {
          throw tws::parser_error() << tws::error_description("Could not find layer style url format in wms config file");
        }
        style_url.format = format.GetString();

        online_resource_t online_resource;
        {
          const rapidjson::Value& online_resource_object = style_url_object["online_resource"];
          if (!online_resource_object.IsObject())
          {
            throw tws::parser_error() << tws::error_description("Could not find layer style url online resource in wms config file");
          }

          const rapidjson::Value& xlink_type = online_resource_object["xlink_type"];
          if (!xlink_type.IsString())
          {
            throw tws::parser_error() << tws::error_description("Could not find layer style url online resource xlink type in wms config file");
          }
          online_resource.xlink_type = xlink_type.GetString();

          const rapidjson::Value& xlink_href = online_resource_object["xlink_href"];
          if (!xlink_href.IsString())
          {
            throw tws::parser_error() << tws::error_description("Could not find layer style url online resource xlink href in wms config file");
          }
          online_resource.xlink_href = xlink_href.GetString();
        }

        style_url.online_resource = online_resource;
      }

      style.style_url = style_url;

      layer.style.push_back(style);
    }

    const rapidjson::Value& min_scale_denominator = layer_object["min_scale_denominator"];
    if (!min_scale_denominator.IsDouble())
    {
      throw tws::parser_error() << tws::error_description("Could not find layer min scale denominator in wms config file");
    }
    layer.min_scale_denominator = min_scale_denominator.GetDouble();

    const rapidjson::Value& max_scale_denominator = layer_object["max_scale_denominator"];
    if (!max_scale_denominator.IsDouble())
    {
      throw tws::parser_error() << tws::error_description("Could not find layer max scale denominator in wms config file");
    }
    layer.max_scale_denominator = max_scale_denominator.GetDouble();

    const rapidjson::Value& queryable = layer_object["queryable"];
    if (!queryable.IsBool())
    {
      throw tws::parser_error() << tws::error_description("Could not find layer queryable in wms config file");
    }
    layer.queryable = queryable.GetBool();

    const rapidjson::Value& cascaded = layer_object["cascaded"];
    if (!cascaded.IsInt())
    {
      throw tws::parser_error() << tws::error_description("Could not find layer cascaded in wms config file");
    }
    layer.cascaded = cascaded.GetInt();

    const rapidjson::Value& opaque = layer_object["opaque"];
    if (!opaque.IsBool())
    {
      throw tws::parser_error() << tws::error_description("Could not find layer opaque in wms config file");
    }
    layer.opaque = opaque.GetBool();

    const rapidjson::Value& no_subsets = layer_object["no_subsets"];
    if (!no_subsets.IsBool())
    {
      throw tws::parser_error() << tws::error_description("Could not find layer subsets in wms config file");
    }
    layer.no_subsets = no_subsets.GetBool();

    const rapidjson::Value& fixed_width = layer_object["fixed_width"];
    if (!fixed_width.IsInt())
    {
      throw tws::parser_error() << tws::error_description("Could not find layer fixed width in wms config file");
    }
    layer.fixed_width = fixed_width.GetInt();

    const rapidjson::Value& fixed_height = layer_object["fixed_height"];
    if (!fixed_height.IsInt())
    {
      throw tws::parser_error() << tws::error_description("Could not find layer fixed height in wms config file");
    }
    layer.fixed_height = fixed_height.GetInt();

    */

    // sub_layers
    const rapidjson::Value& layers_list_object = layer_object["layers"];
    if (!layers_list_object.IsArray())
    {
      throw tws::parser_error() << tws::error_description("Could not find layers list in wms config file");
    }
    layer_t sub_layer;
    for (unsigned int i = 0; i < layers_list_object.Size(); ++i)
    {
      const rapidjson::Value& name = layers_list_object[i]["name"];
      if (!name.IsString())
      {
        throw tws::parser_error() << tws::error_description("Could not find layer name in wms config file");
      }
      sub_layer.name = name.GetString();

      layer.layers.push_back(sub_layer);
    }

    return layer;
  }
  catch (...)
  {
    throw;
  }
}
