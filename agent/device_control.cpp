#include <device_control.hpp>

#include "lawnmower.pb.h"

device_control::device_control(boost::asio::io_context &io_context, boost::asio::ssl::stream<boost::asio::ip::tcp::socket> socket, const agent_settings &settings)
    : device_session(io_context, std::move(socket), settings),
      camera_ {settings},
      effectors_ {settings}
{
}

void device_control::on_alarm(void)
{
    ProtoAgentMessage agent_msg;
    bool have_anything_to_send = false;

    if (camera_.is_new_data())
    {
        have_anything_to_send = true;

        agent_msg.set_videoframe(camera_.get_videoframe());
    }

    if (effectors_.is_new_wheels_position())
    {
        have_anything_to_send = true;

        agent_msg.set_steeringwheelpotentiometer(effectors_.get_wheels_position());
    }

    if (effectors_.is_new_gas_pedal_position())
    {
        have_anything_to_send = true;

        agent_msg.set_accelerationpedalpotentiometer(effectors_.get_gas_pedal_position());
    }

    //
    // Serialize and send message to client.
    //

    if (have_anything_to_send)
    {
        #if GOOGLE_PROTOBUF_VERSION < 3004000
        int size = agent_msg.ByteSize();
        #else
        int size = agent_msg.ByteSizeLong();
        #endif

        std::vector<char> buffer(size);

        agent_msg.SerializeToArray(&buffer.front(), size);

        send_data(buffer);
    }
}

void device_control::on_client_data(const std::vector<char> &data)
{
    ProtoClientMessage client_msg;

    client_msg.ParseFromArray(&data.front(), data.size());

    if (client_msg.has_steeringwheelreq())
    {
        effectors_.set_wheels_position_target(client_msg.steeringwheelreq());
    }

    if (client_msg.has_accelerationpedalreq())
    {
        effectors_.set_gas_pedal_position_target(client_msg.accelerationpedalreq());
    }
}
