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
    DEMARCATE_Flag = false;
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

    //capture.set(CAP_PROP_BUFFERSIZE,0);


    std::thread tw1(&faceGet::frame_write, ca,std::ref(capture), std::ref(buff));
    tw1.detach();
    std::thread tr1(&faceGet::frame_read, ca, std::ref(buff),1, std::ref(capture), std::ref(frame), std::ref(shap1));
    tr1.detach();
    //std::thread tr2(&faceGet::frame_read, ca, std::ref(buff),2, std::ref(capture), std::ref(frame), std::ref(shap1));
    //tr2.detach();
    //std::thread tr3(&faceGet::frame_read, ca, std::ref(buff),3, std::ref(capture), std::ref(frame), std::ref(shap1));
    //tr3.detach();
    thread_state = 0;

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
            capture.set(CAP_PROP_FRAME_WIDTH, 320.0);
            capture.set(CAP_PROP_FRAME_HEIGHT, 240.0);
            thread_state = 1;
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
            thread_state = 0;
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


     Mat frame2 = frame.clone();
     QImage show_image = Mat2QImage(frame2);
     ui->label_2->setPixmap(QPixmap::fromImage(show_image));

    if(capture.isOpened())
    {
        if(DETECTION_Flag)
        {
                int status;
                if(detectionFLAG[3]==true)
                {
                    status = ca.returnFrame(std::ref(shap1),7);

                }
                else
                {
                    int cnt = 0;
                    if(detectionFLAG[0]==true) cnt += 1;
                    if(detectionFLAG[1]==true) cnt += 2;
                    if(detectionFLAG[2]==true) cnt += 4;
                    switch(cnt){
                    case 0:
                    {
                        status = ca.returnFrame(std::ref(shap1),0);
                        break;
                    }
                    case 1:
                    {
                        status = ca.returnFrame(std::ref(shap1),1);
                        break;
                    }
                    case 2:  {
                        status = ca.returnFrame(std::ref(shap1),2);
                        break;
                    }
                    case 3:  {
                        status = ca.returnFrame(std::ref(shap1),4);
                        break;
                    }
                    case 4:  {
                        status = ca.returnFrame(std::ref(shap1),3);
                        break;
                    }
                    case 5:  {
                        status = ca.returnFrame(std::ref(shap1),6);
                        break;
                    }
                    case 6:  {
                        status = ca.returnFrame(std::ref(shap1),5);
                        break;
                    }
                    case 7:  {
                        status = ca.returnFrame(std::ref(shap1),7);
                        break;
                    }

                    }

                }

                QString str;
                //cout<<status<<endl;
                switch(status)
                {
                case 0: str = QString("No fatigue");break;
                case 1: str = QString("Yawing");break;
                case 2: str = QString("Blinking");break;
                case 3: str = QString("Nodding");break;
                case 4: str = QString("Yawing and Blinking");break;
                case 5: str = QString("Blinking and Nodding");break;
                case 6: str = QString("Yawing and Nodding");break;
                case 7: str = QString("Yawing,Blinking,Nodding");break;
                //case 8: str = QString("Finish calibration");break;
                }

                QDateTime timestamp = QDateTime::currentDateTime();
                               QString time = timestamp.toString("yyyy-MM-dd HH:mm:ss ddd \t");
                               if(str!="")
                              {
                                    ui->textEdit->append(time+str);
                              }
                //QDateTime timestamp = QDateTime::currentDateTime();
                //QString time = timestamp.toString("yyyy-MM-dd HH:mm:ss ddd \t\t\t");
                //ui->textEdit->append(time+str);

        }

        if(DEMARCATE_Flag)
        {

             int status = ca.returnFrame(std::ref(shap1),8);
             if(status==8)
             {
                 qDebug()<<QString("demarcate finished");
                 ui->textEdit->append(QString("calibration end"));
                 //QString("demarcate end");
                 DEMARCATE_Flag = false;
             }

        }

    }
    else
    {
        qDebug()<<"Camera not open";
    }

}



void Widget::push_button_2_clicked()
{
    //start detection
    if(DETECTION_Flag==false)
    {
        QMessageBox::StandardButton result = QMessageBox::information(this,
                                                     QString("Tips"),
                                                     QString("Are you sure you want to start detection?"),
                                                     QMessageBox::Yes|QMessageBox::No,
                                                     QMessageBox::Yes);
        if(result==QMessageBox::Yes)
        {
            DETECTION_Flag = true;
            ui->pushButton_2->setText(QString("Stop Detection"));
            qDebug()<<"start detection";

        }
        else
        {

            DETECTION_Flag = false;
            qDebug()<<"Don't start detection";
        }


    }

    else
    {
        QMessageBox::StandardButton result = QMessageBox::information(this,
                                                     QString("Tips"),
                                                     QString("Are you sure you want to stop detection?"),
                                                     QMessageBox::Yes|QMessageBox::No,
                                                     QMessageBox::Yes);
        if(result==QMessageBox::Yes)
        {
            DETECTION_Flag = false;
            ui->pushButton_2->setText(QString("Start Detection"));
            qDebug()<<"stop detection";

        }
        else
        {
            DETECTION_Flag = true;
            qDebug()<<"Don't stop detection";
        }



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
        DEMARCATE_Flag = true;
        qDebug()<<"start demarcate";
    }
    else
    {
        DEMARCATE_Flag = false;
        qDebug()<<"Don't start demarcate";
    }

}
