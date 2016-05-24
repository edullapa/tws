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
  \file tws/wms/json_serializer.cpp

  \brief Utility functions for reading the WMS Capabalities information from a JSON document.

  \author Roger Victor
 */

// TWS
#include "json_serializer.hpp"
#include "exception.hpp"


tws::wms::capabilities_t
tws::wms::read_capabilities(const rapidjson::Value& jcapabilities)
{
  if(!jcapabilities.IsObject())
    throw tws::parse_error() << tws::error_description("error parsing wms_capabilities metadata.");

  const rapidjson::Value& jservice = jcapabilities["service"];
  const rapidjson::Value& jcapability = jcapabilities["capability"];

  capabilities_t capabilities;

  capabilities.service = read_service(jservice);
  capabilities.capability = read_capability(jcapability);
  capabilities.version = "1.3.0";

  return capabilities;
}

tws::wms::service_t
tws::wms::read_service(const rapidjson::Value& jservice)
{
  service_t service;

  if (!jservice.IsObject())
    throw tws::parse_error() << tws::error_description("error parsing service metadata.");

  service.name = "WMS";

  const rapidjson::Value& jtitle = jservice["title"];
  if (!jtitle.IsString())
    throw tws::parse_error() << tws::error_description("error parsing service title metadata.");

  service.title = jtitle.GetString();

  const rapidjson::Value& jabstract = jservice["abstract"];
  if (!jabstract.IsString())
    throw tws::parse_error() << tws::error_description("error parsing service abstract metadata.");

  service.abstract = jabstract.GetString();

  const rapidjson::Value& jkeyword_list = jservice["keyword_list"];

  service.keyword_list = read_keyword_list(jkeyword_list);

  const rapidjson::Value& jonline_resource = jservice["online_resource"];

  service.online_resource = read_online_resource(jonline_resource);

  const rapidjson::Value& jcontact_information = jservice["contact_information"];

  service.contact_information = read_contact_information(jcontact_information);

  const rapidjson::Value& jfees = jservice["fees"];
  if (!jfees.IsString())
    throw tws::parse_error() << tws::error_description("error parsing service fees metadata.");

  service.fees = jfees.GetString();

  const rapidjson::Value& jaccess_constraints = jservice["access_constraints"];
  if (!jaccess_constraints.IsString())
    throw tws::parse_error() << tws::error_description("error parsing service access constraints metadata.");

  service.access_constraints = jaccess_constraints.GetString();

  const rapidjson::Value& jlayer_limit = jservice["layer_limit"];
  if (!jlayer_limit.IsInt())
    throw tws::parse_error() << tws::error_description("error parsing service layer limit metadata.");

  service.layer_limit = jlayer_limit.GetInt();

  const rapidjson::Value& jmax_width = jservice["max_width"];
  if (!jmax_width.IsInt())
    throw tws::parse_error() << tws::error_description("error parsing service max width metadata.");

  service.max_width = jmax_width.GetInt();

  const rapidjson::Value& jmax_height = jservice["max_height"];
  if (!jmax_height.IsInt())
    throw tws::parse_error() << tws::error_description("error parsing service max height metadata.");

  service.max_height = jmax_height.GetInt();

  return service;
}

tws::wms::contact_information_t
tws::wms::read_contact_information(const rapidjson::Value& jcontact_information)
{
  if (!jcontact_information.IsObject())
    throw tws::parse_error() << tws::error_description("error parsing contact information metadata.");

  contact_information_t contact_information;

  const rapidjson::Value& jcontact_person_primary = jcontact_information["contact_person_primary"];

  contact_information.contact_person_primary = read_contact_person_primary(jcontact_person_primary);

  const rapidjson::Value& jcontact_position = jcontact_information["contact_position"];
  if (!jcontact_position.IsString())
    throw tws::parse_error() << tws::error_description("error parsing contact position metadata.");

  contact_information.contact_position = jcontact_position.GetString();

  const rapidjson::Value& jcontact_address = jcontact_information["contact_address"];

  contact_information.contact_address = read_contact_address(jcontact_address);

  const rapidjson::Value& jcontact_voice_telephone = jcontact_information["contact_voice_telephone"];
  if (!jcontact_voice_telephone.IsString())
    throw tws::parse_error() << tws::error_description("error parsing contact voice telephone metadata.");

  contact_information.contact_voice_telephone = jcontact_voice_telephone.GetString();

  const rapidjson::Value& jcontact_electronic_mail_address = jcontact_information["contact_electronic_mail_address"];
  if (!jcontact_electronic_mail_address.IsString())
    throw tws::parse_error() << tws::error_description("error parsing contact electronic mail address metadata.");

  contact_information.contact_electronic_mail_address = jcontact_electronic_mail_address.GetString();

  return contact_information;
}

tws::wms::contact_person_primary_t
tws::wms::read_contact_person_primary(const rapidjson::Value& jcontact_person_primary)
{
  if (!jcontact_person_primary.IsObject())
    throw tws::parse_error() << tws::error_description("error parsing contact person primary metadata.");

  contact_person_primary_t contact_person_primary;

  const rapidjson::Value& jcontact_person = jcontact_person_primary["contact_person"];
  if (!jcontact_person.IsString())
    throw tws::parse_error() << tws::error_description("error parsing contact person metadata. ");

  contact_person_primary.contact_person = jcontact_person.GetString();

  const rapidjson::Value& jcontact_organization = jcontact_person_primary["contact_organization"];
  if (!jcontact_organization.IsString())
    throw tws::parse_error() << tws::error_description("error parsing contact organization metadata.");

  contact_person_primary.contact_organization = jcontact_organization.GetString();

  return contact_person_primary;
}

tws::wms::contact_address_t
tws::wms::read_contact_address(const rapidjson::Value& jcontact_address)
{
  if (!jcontact_address.IsObject())
    throw tws::parse_error() << tws::error_description("error parsing contact address metadata.");

  contact_address_t contact_address;

  const rapidjson::Value& jaddress_type = jcontact_address["address_type"];
  if (!jaddress_type.IsString())
    throw tws::parse_error() << tws::error_description("error parsing contact address type metadata.");

  contact_address.address_type = jaddress_type.GetString();

  const rapidjson::Value& jaddress = jcontact_address["address"];
  if (!jaddress.IsString())
    throw tws::parse_error() << tws::error_description("error parsing contact address metadata.");

  contact_address.address = jaddress.GetString();

  const rapidjson::Value& jcity = jcontact_address["city"];
  if (!jcity.IsString())
    throw tws::parse_error() << tws::error_description("error parsing contact city metadata.");

  contact_address.city = jcity.GetString();

  const rapidjson::Value& jstate_or_province = jcontact_address["state_or_province"];
  if (!jstate_or_province.IsString())
    throw tws::parse_error() << tws::error_description("error parsing contact state or province metadata.");

  contact_address.state_or_province = jstate_or_province.GetString();

  const rapidjson::Value& jpost_code = jcontact_address["post_code"];
  if (!jpost_code.IsString())
    throw tws::parse_error() << tws::error_description("error parsing contact post code metadata.");

  contact_address.post_code = jpost_code.GetString();

  const rapidjson::Value& jcountry = jcontact_address["country"];
  if (!jcountry.IsString())
    throw tws::parse_error() << tws::error_description("error parsing contact country metadata.");

  contact_address.country = jcountry.GetString();

  return contact_address;
}

tws::wms::capability_t
tws::wms::read_capability(const rapidjson::Value& jcapability)
{
  if (!jcapability.IsObject())
    throw tws::parse_error() << tws::error_description("error parsing capability metadata.");

  capability_t capability;

  const rapidjson::Value& jrequest = jcapability["request"];

  capability.request = read_request(jrequest);

  const rapidjson::Value& jexception_formats = jcapability["exception"];

  capability.exception_formats = read_formats(jexception_formats);

  const rapidjson::Value& jlayer = jcapability["layer"];

  capability.layer = read_layer(jlayer);

  return capability;
}

tws::wms::request_t
tws::wms::read_request(const rapidjson::Value& jrequest)
{
  if (!jrequest.IsObject())
    throw tws::parse_error() << tws::error_description("error parsing request metadata.");

  request_t request;

  const rapidjson::Value& jget_capabilities = jrequest["get_capabilities"];

  request.get_capabilities = read_operation(jget_capabilities);

  const rapidjson::Value& jget_map = jrequest["get_map"];

  request.get_map = read_operation(jget_map);

  const rapidjson::Value& jget_feature_info = jrequest["get_feature_info"];

  request.get_feature_info = read_operation(jget_feature_info);

  return request;
}

tws::wms::operation_t
tws::wms::read_operation(const rapidjson::Value& joperation)
{
  if (!joperation.IsObject())
    throw tws::parse_error() << tws::error_description("error parsing operation metadata.");

  operation_t operation;

  const rapidjson::Value& jformat = joperation["format"];

  operation.format = read_formats(jformat);

  const rapidjson::Value& jdcp_type = joperation["dcptype"];

  operation.dcp_type = read_dcp_type(jdcp_type);

  return operation;
}

tws::wms::dcp_type_t
tws::wms::read_dcp_type(const rapidjson::Value& jdcp_type)
{
  if (!jdcp_type.IsObject())
    throw tws::parse_error() << tws::error_description("error parsing dcp type metadata.");

  dcp_type_t dcp_type;

  const rapidjson::Value& jhttp = jdcp_type["http"];

  dcp_type.http = read_http(jhttp);

  return dcp_type;
}

tws::wms::http_t
tws::wms::read_http(const rapidjson::Value& jhttp)
{
  if (!jhttp.IsObject())
    throw tws::parse_error() << tws::error_description("error parsing http metadata.");

  http_t http;

  const rapidjson::Value& jget = jhttp["get"];

  const rapidjson::Value& jget_online_resource = jget["online_resource"];

  http.get = read_online_resource(jget_online_resource);

  if (jhttp.HasMember("post"))
  {
    const rapidjson::Value& jpost = jhttp["post"];

    const rapidjson::Value& jpost_online_resource = jpost["online_resource"];

    http.post = read_online_resource(jpost_online_resource);
  }

  return http;
}

std::vector<tws::wms::keyword_t>
tws::wms::read_keyword_list(const rapidjson::Value& jkeyword_list)
{
  if (!jkeyword_list.IsArray())
    throw tws::parse_error() << tws::error_description("error parsing keyword list metadata.");

  keyword_t keyword;
  std::vector<keyword_t> keyword_list;

  for (unsigned int i = 0; i < jkeyword_list.Size(); ++i)
  {
    keyword.vocabulary = jkeyword_list[i].GetString();
    keyword_list.push_back(keyword);
  }

  return keyword_list;
}

std::vector<std::string>
tws::wms::read_formats(const rapidjson::Value& jformats)
{
  if (!jformats.IsArray())
    throw tws::parse_error() << tws::error_description("error parsing exception formats metadata.");

  std::vector<std::string> formats;

  for (unsigned int i = 0; i < jformats.Size(); ++i)
  {
    formats.push_back(jformats[i].GetString());
  }

  return formats;
}

tws::wms::online_resource_t
tws::wms::read_online_resource(const rapidjson::Value& jonline_resource)
{
  if (!jonline_resource.IsString())
    throw tws::parse_error() << tws::error_description("error parsing online resource metadata.");

  online_resource_t online_resource;

  online_resource.xlink_type = "simple";

  online_resource.xlink_href = jonline_resource.GetString();

  return online_resource;
}

std::vector<std::string>
tws::wms::read_crs(const rapidjson::Value& jcrs_list)
{
  if (!jcrs_list.IsArray())
    throw tws::parse_error() << tws::error_description("error parsing crs metadata.");

  std::vector<std::string> crs_list;

  for (unsigned int i = 0; i < jcrs_list.Size(); ++i)
  {
    crs_list.push_back(jcrs_list[i].GetString());
  }

  return crs_list;
}

tws::wms::layer_t
tws::wms::read_layer(const rapidjson::Value& jlayer)
{
  if (!jlayer.IsObject())
    throw tws::parse_error() << tws::error_description("error parsing layer metadata.");

  layer_t layer;

  const rapidjson::Value& jname = jlayer["name"];
  if (!jname.IsString())
    throw tws::parse_error() << tws::error_description("error parsing layer name metadata.");

  layer.name = jname.GetString();

  const rapidjson::Value& jcrs_list = jlayer["crs"];

  layer.crs = read_crs(jcrs_list);

  const rapidjson::Value& jsub_layers = jlayer["layers"];
  if (!jsub_layers.IsArray())
    throw tws::parse_error() << tws::error_description("error parsing layers list metadata.");

  for (unsigned int i = 0; i < jsub_layers.Size(); ++i)
  {
    layer.layers.push_back(read_layer(jsub_layers[i]));
  }

  return layer;
}
