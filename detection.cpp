#include "detection.h"
using namespace std;
using namespace cv;
using namespace dlib;


float StandardWink = 0;
float StandardYaw = 0;
float StandardNode = 0;
static int SumNode = 0;
static int SumWink = 0;
static int SumYaw = 0;
double pointX[68];
double pointY[68];
bool NodeDetermine;
bool YawDetermine;
bool WinkDetermine;

float SWink(double pointX[] ,double pointY[]) {
    float StandardWink = 0;
    StandardWink = fabs(( pointY[37] + pointY[38]) - (pointY[41] + pointY[42])) / (2 * fabs(pointX[39] - pointX[36]));
    return StandardWink;
}

float SYaw(double pointX[], double pointY[]) {
    float StandardYaw = 0;
    StandardYaw = fabs((pointY[31] + pointY[35]) / 2 - pointY[37]) / fabs((pointY[31] + pointY[35]) / 2 - (pointY[48] + pointY[54]) / 2);
    return StandardYaw;
}

float SNode(double pointY[]) {
    float StandardNode = 0;
    StandardNode =  pointY[33];
    return StandardNode;
}

bool Wink(double pointX[], double pointY[], float StandardWink) {
    float UpperEyebrow = 0;
    float LowerEyebrow = 0;
    float EyeVertical = 0;
    float EyeHorizontal = 0;
    int WinkRatio = 0;

    UpperEyebrow = (pointY[37] +  pointY[38]);
    LowerEyebrow = (pointY[41] + pointY[42]);
    EyeVertical = fabs(UpperEyebrow - LowerEyebrow) / 2;
    EyeHorizontal = fabs(pointX[39] - pointX[36]);
    WinkRatio = ((EyeVertical / EyeHorizontal) > StandardWink);  //大于阈值为正常，小于阈值为疲劳眨眼
    return WinkRatio;
};


bool Yaw(double pointX[], double pointY[], float StandardYaw) {
    float k = 0, MNSO = 0;
    float YawRatio;
    k = fabs((pointY[31] + pointY[35]) / 2 - pointY[37]);
    MNSO = fabs((pointY[31] + pointY[35]) / 2 - (pointY[48] + pointY[54]) / 2);
    YawRatio = fabs(k / MNSO) < StandardYaw;	//小于阈值为正常，大于阈值为疲劳打哈欠
    return YawRatio;
};

bool NNode(double pointY[], float StandardNode) {
    float r = 0, Pich = 0;
    float NodeRatio = 0;
    NodeRatio = fabs((pointY[33] - StandardNode) / StandardNode) > 0.2;	//鼻尖连点
    return NodeRatio;
};


QVector<bool> detection(cv::Mat& image, const QVector<bool> & detectionFlags)
{
    qDebug()<<"detection";
    QVector<bool> result(3,false);
    double pointX[68];
    double pointY[68];
    shape_predictor pos_modle;
    deserialize("data") >> pos_modle;
    frontal_face_detector detector = get_frontal_face_detector();
    Mat temp_gray;
    cvtColor( image , temp_gray , COLOR_BGR2GRAY  );
    equalizeHist( temp_gray, temp_gray );
    cv_image<unsigned char> cimg(temp_gray);
    cv_image<bgr_pixel> temp1 (image);

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
                                 cv::circle(image, cvPoint(shapes[j].part(i).x(), shapes[j].part(i).y()), 1, cv::Scalar(0, 0, 255), -1);
                                 //参数说明 图像 圆心 线条宽度 颜色 线的类型
                                 //显示数字
                                 cv::putText(image, to_string(i), cvPoint(shapes[0].part(i).x(), shapes[0].part(i).y()), cv::FONT_HERSHEY_PLAIN, 1, cv::Scalar(0, 0, 255));

                                 pointX[i] = shapes[0].part(i).x();
                                 pointY[i] = shapes[0].part(i).y();

                                 }

                                StandardNode = SNode(pointY);
                                StandardYaw =  SYaw(pointX, pointY);
                                StandardWink = SWink(pointX,pointY);

                                if(detectionFlags[0])
                                {
                                    if (Yaw(pointX, pointY, StandardYaw) == 1) {
                                                           SumYaw = SumYaw + 1;
                                                           if (SumYaw > 10) {
                                                               YawDetermine = true;
                                                           }
                                                       }
                                                       else {
                                                           SumYaw = 0;
                                                           YawDetermine = false;
                                                       }
                                    if(YawDetermine)
                                    {
                                        qDebug()<<"yaw";
                                        result[0] = true;
                                    }


                                }

                                if(detectionFlags[1])
                                {
                                    if (Wink(pointX,pointY, StandardWink) == 1) {
                                                            SumWink = SumWink + 1;
                                                            if (SumWink > 8) {
                                                                WinkDetermine = true;
                                                            }
                                                        }
                                                        else {
                                                            SumNode = 0;
                                                            WinkDetermine = false;
                                                        }
                                    if(WinkDetermine) {

                                        qDebug()<<"wink";
                                        result[1] = true;
                                    }


                                }
                                if(detectionFlags[2])
                                {

                                    if (NNode(pointY, StandardNode) == 1) {
                                            SumNode = SumNode + 1;
                                            if (SumNode > 5) {
                                                    NodeDetermine = true;
                                            }
                                    }
                                    else {
                                            SumNode = 0;
                                            NodeDetermine = false;
                                    }
                                    if(NodeDetermine) {

                                        qDebug()<<'node';
                                        result[2] = true;
                                    }



                               }

            }
    }
    return result;
}







