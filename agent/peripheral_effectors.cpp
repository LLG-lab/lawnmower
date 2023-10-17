#include <peripheral_effectors.hpp>
#include <easylogging++.h>

#define agent_log(__X__) \
    CLOG(__X__, "peripheral_effectors")

peripheral_effectors::peripheral_effectors(const agent_settings &settings)
    : settings_ {settings},
      wheels_pos_changed_ {false},
      wheels_pos_ {0.0},
      wheels_pos_target_ {0.0},
      gp_pos_changed_ {false},
      gp_pos_ {0.0},
      gp_pos_target_ {0.0}
{
    el::Logger *logger = el::Loggers::getLogger("peripheral_effectors", true);
}

peripheral_effectors::~peripheral_effectors(void)
{
}

double peripheral_effectors::get_wheels_position(void)
{
    wheels_pos_changed_ = false;
    return wheels_pos_;
}

double peripheral_effectors::get_gas_pedal_position(void)
{
    gp_pos_changed_ = false;
    return gp_pos_;
}

void peripheral_effectors::set_wheels_position_target(double pos)
{
    wheels_pos_target_ = pos; // XXX Sprawdzić zakres.

    // XXX Te zmiany są tymczasowe, do momentu zaimplementowania
    //     obsługi siłowników. Stan siłowników fejkowo ustalamy
    //     na zadany.
    if (wheels_pos_ != wheels_pos_target_)
    {
        wheels_pos_ = wheels_pos_target_;
        wheels_pos_changed_ = true;
    }
}

void peripheral_effectors::set_gas_pedal_position_target(double pos)
{
    gp_pos_target_ = pos; // XXX Sprawdzic zakres.

    // XXX Te zmiany są tymczasowe, do momentu zaimplementowania
    //     obsługi siłowników. Stan siłowników fejkowo ustalamy
    //     na zadany.
    if (gp_pos_ != gp_pos_target_)
    {
        gp_pos_ = gp_pos_target_;
        gp_pos_changed_ = true;
    }
}
