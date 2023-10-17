#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <exception>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/asio/signal_set.hpp>

#include <easylogging++.h>

#include <lawnmowerc_settings.hpp>
#include <client.hpp>

#include "lawnmowerc-config.h"

INITIALIZE_EASYLOGGINGPP

static std::string client_version(void)
{
    return std::string(LAWNMOWERC_VERSION_MAJOR) + std::string(".") + std::string(LAWNMOWERC_VERSION_MINOR);
}

//
// glfw: whenever the window size changed (by OS or user resize) this callback function executes
//

static void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

int main(int argc, char *argv[])
{
    try
    {
        std::cout << "LAWNMOWER Client, version " << client_version() << "\n"
                  << " Copyright © 2023 by LLG Ryszard Gradowski, All Rights Reserved.\n\n";

        if (argc == 2 && (strcmp(argv[1], "--help") == 0 || strcmp(argv[1], "-h") == 0))
        {
            std::cout << "Help is not implemented yet\n";

            return 0;
        }

        //
        // Create main configuration object.
        //

        lawnmowerc_settings settings(argc, argv);

        //
        // Setup logging infrastructure.
        //

        std::ostringstream logconf;

        logconf << "* GLOBAL:\n"
                << " FORMAT               =  \"%datetime %level [%logger] %msg\"\n"
                << " FILENAME             =  \"/var/log/lawnmower/client.log\"\n"
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
        // Initialize and configure glfw.
        //

        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        //
        // Create window.
        //

        GLFWwindow *window = glfwCreateWindow(settings.get_camera_width(), settings.get_camera_height(), "Lawnmower", NULL, NULL);

        if (window == NULL)
        {
            std::cerr << "ERROR: Failed to create GLFW window" << std::endl;

            glfwTerminate();

            return 1;
        }

        glfwMakeContextCurrent(window);
        glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

        //
        // Setup GLEW.
        //

        glewExperimental = 1;

        if (glewInit() != GLEW_OK)
        {
            std::cerr << "ERROR: Failed to setup GLEW" << std::endl;

            glfwTerminate();

            return 1;
        }

        //
        // Start client.
        //

        boost::asio::io_context ioctx;

        boost::asio::signal_set signals(ioctx, SIGINT, SIGTERM);
        signals.async_wait(boost::bind(&boost::asio::io_context::stop, &ioctx));

        client lawnmowerc(window, ioctx, settings);

        lawnmowerc.connect();

        ioctx.run();

        glfwTerminate();

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

    glfwTerminate();

    return 1;
}
