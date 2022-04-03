#include<iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/gapi/imgproc.hpp>
#include <vector>
#include <dlib/opencv.h>
#include <opencv2/highgui/highgui.hpp>
#include <dlib/image_processing/frontal_face_detector.h>
#include <dlib/image_processing/render_face_detections.h>
#include <dlib/image_processing.h>
#include <dlib/image_processing/frontal_face_detector.h>
#include <dlib/gui_widgets.h>
#include<cmath>
#include <stdlib.h>
#include<time.h>
#include<string>
#include<thread>
#define FACE_DOWNSAMPLE_RATIO 2
#define SKIP_FRAMES 2

using namespace std;
using namespace dlib;
using namespace cv;

int main(int argc, char *argv[])
{
   string url = R"(http://103.199.161.254/Content/bbcworld/Live/Channel(BBCworld)/index.m3u8)";
    Mat temp,buffer;
    VideoCapture cap(url);
    cap.set(CAP_PROP_FRAME_WIDTH,320.0);
    cap.set(CAP_PROP_BUFFERSIZE,0);
    cap.set(CAP_PROP_FRAME_HEIGHT,240.0);
    cap.set(CAP_PROP_FOURCC,cv::VideoWriter::fourcc('M','P','4','2'));
    
    
    std::mutex mutex;
    std::thread get_frame([](cv::Mat& buffer, cv::VideoCapture& cap, std::mutex& mutex){
       //const std::lock_guard<std::mutex> lock(mutex);
            
        std::cout << "thread_num: " << std::this_thread::get_id() << '\n';
      while(true){

           mutex.lock();
           buffer = tmp.clone(); // copy the value
           mutex.unlock();
        }
            }, std::ref(buffer),std::ref(cap),std::ref(mutex));
    // get_frame.detach();
    cv::namedWindow("dlib_detect",0);
    cv::resizeWindow("dlib_detect",320,240);
    while (waitKey(1) != 27) { // process in the main thread

        cap >> temp

        // temp = buffer.clone();
        if(!temp.data)
            std::cout<<"null"<<std::endl;
        else {
            //std::cout<<"not null"<<std::endl;
            cv::imshow("dlib_detect", temp);
        }

    }
    return 0;
}