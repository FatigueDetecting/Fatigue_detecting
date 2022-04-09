#include<iostream>
#include<math.h>
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
		int Statement[3] = { 0,0,0};

	public: 
		float pointX[68];
		float pointY[68];
		float StandardWink = 0;
		float StandYaw = 0;
		float StandardNode = 0;
		float DetWink = 0;
		float DetNode = 0;
		float DetYaw = 0;
		
		//转换数据
		void PointToMatrix(struct shape) {
			for (int num = 0; num < 68; num++) {
				pointX[num] = shape.part(num).x();
				pointY[num] = shape.part(num).y();
			}
		
		};
		
		//眨眼标准 command == 0为标定模式 command == 0为测试模式
		float WinkStandard(double pointX[], double pointY[], int command) {
			if (command == 0) {
				while (NumWink < 5) {
					StandardWink = (fabs((pointY[37] + pointY[38]) - (pointY[41] + pointY[40])) / (2 * fabs(pointX[39] - pointX[36]))) + StandardWink;
					NumWink = NumWink + 1;
				}
			}
			StandardWink = StandardWink / 5;
			return StandardWink;
		
		};
		
		//哈欠标准
		float YawStandard(double pointY[], int command) {
			float StandardYaw = 0;
			if (command == 0) {
				while(NumYaw < 5){
				StandardYaw = fabs((pointY[31] + pointY[35]) / 2 - pointY[37]) / fabs((pointY[31] + pointY[35]) / 2 - (pointY[48] + pointY[54]) / 2) + StandardYaw;
				NumYaw = NumYaw + 1;
				}
			}
			StandardYaw = StandardYaw / 5;
			return StandardYaw;
		};
		
		//点头标准
		float NodeStandard(double pointX[], double pointY[], int command) {
			if (command == 0){
				while (NumNode < 5) {
					StandardNode = fabs((pointY[30] - pointY[27]) / (pointX[54] - pointX[48])) + StandardNode;
					NumNode = NumNode + 1;
				}
			}
			StandardNode = StandardNode / 5;
			return StandardNode;
		};

		//眨眼检测(正在修改)    输出0意味着为正常， 如果输出为1则意味着为疲劳
		int WinkDetect(double pointX[], double pointY[], int command, float StandardWink) {
			if (command == 2 || 4 || 5 || 7) {
				if (fabs((pointY[37] + pointY[38]) - (pointY[41] + pointY[40])) / (2 * fabs(pointX[39] - pointX[36])) < StandardWink){
					Num2Wink = Num2Wink + 1;
				}
				else {
					Num2Wink = 0;
					DetWink = 0;
				}
				if (Num2Wink > 6) {
					DetWink = 1;
				}
				else {
					DetWink = 0;
				}
			}
			else { 
				 DetWink = 0; 
			}
			return DetWink;
		};
		//哈欠检测  ---------- 疲劳现象消失后，均会回归0
		int YawDetect(double pointX[], double pointY[], float StandardYaw,int command) {
			if (command == 1 || 4 || 6 || 7) {
				if ((fabs((pointY[31] + pointY[35]) / 2 - pointY[37]) / fabs((pointY[31] + pointY[35]) / 2 - (pointY[48] + pointY[54]) / 2)) > StandardYaw) {
					Num2Yaw = Num2Yaw + 1;
				}
				else {
					Num2Yaw = 0;
					DetWink = 0;
				}
				if (Num2Yaw > 4) {
					DetYaw = 1;
				}
				else {
					DetYaw = 0;
				}
			}
			else {
				DetYaw = 0;
			}
			return DetYaw;
		};
		//点头检测
		int NodeDetect(double pointX[], double pointY[], int command, float StandardNode) {
			if (command = 3 || 5 || 6 || 7) {
				if (fabs((pointY[30] - pointY[27]) / (pointX[54] - pointX[48]))/StandardNode > 0.3) {
					Num2Node = Num2Node + 1;
				}
				else {
					Num2Node = 0;
					DetNode = 0;
				}
				if (Num2Yaw > 5) {
					DetNode = 1;
				}
				else {
					DetNode = 0;
				}
			}
			else {
				DetNode = 0;
			}
			return DetNode;
		};
		
		int CommandReturn(int DetWink, int DetYaw, int DetNode) {
			switch ( DetWink + DetYaw + DetNode ) {
			case 0:
				return 0;
			case 1:
				if (DetWink == 1) {
					return 2;
				}
				else if (DetYaw == 1) {
					return 1;
				}
				else {
					return 3;
				}
			case 2:
				if (DetWink == 0) {
					return 6;
				}
				else if (DetYaw == 0) {
					return 5;
				}
				else {
					return 4;
				}
			case 3:
				return 7;
			}
		}
};

