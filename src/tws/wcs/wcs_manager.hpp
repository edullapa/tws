#ifndef __TWS_WCS_WCSMANAGER_HPP__
#define __TWS_WCS_WCSMANAGER_HPP__

namespace tws
{
  namespace wcs
  {
    // forward declarations
    struct capabilities_t;
    struct service_identification_t;
    struct service_provider_t;
    struct service_metadata_t;
    struct describe_coverage_t;

    class wcs_manager
    {
      public:
        //! Singleton method
        static wcs_manager& instance();

        /*!
          \brief It makes a capabilities operation object with wcs metadata
          \return A tws::wcs::capabilities_t struct containing wcs configuration
        */
        capabilities_t capabilities();

        /*!
          \brief It makes a provider object with wcs metadata
          \return A tws::wcs::service_provider_t struct containing wcs configuration
        */
        service_provider_t provider();

        /*!
          \brief It makes a indentification object with wcs metadata
          \return A tws::wcs::service_identification_t struct containing wcs configuration
        */
        service_identification_t identification();

        /*!
          \brief It makes a service metadata object with wcs metadata
          \return A tws::wcs::service_metadata_t struct containing wcs configuration
        */
        service_metadata_t metadata();

        /*!
          \brief It makes a describe coverage operation object with wcs coverages metadata
          \return A tws::wcs::describe_coverage_t struct containing wcs coverage(s) configuration
        */
        describe_coverage_t describe_coverage();

      private:
        //! Constructor
        wcs_manager();

        //! Destructor
        ~wcs_manager();

        struct impl; //!< Pimpl idiom

        impl* pimpl_;
    };
  }
}

#endif
