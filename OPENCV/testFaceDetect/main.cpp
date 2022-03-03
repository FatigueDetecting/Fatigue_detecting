//This is a file create by HONG(2646699H)
//If you want more please email:2646699h@student.gla.ac.uk
#include<iostream>
#include <opencv2/opencv.hpp>
#include <vector>
#include <dlib/opencv.h>
#include <opencv2/highgui/highgui.hpp>
#include <dlib/image_processing/frontal_face_detector.h>
#include <dlib/image_processing/render_face_detections.h>
#include <dlib/image_processing.h>
#include <dlib/image_processing/frontal_face_detector.h>
#include <dlib/gui_widgets.h>
#include <cmath>
#include <stdlib.h>
#include<time.h>

using namespace std;
using namespace dlib;
using namespace cv;

int main() {

        //存储眼睛的上一个点的坐标
        int eye_previous_x = 10;                //原点横坐标
        int eye_previous_y = 890;               //原点纵坐标
        int eye_now_x = 1;
        int eye_now_y = 1;


        //存储眨眼的次数
        unsigned int count_blink = 0;           //眨眼次数


        //每次眨眼EAR都要经历从  大于0.2-小于0.2-大于0.2 的过程
        float blink_EAR_before =0.0;            //眨眼前
        float blink_EAR_now =0.2;               //眨眼中
        float blink_EAR_after = 0.0;            //眨眼后


        try {
                VideoCapture cap(0);
                cap.set(CAP_PROP_FRAME_WIDTH,320.0);
                cap.set(CAP_PROP_BUFFERSIZE,1);
                cap.set(CAP_PROP_FRAME_HEIGHT,240.0);
                cap.set(CAP_PROP_FOURCC,cv::VideoWriter::fourcc('M','J','P','G'));
                if (!cap.isOpened()) {   //打开摄像头
                        printf("Unable to connect a camera");
                        return 1;
                }
                frontal_face_detector detector = get_frontal_face_detector();

                shape_predictor pos_modle;
                //loading files
                deserialize("shape_predictor_68_face_landmarks.dat") >> pos_modle;

                while (waitKey(30) != 27) {
                        Mat temp;
                        cap >> temp;

                        //将图像转化为dlib中的BGR的形式
                        cv_image<bgr_pixel> cimg(temp);

                        std::vector<dlib::rectangle> faces = detector(cimg);
                        std::vector<full_object_detection> shapes;
                        unsigned int faceNumber = faces.size();   //获取容器中向量的个数即人脸的个数
                        for (unsigned int i = 0; i < faceNumber; i++) {
                                shapes.push_back(pos_modle(cimg, faces[i]));
                        }
                        if (!shapes.empty()) {
                                int faceNumber = shapes.size();
                                for (int j = 0; j < faceNumber; j++)
                                {
                                        for (int i = 0; i < 68; i++)
                                        {
                                                //用来画特征值的点
                                                cv::circle(temp, cvPoint(shapes[j].part(i).x(), shapes[j].part(i).y()), 1, cv::Scalar(0, 0, 255), -1);
                                                //参数说明 图像 圆心 线条宽度 颜色 线的类型
                                                //显示数字
                                                cv::putText(temp, to_string(i), cvPoint(shapes[0].part(i).x(), shapes[0].part(i).y()), cv::FONT_HERSHEY_PLAIN, 1, cv::Scalar(0, 0, 255));

                                        }
                                }

                                //左眼

                                //点36的坐标
                                unsigned int x_36 = shapes[0].part(36).x();
                                unsigned int y_36 = shapes[0].part(36).y();

                                //点37的坐标
                                unsigned int x_37 = shapes[0].part(37).x();
                                unsigned int y_37 = shapes[0].part(37).y();

                                //点38的坐标
                                unsigned int x_38 = shapes[0].part(38).x();
                                unsigned int y_38 = shapes[0].part(38).y();

                                //点39的坐标
                                unsigned int x_39 = shapes[0].part(39).x();
                                unsigned int y_39 = shapes[0].part(39).y();

                                //点40的坐标
                                unsigned int x_40 = shapes[0].part(40).x();
                                unsigned int y_40 = shapes[0].part(40).y();

                                //点41的坐标
                                unsigned int x_41 = shapes[0].part(41).x();
                                unsigned int y_41 = shapes[0].part(41).y();




                                int height_left_eye1 = y_41 - y_37;                     //37到41的纵向距离
                                //cout << "左眼高度1\t" << height_left_eye1 << endl;
                                int height_left_eye2 = y_40 - y_38;                     //38到40的纵向距离
                                //cout << "左眼高度2\t" << height_left_eye2 << endl;
                                float height_left_eye = (height_left_eye1 + height_left_eye2) / 2;               //眼睛上下距离
                                //cout << "左眼高度\t" << height_left_eye << endl;
                                int length_left_eye = x_39 - x_36;
                                //cout << "左眼长度\t" << length_left_eye << endl;
                                if (height_left_eye == 0)  //当眼睛闭合的时候，距离可能检测为0，宽高比出错
                                        height_left_eye = 1;

                                float EAR_left_eye;                     //眼睛宽高比
                                EAR_left_eye = height_left_eye / length_left_eye;


                                //右眼

                                //点42的坐标 
                                unsigned int x_42 = shapes[0].part(42).x();
                                unsigned int y_42 = shapes[0].part(42).y();

                                //点37的坐标
                                unsigned int x_43 = shapes[0].part(43).x();
                                unsigned int y_43 = shapes[0].part(43).y();

                                //点38的坐标
                                unsigned int x_44 = shapes[0].part(44).x();
                                unsigned int y_44 = shapes[0].part(44).y();

                                //点39的坐标
                                unsigned int x_45 = shapes[0].part(45).x();
                                unsigned int y_45 = shapes[0].part(45).y();

                                //点40的坐标
                                unsigned int x_46 = shapes[0].part(46).x();
                                unsigned int y_46 = shapes[0].part(46).y();

                                //点41的坐标
                                unsigned int x_47 = shapes[0].part(47).x();
                                unsigned int y_47 = shapes[0].part(47).y();

                                unsigned int height_right_eye1 = y_47 - y_43;           //37到41的纵向距离
                                unsigned int height_right_eye2 = y_46 - y_44;           //38到40的纵向距离
                                float height_right_eye = (height_right_eye1 + height_right_eye2) / 2;            //眼睛上下距离
                                if (height_right_eye == 0)  //当眼睛闭合的时候，距离可能检测为0，宽高比出错
                                        height_right_eye = 1;

                                unsigned int length_right_eye = x_45 - x_42;

                                float EAR_right_eye;
                                //眼睛宽高比
                                EAR_right_eye = height_right_eye / length_right_eye;

                                //取两只眼睛的平均宽高比作为眼睛的宽高比
                                float EAR_eyes = (EAR_left_eye + EAR_right_eye) / 2;

                                //cout << "眼睛的宽高比为" << EAR_eyes << endl;

                                //计算眨眼次数
                                if (blink_EAR_before < EAR_eyes) {
                                        blink_EAR_before = EAR_eyes;
                                }
                                if (blink_EAR_now > EAR_eyes) {
                                        blink_EAR_now = EAR_eyes;
                                }
                                if (blink_EAR_after < EAR_eyes) {
                                        blink_EAR_after = EAR_eyes;
                                }
                                if (blink_EAR_before > 0.2 && blink_EAR_now <= 0.2 && blink_EAR_after > 0.2) {
                                        count_blink = count_blink + 1;
                                        blink_EAR_before = 0.0;
                                        blink_EAR_now = 0.2;
                                        blink_EAR_after = 0.0;
                                }

                        }

                        // 展示每一帧
                        cv::imshow("Dlib标记", temp);



                        //计时一分钟（60秒）
                        clock_t start = clock();
                        clock_t finish = clock();
                        double consumeTime = (double)(finish - start);
                        //注意转换为double的位置
                                if (count_blink >= 10 ) {
                                                if (consumeTime / 1000 < 60) {
                                                cout << "您已疲劳,休息" << endl;
                                                count_blink = 0;
                                                return 0;
                                }
                        }

                }

        }
        catch (serialization_error& e) {
                cout << "landmark 库找不到" << endl;
                cout << endl << e.what() << endl;
        }
        catch (exception& e) {
                cout << e.what() << endl;

        }

}