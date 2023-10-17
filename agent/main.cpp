#include <iostream>
#include <exception>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/asio/signal_set.hpp>
#include <easylogging++.h>

#include <agent_settings.hpp>
#include <device_server.hpp>

#include "agent-config.h"

INITIALIZE_EASYLOGGINGPP

static std::string agent_version(void)
{
    return std::string(AGENT_VERSION_MAJOR) + std::string(".") + std::string(AGENT_VERSION_MINOR);
}

int main(int argc, char *argv[])
{
    try
    {
        std::cout << "LAWNMOWER Device Agent, version " << agent_version() << "\n"
                  << " Copyright © 2023 by LLG Ryszard Gradowski, All Rights Reserved.\n\n";

        if (argc == 2 && (strcmp(argv[1], "--help") == 0 || strcmp(argv[1], "-h") == 0))
        {
            std::cout << "Help is not implemented yet\n";

            return 0;
        }

        //
        // Create main configuration object.
        //

        agent_settings settings(argc, argv);

        //
        // Setup logging infrastructure.
        //

        std::ostringstream logconf;

        logconf << "* GLOBAL:\n"
                << " FORMAT               =  \"%datetime %level [%logger] %msg\"\n"
                << " FILENAME             =  \"/var/log/lawnmower/agent.log\"\n"
                << " ENABLED              =  true\n"
                << " TO_FILE              =  true\n"
                << " TO_STANDARD_OUTPUT   =  false\n"
                << " SUBSECOND_PRECISION  =  1\n"
                << " PERFORMANCE_TRACKING =  true\n"
                << " MAX_LOG_FILE_SIZE    =  10485760 ## 10MiB\n"
                << " LOG_FLUSH_THRESHOLD  =  1 ## Flush after every single log\n"
                << "* FATAL:\n"
                << " ENABLED              =  true\n"
                << "* ERROR:\n"
                << " ENABLED              =  " << "true\n" //(log_severity_ >= logging_severity::HFT_SEVERITY_ERROR ? "true\n" : "false\n")
                << "* WARNING:\n"
                << " ENABLED              =  " << "true\n" /* (log_severity_ >= logging_severity::HFT_SEVERITY_WARNING ? "true\n" : "false\n") */
                << "* INFO:\n"
                << " ENABLED              =  " << "true\n" /* (log_severity_ >= logging_severity::HFT_SEVERITY_INFO ? "true\n" : "false\n") */
                << "* TRACE:\n"
                << " ENABLED              =  " << "true\n" /* (log_severity_ >= logging_severity::HFT_SEVERITY_TRACE ? "true\n" : "false\n") */
                << "* DEBUG:\n"
                << " ENABLED              =  " << "true\n"; /* (log_severity_ >= logging_severity::HFT_SEVERITY_DEBUG ? "true\n" : "false\n"); */

        el::Configurations logger_cfg;
        logger_cfg.setToDefault();
        logger_cfg.parseFromText(logconf.str().c_str());
        el::Loggers::setDefaultConfigurations(logger_cfg);

        START_EASYLOGGINGPP(argc, argv);

        //
        // Start device server.
        //

        boost::asio::io_context ioctx;

        boost::asio::signal_set signals(ioctx, SIGINT, SIGTERM);
        signals.async_wait(boost::bind(&boost::asio::io_context::stop, &ioctx));

        device_server service(ioctx, settings);

        ioctx.run();

        return 0;
    }
    catch (const std::exception &e)
    {
        std::cerr << "fatal error: " << e.what() << "\n";

        el::base::debug::StackTrace();
    }
    catch (...)
    {
        std::cerr << "fatal error: General exception\n";
    }

    return 1;
}
