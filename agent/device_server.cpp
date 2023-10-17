#include <device_server.hpp>

#include <easylogging++.h>

using boost::asio::ip::tcp;

#define agent_log(__X__) \
    CLOG(__X__, "device_server")

device_server::device_server(boost::asio::io_context& io_context, const agent_settings &settings)
    : io_context_(io_context),
      settings_(settings),
      acceptor_(io_context, tcp::endpoint(tcp::v4(), settings_.get_port())),
      context_(boost::asio::ssl::context::sslv23)
{
    el::Logger *logger = el::Loggers::getLogger("device_server", true);

    context_.set_options(boost::asio::ssl::context::default_workarounds
                         | boost::asio::ssl::context::no_sslv2
                         | boost::asio::ssl::context::single_dh_use);
    context_.set_password_callback(std::bind(&device_server::get_password, this));
    context_.use_certificate_chain_file(settings_.get_certificate_chain_file());
    context_.use_private_key_file(settings_.get_private_key_file(), boost::asio::ssl::context::pem);

    if (! settings_.get_tmp_dh_file().empty())
    {
        context_.use_tmp_dh_file(settings_.get_tmp_dh_file());
    }

    do_accept();
}

void device_server::do_accept(void)
{
    agent_log(INFO) << "Starting accept.";

    acceptor_.async_accept(
        [this](const boost::system::error_code& error, tcp::socket socket)
        {
            if (! error)
            {
                if (! device_session::pending_session())
                {
                    std::make_shared<device_control>(io_context_, boost::asio::ssl::stream<tcp::socket>(std::move(socket), context_), settings_) -> start();
                }
                else
                {
                    socket.close();

                    agent_log(WARNING) << "Connection refused – session already pending.";
                }
            }
            else
            {
                agent_log(ERROR) << "Accepting connection error ("
                                 << error.value() << "): "
                                 << error.message() << ".";
            }

            do_accept();
        });
}
