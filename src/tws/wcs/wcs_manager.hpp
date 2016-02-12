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

    class wcs_manager
    {
      public:
        //! Singleton method
        static wcs_manager& instance();

        capabilities_t capabilities();
        service_provider_t provider();

        service_identification_t identification();

      private:
        wcs_manager();
        ~wcs_manager();

        struct impl;

        impl* pimpl_;
    };
  }
}

#endif
