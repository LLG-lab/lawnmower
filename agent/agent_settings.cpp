#include <agent_settings.hpp>

agent_settings::agent_settings(int argc, char *argv[])
    : port_ {8228},
      certificate_chain_file_ {"/home/ryszard/lawnmower/tmp/server.pem"},
      private_key_file_ {"/home/ryszard/lawnmower/tmp/server.pem"},
      tmp_dh_file_ {""},
      camera_width_ {1280},
      camera_height_ {720},
      camera_vendor_id_ {0},
      camera_product_id_ {0},
      camera_serial_number_ {""}

{
    // NOT IMPLEMENTED.
}
