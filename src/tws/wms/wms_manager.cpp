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

  std::shared_ptr<rapidjson::Document> json_file;
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
