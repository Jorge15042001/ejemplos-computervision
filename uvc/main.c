#include <libuvc/libuvc.h>
#include <stdio.h>
#include <unistd.h>

void set_exposure(uvc_device_handle_t *dev_handle, uint32_t exposure_time) {
    uvc_error_t res = uvc_set_exposure_abs(dev_handle, exposure_time);
    if (res != UVC_SUCCESS) {
        fprintf(stderr, "Failed to set exposure time: %s\n", uvc_strerror(res));
    }
}

// Function to disable autoexposure for a given device handle
uvc_error_t disable_autoexposure(uvc_device_handle_t *dev_handle) {
    // Check if the device handle is valid
    if (!dev_handle) {
        return UVC_ERROR_INVALID_PARAM;
    }

    // Disable autoexposure
    return uvc_set_ae_mode(dev_handle, 1); // 0 indicates manual exposure mode
}

static int save_frame = 0;
void frame_callback(uvc_frame_t *frame, void *ptr) {
    if (save_frame == 0) {
      return;
    }
    save_frame = 0;

    static int frame_count = 0;
    char filename[32];

    snprintf(filename, sizeof(filename), "images/frame_%02d.jpg", frame_count++);
    FILE *file = fopen(filename, "wb");
    if (file) {
        fwrite(frame->data, 1, frame->data_bytes, file);
        fclose(file);
        printf("Saved frame to %s\n", filename);
    } else {
        perror("Error saving frame");
    }
}

int main() {
    uvc_context_t *ctx;
    uvc_device_t *dev;
    uvc_device_handle_t *dev_handle;
    uvc_stream_ctrl_t ctrl;
    uvc_error_t res;

    // Initialize the UVC context
    res = uvc_init(&ctx, NULL);
    if (res != UVC_SUCCESS) {
        uvc_perror(res, "uvc_init");
        return res;
    }

    // Find the device
    res = uvc_find_device(ctx, &dev, 0, 0, NULL);
    if (res != UVC_SUCCESS) {
        uvc_perror(res, "uvc_find_device");
        uvc_exit(ctx);
        return res;
    }

    // Open the device
    res = uvc_open(dev, &dev_handle);
    if (res != UVC_SUCCESS) {
        uvc_perror(res, "uvc_open");
        uvc_unref_device(dev);
        uvc_exit(ctx);
        return res;
    }

    // Configure the stream (e.g., 640x480 @ 30fps)
    res = uvc_get_stream_ctrl_format_size(dev_handle, &ctrl, UVC_FRAME_FORMAT_MJPEG, 640, 480, 30);
    if (res != UVC_SUCCESS) {
        uvc_perror(res, "uvc_get_stream_ctrl_format_size");
        uvc_close(dev_handle);
        uvc_unref_device(dev);
        uvc_exit(ctx);
        return res;
    }

    res = uvc_start_streaming(dev_handle, &ctrl, frame_callback, NULL, 0);
  if (res != UVC_SUCCESS) {
      uvc_perror(res, "uvc_start_streaming");
      return -1;
  }
    disable_autoexposure(dev_handle);
    set_exposure(dev_handle, 200);
    sleep(1);// wait 1 second for autofocus to disabled



    // Stream frames with different exposure times
    for (int i = 0; i < 20; i++) {
        const uint32_t exposure_time = 100 + i * 100; // Example exposure times
        set_exposure(dev_handle, exposure_time);

        usleep(100000); // wait for 1/10 second to ensure the exposure time is setwait for 1 second to ensure the exposure time is set
        save_frame = 1;
        usleep(100000); 

    }
    sleep(1);//waiwt for 1 second to make sure last frame is capture

    // Clean up
    uvc_stop_streaming(dev_handle);
    uvc_close(dev_handle);
    uvc_unref_device(dev);
    uvc_exit(ctx);

    return 0;
}

