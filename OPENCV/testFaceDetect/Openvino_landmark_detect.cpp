//This is a file create by HONG(2646699H)
//If you want more please email:2646699h@student.gla.ac.uk
//this file use openvino 
//INTEL NCS2
#include <iostream>
#include <string>
#include <sstream>
#include <opencv2/dnn/dnn.hpp>
// OpenCV
#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"
#include <opencv2/videoio.hpp>
#include <opencv2/face.hpp>
#include <opencv2/objdetect.hpp>
#include "drawLandmarks.hpp"
using namespace std;
using namespace cv;
using namespace cv::face;
using namespace cv::dnn;
int main(int argc,char** argv)

{
        //loading sth about model from DNN
        string landmark_xml = "";
        string landmark_bin = "";
        string tensorflowWeightFile = "";
        string tensorflowConfigFile = "";
        // load LANDMARK
        Net mkNet = cv::dnn::readNetFromModelOptimizer(landmark_xml, landmark_bin);
        mkNet.setPreferableBackend(DNN_BACKEND_INFERENCE_ENGINE);
        mkNet.setPreferableTarget(DNN_TARGET_CPU);

        // load netural net
        Net net = cv::dnn::readNetFromTensorflow(tensorflowWeightFile, tensorflowConfigFile);
        net.setPreferableBackend(DNN_BACKEND_INFERENCE_ENGINE);
        net.setPreferableTarget(DNN_TARGET_CPU);    

        //here I change the video Writer and set the solution about camera
        VideoCapture cap;
                cap.open(0);
                cap.set(CAP_PROP_FRAME_WIDTH,320.0);
                cap.set(CAP_PROP_BUFFERSIZE,2);
                cap.set(CAP_PROP_FRAME_HEIGHT,240.0);
                cap.set(CAP_PROP_FOURCC,cv::VideoWriter::fourcc('M','J','P','G'));
        if(!cap.isOpened())
                return -1;
        Mat frame, gray;
        namedWindow("Video",WINDOW_NORMAL);
        // Read a frame
                int w = 1;
                int h = 1;
        while (true) {
                bool ret = cap.read(frame);
        if (!ret) {
                break;
        }
        // flip(frame, frame, 1);
        cv::Mat inputBlob = cv::dnn::blobFromImage(frame, 1.0, cv::Size(300, 300),
        Scalar(104.0, 177.0, 123.0), false, false);
        net.setInput(inputBlob, "data");
        cv::Mat detection = net.forward("detection_out");
        cv::Mat detectionMat(detection.size[2], detection.size[3], CV_32F, detection.ptr<float>());
        for (int i = 0; i < detectionMat.rows; i++)
        {
        float confidence = detectionMat.at<float>(i, 2);
                if (confidence > 0.5)
                {
                        int x1 = static_cast<int>(detectionMat.at<float>(i, 3) * w);
                        int y1 = static_cast<int>(detectionMat.at<float>(i, 4) * h);
                        int x2 = static_cast<int>(detectionMat.at<float>(i, 5) * w);
                        int y2 = static_cast<int>(detectionMat.at<float>(i, 6) * h);
            
                        Mat roi = frame(Range(y1, y2), Range(x1, x2));
                        Mat blob = cv::dnn::blobFromImage(roi, 1.0, Size(60, 60), Scalar(), false, false);
                        mkNet.setInput(blob);
                        Mat landmark_data = mkNet.forward();
                        // printf("rows: %d \n, cols : %d \n", landmark_data.rows, landmark_data.cols);
                        for (int i = 0; i < landmark_data.cols; i += 2) {
                                float x = landmark_data.at<float>(0, i)*roi.cols+x1;
                                float y = landmark_data.at<float>(0, i + 1)*roi.rows+y1;
                        // mkList.push_back(Point(x, y));
                        circle(frame, Point(x, y), 2, Scalar(0, 0, 255), 2, 8, 0);
                        }
                        cv::rectangle(frame, cv::Point(x1, y1), cv::Point(x2, y2), cv::Scalar(0, 255, 0), 2, 8);
                }
        }
        imshow("Face-Detection Demo", frame);
        if (waitKey(100) >= 0) break;
    }
     cap.release();
    return 0;
}