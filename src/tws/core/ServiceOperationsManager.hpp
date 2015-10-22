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
  \file tws/core/service_operations_manager.hpp

  \brief A singleton for registering service operations.

  \author Gilberto Ribeiro de Queiroz
 */

#ifndef __TWS_CORE_SERVICE_OPERATIONS_MANAGER_HPP__
#define __TWS_CORE_SERVICE_OPERATIONS_MANAGER_HPP__

// TWS
#include "config.hpp"
#include "exception.hpp"

// STL
#include <cassert>
#include <map>
#include <string>
#include <vector>

// Boost
#include <boost/format.hpp>
#include <boost/function.hpp>
#include <boost/noncopyable.hpp>

namespace tws
{
  namespace core
  {

// Forward declaration
    class http_request;
    class http_response;

    //! The type of a service operation handler.
    typedef boost::function2<void, const http_request&, http_response&> service_operation_handler_t;

    //! Basic information about a service operation.
    struct service_operation
    {
      std::string name;                     //!< The operation name. Ex: getmap.
      std::string description;              //!< A brief description about the operation. Ex: retrieve a map display for a set of layers.
      service_operation_handler_t handler;  //!< The operation handler may be a function or functor.
    };

    //! Basic information of service.
    struct service_metadata
    {
      std::string name;                           //!< The service name.
      std::vector<service_operation> operations;  //!< The list of service operations.
    };

    //! A singleton for managing service operations.
    class service_operations_manager : public boost::noncopyable
    {
      public:

        //! Return an operation handle.
        /*!
          \exception tws::item_not_found_error It may throws an exception if operation is not found.
         */
        service_operation_handler_t& get(const std::string& op_id);

        //! Insert a new service registering all its operations.
        /*!
          \exception invalid_service_error It may throws an exception if the service can not be registered.
          \exception invalid_service_operation_error It may throws an error if the operation can not be registered.
         */
        void insert(const service_metadata& smeta);

        //! Access the singleton.
        static service_operations_manager& instance();

        //! It is called at Core Runtime startup.
        static void init();

      protected:

        //! Constructor.
        service_operations_manager();

        //! Destructor.
        ~service_operations_manager();

      private:

        std::map<std::string, service_operation_handler_t> operations_idx_;  //!< An index for service operations.
        std::vector<service_metadata> services_;                             //!< The registered services.

        static service_operations_manager* instance_;  //!< The singleton instance.
    };

    inline service_operation_handler_t&
    service_operations_manager::get(const std::string& op_id)
    {
        std::map<std::string, service_operation_handler_t>::iterator it = operations_idx_.find(op_id);

        if(it == operations_idx_.end())
        {
          boost::format err_msg("could not find requested service operation: %1%.");
          
          throw tws::item_not_found_error() << tws::error_description((err_msg % op_id).str());
        }

        return it->second;
      }

      inline service_operations_manager&
      service_operations_manager::instance()
      {
        assert(instance_);

        return *instance_;
      }

  }   // end namespace core
}     // end namespace tws

#endif  // __TWS_CORE_SERVICE_OPERATIONS_MANAGER_HPP__
