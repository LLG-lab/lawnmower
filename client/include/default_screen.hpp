#ifndef __DEFAULT_SCREEN_HPP__
#define __DEFAULT_SCREEN_HPP__

#include <screen_buffer.hpp>

class default_screen
{
public:

    static void make(screen_buffer &sb);

private:

    static void get_pixel(int x, int y, pixel &p);

    static uint8_t screen_data_32x32[];

    // Powinien byc jakiś backbuffer
    // static screen_buffer back_buffer;
};

#endif /* __DEFAULT_SCREEN_HPP__ */