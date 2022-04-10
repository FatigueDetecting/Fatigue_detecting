
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
#include "videoCap.hpp"
#include <stdio.h>
#include <time.h>
#include "facedetectcnn.h"
#include <thread>
#include <chrono>
#include <stdlib.h>
#define DETECT_BUFFER_SIZE 0x20000
const char * video_path = R"(https://vs-hls-push-uk-live.akamaized.net/x=3/i=urn:bbc:pips:service:bbc_one_hd/t=3840/v=pv14/b=5070016/main.m3u8)";
const int det_w = 112;
const int det_h = 112;
const int bStart = 1;
const int sizes = 20;
const int scale = 130;
int flag = 0;
time_t timestamp;
int faceResult = -1;
struct returnVector
{
    int flags;
    int timestamps;
    std::vector<dlib::full_object_detection> shapesVectors;
};
std::vector<returnVector> return_vector;
