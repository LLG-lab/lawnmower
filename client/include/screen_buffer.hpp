#ifndef __SCREEN_BUFFER_HPP__
#define __SCREEN_BUFFER_HPP__

#include <vector>
#include <pixel.hpp>

class screen_buffer
{
public:

    screen_buffer(void) = delete;
    screen_buffer(int width, int height);

    char *get_raw_data(void) { return &data_.front(); }

    int get_width(void) const { return width_; }
    int get_height(void) const { return height_; }

    void get_pixel(int x, int y, pixel &p) const;
    void put_pixel(int x, int y, const pixel &p);

private:

    std::vector<char> data_;
    int width_;
    int height_;
};

#endif /* __SCREEN_BUFFER_HPP__ */
