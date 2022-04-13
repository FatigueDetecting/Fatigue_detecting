#include "widget.h"
#include "ui_widget.h"


QImage Mat2QImage(cv::Mat& image)
{
    QImage img;
    if (image.channels()==3) {
        cvtColor(image, image, CV_BGR2RGB);
        img = QImage((const unsigned char *)(image.data), image.cols, image.rows,
                image.cols*image.channels(), QImage::Format_RGB888);
    } else if (image.channels()==1) {
        img = QImage((const unsigned char *)(image.data), image.cols, image.rows,
                image.cols*image.channels(), QImage::Format_ARGB32);
    } else {
        img = QImage((const unsigned char *)(image.data), image.cols, image.rows,
                image.cols*image.channels(), QImage::Format_RGB888);
    }

    return img;
}

void Widget::detectAvailableCameras(void)
{
    for(int cnt = 0;cnt<MAX_NUMS;cnt++)
    {
        capture.open(cnt);
        bool flag = capture.isOpened();
        if(flag) available_cams.push_back(cnt);
        capture.release();

    }

}



Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    //get avaliable cameras os system

    CAMERA_OPENED = false;
    DETECTION_Flag = false;
    timer   = new QTimer(this);
    image    = new QImage();


    detectAvailableCameras();
    for(auto cameraIndex:available_cams)
    {
        QString index = QString("CameraID_") + QString::number(cameraIndex);
        ui->comboBox->addItem(index);
    }
    for(int i=0;i<4;i++)
    {
        detectionFLAG.push_back(false);
    }

    capture.set(CAP_PROP_BUFFERSIZE,0);
    capture.set(CAP_PROP_FRAME_WIDTH, 320.0);
    capture.set(CAP_PROP_FRAME_HEIGHT, 240.0);

    connect(timer,&QTimer::timeout,this,&Widget::showFrame);
    connect(ui->pushButton,&QPushButton::clicked,this,&Widget::openCamara);
    connect(ui->pushButton_2,SIGNAL(clicked()),this,SLOT(push_button_2_clicked()));
    connect(ui->pushButton_4,SIGNAL(clicked()),this,SLOT(push_button_4_clicked()));

    connect(ui->checkBox_1,SIGNAL(stateChanged(int)),this,SLOT(onStateChanged(int)));
    connect(ui->checkBox_2,SIGNAL(stateChanged(int)),this,SLOT(onStateChanged(int)));
    connect(ui->checkBox_3,SIGNAL(stateChanged(int)),this,SLOT(onStateChanged(int)));
    connect(ui->checkBox_4,SIGNAL(stateChanged(int)),this,SLOT(onStateChanged(int)));

}

void Widget::onStateChanged(int state)
{
    QCheckBox *checkbox=qobject_cast<QCheckBox *>(sender());
    QString name = checkbox->objectName();
    QChar index = name.at(name.size()-1);
    if (state == Qt::Checked)
    {
           if(index=='1') detectionFLAG[0] = true;
           else if(index=='2') detectionFLAG[1] = true;
           else if(index=='3') detectionFLAG[2] = true;
           else if(index=='4') detectionFLAG[3] = true;
    }
    else
    {
        if(index=='1') detectionFLAG[0] = false;
        else if(index=='2') detectionFLAG[1] = false;
        else if(index=='3') detectionFLAG[2] = false;
        else if(index=='4') detectionFLAG[3] = false;

    }
}


Widget::~Widget()
{

    delete ui;
    delete timer;
    delete image;
}

void Widget::openCamara()
{
    if(!CAMERA_OPENED)
    {
        if(available_cams.size()>0)
        {
            int index = ui->comboBox->currentText().split("_")[1].toInt();

            capture.open(index);

            std::thread tw1(&faceGet::frame_write, ca,std::ref(capture), std::ref(buff));
            tw1.detach();
            std::thread tr1(&faceGet::frame_read, ca, std::ref(buff),1, std::ref(capture), std::ref(frame), std::ref(shap1));
            tr1.detach();

            timer->start(10);
            ui->pushButton->setText(QString("Close Camera"));
            CAMERA_OPENED = true;
            frame_cnt = 0;

        }
        else
        {
            qDebug()<<"No available cameras";
        }

    }
    else
    {

        //stop the camera
        QMessageBox::StandardButton result = QMessageBox::information(this,
                                                     QString("Tips"),
                                                     QString("Are you sure you want to turn off the camera"),
                                                     QMessageBox::Yes|QMessageBox::No,
                                                     QMessageBox::Yes);
        if(result==QMessageBox::Yes)
        {
            qDebug()<<"turn off the camera";
            timer->stop();
            capture.release();
            ui->pushButton->setText(QString("Open Camera"));
            CAMERA_OPENED = false;
            DETECTION_Flag = false;
        }

        else
        {
            qDebug()<<"Don't turn off the camera";

        }


    }

}

void Widget::showFrame()
{

     int status = ca.returnFrame(std::ref(shap1),9);
     cout<<status<<endl;
     QDateTime time = QDateTime::currentDateTime();
     Mat frame2 = frame.clone();
     QImage show_image = Mat2QImage(frame2);
     ui->label_2->setPixmap(QPixmap::fromImage(show_image));

//    if(capture.isOpened())
//    {
//        capture.read(frame);
//        //deal with frame
//        if(DETECTION_Flag)
//        {
//            if(frame_cnt%SKIP_FRAMES==0)
//            {
//                QVector<bool> result = detection(frame,detectionFLAG);
//                if(result[0]==true) {
//                    ui->textEdit->setText(QString("yaw detected"));
//                }
//                if(result[1]==true)
//                {
//                    ui->textEdit->setText(QString("closed eye detected"));
//                }
//                if(result[2]==true)
//                {
//                    ui->textEdit->setText(QString("nod detected"));
//                }
//            }

//        }



//    }
//    else
//    {
//        qDebug()<<"Camera not open";
//    }

}



void Widget::push_button_2_clicked()
{
    //start detection
    QMessageBox::StandardButton result = QMessageBox::information(this,
                                                 QString("Tips"),
                                                 QString("Are you sure you want to start detection?"),
                                                 QMessageBox::Yes|QMessageBox::No,
                                                 QMessageBox::Yes);
    if(result==QMessageBox::Yes)
    {
        DETECTION_Flag = true;
        qDebug()<<"start detection";

    }
    else
    {
        DETECTION_Flag = false;
        qDebug()<<"Don't start detection";
    }


}


void Widget::push_button_4_clicked()
{
    //start demarcate
    QMessageBox::StandardButton result = QMessageBox::information(this,
                                                 QString("Tips"),
                                                 QString("Are you sure you want to start demarcate?"),
                                                 QMessageBox::Yes|QMessageBox::No,
                                                 QMessageBox::Yes);
    if(result==QMessageBox::Yes)
    {
        qDebug()<<"start demarcate";
    }
    else
    {
        qDebug()<<"Don't start demarcate";
    }

}
