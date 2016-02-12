// tws
#include "wcs_manager.hpp"
#include "../core/utils.hpp"
#include "data_types.hpp"

// rapidjson
#include <rapidjson/document.h>

// std
#include <memory>

struct tws::wcs::wcs_manager::impl
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

tws::wcs::wcs_manager::wcs_manager()
  : pimpl_(nullptr)
{
  pimpl_ = new impl;

  std::string wcs_file = tws::core::find_in_app_path("share/tws/config/wcs.json");
  pimpl_->json_file.reset(tws::core::open_json_file(wcs_file));
}

tws::wcs::wcs_manager::~wcs_manager()
{
  delete pimpl_;
}

tws::wcs::wcs_manager& tws::wcs::wcs_manager::instance()
{
  static wcs_manager instance;
  return instance;
}

tws::wcs::capabilities_t tws::wcs::wcs_manager::capabilities()
{
  capabilities_t capabilities;

  capabilities.identification = identification();
  capabilities.provider = provider();

  return capabilities;
}

tws::wcs::service_provider_t tws::wcs::wcs_manager::provider()
{
  try
  {
    service_provider_t provider;
    const rapidjson::Value& provider_object = (*pimpl_->json_file)["provider"];
    if (!provider_object.IsObject())
    {
      throw tws::parser_error() << tws::error_description("Could not find provider in wcs config file");
    }

    const rapidjson::Value& name = provider_object["name"];
    if (!name.IsString())
    {
      throw tws::parser_error() << tws::error_description("Could not find provider name in wcs config file");
    }
    provider.name = name.GetString();

    const rapidjson::Value& site = provider_object["site"];
    if (!site.IsString())
      provider.site = "";
    else
      provider.site = site.GetString();

    const rapidjson::Value& contact_object = provider_object["contact"];
    if (!contact_object.IsObject())
    {
      throw tws::parser_error() << tws::error_description("Could not find contact in wcs config file");
    }

    contact_metadata_t contact;
    const rapidjson::Value& person = contact_object["person"];
    if (!person.IsString())
    {
      throw tws::parser_error() << tws::error_description("Could not find contact person in wcs config file");
    }

    contact.person = person.GetString();

    provider.contact = contact;

    return provider;
  }
  catch(...)
  {
    throw;
  }
}

tws::wcs::service_identification_t tws::wcs::wcs_manager::identification()
{
  try
  {
    service_identification_t identification;
    const rapidjson::Value& title = (*pimpl_->json_file)["title"];
    if (!title.IsString())
    {
      throw tws::parser_error() << tws::error_description("Could not find title in wcs config file");
    }

    identification.title = title.GetString();

    const rapidjson::Value& abstract = (*pimpl_->json_file)["abstract"];
    if (!abstract.IsString())
    {
      throw tws::parser_error() << tws::error_description("Could not find abstract in wcs config file");
    }

    identification.abstract = abstract.GetString();

    const rapidjson::Value& profiles = (*pimpl_->json_file)["profiles"];

    if (!profiles.IsArray())
    {
      throw tws::parser_error() << tws::error_description("Could not find profiles in wcs config file");
    }

    for (unsigned int i = 0; i < profiles.Size(); ++i)
    {
      identification.profiles.push_back(profiles[i].GetString());
    }

    identification.service_type = "WCS";

    return identification;
  }
  catch(...)
  {
    throw;
  }
}
