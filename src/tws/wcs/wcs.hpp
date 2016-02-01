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
  \file tws/wcs/wcs.hpp

  \brief Web Coverage Service.

  \author Raphael Willian da Costa
 */

#ifndef __TWS_WCS_WCS_HPP__
#define __TWS_WCS_WCS_HPP__

// TWS
//#include "config.hpp"

namespace tws
{
  namespace core
  {
    class http_request;
    class http_response;
  }

  namespace wcs
  {

    //! List the name of coverages managed by the server.
    /*! http://chronos.dpi.inpe.br:6543/wcs/GetCapabilities */
    struct get_capabilities_functor
    {
      void operator()(const tws::core::http_request& request,
                      tws::core::http_response& response);
    };

    //! List the metadata about a given coverage.
    /*! http://chronos.dpi.inpe.br:6543/wcs/DescribeCoverage?coverageID=mod09q1 */
    struct describe_coverage_functor
    {
      void operator()(const tws::core::http_request& request,
                      tws::core::http_response& response);
    };

    //! Retrieve the time series for a given coverage.
    /*! http://chronos.dpi.inpe.br:6543/wcs/GetCoverage?coverageID=mod09q1 */
    struct get_coverage_functor
    {
      void operator()(const tws::core::http_request& request,
                      tws::core::http_response& response);
    };

    //! Register all service operations.
    void register_operations();

    //! Initialize all service operations.
    void initialize_operations();

  }   // end namespace wcs
}     // end namespace tws

#endif  // __TWS_WCS_WCS_HPP__
