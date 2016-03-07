// tws
#include "logger.hpp"

// Boost
#include <boost/log/attributes.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
//#include <boost/core/null_deleter.hpp>
#include <boost/log/expressions.hpp>

// STL
#include <fstream>


BOOST_LOG_ATTRIBUTE_KEYWORD(line_id, "SrcFile", unsigned int)
BOOST_LOG_ATTRIBUTE_KEYWORD(severity, "Severity", tws::core::logger::severity_level)
BOOST_LOG_ATTRIBUTE_KEYWORD(tag_attr, "Tag", std::string)

void log_formatter(const boost::log::record_view& rec, boost::log::formatting_ostream& stream)
{
  const auto severity = boost::log::extract<tws::core::logger::severity_level>("Severity", rec);

  const auto timeStamp = boost::log::extract<boost::posix_time::ptime>("TimeStamp", rec);

  if (timeStamp.empty())
    return;

  boost::posix_time::time_facet* facet = new boost::posix_time::time_facet("%d-%b-%Y %H:%M:%S");
  stream.imbue(std::locale(stream.getloc(), facet));

  stream << "[" << timeStamp << "] ";

  if(severity.empty())
    return;

  switch (*severity)
  {
    case tws::core::logger::TRACE:
      stream << "<TRACE>";
      break;
    case tws::core::logger::DEBUG:
      stream << "<DEBUG>";
      break;
    case tws::core::logger::INFO:
      stream << "<INFO>";
      break;
    case tws::core::logger::WARNING:
      stream << "<WARNING>";
      break;
    case tws::core::logger::ERROR:
    {
      stream << "<ERROR>";
      stream << " {" << boost::log::extract<std::string>("SrcFile", rec) << ", " << boost::log::extract<int>("RecordLine", rec) << "}";
      break;
    }
    case tws::core::logger::FATAL:
      stream << "<FATAL>";
      stream << " {" << boost::log::extract<std::string>("SrcFile", rec) << ", " << boost::log::extract<int>("RecordLine", rec) << "}";
      break;
    default:
      stream << "<" << *severity << ">";
  }

  stream << " " << rec[boost::log::expressions::smessage];
}

tws::core::logger::logger()
  : sink_(new text_sink)
{

}

tws::core::logger::~logger()
{

}

tws::core::logger &tws::core::logger::instance()
{
  static logger instance;
  return instance;
}

void tws::core::logger::initialize()
{
  sink_->locked_backend()->auto_flush(true);

  sink_->set_formatter(&log_formatter);

  // It sets the boost log filter
//  sink_->set_filter(severity != DEBUG);

  boost::log::core::get()->add_sink(sink_);

  boost::log::core::get()->add_global_attribute("TimeStamp", boost::log::attributes::local_clock());

  enable_log();
}

void tws::core::logger::enable_log()
{
  boost::log::core::get()->set_logging_enabled(true);
}

void tws::core::logger::disable_log()
{
  boost::log::core::get()->set_logging_enabled(false);
}

void tws::core::logger::add_stream(const std::string& stream_name)
{
  logger_path_ = stream_name;
  boost::shared_ptr<std::ostream> stream_out(&std::clog);//, boost::null_deleter());
  boost::shared_ptr<std::ostream> stream_file(new std::ofstream(stream_name, std::ostream::app));
  sink_->locked_backend()->add_stream(stream_file);
  sink_->locked_backend()->add_stream(stream_out);
}

const std::string& tws::core::logger::path() const
{
  return logger_path_;
}
