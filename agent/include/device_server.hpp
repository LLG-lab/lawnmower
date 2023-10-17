#ifndef __DEVICE_SERVER_HPP__
#define __DEVICE_SERVER_HPP__

#include <agent_settings.hpp>
#include <device_control.hpp>

class device_server
{
public:

    device_server(boost::asio::io_context &io_context, const agent_settings &settings);

private:

    std::string get_password(void) const
    {
        return "test";
    }

    void do_accept(void);

    boost::asio::io_context &io_context_;
    const agent_settings &settings_;
    boost::asio::ip::tcp::acceptor acceptor_;
    boost::asio::ssl::context context_;
};

#endif /* __DEVICE_SERVER_HPP__ */
