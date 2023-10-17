#ifndef __CLIENT_HPP__
#define __CLIENT_HPP__

#include <memory>

#include <GLFW/glfw3.h>

#include <screen_buffer.hpp>
#include <camera.hpp>
#include <output_terminal.hpp>
#include <indications.hpp>
#include <ssl_connection.hpp>

class client
{
public:

    client(void) = delete;
    client(client &) = delete;
    client(client &&) = delete;
    client(GLFWwindow *window, boost::asio::io_context &io_context, const lawnmowerc_settings &cfg);
    ~client(void);

    void connect(void);

private:

    class state
    {
    public:
        state(void)
            : changed_{false}, data_{0.0} {}

        bool is_data(void) const { return changed_; }

        double get_data(void) { changed_ = false; return data_; }

        void put_data(double data)
        {
            if (data != data_)
            {
                data_ = data;
                changed_ = true;
            }
        }

    private:

        bool changed_;
        double data_;
    }; /* class state */

    enum class connection_stage
    {
        DISCONNECTED,
        CONNECTING,
        CONNECTED
    };

    void on_alarm(void);
    void on_device_data(const std::vector<char> &data);
    void on_connected(void);
    void on_connection_error(const boost::system::error_code &ec);

    void process_input(void);
    void render(void);

    void init_gdi(void);
    void init_timer(int millis);
    bool test_and_setup_joystick(void);
    void setup_connection_callbacks(void);

    state acceleration_pedal_;
    state steering_wheel_;

    // Asio stuff.
    boost::asio::io_context &ioctx_;
    boost::asio::steady_timer timer_;

    const lawnmowerc_settings &settings_;
    screen_buffer sb_;
    camera camera_;
    output_terminal console_;
    indications indications_;
    std::shared_ptr<ssl_connection> connection_;
    connection_stage connection_stage_;
    int joystick_id_;

    // Window stuff.
    GLFWwindow *window_;
    unsigned int VBO_;
    unsigned int VAO_;
    unsigned int EBO_;
    unsigned int ID_;
    unsigned int texture_;

    // Static constants.
    static const char *vertex_shader_code_;
    static const char *fragment_shader_code_;

};

#endif /* __CLIENT_HPP__ */
