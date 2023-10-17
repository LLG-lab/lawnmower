#ifndef __CAMERA_WORKER_HPP__
#define __CAMERA_WORKER_HPP__

#include <libuvc/libuvc.h>
#include <thread>
#include <string>
#include <mutex>

#include <agent_settings.hpp>

class camera_worker
{
public:

    camera_worker(void) = delete;
    camera_worker(camera_worker &) = delete;
    camera_worker(camera_worker &&) = delete;

    camera_worker(const agent_settings &settings);
    ~camera_worker(void);

    bool is_new_data(void) const;

    std::string get_videoframe(void);

private:

    void close_camera_worker(void)
    {
        if (devh_) uvc_close(devh_);
        if (dev_)  uvc_unref_device(dev_);
        if (ctx_)  uvc_exit(ctx_);
    }

    bool check_device_capabilities(int &fps, enum uvc_frame_format &frame_format);

    static void videoframe_callback(uvc_frame_t *frame, void *ptr);

    uvc_context_t *ctx_;
    uvc_device_t *dev_;
    uvc_device_handle_t *devh_;
    bool video_capture_in_progress_;

    std::string frame_;
    const agent_settings &settings_;

    mutable std::mutex mtx_;
};

#endif /* __CAMERA_WORKER_HPP__ */
