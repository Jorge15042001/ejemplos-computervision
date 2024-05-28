#include "opencv2/imgcodecs.hpp"
#include "opencv2/videoio.hpp"
#include <chrono>
#include <cstdio>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <string>
#include <thread>
int main(const int argc, const char *const *const argv) {
    // Open the default camera (device 0)
    int n_frames = 0;
    bool live_stream = false;

    if (argc >2){
      fprintf(stderr, "numero de parametros invalidos");
      return 1;
    }
    else if (argc == 1) {
      live_stream = true;
    }
    else {
      try{
        n_frames = std::stoi(argv[1]);
      }
      catch(...){
        fprintf(stderr,"el segundo argumento debe ser un numero mayor a 0");
        return 1;
      }

      if (n_frames <=0){
        fprintf(stderr,"el segundo argumento debe ser un numero mayor a 0");
        return 1;
      }
    }



    cv::VideoCapture cap(0);

    if (!cap.isOpened()) {
        std::cerr << "Error: Could not open camera" << std::endl;
        return -1;
    }

    cap.set(cv::CAP_PROP_FRAME_HEIGHT, 480);
    cap.set(cv::CAP_PROP_FRAME_WIDTH, 640);

    cap.set(cv::CAP_PROP_AUTO_EXPOSURE, 3);



    // Create a window to display the video
    cv::namedWindow("Video", cv::WINDOW_AUTOSIZE);

    std::this_thread::sleep_for(std::chrono::seconds(1));

    // Main loop
    if (live_stream){
      int image_counter = 0;
      while (true) {
          // Capture frame-by-frame
          cv::Mat frame;
          cap >> frame;

          // If the frame was not captured correctly, break the loop
          if (frame.empty()) {
              break;
          }

          // Display the current frame
          cv::imshow("Video", frame);

          // Check for key press
          int key = cv::waitKey(1);

          // Save the current frame if 'p' key is pressed
          if (key == 'p') {
              const std::string img_name = "./images/image_"+ std::to_string(image_counter) +".png";
              cv::imwrite(img_name, frame);
              std::cout<<"frame "<< img_name<< " written!"<<std::endl;
              image_counter ++;

          }

          // Terminate the program if 'q' key is pressed
          if (key == 'q') {
              break;
          }
      }
    }
    else{
      for (int i = 0; i<n_frames;i++){
        std::this_thread::sleep_for(std::chrono::milliseconds(100));

        cv::Mat frame;
        cap >> frame;

        const std::string img_name = "./images/image_"+ std::to_string(i) +".png";
        cv::imwrite(img_name, frame);
        std::cout<<"frame "<< img_name<< " written!"<<std::endl;
      }

    }

    // Release the camera and close OpenCV windows
    cap.release();
    cv::destroyAllWindows();

    return 0;
}

