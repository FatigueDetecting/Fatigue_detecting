#include "parallelvideocapture.h"

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
