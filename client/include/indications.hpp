#ifndef __INDICATIONS_HPP__
#define __INDICATIONS_HPP__

#include "screen_buffer.hpp"

class indications
{
public:

    indications(void)
        : visible_ {false}, wheel_turn_ {0.0}, gas_{0.0}
    {}

    void visible(void) { visible_ = true; }
    void invisible(void) { visible_ = false; }

    void render(screen_buffer &sb);

    void set_wheel_turn(double wt) { wheel_turn_ = wt; }
    void set_gas(double g) { gas_ = g; }

private:

    bool visible_;

    double wheel_turn_;
    double gas_;
};

#endif /* __INDICATIONS_HPP__ */
