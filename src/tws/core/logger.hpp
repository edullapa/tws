#ifndef __TWS_CORE_LOGGER_HPP__
#define __TWS_CORE_LOGGER_HPP__

// TWS
#include "exception.hpp"

// Boost
#include <boost/shared_ptr.hpp>
#include <boost/log/common.hpp>
#include <boost/log/sinks/async_frontend.hpp>
#include <boost/log/sinks/text_ostream_backend.hpp>
#include <boost/log/utility/manipulators/add_value.hpp>


namespace tws
{
  namespace core
  {
    class logger
    {

      public:

        //! Defines level of severity message in \c tws log.
        enum severity_level
        {
          TRACE,   ///< Define a trace system alert in file. It will be useful to trace where it has gone
          DEBUG,   ///< Define a debug severity message in file. It will be useful in development side.
          INFO,    ///< Define a info message for notify a common action.
          WARNING, ///< Define a warning alert in log file, representing that something may go wrong.
          ERROR,   ///< Define a error system alert. Useful when something unexpected occurs.
          FATAL    ///< Define a fatal application alert. A runtime error or uncaught exception that shutdown application.
        };

        /*!
          \brief An abreviation of a boost text log stream
        */
        typedef boost::log::sinks::asynchronous_sink<boost::log::sinks::text_ostream_backend> text_sink;

        //! Singleton
        static logger& instance();

        //! It initializes configuration to global boost logger.
        void initialize();

        //! It enables the global tws logger to allow display status message.
        void enable_log();

        //! It disables the global tws logger to avoid display status message.
        void disable_log();

        //! It retrieves the stream name where the tws.log will be saved
        const std::string& path() const;

        //! It sets path of tws log and add ostream to sink backend
        void add_stream(const std::string& stream_name);

      private:
        /*!
          \brief Protected default constructor.
        */
        logger();

        //! Destructor
        ~logger();

      private:
        boost::shared_ptr<text_sink> sink_; //!< Sink for handling records.
        std::string logger_path_;            //!< Path to the log.
    }; // end class logger

    //! Note: Its not working
    //! Override operator<< to enable sets tws exception in log. It formats the exception and put it in stream
    inline std::ostream& operator<<(std::ostream& stream, const tws::exception& exception)
    {
      const auto msg = boost::get_error_info<tws::error_description>(exception);
      std::string message = "** An exception occurred **! \t";
      if (msg != nullptr)
        message.append(msg->c_str());
      return stream << message;
    }

  } // end core
} // end tws

BOOST_LOG_INLINE_GLOBAL_LOGGER_DEFAULT(tws_logger, boost::log::sources::severity_logger<tws::core::logger::severity_level>)

#define TWS_LOG(severity) BOOST_LOG_SEV(tws_logger::get(), severity) \
                                                      << boost::log::add_value("RecordLine", __LINE__) \
                                                      << boost::log::add_value("SrcFile", __FILE__) \
                                                      << boost::log::add_value("CurrentFunction", BOOST_CURRENT_FUNCTION)

#define TWS_LOG_TRACE() TWS_LOG(tws::core::logger::TRACE)
#define TWS_LOG_DEBUG() TWS_LOG(tws::core::logger::DEBUG)
#define TWS_LOG_INFO() TWS_LOG(tws::core::logger::INFO)
#define TWS_LOG_WARNING() TWS_LOG(tws::core::logger::WARNING)
#define TWS_LOG_ERROR() TWS_LOG(tws::core::logger::ERROR)
#define TWS_LOG_FATAL() TWS_LOG(tws::core::logger::FATAL)


#endif // __TWS_CORE_LOGGER_HPP__
