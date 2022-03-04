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
#include<cmath>
#include <stdlib.h>
#include<time.h>
#define FACE_DOWNSAMPLE_RATIO 2
#define SKIP_FRAMES 2

using namespace std;
using namespace dlib;
using namespace cv;

int main() {

        unsigned int count_blink = 0;                   //次数



    try{
                shape_predictor pos_modle;
                //loading files
                deserialize("shape_predictor_68_face_landmarks.dat") >> pos_modle;

                //object cap
                VideoCapture cap(0);
                cap.set(CAP_PROP_FRAME_WIDTH,320.0);
                cap.set(CAP_PROP_BUFFERSIZE,0);
                cap.set(CAP_PROP_FRAME_HEIGHT,240.0);
                cap.set(CAP_PROP_FOURCC,cv::VideoWriter::fourcc('M','J','P','G'));

                //open camera
                if (!cap.isOpened()) {
                        printf("Unable to connect a camera");
                        return 1;
                }
                frontal_face_detector detector = get_frontal_face_detector();


                while (waitKey(1) != 27) {
                        Mat temp;
                        Mat temp_small;
                        cap >> temp;
                        cv::resize(temp, temp_small, cv::Size(), 1.0 / FACE_DOWNSAMPLE_RATIO, 1.0 / FACE_DOWNSAMPLE_RATIO);       
                        //tramsform to dlib BGR format
                        cv_image<bgr_pixel> cimg_small(temp_small);
                        cv_image<bgr_pixel> cimg(temp);
                    if ( count_blink % SKIP_FRAMES == 0 ){
                        std::vector<dlib::rectangle> faces = detector(cimg_small);
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
                                                cv::circle(temp, cvPoint(shapes[j].part(i).x()*FACE_DOWNSAMPLE_RATIO, shapes[j].part(i).y()*FACE_DOWNSAMPLE_RATIO), 1, cv::Scalar(0, 0, 255), -1);
                                                //参数说明 图像 圆心 线条宽度 颜色 线的类型
                                                //显示数字
                                                cv::putText(temp, to_string(i), cvPoint(shapes[0].part(i).x()*FACE_DOWNSAMPLE_RATIO, shapes[0].part(i).y()*FACE_DOWNSAMPLE_RATIO), cv::FONT_HERSHEY_PLAIN, 1, cv::Scalar(0, 0, 255));

                                        }
                                }

                                //左眼

                                // //点36的坐标
                                // unsigned int x_36 = shapes[0].part(36).x();
                                // unsigned int y_36 = shapes[0].part(36).y();

                                // //点37的坐标
                                // unsigned int x_37 = shapes[0].part(37).x();
                                // unsigned int y_37 = shapes[0].part(37).y();

                                // //点38的坐标
                                // unsigned int x_38 = shapes[0].part(38).x();
                                // unsigned int y_38 = shapes[0].part(38).y();

                                // //点39的坐标
                                // unsigned int x_39 = shapes[0].part(39).x();
                                // unsigned int y_39 = shapes[0].part(39).y();

                                // //点40的坐标
                                // unsigned int x_40 = shapes[0].part(40).x();
                                // unsigned int y_40 = shapes[0].part(40).y();

                                // //点41的坐标
                                // unsigned int x_41 = shapes[0].part(41).x();
                                // unsigned int y_41 = shapes[0].part(41).y();

                                // /*
                                // //把左眼的轮廓画出来,为了证明眼睛上的取点没有取错
                                // Point pts[1][6];
                                // pts[0][0] = Point(x_36, y_36);
                                // pts[0][1] = Point(x_37, y_37);
                                // pts[0][2] = Point(x_38, y_38);
                                // pts[0][3] = Point(x_39, y_39);
                                // pts[0][4] = Point(x_40, y_40);
                                // pts[0][5] = Point(x_41, y_41);

                                // const Point* ppts[] = { pts[0] };
                                // int npt[] = { 6 };
                                // Scalar eye_color = Scalar(0, 0, 255);
                                // fillPoly(temp, ppts, npt, 1, eye_color, LINE_AA);  //在眼睛上勾勒多边形

                                // */



                                // int height_left_eye1 = y_41 - y_37;                     //37到41的纵向距离
                                // //cout << "左眼高度1\t" << height_left_eye1 << endl;
                                // int height_left_eye2 = y_40 - y_38;                     //38到40的纵向距离
                                // //cout << "左眼高度2\t" << height_left_eye2 << endl;
                                // float height_left_eye = (height_left_eye1 + height_left_eye2) / 2;               //眼睛上下距离
                                // //cout << "左眼高度\t" << height_left_eye << endl;
                                // int length_left_eye = x_39 - x_36;
                                // //cout << "左眼长度\t" << length_left_eye << endl;
                                // if (height_left_eye == 0)  //当眼睛闭合的时候，距离可能检测为0，宽高比出错
                                //         height_left_eye = 1;

                                // float EAR_left_eye;                     //眼睛宽高比
                                // EAR_left_eye = height_left_eye / length_left_eye;

                                // /*
                                // //在屏幕上显示眼睛的高度及宽高比

                                // cout << "左眼宽高比" << EAR_left_eye << endl;

                                // 显示height_left_eye、length_left_eye以及ERA_left_eye

                                // 把hight_left_eye从float类型转化成字符串类型
                                // char height_left_eye_text[30];
                                // char length_left_eye_text[30];
                                // char ERA_left_eye_text[30];

                                // _gcvt_s(height_left_eye_text, height_left_eye, 10);   //把hight_left_eye从float类型转化成字符串类型
                                // _gcvt_s(length_left_eye_text, length_left_eye,10);
                                // _gcvt_s(ERA_left_eye_text, EAR_left_eye, 10);

                                // putText(temp, height_left_eye_text, Point(10, 100), FONT_HERSHEY_COMPLEX, 1.0, Scalar(12, 255, 200), 1, LINE_AA);
                                // putText(temp,height_left_eye_text, Point(10, 200), FONT_HERSHEY_COMPLEX, 1.0, Scalar(12, 255, 200), 1, LINE_AA);
                                // putText(temp, height_left_eye_text, Point(10, 300), FONT_HERSHEY_COMPLEX, 1.0, Scalar(12, 255, 200), 1, LINE_AA);
                                // */

                                // //右眼

                                // //点42的坐标
                                // unsigned int x_42 = shapes[0].part(42).x();
                                // unsigned int y_42 = shapes[0].part(42).y();

                                // //点37的坐标
                                // unsigned int x_43 = shapes[0].part(43).x();
                                // unsigned int y_43 = shapes[0].part(43).y();

                                // //点38的坐标
                                // unsigned int x_44 = shapes[0].part(44).x();
                                // unsigned int y_44 = shapes[0].part(44).y();

                                // //点39的坐标
                                // unsigned int x_45 = shapes[0].part(45).x();
                                // unsigned int y_45 = shapes[0].part(45).y();

                                // //点40的坐标
                                // unsigned int x_46 = shapes[0].part(46).x();
                                // unsigned int y_46 = shapes[0].part(46).y();

                                // //点41的坐标
                                // unsigned int x_47 = shapes[0].part(47).x();
                                // unsigned int y_47 = shapes[0].part(47).y();

                                // unsigned int height_right_eye1 = y_47 - y_43;           //37到41的纵向距离
                                // unsigned int height_right_eye2 = y_46 - y_44;           //38到40的纵向距离
                                // float height_right_eye = (height_right_eye1 + height_right_eye2) / 2;            //眼睛上下距离
                                // if (height_right_eye == 0)  //当眼睛闭合的时候，距离可能检测为0，宽高比出错
                                //         height_right_eye = 1;

                                // unsigned int length_right_eye = x_45 - x_42;

                                // float EAR_right_eye;                    //眼睛宽高比
                                // EAR_right_eye = height_right_eye / length_right_eye;

                                //取两只眼睛的平均宽高比作为眼睛的宽高比
                                // float EAR_eyes = (EAR_left_eye + EAR_right_eye) / 2;

                                // cout << "眼睛的宽高比为" << EAR_eyes << endl;

                                // //画眼睛的波形图
                                // eye_now_x = eye_now_x + 1;                      //横坐标（每10个像素描一个点）
                                // eye_now_y = 900 - ( EAR_eyes * 900 );           //纵坐标
                                // Point poi1 = Point(eye_previous_x, eye_previous_y);     //上一个点
                                // Point poi2 = Point(eye_now_x, eye_now_y);               //现在的点
                                // Scalar eyes_color = Scalar(0, 255, 0);
                                // cv::line(Eye_Waveform, poi1, poi2, eyes_color,1, LINE_AA);                       //画线
                                // eye_previous_x = eye_now_x;
                                // eye_previous_y = eye_now_y;
                                // namedWindow("Blink waveform figure", WINDOW_AUTOSIZE);

                                //计算眨眼次数
                                // if (blink_EAR_before < EAR_eyes) {
                                //         blink_EAR_before = EAR_eyes;
                                // }
                                // if (blink_EAR_now > EAR_eyes) {
                                //         blink_EAR_now = EAR_eyes;
                                // }
                                // if (blink_EAR_after < EAR_eyes) {
                                //         blink_EAR_after = EAR_eyes;
                                // }
                                // if (blink_EAR_before > 0.2 && blink_EAR_now <= 0.2 && blink_EAR_after > 0.2) {
                                //  count_blink = count_blink + 1;
                                //         blink_EAR_before = 0.0;
                                //         blink_EAR_now = 0.2;
                                //         blink_EAR_after = 0.0;
                                // }

                                //显示height_left_eye、length_left_eye以及ERA_left_eye

                                //把hight_left_eye从float类型转化成字符串类型
                                // char count_blink_text[30];



                                //_gcvt_s(count_blink_text, count_blink, 10);   //把hight_left_eye从float类型转化成字符串类型

                                //putText(temp, count_blink_text, Point(10, 100), FONT_HERSHEY_COMPLEX, 1.0, Scalar(0, 0, 255), 1, LINE_AA);

                            }
                    }
                        count_blink ++;
                        //Display it all on the screen
                        cv::imshow("dlib_detect", temp);

                }


        }
        catch (serialization_error& e) {
                cout << "You need dlib‘s default face landmarking file to run this example." << endl;
                cout << endl << e.what() << endl;
        }
        catch (exception& e) {
                cout << e.what() << endl;

        }

}