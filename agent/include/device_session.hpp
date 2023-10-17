#ifndef __DEVICE_SESSION_HPP__
#define __DEVICE_SESSION_HPP__

#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>

#include <agent_settings.hpp>

class device_session : public std::enable_shared_from_this<device_session>
{
public:

    device_session(void) = delete;
    device_session(device_session &) = delete;
    device_session(device_session &&) = delete;
    device_session(boost::asio::io_context &io_context, boost::asio::ssl::stream<boost::asio::ip::tcp::socket> socket, const agent_settings &settings);

    ~device_session(void);

    void start(void) { do_handshake(); }

    static bool pending_session(void) { return device_session::pending_session_; }

protected:

    virtual void on_alarm(void) = 0;
    virtual void on_client_data(const std::vector<char> &data) = 0;

    void send_data(const std::vector<char> &data);

    const agent_settings &get_settings(void) const { return settings_; }

private:

    void do_handshake(void);

    void init_timer(int millis);

    void async_read_raw_message(void);
    void async_read_msg_chunk(void);

    //
    // Connection stuff.
    //

    boost::asio::io_context &io_context_;
    boost::asio::steady_timer timer_;
    boost::asio::ssl::stream<boost::asio::ip::tcp::socket> socket_;

    //
    // Stuff related to read data.
    //

    char read_message_len_buf_[4];
    int read_message_len_;
    int read_message_total_read_;
    std::vector<char> read_message_buf_;

    //
    // Stuff related to send data.
    //

    bool write_status_; // Indicates async write is completed.
    std::vector<char> send_message_buf_;

    //
    // Flag indicates that session is running.
    //

    static bool pending_session_;

    const agent_settings &settings_;
};

#endif /* __DEVICE_SESSION_HPP__ */
