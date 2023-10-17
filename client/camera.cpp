#include <camera.hpp>
#include <default_screen.hpp>

#include <easylogging++.h>

#include <cstring>
#include <cmath>
#include <stdexcept>

#define client_log(__X__) \
    CLOG(__X__, "camera")

camera::camera(int width, int height)
    : output_buffer_ {width, height},
      jpeg_handle_ {nullptr}
{
    input_buffer_.resize(width * height * 3);
    default_screen::make(output_buffer_);

    el::Loggers::getLogger("camera", true);

    jpeg_handle_ = tjInitDecompress();

    if (jpeg_handle_ == nullptr)
    {
        client_log(ERROR) << "Unable to initialize jpeg decompressor"
                          << tjGetErrorStr2(jpeg_handle_);

        throw std::runtime_error("Unable to initialize jpeg decompressor");
    }
}

camera::~camera(void)
{
    tjDestroy(jpeg_handle_);
}

void camera::render(screen_buffer &sb)
{
    std::size_t sz = sb.get_width() * sb.get_height() * 3;
    std::memcpy(sb.get_raw_data(), output_buffer_.get_raw_data(), sz);
}

void camera::put_data(const std::string &data)
{
    int result = tjDecompress2(
                     jpeg_handle_,
                     reinterpret_cast<const unsigned char *>(&data.front()),
                     data.size(),
                     reinterpret_cast<unsigned char *>(output_buffer_.get_raw_data()),
                     output_buffer_.get_width(),
                     0,
                     output_buffer_.get_height(),
                     TJPF_RGB,
                     TJFLAG_BOTTOMUP
                 );
}

void camera::monochrome(void)
{
    double a;
    pixel p;

    for (int x = 0; x < output_buffer_.get_width(); x++)
    {
        for (int y = 0; y < output_buffer_.get_height(); y++)
        {
            output_buffer_.get_pixel(x, y, p);

            a = sqrt(p.r*p.r + p.g*p.g + p.b*p.b)*0.577350269;
            
            p.r = a;
            p.g = a;
            p.b = a;

            output_buffer_.put_pixel(x, y, p);
        }
    }
}
