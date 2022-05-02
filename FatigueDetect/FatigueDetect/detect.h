#ifndef DETECT_H
#define DETECT_H

#include <iostream>
#include <math.h>
#include <dlib/opencv.h>
#include <dlib/image_processing/frontal_face_detector.h>
#include <dlib/image_processing/render_face_detections.h>
#include <dlib/image_processing.h>
#include <dlib/gui_widgets.h>

struct returnVector
{
    int flags;
    int timestamps;
    std::vector<dlib::full_object_detection> shapesVectors;
};


using namespace std;
class Detect
{
    private:
        int NumWink = 0;
        int NumYaw = 0;
        int NumNode = 0;
        int Num2Wink = 0;
        int Num2Yaw = 0;
        int Num2Node = 0;
        // int Statement[3] = { 0,0,0};
        double pointX[68];
        double pointY[68];
        float StandardWink = 0;
        float StandardYaw = 0;
        float StandardNode = 0;
        float DetWink = 0;
        float DetNode = 0;
        float DetYaw = 0;
        int command = 0;
        struct return_vector
        {
            int flags;
            std::vector<dlib::full_object_detection> shapesVectors;
        };
        int flag_detects;
        //Wink Standard 
        float WinkStandard(double pointX[], double pointY[], int command);
        //Yaw Standard
        float YawStandard(double pointY[], int command);
        //Nod Standard
        float NodeStandard(double pointX[], double pointY[], int command) ;
        //Wink detect    0 represents not fatigue， 1 means fatigue, if the fatigue happens, the output will hold on 1
        int WinkDetect(double pointX[], double pointY[], int command, float StandardWink);
        //Yaw Detect     0 represents not fatigue， 1 means fatigue
        int YawDetect(double pointY[], float StandardYaw, int command) ;
        //Nod Detect     0 represents not fatigue， 1 means fatigue
        int NodeDetect(double pointX[], double pointY[], int command, float StandardNode);
        int CommandReturn(int DetWink, int DetYaw, int DetNode);
    public:
       
        // returnVector inputData;
        int outputData;
        std::vector<dlib::full_object_detection> shapesVector_detects;
        int reciver(std::vector<returnVector> vector);
        int flag_detect1;
        //recieve the struct and output the state
        int PointToMatrix(dlib::full_object_detection inputData,int flag);
        int  Calibration(returnVector inputData);
        int is_calibration =0;
};

#endif // DETECT_H
