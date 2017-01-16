/*
  Copyright (C) 2014 National Institute For Space Research (INPE) - Brazil.

  This file is part of the TerraLib GeoWeb Services.

  TerraLib GeoWeb Services is free software: you can redistribute it and/or
  modify
  it under the terms of the GNU General Public License version 3 as
  published by the Free Software Foundation.

  TerraLib GeoWeb Services is distributed  "AS-IS" in the hope that it will be
  useful,
  but WITHOUT ANY WARRANTY OF ANY KIND; without even the implied warranty
  of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
  GNU Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public License along
  with TerraLib Web Services. See COPYING. If not, see
  <http://www.gnu.org/licenses/lgpl-3.0.html>.
 */

/*!
  \file tws/mongoose/server.cpp

  \brief TWS Server over Mongoose.

  \author Gilberto Ribeiro de Queiroz
 */

// TWS
#include "server.hpp"
#include "../core/service_operations_manager.hpp"
#include "../core/utils.hpp"
#include "exception.hpp"
#include "http_request.hpp"
#include "http_response.hpp"

// STL
#include <cstdio>

// Boost
#include <boost/filesystem.hpp>
#include <boost/format.hpp>
#include <boost/lexical_cast.hpp>

// Mongoose
#include "mongoose.h"

// RapidJSON
#include <rapidjson/document.h>
#include <rapidjson/filestream.h>

static void tws_mongoose_event_handler(mg_connection* conn, int ev,
                                       void* ev_data);

struct tws_mongoose_http_config
{
  std::string log_file;
  std::string document_root;
  uint32_t listening_port;
  uint32_t max_threads;
  uint32_t max_connections;
};

tws_mongoose_http_config tws_mongoose_read_config_file();

struct tws::mongoose::server::impl
{
  bool stop_;

  impl();

  ~impl();
};

tws::mongoose::server::impl::impl() : stop_(true) {}
tws::mongoose::server::impl::~impl() {}
tws::mongoose::server::server() : pimpl_(nullptr) { pimpl_ = new impl; }
tws::mongoose::server::~server() { delete pimpl_; }
void tws::mongoose::server::start()
{
  pimpl_->stop_ = false;

  struct mg_mgr server_;
  struct mg_connection* conn_;

  tws_mongoose_http_config conf = tws_mongoose_read_config_file();

  mg_mgr_init(&server_, NULL);

  conn_ = mg_bind(&server_,
                  boost::lexical_cast<std::string>(conf.listening_port).c_str(),
                  tws_mongoose_event_handler);

  mg_set_protocol_http_websocket(conn_);

  if(conf.max_threads > 1)
    mg_enable_multithreading(conn_);\

  while(pimpl_->stop_ == false)
  {
    mg_mgr_poll(&server_, 1000);
  }
  mg_mgr_free(&server_);
}

void tws::mongoose::server::stop() { pimpl_->stop_ = true; }
void tws_mongoose_event_handler(struct mg_connection* conn, int ev,
                                void* ev_data)
{
  if(ev == MG_EV_HTTP_REQUEST)
  {
    try
    {
      struct http_message* hm = (struct http_message*)ev_data;

      tws::core::service_operation_handler_t& op =
          tws::core::service_operations_manager::instance().get(
              std::string(hm->uri.p, hm->uri.len));

      tws::mongoose::http_request sg_request(hm);
      tws::mongoose::http_response sg_response(conn);

      op(sg_request, sg_response);
    }
    catch(const boost::exception& e)
    {
      std::string err_msg = "Error: ";

      if(const std::string* d =
             boost::get_error_info<tws::error_description>(e))
        err_msg += *d;
      else
        err_msg += "unknown";

      mg_send_head(conn, 400, err_msg.size(), "Content-Type: text/plain");
      mg_send(conn, err_msg.c_str(), err_msg.size());
    }
  }
}

tws_mongoose_http_config tws_mongoose_read_config_file()
{
  tws_mongoose_http_config result;

  std::string input_file =
      tws::core::find_in_app_path("share/tws/config/mongoose_web_server.json");

  if(input_file.empty())
    throw tws::file_exists_error() << tws::error_description(
        "could not locate web server config file: "
        "'share/tws/config/mongoose_web_server.json'.");

  if(!boost::filesystem::is_regular(input_file))
  {
    boost::format err_msg("input file '%1%' doesn't exist.");

    throw tws::file_exists_error()
        << tws::error_description((err_msg % input_file).str());
  }

  FILE* pfile = fopen(input_file.c_str(), "r");

  if(pfile == nullptr)
  {
    boost::format err_msg("error opening input file '%1%'.");

    throw tws::file_open_error()
        << tws::error_description((err_msg % input_file).str());
  }

  try
  {
    rapidjson::FileStream istr(pfile);

    rapidjson::Document doc;

    doc.ParseStream<0>(istr);

    if(doc.HasParseError())
    {
      boost::format err_msg("error parsing input file '%1%': %2%.");

      throw tws::parse_error() << tws::error_description(
          (err_msg % input_file % doc.GetParseError()).str());
    }

    if(!doc.IsObject() || doc.IsNull())
    {
      boost::format err_msg(
          "error parsing input file '%1%': unexpected file format.");

      throw tws::parse_error()
          << tws::error_description((err_msg % input_file).str());
    }

    const rapidjson::Value& jlistening_port = doc["listening_port"];

    if(!jlistening_port.IsNumber() || jlistening_port.IsNull())
    {
      boost::format err_msg(
          "error parsing input file '%1%': expecting listening_port argument.");

      throw tws::parse_error()
          << tws::error_description((err_msg % input_file).str());
    }

    result.listening_port = jlistening_port.GetUint();

    const rapidjson::Value& jmax_threads = doc["max_threads"];

    if(!jmax_threads.IsNumber() || jmax_threads.IsNull())
    {
      boost::format err_msg(
          "error parsing input file '%1%': expecting max_threads argument.");

      throw tws::parse_error()
          << tws::error_description((err_msg % input_file).str());
    }

    result.max_threads = jmax_threads.GetUint();

    const rapidjson::Value& jmax_connections = doc["max_connections"];

    if(!jmax_connections.IsNumber() || jmax_connections.IsNull())
    {
      boost::format err_msg(
          "error parsing input file '%1%': expecting max_connections "
          "argument.");

      throw tws::parse_error()
          << tws::error_description((err_msg % input_file).str());
    }

    result.max_connections = jmax_connections.GetUint();

    const rapidjson::Value& jlog_file = doc["log_file"];

    if(!jlog_file.IsString() || jlog_file.IsNull())
    {
      boost::format err_msg(
          "error parsing input file '%1%': expecting log_file argument.");

      throw tws::parse_error()
          << tws::error_description((err_msg % input_file).str());
    }

    result.log_file = jlog_file.GetString();

    const rapidjson::Value& jdocument_root = doc["document_root"];

    if(!jdocument_root.IsString() || jdocument_root.IsNull())
    {
      boost::format err_msg(
          "error parsing input file '%1%': expecting document_root argument.");

      throw tws::parse_error()
          << tws::error_description((err_msg % input_file).str());
    }

    result.document_root = jdocument_root.GetString();
  }
  catch(...)
  {
    fclose(pfile);
    throw;
  }

  fclose(pfile);

  // result.log_file = "tws.log";
  // result.document_root = "/opt/www";
  // result.listening_port = 6543;
  // result.max_threads = 100;
  // result.max_connections = 1000;

  return result;
}
