/* V4L2 video picture grabber
   Copyright (C) 2009 Mauro Carvalho Chehab <mchehab@infradead.org>

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation version 2 of the License.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
 */

#include <errno.h>
#include <fcntl.h>
#include <linux/videodev2.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <sys/time.h>
#include <sys/types.h>
#include <libv4l2.h>

// Abreviation for clearing buffer
#define CLEAR(x) memset(&(x), 0, sizeof(x))

// structure for images
struct buffer {
  void *start;
  size_t length;
};

static int xioctl(int fh, int request, void *arg) {
  int r;
  do {
    r = v4l2_ioctl(fh, request, arg);
  } while (r == -1 && ((errno == EINTR) || (errno == EAGAIN)));

  if (r == -1) {
    fprintf(stderr, "error %d, %s\n", errno, strerror(errno));
    /** exit(EXIT_FAILURE); */
    return 0;
  }
  return 1;
}

int main(int argc, char **argv) {
  const char *const dev_name = "/dev/video0";//cameta path
  enum v4l2_buf_type type;
  fd_set fds;
  int r;
  unsigned int n_buffers;
  char out_name[256];
  FILE *fout;
  struct buffer *buffers;

  const int camera_fd = v4l2_open(dev_name, O_RDWR | O_NONBLOCK, 0);

  if (camera_fd < 0) {
    perror("Cannot open device");
    exit(EXIT_FAILURE);
  }

  struct v4l2_format fmt;// struct for image format
  CLEAR(fmt);//clear the format buffer
  // setting capture options
  fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
  fmt.fmt.pix.width = 1920;
  fmt.fmt.pix.height = 1080;
  fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_RGB24;
  fmt.fmt.pix.field = V4L2_FIELD_NONE;


  //set capture format to tue camera
  xioctl(camera_fd, VIDIOC_S_FMT, &fmt);

  //chech if the cameta accepted the capture format
  if (fmt.fmt.pix.pixelformat != V4L2_PIX_FMT_RGB24) {
    printf("Libv4l didn't accept RGB24 format. Can't proceed.\\n");
    exit(EXIT_FAILURE);
  }

  // check if the cammera accepted the specified resolution
  if ((fmt.fmt.pix.width != 2592) || (fmt.fmt.pix.height != 1944))
    printf("Warning: driver is sending image at %dx%d\\n", fmt.fmt.pix.width,
           fmt.fmt.pix.height);

  struct v4l2_requestbuffers req;// object for creationg buffers
  CLEAR(req);// clearing the object
  req.count = 2;// request 2 buffers
  req.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
  req.memory = V4L2_MEMORY_MMAP;

  // set the capture buffers 
  xioctl(camera_fd, VIDIOC_REQBUFS, &req);

  // allocate as many buffers as requested to v4l2
  buffers = calloc(req.count, sizeof(*buffers));

  for (n_buffers = 0; n_buffers < req.count; ++n_buffers) {
    // struct for image buffer information
    struct v4l2_buffer buf;
    //clear all the buffer
    CLEAR(buf);

    buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    buf.memory = V4L2_MEMORY_MMAP;
    buf.index = n_buffers;

    xioctl(camera_fd, VIDIOC_QUERYBUF, &buf);

    buffers[n_buffers].length = buf.length;
    //mapping space for the image in memory
    buffers[n_buffers].start = v4l2_mmap(
        NULL, buf.length, PROT_READ | PROT_WRITE, MAP_SHARED, camera_fd, buf.m.offset);

    // if failed to map space for the image
    if (MAP_FAILED == buffers[n_buffers].start) {
      perror("mmap");
      exit(EXIT_FAILURE);
    }
  }

  for (unsigned int i = 0; i < n_buffers; ++i) {
    // struct for image buffer information
    struct v4l2_buffer buf;
    CLEAR(buf);
    buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    buf.memory = V4L2_MEMORY_MMAP;
    buf.index = i;
    xioctl(camera_fd, VIDIOC_QBUF, &buf);
  }
  type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

  //start stream
  xioctl(camera_fd, VIDIOC_STREAMON, &type);
  // capture 20 images
  for (unsigned int i = 0; i < 20; i++) {
    struct v4l2_control control;
    control.id = V4L2_CID_EXPOSURE;
    control.value = 300;
    const int success = xioctl(camera_fd, VIDIOC_S_CTRL,&control); 
    if (success == 0){
      printf("Failed to set expousure\n");
    }
    do {
      FD_ZERO(&fds);
      FD_SET(camera_fd, &fds);

      /* Timeout. 2 seconds */
      struct timeval tv = {2,0};

      //wait for the camera to be reade
      r = select(camera_fd + 1, &fds, NULL, NULL, &tv);
    } while ((r == -1 && (errno = EINTR)));
    if (r == -1) {
      perror("select");
      return errno;
    }

    // struct for image buffer information
    struct v4l2_buffer buf;
    CLEAR(buf);//clear the buffer

    //setting buffer configuration
    buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    buf.memory = V4L2_MEMORY_MMAP;

    //deque buffer with the image 
    xioctl(camera_fd, VIDIOC_DQBUF, &buf);

    // image file path
    sprintf(out_name, "images/out%03d.ppm", i);
    fout = fopen(out_name, "w");//open the file
    if (!fout) {
      perror("Cannot open image");
      exit(EXIT_FAILURE);
    }
    //write image header
    fprintf(fout, "P6 %d %d 255 ", fmt.fmt.pix.width, fmt.fmt.pix.height);
    //write image content
    fwrite(buffers[buf.index].start, buf.bytesused, 1, fout);
    //close the file
    fclose(fout);

    //enque a buffer for capture another image
    xioctl(camera_fd, VIDIOC_QBUF, &buf);
  }

  type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
  // stop scream
  xioctl(camera_fd, VIDIOC_STREAMOFF, &type);
  // realease the memory
  for (unsigned int i = 0; i < n_buffers; ++i)
    v4l2_munmap(buffers[i].start, buffers[i].length);

  //close the camera
  v4l2_close(camera_fd);

  return 0;
}
