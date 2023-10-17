#ifndef __DEVICE_CONTROL_HPP__
#define __DEVICE_CONTROL_HPP__

#include <device_session.hpp>

#include <camera_worker.hpp>
#include <peripheral_effectors.hpp>

class device_control : public device_session
{
public:

    device_control(void) = delete;
    device_control(device_control &) = delete;
    device_control(device_control &&) = delete;
    device_control(boost::asio::io_context &io_context, boost::asio::ssl::stream<boost::asio::ip::tcp::socket> socket, const agent_settings &settings);

private:

    virtual void on_alarm(void);
    virtual void on_client_data(const std::vector<char> &data);

    camera_worker camera_;
    peripheral_effectors effectors_;
};

#endif /* __DEVICE_CONTROL_HPP__ */
