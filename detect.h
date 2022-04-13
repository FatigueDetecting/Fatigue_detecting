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
        //眨眼标准 command == 0为标定模式 command == 0为测试模式
        float WinkStandard(double pointX[], double pointY[], int command);
        //哈欠标准
        float YawStandard(double pointY[], int command);
        //点头标准
        float NodeStandard(double pointX[], double pointY[], int command) ;
        //眨眼检测(正在修改)    输出0意味着为正常， 如果输出为1则意味着为疲劳
        int WinkDetect(double pointX[], double pointY[], int command, float StandardWink);
        //哈欠检测  ---------- 疲劳现象消失后，均会回归0
        int YawDetect(double pointY[], float StandardYaw, int command) ;
        //点头检测
        int NodeDetect(double pointX[], double pointY[], int command, float StandardNode);
        int CommandReturn(int DetWink, int DetYaw, int DetNode);
    public:
        //输入的结构体：FLag,，68点矩阵
        // returnVector inputData;
        int outputData;
        std::vector<dlib::full_object_detection> shapesVector_detects;
        int reciver(std::vector<returnVector> vector);
        int flag_detect1;
        //收到结构体，并输出状态
        int PointToMatrix(dlib::full_object_detection inputData);

};

#endif // DETECT_H
