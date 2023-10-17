#ifndef __SSL_CONNECTION_HPP__
#define __SSL_CONNECTION_HPP__

#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>
#include <functional>
#include <vector>

#include <lawnmowerc_settings.hpp>
#include <output_terminal.hpp>

class ssl_connection
{
public:

    ssl_connection(void) = delete;
    ssl_connection(ssl_connection &) = delete;
    ssl_connection(ssl_connection &&) = delete;
    ssl_connection(boost::asio::io_context &io_context, const lawnmowerc_settings &cfg, output_terminal &console);
    ~ssl_connection(void);

    void connect(void);

    void send_data(const std::vector<char> &data);
    bool can_send(void) const { return write_status_; }

    //
    // Configurable callbacks.
    //

    void set_on_error_callback(const std::function<void(const boost::system::error_code &)> &callback) { on_error = callback; }
    void set_on_data_callback(const std::function<void(const std::vector<char> &)> &callback) { on_data = callback; }
    void set_on_connected_callback(const std::function<void(void)> &callback) { on_connected = callback; }

private:

    void handshake(void);

    void async_read_raw_message(void);
    void async_read_msg_chunk(void);

    const lawnmowerc_settings &settings_;
    output_terminal &console_;

    boost::asio::ssl::context ssl_ctx_;
    boost::asio::ssl::stream<boost::asio::ip::tcp::socket> socket_;
    boost::asio::io_context &ioctx_;

    std::function<void(const boost::system::error_code &)> on_error;
    std::function<void(const std::vector<char> &)> on_data;
    std::function<void(void)> on_connected;

    char message_len_buf_[4];
    int message_len_;
    int message_total_read_;
    std::vector<char> message_buf_;

    bool write_status_; // Indicates async write is completed.
    std::vector<char> send_buffer_;
};

#endif /* __SSL_CONNECTION_HPP__ */
