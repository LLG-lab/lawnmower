#include <device_session.hpp>

#include <easylogging++.h>

#include <iostream>

using boost::asio::ip::tcp;

#define TIMER_INTERVAL_MILLIS 50

#define agent_log(__X__) \
    CLOG(__X__, "session")

bool device_session::pending_session_ = false;

device_session::device_session(boost::asio::io_context &io_context, boost::asio::ssl::stream<tcp::socket> socket, const agent_settings &settings)
    : write_status_(true),
      io_context_(io_context),
      timer_(io_context_),
      socket_(std::move(socket)),
      settings_{settings}
{
    el::Logger *logger = el::Loggers::getLogger("session", true);

    device_session::pending_session_ = true;
}

device_session::~device_session(void)
{
    agent_log(TRACE) << "session::~session() – got called.";

    timer_.cancel();

    device_session::pending_session_ = false;
}

void device_session::do_handshake(void)
{
    auto self(shared_from_this());

    socket_.async_handshake(boost::asio::ssl::stream_base::server, 
        [this, self](const boost::system::error_code& ec)
        {
            if (! ec)
            {
                init_timer(TIMER_INTERVAL_MILLIS);
                async_read_raw_message();
            }
            // XXX Obsługa błędu!
        });
}

void device_session::init_timer(int millis)
{
    auto self(shared_from_this());

    timer_.cancel();
    timer_.expires_after(boost::asio::chrono::milliseconds(millis));
    timer_.async_wait(
        [this, self](const boost::system::error_code &error)
        {
            if (! error)
            {
                if (write_status_)
                {
                    on_alarm();
                }

                init_timer(TIMER_INTERVAL_MILLIS);
            }
            else
            {
                agent_log(ERROR) << "Timer error ("
                                 << error.value() << "): "
                                 << error.message() << ".";

                timer_.cancel();
            }
        });
}

void device_session::async_read_raw_message(void)
{
    auto self(shared_from_this());

    read_message_len_ = 0;

    boost::asio::async_read(socket_, boost::asio::buffer(read_message_len_buf_, 4),
        [this, self](const boost::system::error_code &error, std::size_t length)
        {
            if (! error)
            {
                if (length != 4)
                {
                    agent_log(ERROR) << "SSL Read error: Incomplete message.";
                    timer_.cancel();

                    return;
                }

                memcpy(&read_message_len_, read_message_len_buf_, 4);
                read_message_len_ = ntohl(read_message_len_);
                read_message_buf_.clear();
                read_message_buf_.resize(read_message_len_);
                read_message_total_read_ = 0;

                async_read_msg_chunk();
            }
            else
            {
                agent_log(ERROR) << "SSL Read error ("
                                 << error.value() << "): "
                                 << error.message() << ".";

                timer_.cancel();
            }
        });
}

void device_session::async_read_msg_chunk(void)
{
    auto self(shared_from_this());

    boost::asio::async_read(socket_, boost::asio::buffer(&read_message_buf_.front()+read_message_total_read_, read_message_len_-read_message_total_read_),
        [this, self](const boost::system::error_code &error, std::size_t length)
        {
            if (! error)
            {
                read_message_total_read_ += length;

                if (read_message_len_ == read_message_total_read_)
                {
                    on_client_data(read_message_buf_);

                    async_read_raw_message();
                }
                else
                {
                    async_read_msg_chunk();
                }
            }
            else
            {
                agent_log(ERROR) << "SSL Read error ("
                                 << error.value() << "): "
                                 << error.message() << ".";

                timer_.cancel();
            }
        });
}

void device_session::send_data(const std::vector<char> &data)
{
    auto self(shared_from_this());

    write_status_ = false;

    send_message_buf_.resize(data.size() + 4);
    int data_size_big_endian = htonl(data.size());
    memcpy(&send_message_buf_.front(), &data_size_big_endian, 4);
    memcpy(&send_message_buf_.front() + 4, &data.front(), data.size());    

    boost::asio::async_write(socket_, boost::asio::buffer(&send_message_buf_.front(), send_message_buf_.size()),
        [this, self](const boost::system::error_code &error, std::size_t length)
        {
            if (! error)
            {
                write_status_ = true;
            }
            else
            {
                agent_log(ERROR) << "SSL Write data error ("
                                 << error.value() << "): "
                                 << error.message() << ".";

                timer_.cancel();
            }
        });
}
