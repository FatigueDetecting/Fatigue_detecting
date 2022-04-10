#include<dlib/image_processing/frontal_face_detector.h>
#include<dlib/image_processing.h>
#include<dlib/opencv.h>
#include<opencv2/imgproc.hpp>
#include<opencv2/highgui.hpp>
#include<iostream>

using namespace cv;
using namespace std;
using namespace dlib;

//draw polyline
void drawPolyline(cv::Mat &image, full_object_detection landmarks, int start, int end, bool isClosed=false){
    std::vector<cv::Point> points;
    for(int i=start; i<=end; i++){
        points.push_back(cv::Point(landmarks.part(i).x(), landmarks.part(i).y()));
    }
    cv::polylines(image, points, isClosed, cv::Scalar(0, 255, 255), 2, 16);
}

void drawPolylines(cv::Mat &image, full_object_detection landmarks){
    drawPolyline(image, landmarks, 0, 16);              //jaw line
    drawPolyline(image, landmarks, 17, 21);             //left eyebrow
    drawPolyline(image, landmarks, 22, 26);             //right eyebrow
    drawPolyline(image, landmarks, 27, 30);             //Nose bridge
    drawPolyline(image, landmarks, 30, 35, true);       //lower nose
    drawPolyline(image, landmarks, 36, 41, true);       //left eye
    drawPolyline(image, landmarks, 42, 47, true);       //right eye
    drawPolyline(image, landmarks, 48, 59, true);       //outer lip
    drawPolyline(image, landmarks, 60, 67, true);       //inner lip
}

//finds face landmark points and draw polylines around face landmarks
void findFaceLandmarksAndDrawPolylines(Mat &frame, frontal_face_detector faceDetector, shape_predictor landmarkDetector, 
    std::vector<dlib::rectangle> &faces, float resizeScale, int skipFrames, int frameCounter){

        //to store resized image
        Mat smallFrame;

        //resize frame to smaller image
        resize(frame, smallFrame, Size(), 1.0/resizeScale, 1.0/resizeScale);

        //change to dlib image format
        cv_image<bgr_pixel> dlibImageSmall(smallFrame);
        cv_image<bgr_pixel> dlibImage(frame);

        //detect faces at interval of skipFrames
        if(frameCounter % skipFrames == 0){
            faces = faceDetector(dlibImageSmall);
        }

        //loop over faces
        for(int i=0; i<faces.size(); i++){
            
            //scale the rectangle coordinates as we did face detection on resized smaller image
            dlib::rectangle rect(int(faces[i].left() * resizeScale),
                int(faces[i].top() * resizeScale),
                int(faces[i].right() * resizeScale),
                int(faces[i].bottom() * resizeScale));

            //Face landmark detection
            full_object_detection faceLandmark = landmarkDetector(dlibImage, rect);

            //draw poly lines around face landmarks
            drawPolylines(frame, faceLandmark);
        }
}

int main(){

    //create videoCapture object to show wbecam video
    VideoCapture videoCapture(-1);

    //Check if opencv is able to open camera
    if(!videoCapture.isOpened()){
        cout<<"can not open webcam"<<endl;
        return 0;
    }

    //define the face detector
    frontal_face_detector faceDetector = get_frontal_face_detector();

    //define landmark detector
    shape_predictor landmarkDetector;
    
    //load face landmark model
    deserialize("shape_predictor_68_face_landmarks.dat") >> landmarkDetector;

    //define resize height
    float resizeHeight = 480;

    //define skip frames
    int skipFrames = 3;

    //Get first frame
    Mat frame;
    videoCapture >> frame;

    //calculate resize scale
    float height = frame.rows;
    float resizeScale = height/resizeHeight;

    //initiate the tickCounter
    double tick = getTickCount();
    int frameCounter=0;

    //create window to display frame
    namedWindow("frame", WINDOW_NORMAL);

    //create variable to store fps
    double fps = 30.0;

    //define to hold detected faces
    std::vector<dlib::rectangle> faces;

    while (1){
        if(frameCounter == 0){
            tick = getTickCount();
        }

        //read a frame
        videoCapture >> frame;

        findFaceLandmarksAndDrawPolylines(frame, faceDetector, landmarkDetector, faces, resizeScale, skipFrames, frameCounter);

        //change fps to string
        String fpss;
        fpss = to_string(fps);

        //draw fps on the frame
        putText(frame, fpss, Point(50, 50), FONT_HERSHEY_SIMPLEX, 1, Scalar(0, 0, 255), 5);

        //show frame
        imshow("frame", frame);

        //Press esc to exit the program
        char key = waitKey(1);
        if(key == 27){
            break;
        }

        //increment frame counter
        frameCounter++;

        //calculate fps after every 100 frames
        if(frameCounter == 100){
            tick = ((double)getTickCount() - tick)/getTickFrequency();
            fps = 100.0/tick;
            frameCounter = 0;
        }
    }

    //release video capture object
    videoCapture.release();

    //close all the opened windows
    destroyAllWindows();
    
    return 0;
}