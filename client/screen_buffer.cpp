#include <screen_buffer.hpp>

screen_buffer::screen_buffer(int width, int height)
    : width_ {width},
      height_ {height}
{
    data_.resize(width_ * height_ * 3);
}

void screen_buffer::get_pixel(int x, int y, pixel &p) const
{
    int offset = width_*y*3 + x*3;
    p.r = data_[offset+0];
    p.g = data_[offset+1];
    p.b = data_[offset+2];
}

void screen_buffer::put_pixel(int x, int y, const pixel &p)
{
    int offset = width_*y*3 + x*3;
    data_[offset+0] = p.r;
    data_[offset+1] = p.g;
    data_[offset+2] = p.b;
}
