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
  \file tws/core/service_operations_manager.cpp

  \brief A singleton for registering service operations.

  \author Gilberto Ribeiro de Queiroz
 */

// TWS
#include "service_operations_manager.hpp"

// Boost
#include <boost/foreach.hpp>

tws::core::service_operations_manager*
tws::core::service_operations_manager::instance_(nullptr);

void
tws::core::service_operations_manager::insert(const service_metadata& smeta)
{
// check if a service with the same name is already registered
  BOOST_FOREACH(const service_metadata& reg_serv, services_)
  {
    if(reg_serv.name == smeta.name)
    {
      boost::format err_msg("there is already a service registered with the informed name: %1%.");

      throw invalid_service_error() << tws::error_description((err_msg % smeta.name).str());
    }

// check if any operation of the service doesn't conflict with other names
    BOOST_FOREACH(const service_operation& op, reg_serv.operations)
    {
      std::string id = smeta.name + "/" + op.name;

      if(operations_idx_.count(id) != 0)
      {
        boost::format err_msg("there is already a service named '%1%' with an operation registered with the informed name: %2%.");

        throw invalid_service_operation_error() << tws::error_description((err_msg % smeta.name % op.name).str());
      }
    }
  }

// add the service and index the operations
  services_.push_back(smeta);

  BOOST_FOREACH(const service_operation& op, smeta.operations)
  {
    std::string id = "/" + smeta.name + "/" + op.name;

    operations_idx_[id] = op.handler;
  }
}

void
tws::core::service_operations_manager::init()
{
  if(instance_)
    delete instance_;
  
  instance_ = new service_operations_manager;
}

tws::core::service_operations_manager::service_operations_manager()
{
}

tws::core::service_operations_manager::~service_operations_manager()
{
}
