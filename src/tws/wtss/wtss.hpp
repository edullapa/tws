/*
  Copyright (C) 2014-2014 National Institute For Space Research (INPE) - Brazil.

  This file is part of the TerraLib Web Services.

  TerraLib Web Services is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License version 3 as
  published by the Free Software Foundation.

  TerraLib Web Services is distributed  "AS-IS" in the hope that it will be useful,
  but WITHOUT ANY WARRANTY OF ANY KIND; without even the implied warranty
  of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License along
  with TerraLib Web Services. See COPYING. If not, see <http://www.gnu.org/licenses/gpl-3.0.html>.
 */

/*!
  \file tws/wtss/wtss.hpp

  \brief Web Time Series Data Service.

  \author Gilberto Ribeiro de Queiroz <gribeiro@dpi.inpe.br>

  \date 2014-2014

  \copyright GNU General Public License version 3
 */

#ifndef __TWS_WTSS_WTSS_HPP__
#define __TWS_WTSS_WTSS_HPP__

// TWS
//#include "config.hpp"

namespace tws
{
  namespace core
  {
    class http_request;
    class http_response;
  }

  namespace wtss
  {

    //! List the name of coverages managed by the server.
    /*! http://chronos.dpi.inpe.br:6543/wtss/list_coverages */
    struct list_coverages_functor
    {
      void operator()(const tws::core::http_request& request,
                      tws::core::http_response& response);
    };

    //! List the metadata about a given coverage.
    /*! http://chronos.dpi.inpe.br:6543/wtss/describe_coverage?name=mod09q1 */
    struct describe_coverage_functor
    {
      void operator()(const tws::core::http_request& request,
                      tws::core::http_response& response);
    };

    //! Retrieve the time series for a given coverage.
    /*! http://chronos.dpi.inpe.br:6543/wtss/time_series?coverage=mod09q1&attributes=red,nir&longitude=-54.0&latitude=-12.0 */
    struct time_series_functor
    {
      void operator()(const tws::core::http_request& request,
                      tws::core::http_response& response);
    };

    //! Register all service operations.
    void register_operations();

    //! Initialize all service operations.
    void initialize_operations();

  }   // end namespace wtss
}     // end namespace tws

#endif  // __TWS_WTSS_WTSS_HPP__
