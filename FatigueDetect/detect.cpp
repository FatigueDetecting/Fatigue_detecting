#include "detect.h"


float Detect::WinkStandard(double pointX[], double pointY[], int command) {
    if (command == 8) {
        if (NumWink < 5) {
            StandardWink = (fabs((pointY[37] + pointY[38]) - (pointY[41] + pointY[40])) / (2 * fabs(pointX[39] - pointX[36]))) + StandardWink;
            NumWink = NumWink + 1;
        }
        else{
             StandardWink = StandardWink / 5;
        }

    }

    // ----
    cout<< "Winkdetect standard result:"<<StandardWink<<endl;
    return StandardWink;

};
float Detect::YawStandard(double pointY[], int command) {

    if (command == 8) {
        if (NumYaw < 5) {
            StandardYaw = fabs((pointY[31] + pointY[35]) / 2 - pointY[37]) / fabs((pointY[31] + pointY[35]) / 2 - (pointY[48] + pointY[54]) / 2) + StandardYaw;
            NumYaw = NumYaw + 1;
        }
        else{
            StandardYaw = StandardYaw / 5;
        }
    }

    cout<< "Yawdetect standard result:"<<StandardYaw<<endl;
    return StandardYaw;
};

float Detect::NodeStandard(double pointX[], double pointY[], int command) {
    if (command == 8) {
        if (NumNode < 5) {
            StandardNode = fabs((pointY[30] - pointY[27]) / (pointX[54] - pointX[48])) + StandardNode;
            NumNode = NumNode + 1;
        }
        else{
            StandardNode = StandardNode / 5;
        }
    }
    return StandardNode;
};

int Detect::WinkDetect(double pointX[], double pointY[], int command, float StandardWink) {
    if ((command == 2 )||(command ==4) ||(command == 5)  ||(command == 7) ) {
        cout << "WinkDetect is : " << fabs((pointY[37] + pointY[38]) - (pointY[41] + pointY[40])) / (2 * fabs(pointX[39] - pointX[36]))<<endl;
        if (fabs((pointY[37] + pointY[38]) - (pointY[41] + pointY[40])) / (2 * fabs(pointX[39] - pointX[36])) < (StandardWink/5)) {
            Num2Wink = Num2Wink + 1;
        }
        else {
            Num2Wink = 0;
           // DetWink = 0;
        }
        if (Num2Wink > 6) {
            DetWink = 1;
        }
        //else {
        //    DetWink = 0;
        //}
    }
    else{
        DetWink = 0;
    }
    return DetWink;
};

int Detect::YawDetect(double pointY[], float StandardYaw, int command) {
    if ((command == 1) || (command == 4) || (command == 6) || (command == 7)) {
         cout << "YawDetect is : " << fabs((pointY[31] + pointY[35]) / 2 - pointY[37]) / fabs((pointY[31] + pointY[35]) / 2 - (pointY[48] + pointY[54]) / 2)<<endl;
        if ((fabs((pointY[31] + pointY[35]) / 2 - pointY[37]) / fabs((pointY[31] + pointY[35]) / 2 - (pointY[48] + pointY[54]) / 2)) > (StandardYaw/5)) {

            Num2Yaw = Num2Yaw + 1;
        }
        else {
            Num2Yaw = 0;
           // DetWink = 0;
        }
        if (Num2Yaw > 4) {
            DetYaw = 1;
        }
        //else {
        //    DetYaw = 0;
        //}
    }
    else {
        DetYaw = 0;
    }
    return DetYaw;
};
//change to  hold on the fatigue statement
int Detect::NodeDetect(double pointX[], double pointY[], int command, float StandardNode) {
    cout << "command is :" << command << endl;
    if ((command == 3) ||(command == 5)  || (command == 6) || (command == 7)) {
        //cout<< "Nodedetect result:"<<fabs((pointY[30] - pointY[27]) / (pointX[54] - pointX[48])) <<endl;
                if (fabs(((pointY[30] - pointY[27]) / (pointX[54] - pointX[48]))-StandardNode) / StandardNode > 0.3) {
                    Num2Node = Num2Node + 1;
                }
                else {
                    Num2Node = 0;
                    //DetNode = 0;
                }
                if (Num2Node > 5) {
                    DetNode = 1;
                }
               // else {
                //    DetNode = 0;
                //}
            }
            else {
                DetNode = 0;
                cout << "SafeNode " << endl;
            }
            return DetNode;
        };
int Detect::CommandReturn(int DetWink, int DetYaw, int DetNode) {
    switch (DetWink + DetYaw + DetNode) {
    case 0:
        flag_detects = 0;
        return flag_detects;
    case 1:
        if (DetWink == 1) {
            flag_detects = 2;
            return flag_detects;
        }
        else if (DetYaw == 1) {
            flag_detects = 1;
            return flag_detects;
        }
        else {
            flag_detects = 3;
            return flag_detects;
        }
    case 2:
        if (DetWink == 0) {
            flag_detects = 6;
            return flag_detects;
        }
        else if (DetYaw == 0) {
            flag_detects = 5;
            return flag_detects;
        }
        else {
            flag_detects = 4;
            return flag_detects;
        }
    case 3:
        flag_detects = 7;
        return flag_detects;
    default:
        flag_detects = 8;
        return flag_detects;
    }


}



        //收到结构体，并输出状态
int Detect::PointToMatrix(dlib::full_object_detection inputData,int command) {
    cout << "start-------------------"<<endl;

    for (int num = 0; num < 68; num++) {
        pointX[num] = inputData.part(num).x();
        cout << "pointX["<< num << "]" << pointX[num]  <<endl;
        pointY[num] = inputData.part(num).y();
        cout << "pointY["<< num << "]" << pointY[num]  <<endl;
        //if (pointX[num]> 112 || pointX[num] < 0 || pointY[num] < 0 || pointY[num] > 112 ){
                // return shapesFace.part(36).x();
                //return 888;

        //}
    }
    cout << "end-------------------"<<endl;
    //cout << "start-------------------"<<endl;
    //for (int num = 0; num < 68; num++) {
        //cout << "pointX["<< num << "]" << pointX[num]  <<endl;
        //cout << "pointY["<< num << "]" << pointY[num]  <<endl;
    //}
    //cout << "end-------------------"<<endl;


            if(command == 8){
                StandardWink = WinkStandard(pointX, pointY, command);
                StandardYaw = YawStandard(pointY, command);
                StandardNode = NodeStandard(pointX, pointY, command);

                //cout<<"the claibration: "<<StandardWink << ","<<StandardYaw << ","<<StandardNode<<endl;
                if(NumWink == 5){is_calibration = 1;return 8;}
                return 10001;
            }
            if(is_calibration ==0){
                return 10001;
            }else{
                DetWink = WinkDetect(pointX, pointY,command,StandardWink);
                DetYaw =  YawDetect(pointY, command, StandardWink);
                DetNode = NodeDetect(pointX, pointY, command, StandardNode);
                flag_detects = CommandReturn(DetWink, DetYaw, DetNode);
                //cout<<"the WinkDetect is: "<<DetWink<<endl;
               }
            cout << "detect is : [" << DetYaw << ", "<< DetWink << ", " << DetNode << "]"<< endl;
            return flag_detects;
}





int Detect::reciver(std::vector<returnVector> return_vector) {
      cout<< "---------------------"<<endl;
        flag_detect1 = return_vector[0].flags;
        command = flag_detect1;
        //timestamp_detects = return_vector[0].timestamps; 这里我已不需要时间戳
        shapesVector_detects = return_vector[0].shapesVectors;
        return PointToMatrix(shapesVector_detects[0], command);
        // cout<< shapesVector_detects[0].part(36).x()<<endl;
}
