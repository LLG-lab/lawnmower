#ifndef __AGENT_SETTINGS_HPP__
#define __AGENT_SETTINGS_HPP__

#include <string>

class agent_settings
{
public:

    agent_settings(void) = delete;
    agent_settings(int argc, char *argv[]);

    ~agent_settings(void) = default;

    int get_port(void) const { return port_; }
    std::string get_certificate_chain_file(void) const { return certificate_chain_file_; }
    std::string get_private_key_file(void) const { return private_key_file_; }
    std::string get_tmp_dh_file(void) const { return tmp_dh_file_; }

    //
    // Video informations.
    //

    int get_camera_width(void) const { return camera_width_; }
    int get_camera_height(void) const { return camera_height_; }
    int get_camera_vendor_id(void) const { return camera_vendor_id_; }
    int get_camera_product_id(void) const { return camera_product_id_; }
    std::string get_camera_serial_number(void) const { return camera_serial_number_; }

private:

    int port_;
    std::string certificate_chain_file_;
    std::string private_key_file_;
    std::string tmp_dh_file_;

    int camera_width_;
    int camera_height_;
    int camera_vendor_id_;
    int camera_product_id_;
    std::string camera_serial_number_;
};

#endif /* __AGENT_SETTINGS_HPP__ */
