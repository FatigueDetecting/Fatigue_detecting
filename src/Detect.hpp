#include <iostream>
#include <math.h>
#include <dlib/opencv.h>
#include <dlib/image_processing/frontal_face_detector.h>
#include <dlib/image_processing/render_face_detections.h>
#include <dlib/image_processing.h>
#include <dlib/gui_widgets.h>
using namespace std;
class Detect
{
private:
	int NumWink = 0;
	int NumYaw = 0;
	int NumNode = 0;
	int Num2Wink = 0;
	int Num2Yaw = 0;
	int Num2Node = 0;
	int Statement[3] = {0, 0, 0};
	struct returnVector
		{
    		int flags;
    		int timestamps;
    		std::vector<dlib::full_object_detection> shapesVectors;
		};
	int flag_detects ;
	int timestamp_detects ;
	std::vector<dlib::full_object_detection> shapesVector_detects ;
public:
	float pointX[68];
	float pointY[68];
	float StandardWink = 0;
	float StandYaw = 0;
	float StandardNode = 0;
	float DetWink = 0;
	float DetNode = 0;
	float DetYaw = 0;
	int reciver(std::vector<returnVector>);

};
int Detect::reciver(std::vector<returnVector> return_vector){
	flag_detects = return_vector[0].flags;
	timestamp_detects = return_vector[0].timestamps;
	shapesVector_detects = return_vector[0].shapesVectors;
}
