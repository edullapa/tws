// tws
#include "wcs_manager.hpp"
#include "../core/utils.hpp"
#include "data_types.hpp"
#include "../geoarray/geoarray_manager.hpp"
#include "../geoarray/data_types.hpp"

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
  capabilities.metadata = metadata();

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
      throw tws::parse_error() << tws::error_description("Could not find provider in wcs config file");
    }

    const rapidjson::Value& name = provider_object["name"];
    if (!name.IsString())
    {
      throw tws::parse_error() << tws::error_description("Could not find provider name in wcs config file");
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
      throw tws::parse_error() << tws::error_description("Could not find contact in wcs config file");
    }

    contact_metadata_t contact;
    const rapidjson::Value& person = contact_object["person"];
    if (!person.IsString())
    {
      throw tws::parse_error() << tws::error_description("Could not find contact person in wcs config file");
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
      throw tws::parse_error() << tws::error_description("Could not find title in wcs config file");
    }

    identification.title = title.GetString();

    const rapidjson::Value& abstract = (*pimpl_->json_file)["abstract"];
    if (!abstract.IsString())
    {
      throw tws::parse_error() << tws::error_description("Could not find abstract in wcs config file");
    }

    identification.abstract = abstract.GetString();

    const rapidjson::Value& profiles = (*pimpl_->json_file)["profiles"];

    if (!profiles.IsArray())
    {
      throw tws::parse_error() << tws::error_description("Could not find profiles in wcs config file");
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

tws::wcs::service_metadata_t tws::wcs::wcs_manager::metadata()
{
  try
  {
    service_metadata_t metadata;
    const rapidjson::Value& formats = (*pimpl_->json_file)["formats_supported"];
    if (!formats.IsArray())
    {
      throw tws::parse_error() << tws::error_description("Could not find formats supported in wcs config file");
    }

    for (unsigned int i = 0; i < formats.Size(); ++i)
    {
      metadata.formats_supported.push_back(formats[i].GetString());
    }

    return metadata;
  }
  catch(...)
  {
    throw;
  }
}

tws::wcs::describe_coverage_t tws::wcs::wcs_manager::describe_coverage()
{
  describe_coverage_t describe;

  std::vector<std::string> arrays = tws::geoarray::geoarray_manager::instance().list_arrays();
  for(const auto& array: arrays)
  {
    auto geoarray = tws::geoarray::geoarray_manager::instance().get(array);

    coverage_description_t description;
    description.id = geoarray.name;

    envelope_t envelope;

    std::string min;
    std::string max;
    std::string axis;
    int dimension_size = 0;

    for(const auto& dimension: geoarray.dimensions)
    {
      axis.append(dimension.name + " ");
      min.append(std::to_string(dimension.min_idx) + " ");
      max.append(std::to_string(dimension.max_idx) + " ");
      ++dimension_size;
    }

    // todo: remove last whitespace
    envelope.axis = axis;
    envelope.min = min;
    envelope.max = max;
    envelope.dimension = dimension_size;


    bounded_by_t bounded;
    bounded.envelope = envelope;
    description.bounded_by = bounded;

    description.geoarray = geoarray;

    describe.coverages_description.push_back(description);
  }

  return describe;
}
