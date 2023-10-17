#ifndef __LAWNMOWERC_SETTINGS_HPP__
#define __LAWNMOWERC_SETTINGS_HPP__

#include <string>

class lawnmowerc_settings
{
public:

    lawnmowerc_settings(void) = delete;
    lawnmowerc_settings(int argc, char *argv[]);

    ~lawnmowerc_settings(void) = default;

    //
    // Connection.
    //

    std::string get_host(void) const { return host_; }
    std::string get_port(void) const { return port_; }

    //
    // Display.
    //

    int get_camera_width(void) const { return camera_width_; }
    int get_camera_height(void) const { return camera_height_; }

    //
    // Joystick.
    //

    int get_select_button_index(void) const { return select_button_index_; }
    int get_start_button_index(void) const { return start_button_index_; }
    int get_triangle_button_index(void) const { return triangle_button_index_; }
    int get_circle_button_index(void) const { return circle_button_index_; }
    int get_square_button_index(void) const { return square_button_index_; }
    int get_cross_button_index(void) const { return cross_button_index_; }
    int get_steering_wheel_index(void) const { return steering_wheel_index_; }
    int get_acceleration_pedal_index(void) const { return acceleration_pedal_index_; }

private:

    std::string host_;
    std::string port_;

    int camera_width_;
    int camera_height_;

    int select_button_index_;
    int start_button_index_;
    int triangle_button_index_;
    int circle_button_index_;
    int square_button_index_;
    int cross_button_index_;
    int steering_wheel_index_;
    int acceleration_pedal_index_;
};

#endif /* __LAWNMOWERC_SETTINGS_HPP__ */
