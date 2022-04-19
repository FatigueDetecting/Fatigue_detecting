#ifndef FACEGET_H
#define FACEGET_H

#include <unistd.h>
#include "detect.h"
#include <ctime>
#include <iostream>
#include "CRingBuffer.h"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <dlib/opencv.h>
#include <dlib/image_processing/frontal_face_detector.h>
#include <dlib/image_processing/render_face_detections.h>
#include <dlib/image_processing.h>
#include <dlib/gui_widgets.h>
#include "parallelvideocapture.h"
#include <stdio.h>
#include <time.h>
#include "facedetectcnn.h"
#include <thread>
#include <chrono>
#include <stdlib.h>

using namespace cv;
using namespace dlib;
extern CRingBuffer<Mat> buff;
extern CRingBuffer<dlib::full_object_detection> shap1;
extern int thread_state;

class faceGet
{
public:
    int returnFrame(CRingBuffer<dlib::full_object_detection>&,int);
    std::vector<returnVector> sendFace4Test(CRingBuffer<dlib::full_object_detection> &shape1);
    void frame_write(cv::VideoCapture &cap, CRingBuffer<Mat> &buff);
    void frame_read(CRingBuffer<Mat> &buff, int num, VideoCapture &cap, Mat &result_image, CRingBuffer<dlib::full_object_detection> &shap1);
    void Delay(int time) // time*1000为秒数
    {
        clock_t now = clock();
        while (clock() - now < time);
    }
private:
    dlib::full_object_detection shapesFace;
    int shape1_length;
    int sendFace(CRingBuffer<dlib::full_object_detection> &shape1, int flags);
    std::time_t getTimeStamp();
    Detect de;
};


#endif // FACEGET_H
