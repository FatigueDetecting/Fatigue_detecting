#ifndef PARALLELVIDEOCAPTURE_H
#define PARALLELVIDEOCAPTURE_H
#include <iostream>
#include <opencv2/opencv.hpp>
#include <atomic>
#include <thread>

class ParallelVideoCapture : public cv::VideoCapture
{
public:
    ParallelVideoCapture() = delete;

    ParallelVideoCapture(int index) : cv::VideoCapture(index){}
        // ParallelVideoCapture.set(CAP_PROP_BUFFERSIZE,0);
        // ParallelVideoCapture.set(CAP_PROP_FRAME_WIDTH, 320.0);
        // ParallelVideoCapture.set(CAP_PROP_FRAME_HEIGHT, 240.0);
    // ~ParallelVideoCapture();
    ~ParallelVideoCapture(){
        stopCapture();
    }
    bool isOpened() const;
    // bool grab(); // base class method
    void getFrame(cv::Mat &frame);
    void startCapture();

    void stopCapture();
private:
    // ParallelVideoCapture
    std::unique_ptr<std::thread> thread_ptr_;
    bool running_ = true;;

    bool is_capturing_;
};


#endif // PARALLELVIDEOCAPTURE_H
