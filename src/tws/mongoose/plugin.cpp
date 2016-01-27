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
  \file tws/mongoose/plugin.cpp

  \brief Add a plugin interface for dynamic loading of the Mongoose module.

  \author Gilberto Ribeiro de Queiroz
 */

// TWS
#include "../core/http_server.hpp"
#include "../core/http_server_builder.hpp"
#include "config.hpp"
#include "server.hpp"

// TerraLib
#include <terralib/plugin/Plugin.h>

// STL
#include <memory>

std::unique_ptr<tws::core::http_server> build_mongoose()
{
  return std::unique_ptr<tws::core::http_server>(new tws::mongoose::server);
}

class Plugin : public te::plugin::Plugin
{
  public:
  
    Plugin(const te::plugin::PluginInfo& pluginInfo)
      : te::plugin::Plugin(pluginInfo)
    {
    }
  
    ~Plugin()
    {
    }
  
    void startup()
    {
      if(m_initialized)
        return;
      
      tws::core::http_server_builder::instance().insert("mongoose", build_mongoose);
      
      m_initialized = true;
    }
  
    void shutdown()
    {
      if(!m_initialized)
        return;
      
      tws::core::http_server_builder::instance().remove("mongoose");
      
      m_initialized = false;
    }
};

#define export_macro

PLUGIN_CALL_BACK_DECLARATION(export_macro);

PLUGIN_CALL_BACK_IMPL(Plugin)

