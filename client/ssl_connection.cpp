#include <ssl_connection.hpp>

#include <easylogging++.h>

#define TIMER_INTERVAL_MILLIS 50

#define client_log(__X__) \
    CLOG(__X__, "ssl_connection")
    
ssl_connection::ssl_connection(boost::asio::io_context &io_context, const lawnmowerc_settings &cfg, output_terminal &console)
    : settings_ {cfg},
      console_ {console},
      ssl_ctx_ {boost::asio::ssl::context::sslv23},
      socket_ {io_context, ssl_ctx_},
      ioctx_ {io_context},
      message_len_buf_ {'\0', '\0', '\0', '\0'},
      message_len_ {0},
      message_total_read_ {0},
      write_status_ {false}
{
    el::Loggers::getLogger("ssl_connection", true);
}

ssl_connection::~ssl_connection(void)
{
    boost::system::error_code ec;

    socket_.shutdown(ec);

    if (ec)
    {
        client_log(ERROR) << "Shutdown connection error ("
                          << ec.value() << "): "
                          << ec.message() << ".";

        console_ << "E: SSL Shutdown connection error ("
                 << ec.value() << "): "
                 << ec.message() << ".\n";
    }

    socket_.lowest_layer().close(ec);

    if (ec)
    {
        client_log(ERROR) << "Close connection error ("
                          << ec.value() << "): "
                          << ec.message() << ".";

        console_ << "E: SSL Close connection error ("
                 << ec.value() << "): "
                 << ec.message() << ".\n";
    }

    client_log(INFO) << "Connection to device closed.";

    console_ << "SSL: Connection to device closed.\n";
}

void ssl_connection::connect(void)
{
    //
    // Check callbacks.
    //

    if (! on_connected)
    {
        client_log(ERROR) << "Undefined ‘on_connected’ callback";

        throw std::logic_error("Undefined ‘on_connected’ callback");
    }

    if (! on_error)
    {
        client_log(ERROR) << "Undefined ‘on_error’ callback";

        throw std::logic_error("Undefined ‘on_error’ callback");
    }

    if (! on_data)
    {
        client_log(ERROR) << "Undefined ‘on_data’ callback";

        throw std::logic_error("Undefined ‘on_data’ callback");
    }

    boost::asio::ip::tcp::resolver resolver(ioctx_);
    auto endpoint = resolver.resolve(settings_.get_host(), settings_.get_port());

    socket_.set_verify_mode(boost::asio::ssl::verify_peer);
    socket_.set_verify_callback([this](auto preverified, auto &ctx) {
            // FIXME: Potem rozkminić jak to obsłużyć pożądnie.
            char subject_name[256];
            X509* cert = X509_STORE_CTX_get_current_cert(ctx.native_handle());
            X509_NAME_oneline(X509_get_subject_name(cert), subject_name, 256);
            client_log(INFO) << "Verifying " << subject_name;
            console_  << "Verifying " << subject_name << "\n";

			return true; // Jeśli cert ok, ma zwrócić true.
	});

    client_log(INFO) << "Starting async connection to the device "
                     << (*endpoint.begin()).host_name() << " ("
                     << (*endpoint.begin()).endpoint().address().to_string()
                     << ")";

    console_ << "SSL: Starting async connection to the device "
             << (*endpoint.begin()).host_name() << " ("
             << (*endpoint.begin()).endpoint().address().to_string()
             << ")\n";

    boost::asio::async_connect(socket_.lowest_layer(), endpoint,
        [this](const boost::system::error_code &error, const boost::asio::ip::tcp::endpoint &endpoint)
        {
            if (! error)
            {
                handshake();
            }
            else
            {
                client_log(ERROR) << "Connection to the device "
                                  << endpoint.address().to_string()
                                  << " failed ("
                                  << error.value() << "): "
                                  << error.message() << ".";

                console_ << "E: SSL Connection to the device "
                         << endpoint.address().to_string()
                         << " failed ("
                         << error.value() << "): "
                         << error.message() << ".\n";

                on_error(error);
            }
        });
}

void ssl_connection::send_data(const std::vector<char> &data)
{
    write_status_ = false;

    send_buffer_.resize(data.size() + 4);
    int data_size_big_endian = htonl(data.size());
    memcpy(&send_buffer_.front(), &data_size_big_endian, 4);
    memcpy(&send_buffer_.front() + 4, &data.front(), data.size());

    boost::asio::async_write(socket_, boost::asio::buffer(&send_buffer_.front(), send_buffer_.size()),
        [this](const boost::system::error_code &error, std::size_t length)
        {
            if (! error)
            {
                write_status_ = true;
            }
            else
            {
                client_log(ERROR) << "SSL Write data error ("
                                  << error.value() << "): "
                                  << error.message() << ".";

                console_ << "E: SSL Write data error ("
                         << error.value() << "): "
                         << error.message() << ".\n";

                on_error(error);
            }
        });
}

void ssl_connection::handshake(void)
{
    client_log(INFO) << "Starting async SSL/TLS negotiation.";
    console_ << "SSL: Starting async SSL/TLS negotiation.\n";

    socket_.async_handshake(boost::asio::ssl::stream_base::client,
        [this](const boost::system::error_code &error)
        {
            if (! error)
            {
                write_status_ = true; // Connection established, so can write.
                on_connected();
                async_read_raw_message();
            }
            else
            {
                client_log(ERROR) << "Handshake failed ("
                                  << error.value() << "): "
                                  << error.message() << ".";

                console_ << "E: SSL Handshake failed ("
                         << error.value() << "): "
                         << error.message() << ".\n";

                on_error(error);
            }
        });
}

void ssl_connection::async_read_raw_message(void)
{
    message_len_ = 0;

    boost::asio::async_read(socket_, boost::asio::buffer(message_len_buf_, 4),
        [this](const boost::system::error_code &error, std::size_t length)
        {
            if (! error)
            {
                if (length != 4)
                {
                    client_log(ERROR) << "SSL Read error: Incomplete message.";

                    console_ << "E: SSL Read error: Incomplete message.\n";

                    on_error(error);

                    return;
                }

                memcpy(&message_len_, message_len_buf_, 4);
                message_len_ = ntohl(message_len_);
                message_buf_.clear();
                message_buf_.resize(message_len_);
                message_total_read_ = 0;

                async_read_msg_chunk();
            }
            else
            {
                client_log(ERROR) << "SSL Read data error ("
                                  << error.value() << "): "
                                  << error.message() << ".";

                console_ << "E: SSL Read data error ("
                         << error.value() << "): "
                         << error.message() << ".\n";

                on_error(error);
            }
        });
}

void ssl_connection::async_read_msg_chunk(void)
{
    boost::asio::async_read(socket_, boost::asio::buffer(&message_buf_.front()+message_total_read_, message_len_-message_total_read_),
        [this](const boost::system::error_code &error, std::size_t length)
        {
            if (! error)
            {
                message_total_read_ += length;

                if (message_len_ == message_total_read_)
                {
                    on_data(message_buf_);

                    async_read_raw_message();
                }
                else
                {
                    async_read_msg_chunk();
                }
            }
            else
            {
                client_log(ERROR) << "SSL Read data error ("
                                  << error.value() << "): "
                                  << error.message() << ".";

                console_ << " E: SSL Read data error ("
                         << error.value() << "): "
                         << error.message() << ".\n";

                on_error(error);
            }
        });
}
