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
  \file tws/core/Utils.hpp

  \brief Utility functions.

  \author Gilberto Ribeiro de Queiroz
 */

#ifndef __TWS_CORE_LOGGER_HPP__
#define __TWS_CORE_LOGGER_HPP__

// TWS
#include <terralib/core/logger/Logger.h>

#define TWS_LOGGER "tws"
#define TWS_LOGGER_FORMAT "[%TimeStamp%] <%Severity%>: %Message%"

#define TWS_LOG_TRACE(message) TE_CORE_LOG_TRACE(TWS_LOGGER, message)
#define TWS_LOG_DEBUG(message) TE_CORE_LOG_DEBUG(TWS_LOGGER, message)
#define TWS_LOG_INFO(message) TE_CORE_LOG_INFO(TWS_LOGGER, message)
#define TWS_LOG_WARN(message) TE_CORE_LOG_WARN(TWS_LOGGER, message)
#define TWS_LOG_ERROR(message) TE_CORE_LOG_ERROR(TWS_LOGGER, message)
#define TWS_LOG_FATAL(message) TE_CORE_LOG_FATAL(TWS_LOGGER, message)


#endif // __TWS_CORE_LOGGER_HPP__
