#include <iostream>
#include <opencv2/opencv.hpp>
#include <atomic>


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

// cap.set(CAP_PROP_FOURCC, cv::VideoWriter::fourcc('M', 'P', '4', '2'));
bool ParallelVideoCapture::isOpened() const
{
    bool opened = cv::VideoCapture::isOpened();
    
    return opened;
}
void ParallelVideoCapture::stopCapture()
{
    is_capturing_ = false;

    if(running_)
    {
        running_ = false;
        // we join the thread to sync it 
        // this will make out stopCapture function wait for the thread exits from it loop
        // thread_ptr_->join();    
        
    }

}