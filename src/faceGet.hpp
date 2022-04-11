// 2646699H owns all rights in this document
#include "main.hpp"
#include <unistd.h>
#include "Detect.hpp"
#include <ctime>

using namespace cv;
using namespace dlib;
CRingBuffer<Mat> buff(sizes);
CRingBuffer<dlib::full_object_detection> shap1(30);
Detect de;
// status:1:2:3:4:5:6
//
//
//
//
class faceGet
{
public:
    int returnFrame(CRingBuffer<dlib::full_object_detection>& ,int);
    std::vector<returnVector> sendFace4Test(CRingBuffer<dlib::full_object_detection> &shape1);
    void frame_write(cv::VideoCapture &cap, CRingBuffer<Mat> &buff);
    void frame_read(CRingBuffer<Mat> &buff, int num, VideoCapture &cap, Mat &result_image, CRingBuffer<dlib::full_object_detection> &shap1);
    int sendFace(CRingBuffer<dlib::full_object_detection> &shape1,int);
    void Delay(int time) // time*1000为秒数
    {
        clock_t now = clock();
        while (clock() - now < time);
    }
private:
    dlib::full_object_detection shapesFace;
    int shape1_length;
    std::time_t getTimeStamp();
    Detect de;
};


std::time_t faceGet::getTimeStamp()
{
    std::chrono::time_point<std::chrono::system_clock, std::chrono::milliseconds> tp = std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::system_clock::now()); // obtain date time
    std::time_t timestamp = tp.time_since_epoch().count();                                                                                                                        // calculate timestamp
    return timestamp;
}
void faceGet::frame_read(CRingBuffer<Mat> &buff, int num, VideoCapture &cap, Mat &result_image, CRingBuffer<dlib::full_object_detection> &shap1)
{

    while (bStart)
    {
        // TickMeter timer;
        // timer.start();
        Delay(100);
        cout << "this is read" << endl;
        Mat frame, temp_gray, det_input;
        dlib::frontal_face_detector detector = dlib::get_frontal_face_detector();
        dlib::shape_predictor pos_modle;
        dlib::deserialize("../shape_predictor_68_face_landmarks.dat") >> pos_modle;
        int ori_w = cap.get(cv::CAP_PROP_FRAME_WIDTH);  // frame width
        int ori_h = cap.get(cv::CAP_PROP_FRAME_HEIGHT); // frame height
        int *pResults = NULL;
        unsigned char *pBuffer;

        pBuffer = (unsigned char *)malloc(DETECT_BUFFER_SIZE);

        if (!pBuffer)
        {
            fprintf(stderr, "Can not alloc buffer.\n");
            return;
        }

        while (true)
        {
            if (flag == 9)
            {
                Delay(1000);
                continue;
            }
            if (buff.GetLength() == 0)
            {
                // std::this_thread::sleep_for(std::chrono::milliseconds(100));
                // cout<<"tr1_buffer_length:"<<buff.GetLength()<<endl;
                continue;
            }
            buff.ReadData(&result_image);
            resize(result_image, det_input, Size(det_w, det_h));
            pResults = facedetect_cnn(pBuffer, (unsigned char *)(det_input.ptr(0)), det_w, det_h, (int)det_input.step);
            //  print the detection results
            for (int i = 0; i < (pResults ? *pResults : 0); i++)
            {
                short *p = ((short *)(pResults + 1)) + 142 * i;
                int confidence = p[0];
                int x = p[1];
                int y = p[2];
                int w = p[3];
                int h = p[4];
                int shift = w * 0.1;
                x = (x - shift) < 0 ? 0 : x - shift;
                y = (y - shift) < 0 ? 0 : y - shift;
                w = w + shift * 2;
                h = h + shift * 2;

                x = int(x * 1.0 / det_w * ori_w);
                y = int(y * 1.0 / det_h * ori_h);
                w = int(w * 1.0 / det_w * ori_w);
                h = int(h * 1.0 / det_h * ori_h);
                w = (w > ori_w) ? ori_w : w;
                h = (h > ori_h) ? ori_h : h;
                char sScore[256];
                snprintf(sScore, 256, "%d", confidence);
                if (confidence > 90)
                {
                    // draw face rectangle
                    if (x + w >= ori_w)
                        w = ori_w - x;
                    if (y + h >= ori_h)
                        h = ori_h - y;

                    cv::putText(result_image, sScore, cv::Point(x, y - 3), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 255, 0), 1);
                    cv::rectangle(result_image, Rect(x, y, w, h), Scalar(0, 255, 0), 2);

                    cv::Mat face = result_image(cv::Rect(x, y, w, h));
                    // cout<<"this is a breakPoint of resize"<<endl;
                    cv::resize(face, face, cv::Size(112, 112));
                    cv::cvtColor(face, temp_gray, COLOR_BGR2GRAY);
                    equalizeHist(temp_gray, temp_gray);

                    // dlib
                    try
                    {
                        cv_image<unsigned char> cimg(temp_gray);
                        // 返回脸的坐标
                        // std::vector<dlib::rectangle> faces = detector(cimg);
                        long int a = 0;
                        long int b = 0;
                        long int c = 112;
                        long int d = 112;
                        dlib::rectangle faces(a, b, c, d);
                        dlib::full_object_detection shapes;

                        cv::rectangle(result_image, Rect(0, 0, 112, 112), Scalar(255, 255, 254), -1);

                        shapes = pos_modle(cimg, faces);
                        if (shapes.part(36).x() < 110 && shapes.part(36).x() > 0)
                        {
                            for (int i = 0; i < 68; i++)
                            {

                                cv::circle(result_image, cvPoint(shapes.part(i).x(), shapes.part(i).y()), 1, cv::Scalar(0, 0, 255), -1);
                                // cv::putText(result_image, to_string(i), cvPoint(shapes.part(i).x(), shapes.part(i).y()), cv::FONT_HERSHEY_PLAIN, 1, cv::Scalar(0, 0, 255));
                            }
                        }

                        shap1.WriteData(&shapes);
                        // returnFrame()
                        // cout<<"shap1_length:"<<shap1.GetLength()<<endl;
                    }
                    catch (exception &e)
                    {
                        cout << e.what() << endl;
                        free(pBuffer);
                    }
                }
            }
        }
        free(pBuffer);
        cout << "thread ==============> read stop" << endl;
    }
}
void faceGet::frame_write(cv::VideoCapture &cap, CRingBuffer<Mat> &buff)
{
    // while (bStart){
    cout << "this is write" << endl;
    Mat input;

    if (cap.isOpened())
    {
        cout << "Capture is opened" << endl;
        while (1)
        {
            if (flag == 9)
            {
                Delay(1000);
                continue;
            }
            if (buff.GetLength() == sizes)
            {
                Delay(1000);
                continue;
            }

            cap >> input;
            buff.WriteData(&input);
        }
    }
    else
    {
        cout << "open camera failed" << endl;
    }
}
// arg :buffer_name
// return struct flag timestamp faceDetect
int faceGet::sendFace(CRingBuffer<dlib::full_object_detection> &shape1,int flags)
{
    std::vector<full_object_detection> shapesVector;
    returnVector rv;
    timestamp = getTimeStamp();
    shape1_length = shape1.GetLength();

    if (shape1_length == 0)
    {
        // Delay(1000);
        return 100;
    }
    else
    {
        shape1.ReadData(&shapesFace);
        if (shapesFace.part(36).x() > 112 && shapesFace.part(36).x() < 0)
        {
            // return shapesFace.part(36).x();
            return 101;
        }
        shapesVector.clear();
        shapesVector.push_back(shapesFace);
        return_vector.clear();
        rv.flags = flags;
        // rv.timestamps = timestamp;
        rv.shapesVectors = shapesVector;
        return_vector.push_back(rv);
        // cout<<rv.timestamps<<endl;
        faceResult = de.reciver(return_vector);
        return faceResult;
    }
}
std::vector<returnVector> faceGet::sendFace4Test(CRingBuffer<dlib::full_object_detection> &shape1)
{
    // struct returnVector  *return_vector = (returnVector *)malloc(sizeof(returnVector));
    // struct returnVector  return_vector;
    std::vector<full_object_detection> shapesVector;
    // // time_t timestamp;
    // returnVector  return_vector;
    // while(1){
    returnVector rv;
    timestamp = getTimeStamp();
    int shape1_length = shape1.GetLength();
    if (shape1_length == 0)
    {
        // shapesVector.clear();
        return_vector.clear();
        rv.flags = 6;
        rv.timestamps = timestamp;
        rv.shapesVectors = shapesVector;
        return_vector.push_back(rv);

        return return_vector;
    }
    else
    {
        shape1.ReadData(&shapesFace);
        shapesVector.clear();
        shapesVector.push_back(shapesFace);
        return_vector.clear();
        // if(shapesVector[0].part(36).x()<110&&shapesVector[0].part(36).x()>0){
        rv.flags = flag;
        rv.timestamps = timestamp;
        // return_vector->shapesVectors = (std::vector<full_object_detection> *)malloc(sizeof(std::vector<full_object_detection>));
        rv.shapesVectors = shapesVector;
        return_vector.push_back(rv);
        // cout<<"x36:"<<shapesVector[0].part(36).x()<<endl;
        ////return timeStamp+shape+flag;
        // std::cout << return_vector.timestamps << std::endl;//毫秒级时间戳
        // std::cout << "\n" << std::endl;//毫秒级时间戳
        // std::cout << return_vector[0].shapesVectors[0].part(36).x()<< std::endl;//毫秒级时间戳
        cout << "shap1_length:" << shape1_length << endl;
        cout << "shapesVector_length:" << shapesVector.size() << endl;
        // de.reciver(return_vector);
        // return return_vector;
        // call function
        // recive status
        // free(return_vector);
        // here just use function to callback return_vector
        return return_vector;
        // }
    }
}
// arg:buff_name,frame,flag
// return status
//*frame
int faceGet::returnFrame(CRingBuffer<dlib::full_object_detection>& shape_send,int flags)
{
    status = sendFace(shape_send,flags);
    return status;
}