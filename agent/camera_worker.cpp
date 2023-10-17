#include <stdexcept>
#include <camera_worker.hpp>

#include <easylogging++.h>

#define agent_log(__X__) \
    CLOG(__X__, "camera")


camera_worker::camera_worker(const agent_settings &settings)
    : ctx_ {nullptr},
      dev_ {nullptr},
      devh_ {nullptr},
      video_capture_in_progress_ {false},
      settings_ {settings}
{
    el::Logger *logger = el::Loggers::getLogger("camera", true);

    uvc_error_t res;

    res = uvc_init(&ctx_, NULL);
 
    if (res < 0)
    {
        agent_log(ERROR) << "UVC init failed.";

        close_camera_worker();

        throw std::runtime_error("UVC init failed");
    }

    const char *sn = (settings_.get_camera_serial_number() == "") ? nullptr : settings_.get_camera_serial_number().c_str();

    res = uvc_find_device(ctx_, &dev_,
                          settings_.get_camera_vendor_id(),  // 0 - ignore filter,
                          settings_.get_camera_product_id(), // 0 - ignore filter,
                          sn);                               // nullptr - ignore filter.
 
    if (res < 0)
    {
        agent_log(ERROR) << "UVC no device found.";

        close_camera_worker();

        throw std::runtime_error("UVC no device found");
    }

    res = uvc_open(dev_, &devh_);

    if (res < 0)
    {
        agent_log(ERROR) << "UVC unable to open device.";

        close_camera_worker();

        throw std::runtime_error("UVC unable to open device");
    }

    enum uvc_frame_format frame_format;
    int fps;
    uvc_stream_ctrl_t ctrl;

    if (! check_device_capabilities(fps, frame_format))
    {
        agent_log(ERROR) << "UVC device doesn't provide a matching stream.";

        close_camera_worker();

        throw std::runtime_error("UVC device doesn't provide a matching stream");
    }

    //
    // Try to negotiate stream profile.
    //

    res = uvc_get_stream_ctrl_format_size(devh_, &ctrl, frame_format,
                                          settings_.get_camera_width(),
                                          settings_.get_camera_height(),
                                          fps);

    if (res < 0)
    {
        agent_log(ERROR) << "UVC device doesn't provide a matching stream.";

        close_camera_worker();

        throw std::runtime_error("UVC device doesn't provide a matching stream");
    }

    res = uvc_start_streaming(devh_, &ctrl, videoframe_callback, this, 0);

    if (res < 0)
    {
        agent_log(ERROR) << "UVC unable to start stream.";

        close_camera_worker();

        throw std::runtime_error("UVC unable to start stream");
    }

    //
    // Enable auto exposure.
    //

    const uint8_t UVC_AUTO_EXPOSURE_MODE_AUTO = 2;

    res = uvc_set_ae_mode(devh_, UVC_AUTO_EXPOSURE_MODE_AUTO);

    if (res == UVC_SUCCESS)
    {
        agent_log(INFO) << "Enabled auto exposure"; 
    }
    else if (res == UVC_ERROR_PIPE)
    {
        //
        // This error indicates that the camera
        // does not support the full AE mode;
        // try again, using aperture priority mode
        // (fixed aperture, variable exposure time).
        //

        agent_log(WARNING) << "Full AE not supported, trying aperture priority mode";

        const uint8_t UVC_AUTO_EXPOSURE_MODE_APERTURE_PRIORITY = 8;

        res = uvc_set_ae_mode(devh_, UVC_AUTO_EXPOSURE_MODE_APERTURE_PRIORITY);

        if (res < 0)
        {
            agent_log(ERROR) << "uvc_set_ae_mode failed to enable aperture priority mode";
        }
        else
        {
            agent_log(INFO) << "Enabled aperture priority auto exposure mode";
        }
    }
    else
    {
        agent_log(ERROR) << "uvc_set_ae_mode failed to enable auto exposure mode";
    }
}

camera_worker::~camera_worker(void)
{
    if (video_capture_in_progress_)
    {
        uvc_stop_streaming(devh_);
    }

    close_camera_worker();
}

bool camera_worker::is_new_data(void) const
{
    mtx_.lock();
    bool result = (frame_.size() > 0);
    mtx_.unlock();

    return result;
}

std::string camera_worker::get_videoframe(void)
{
    mtx_.lock();
    std::string frame(frame_);
    frame_.clear();
    mtx_.unlock();

    return frame;
}

bool camera_worker::check_device_capabilities(int &fps, enum uvc_frame_format &frame_format)
{
    const uvc_format_desc_t *format_desc = uvc_get_format_descs(devh_);

    while (format_desc)
    {
        if (format_desc -> bDescriptorSubtype == UVC_VS_FORMAT_MJPEG)
        {
            const uvc_frame_desc_t *frame_desc = format_desc -> frame_descs;

            while (frame_desc)
            {
                if (frame_desc -> wWidth == settings_.get_camera_width()
                      && frame_desc -> wHeight == settings_.get_camera_height())
                {
                    fps = 10000000 / frame_desc -> dwDefaultFrameInterval;
                    frame_format = UVC_COLOR_FORMAT_MJPEG;

                    return true;
                }

                frame_desc = frame_desc -> next;
            }

            return false;
        }

        format_desc = format_desc -> next;
    }

    return false;
}

void camera_worker::videoframe_callback(uvc_frame_t *frame, void *ptr)
{
    camera_worker *p_camera = reinterpret_cast<camera_worker *>(ptr);
    p_camera -> video_capture_in_progress_ = true;

    p_camera -> mtx_.lock();
    p_camera -> frame_.resize(frame -> data_bytes);
    memcpy(&p_camera -> frame_.front(), frame -> data, frame -> data_bytes);
    p_camera -> mtx_.unlock();
}
