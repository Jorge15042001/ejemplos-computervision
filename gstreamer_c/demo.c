/*
 * Copyright 2016 The Imaging Source Europe GmbH
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/*
  This example will show you how to receive data
  from gstreamer in your application
  and how to get the actual image data
*/

#include <gst/gst.h>
#include <gst/video/video.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>



static int save_image = 0;
/*
  This function will be called in a separate thread when our appsink
  says there is data for us. user_data has to be defined
  when calling g_signal_connect. It can be used to pass objects etc.
  from your other function to the callback.

  user_data can point to additional data for your usage
            marked as unused to prevent compiler warnings
*/
static GstFlowReturn callback(GstElement *sink,
                              void *user_data __attribute__((unused))) {
  GstSample *sample = NULL;
  /* Retrieve the buffer */
  g_signal_emit_by_name(sink, "pull-sample", &sample, NULL);

  if (sample) {
    // we have a valid sample
    // do things with the image here
    static guint framecount = 0;
    static guint saved_frames= 0;
    /** int pixel_data = -1; */

    GstBuffer *buffer = gst_sample_get_buffer(sample);
    GstMapInfo info; // contains the actual image
    if (gst_buffer_map(buffer, &info, GST_MAP_READ)) {
      GstVideoInfo *video_info = gst_video_info_new();
      if (!gst_video_info_from_caps(video_info, gst_sample_get_caps(sample))) {
        // Could not parse video info (should not happen)
        g_warning("Failed to parse video info");
        return GST_FLOW_ERROR;
      }


      // pointer to the image data

      // unsigned char* data = info.data;

      // Get the pixel value of the center pixel

      // int stride = video_info->finfo->bits / 8;
      // unsigned int pixel_offset = video_info->width / 2 * stride
      //                             + video_info->width * video_info->height /
      //                             2 * stride;

      // this is only one pixel
      // when dealing with formats like BGRx
      // pixel_data will consist out of
      // pixel_offset   => B
      // pixel_offset+1 => G
      // pixel_offset+2 => R
      // pixel_offset+3 => x

      // pixel_data = info.data[pixel_offset];

      if(save_image == 1){
        char filename [100];
        sprintf(filename,"images/%03d.jpg",saved_frames); 

        FILE* image_file = fopen(filename, "wb") ;
        fwrite(info.data, 1, info.size, image_file);
        fclose(image_file);
        save_image = 0;
        saved_frames ++;
        printf("Image saved! at %s\n", filename);
      }

      gst_buffer_unmap(buffer, &info);
      gst_video_info_free(video_info);
    }

    /** GstClockTime timestamp = GST_BUFFER_PTS(buffer); */
    /** g_print("Captured frame %d, Pixel Value=%03d Timestamp=%" GST_TIME_FORMAT */
    /**         "            \r", */
    /**         framecount, pixel_data, GST_TIME_ARGS(timestamp)); */
    framecount++;

    // delete our reference so that gstreamer can handle the sample
    gst_sample_unref(sample);
  }
  return GST_FLOW_OK;
}

int main(int argc, char *argv[]) {
  /* this line sets the gstreamer default logging level
     it can be removed in normal applications
     gstreamer logging can contain verry useful information
     when debugging your application
     # see
     https://gstreamer.freedesktop.org/documentation/tutorials/basic/debugging-tools.html
     for further details
  */
  int n_frames = -1;
  if (argc == 2){
    const int frames= atoi(argv[1]);
    if (frames<= 0){
      fprintf(stderr,"El segundo argumento deber ser un numero entero positivo") ;
      return 1;
    }
    n_frames = frames;
  }
  else if (argc >2) {
      fprintf(stderr,"Maximo 2 argumentos permitidos") ;
      return 1;
  }
  gst_debug_set_default_threshold(GST_LEVEL_WARNING);

  gst_init(&argc, &argv);

  const char *serial = NULL; // the serial number of the camera we want to use

  char *pipeline_str =
      "v4l2src  device=/dev/video0  ! "
      "video/x-raw,format=YUY2,width=640,height=480,framerate=30/1 ! "
      "tee name=t ! queue ! videoconvert ! jpegenc !  appsink name=sink t. !  queue "
      "!videoconvert ! autovideosink";
  if(n_frames > 0){
      pipeline_str =
      "v4l2src  device=/dev/video0  ! "
      "video/x-raw,format=YUY2,width=640,height=480,framerate=30/1 ! "
      "queue ! videoconvert ! jpegenc !  appsink name=sink ";
  }


  GError *err = NULL;
  GstElement *pipeline = gst_parse_launch(pipeline_str, &err);

  /* test for error */
  if (pipeline == NULL) {
    printf("Could not create pipeline. Cause: %s\n", err->message);
    return 1;
  }

  if (serial != NULL) {
    GstElement *source = gst_bin_get_by_name(GST_BIN(pipeline), "source");

    GValue val = {};
    g_value_init(&val, G_TYPE_STRING);
    g_value_set_static_string(&val, serial);

    g_object_set_property(G_OBJECT(source), "serial", &val);
    gst_object_unref(source);
  }

  /* retrieve the appsink from the pipeline */
  GstElement *sink = gst_bin_get_by_name(GST_BIN(pipeline), "sink");

  // tell appsink to notify us when it receives an image
  g_object_set(G_OBJECT(sink), "emit-signals", TRUE, NULL);

  // tell appsink what function to call when it notifies us
  g_signal_connect(sink, "new-sample", G_CALLBACK(callback), NULL);

  gst_object_unref(sink);

  gst_element_set_state(pipeline, GST_STATE_PLAYING);

  // wait for a second while the pipeline stars
  sleep(1);
  if (n_frames == -1){
    g_print("Press 'enter' to stop the stream.\n");
    /* wait for user input to end the program */
    getchar();
  }
  else{
    for (int i = 0; i<n_frames;i++){
      usleep(100000);
      save_image = 1;
      usleep(100000);
    }
  }

  // this stops the pipeline and frees all resources
  gst_element_set_state(pipeline, GST_STATE_NULL);

  /*
    the pipeline automatically handles
    all elements that have been added to it.
    thus they do not have to be cleaned up manually
  */
  gst_object_unref(pipeline);

  return 0;
}
