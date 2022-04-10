#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <boost/thread.hpp>
#include <boost/thread/mutex.hpp>
#include <iostream>
#include <queue>
//互斥体
boost::mutex io_mutex;
using namespace cv;

using namespace std;
Mat image_catch;
volatile bool thread_all_exit = false;
Mat display_mat_origin;
Mat display_mat_process;
void catch_image()
{

//open camera
  Mat frame;
  VideoCapture capture(0);
  if(!capture.isOpened()){
      //Multi Thread lock
        boost::mutex::scoped_lock lock(io_mutex);
        cout<<"fail to open!"<<endl;
        thread_all_exit = true;
        return;
  }


   while(1){
        capture.read(frame);
        if (frame.empty())
        {
           capture.release();
           thread_all_exit = true;
           break;
        }
        resize(frame,frame,Size(640,480),0,0,INTER_LINEAR);

        boost::mutex::scoped_lock lock(io_mutex);
        image_catch=frame.clone();
        display_mat_origin =frame.clone();
        if(thread_all_exit){
          capture.release();
          break;
        }
   }
  capture.release();
}
void process_image()
{
    Mat img;
    while (1){
        if (!image_catch.empty())
        {
          boost::mutex::scoped_lock lock(io_mutex);
          img = image_catch.clone();
          resize(img,img,Size(320,240),0,0,INTER_LINEAR);
          display_mat_process =img.clone();
if(thread_all_exit){
              break;
         }
      }
    }
}

int main(int argc, char* argv[])
{
  boost::thread thread_catch_img(boost::bind(&catch_image));
  boost::thread thread_process_img(boost::bind(&process_image));
  while(1){
    if (!display_mat_origin.empty())
    {
        boost::mutex::scoped_lock lock(io_mutex);
        imshow("data",display_mat_origin);
        imshow("process",display_mat_process);
        if (waitKey(1)==27 || thread_all_exit)
        {
          thread_all_exit = true;
          break;
        }
    }
    if (!display_mat_process.empty())
    {
        boost::mutex::scoped_lock lock(io_mutex);
        imshow("process",display_mat_process);
        if (waitKey(1)==27||thread_all_exit)
        {
          thread_all_exit = true;
          break;
        }
    }

  thread_catch_img.join();
  thread_process_img.join();
  return 0;
}
