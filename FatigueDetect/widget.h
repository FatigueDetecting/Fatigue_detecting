#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QDebug>
#include <QMessageBox>
#include <QString>
#include <QTimer>
#include <QImage>
#include <QVector>
#include <QChar>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/core/mat.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/core/types_c.h>
#include <opencv2/imgproc/imgproc_c.h>
#include <opencv2/videoio.hpp>
#include <thread>
#include <QDateTime>
#include "CRingBuffer.h"
#include "faceget.h"

#define SKIP_FRAMES 2


QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();
    cv::Mat frame;
    faceGet ca;
    QVector<bool> detectionFLAG;

private:
    Ui::Widget *ui;
    QTimer    *timer;
    QImage    *image;
    cv::VideoCapture capture;
    bool CAMERA_OPENED;
    bool DETECTION_Flag;
    bool DEMARCATE_Flag;
    int MAX_NUMS = 10;
    void detectAvailableCameras(void);
    QVector<int> available_cams; 
    int frame_cnt = 0;

public slots:
    void push_button_2_clicked();
    void push_button_4_clicked();
    void openCamara();
    void showFrame();
    void onStateChanged(int state);


};
#endif // WIDGET_H
