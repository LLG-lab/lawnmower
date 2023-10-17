#ifndef __CAMERA_HPP__
#define __CAMERA_HPP__

#include <turbojpeg.h>

#include <screen_buffer.hpp>
#include <string>

class camera
{
public:

    camera(void) = delete;
    camera(int width, int height);
    ~camera(void);

    void render(screen_buffer &sb);
    void put_data(const std::string &data);

    void monochrome(void);

private:

    std::vector<char> input_buffer_;
    screen_buffer output_buffer_;

    tjhandle jpeg_handle_;
};

#endif /* __CAMERA_HPP__ */
