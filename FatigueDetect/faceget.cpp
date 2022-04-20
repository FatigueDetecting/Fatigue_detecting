#include "faceget.h"

#define DETECT_BUFFER_SIZE 0x20000
const char * video_path = R"(https://vs-hls-push-uk-live.akamaized.net/x=3/i=urn:bbc:pips:service:bbc_one_hd/t=3840/v=pv14/b=5070016/main.m3u8)";
const int det_w = 112;
const int det_h = 112;
const int bStart = 1;
const int sizes = 20;
const int scale = 130;

//线程状态  0 未打开摄像头  1 打开摄像头
int thread_state = 0;

time_t timestamp;
int faceResult = -1;
std::vector<returnVector> return_vector;
CRingBuffer<Mat> buff(sizes);
CRingBuffer<dlib::full_object_detection> shap1(30);


std::time_t faceGet::getTimeStamp()
{
    std::chrono::time_point<std::chrono::system_clock, std::chrono::milliseconds> tp = std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::system_clock::now()); // obtain date time
    std::time_t timestamp = tp.time_since_epoch().count();                                                                                                                        // calculate timestamp
    return timestamp;
}
void faceGet::frame_read(CRingBuffer<Mat> &buff, int num, VideoCapture &cap, Mat &result_image, CRingBuffer<dlib::full_object_detection> &shap1)
{
    cout << "this is read" << endl;
    int is_format=1;
    while(true)
    {
        if(thread_state==0)
        {
            //camera closed do nothing

            Delay(1000);
            continue;
        }
        else if(thread_state==1)
        {
            buff.Clear();
            shap1.Clear();
            Mat frame, temp_gray, det_input;
            dlib::shape_predictor pos_modle;
            dlib::deserialize("data") >> pos_modle;
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

            while (cap.isOpened())
            {

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

                            //cv::rectangle(result_image, Rect(0, 0, 224, 224), Scalar(255, 255, 254), -1); //左上角白框

                            shapes = pos_modle(cimg, faces);
                            for (int i = 0; i < 68; i++)
                            {
                                if (shapes.part(i).x() > 110 && shapes.part(i).x() < 0){
                                    //continue;
                                    is_format = 0;

                                }
                            }
                            if(is_format == 0){
                                continue;
                            }
                            {
                                //cout << "start1-------------------"<<endl;
                                for (int i = 0; i < 68; i++)
                                {
                                    //cout << "pointX["<< i << "]" << shapes.part(i).x()  <<endl;
                                    //cout << "pointY["<< i << "]" << shapes.part(i).y()  <<endl;

                                    cv::circle(result_image, cvPoint((shapes.part(i).x()*w/112)+x, (shapes.part(i).y()*h/112)+y), 1, cv::Scalar(0, 0, 255), -1);
                                    // cv::putText(result_image, to_string(i), cvPoint(shapes.part(i).x(), shapes.part(i).y()), cv::FONT_HERSHEY_PLAIN, 1, cv::Scalar(0, 0, 255));
                                }
                                //cout << "end1-------------------"<<endl;
                                shap1.WriteData(&shapes);


                            }



                            //cout<<"shap1_length:"<<shap1.GetLength()<<endl;

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

        }


    }
    cout << "thread ==============> read stop" << endl;

}

void faceGet::frame_write(cv::VideoCapture &cap, CRingBuffer<Mat> &buff)
{
    cout<<"this is write"<<endl;
    while(true)
    {
        if(thread_state==0)
        {
            //camera closed do nothing
            //Delay(1000);
            continue;
        }
        else if(thread_state==1)
        {
            //camera opened store image
            Mat input;
            buff.Clear();
            while(cap.isOpened())
            {
                if (buff.GetLength() == sizes)
                {
                    //Delay(1000);
                    continue;
                }

                cap >> input;
                buff.WriteData(&input);

            }

        }

    }
    cout << "thread ==============> write stop" << endl;

}


// arg :buffer_name
// return struct flag timestamp faceDetect
int faceGet::sendFace(CRingBuffer<dlib::full_object_detection> &shape1,int flags)
{
    std::vector<full_object_detection> shapesVector;
    returnVector rv;
    timestamp = getTimeStamp();
    shape1_length = shape1.GetLength();
    //dlib::full_object_detection shapesFace;
    if (shape1_length >= 3){

        shape1.ReadData(&shapesFace);
        return 100;
    }


    if(shape1_length < 1)
    {
        //cout << "shape1_length"<<shape1_length<<endl;
        //Delay(100);
        return 100;
    }
    else
    {
        shape1.ReadData(&shapesFace);
        if (shapesFace.part(36).x() > 112 || shapesFace.part(36).x() < 0)
        {
            // return shapesFace.part(36).x();
           shape1.ReadData(&shapesFace);
            return 101;
        }


        //---------
        //cout << "start-------------------"<<endl;
        //for (int i = 0; i < 68; i++)
        //{
            //cout << "shapeX["<< i << "]" << shapesFace.part(i).x()  <<endl;
            //cout << "shapeY["<< i << "]" << shapesFace.part(i).y()  <<endl;
            //cv::putText(result_image, to_string(i), cvPoint(shapes.part(i).x(), shapes.part(i).y()), cv::FONT_HERSHEY_PLAIN, 1, cv::Scalar(0, 0, 255));
        //}
        //cout << "end-------------------"<<endl;
        //--------------
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
        rv.flags = thread_state;
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
int faceGet::returnFrame(CRingBuffer<dlib::full_object_detection>& shape_send,int flags = 9)
{
    int status = -1;
    status = sendFace(shape_send,flags);
    return status;
}
