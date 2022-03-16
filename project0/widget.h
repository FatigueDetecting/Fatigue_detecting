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

#include "detection.h"



QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private:
    Ui::Widget *ui;
    QTimer    *timer;
    QImage    *image;
    cv::Mat    frame;
    cv::VideoCapture capture;
    bool CAMERA_OPENED;
    bool DETECTION_Flag;
    int MAX_NUMS = 10;
    void detectAvailableCameras(void);
    QVector<int> available_cams;
    QVector<bool> detectionFLAG;

public slots:
    void push_button_2_clicked();
    void push_button_4_clicked();
    void openCamara();
    void readFrame();
    void onStateChanged(int state);


};
#endif // WIDGET_H
