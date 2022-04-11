#include<iostream>
#include "faceGet.hpp"

int main(int argc, char **argv){
    // TickMeter timer;

    Mat frame5;
   
    faceGet ca;
    VideoCapture cap(0);
    cap.set(CAP_PROP_BUFFERSIZE,0);
    cap.set(CAP_PROP_FRAME_WIDTH, 320.0);
    cap.set(CAP_PROP_FRAME_HEIGHT, 240.0);
    cv::namedWindow("dlib_detect", WINDOW_FREERATIO);

    if (!cap.isOpened())
        std::cout << "video cant be read" << std::endl;

    // // //automatic write
    std::thread tw1(&faceGet::frame_write, ca,std::ref(cap), std::ref(buff));
    tw1.detach();

    // // //automatic detact
    std::thread tr1(&faceGet::frame_read, ca, std::ref(buff),1, std::ref(cap), std::ref(frame5), std::ref(shap1)); // pass by value
    tr1.detach();
    int test;
    frame5 = imread("../noSignal.jpeg");
    while (waitKey(1) != 27)
    {
        //测试Qt5 至 类调用
        // ca.returnFrame(std::ref(shap1),2);
        test = ca.sendFace (std::ref(shap1),2);
        if(test != 100 && test != 0)
            cout<<test<<endl;
        cv::imshow("dlib_detect", frame5);
    }

    cap.release();

    return 0;
}