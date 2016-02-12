namespace tws
{
  namespace wcs
  {
    //! Struct for handling contact provider metadata
    struct contact_metadata_t
    {
      std::string person;
      std::string organization;
      std::string position;
      std::string address;
      std::string voice_telephone;
      std::string electronic_mail_address;
    };

    //! Struct for handling service metadata
    struct service_identification_t
    {
      std::string title;
      std::string abstract;
      std::string service_type;
      std::vector<std::string> profiles;
    };

    // Struct for handling service provider metadata
    struct service_provider_t
    {
      std::string name;
      std::string site;
      contact_metadata_t contact;
    };

    struct interpolation_metadata_t
    {
      std::vector<std::string> interpolation_supported;
    };

    struct extension_t
    {
      interpolation_metadata_t interpolation_metadata;
    };

    struct service_metadata_t
    {
      std::vector<std::string> formats_supported;
      extension_t extension;
    };

    struct coverage_sub_type_parent_t
    {
      std::string coverage_sub_type;
      coverage_sub_type_parent_t* parent;
    };

    struct coverage_summary_t
    {
      std::string coverage_id;
      std::string coverage_sub_type; // TODO: enum with constants
      coverage_sub_type_parent_t coverage_sub_type_parent;
    };

    struct contents_t
    {
      std::vector<coverage_summary_t> summaries;
    };

    struct capabilities_t
    {
      service_identification_t identification;
      service_provider_t provider;
      contents_t contents;
    };
  }
}
