#ifndef __PERIPHERAL_EFFECTORS_HPP__
#define __PERIPHERAL_EFFECTORS_HPP__

#include <agent_settings.hpp>

class peripheral_effectors
{
public:

     peripheral_effectors(void) = delete;
     peripheral_effectors(peripheral_effectors &) = delete;
     peripheral_effectors(peripheral_effectors &&) = delete;

     peripheral_effectors(const agent_settings &settings);
     ~peripheral_effectors(void);

     bool is_new_wheels_position(void) const { return wheels_pos_changed_; }
     double get_wheels_position(void);
     bool is_new_gas_pedal_position(void) const { return gp_pos_changed_; }
     double get_gas_pedal_position(void);

     void set_wheels_position_target(double pos);
     void set_gas_pedal_position_target(double pos);

private:

     const agent_settings &settings_;

     //
     // Wheels.
     //

     bool wheels_pos_changed_;
     double wheels_pos_;
     double wheels_pos_target_;

     //
     // Gas pedal.
     //

     bool gp_pos_changed_;
     double gp_pos_;
     double gp_pos_target_;
};

#endif /* __PERIPHERAL_EFFECTORS_HPP__ */
