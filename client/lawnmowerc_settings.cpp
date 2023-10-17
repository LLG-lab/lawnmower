#include <lawnmowerc_settings.hpp>

lawnmowerc_settings::lawnmowerc_settings(int argc, char *argv[])
    : host_ {"127.0.0.1"},
      port_ {"8228"},
      camera_width_ {1280},
      camera_height_ {720},
      select_button_index_ {8},
      start_button_index_ {9},
      triangle_button_index_ {0},
      circle_button_index_ {1},
      square_button_index_ {3},
      cross_button_index_ {2},
      steering_wheel_index_ {0},
      acceleration_pedal_index_ {1}

{
    // NOT IMPLEMENTED.
}
