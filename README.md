<div align="center">
<img src="assets/logo1.png" width = "20%">
</div>
<h1 align="center">Fatigue_detecting</h1>

<div align="center">
  
![contributors](https://img.shields.io/github/contributors/FatigueDetecting/Fatigue_detecting?style=flat-square)
[![license](https://img.shields.io/github/license/FatigueDetecting/Fatigue_detecting?style=flat-square)](https://github.com/FatigueDetecting/Fatigue_detecting/blob/main/LICENSE)
[![standard-readme compliant](https://img.shields.io/badge/readme%20style-standard-brightgreen.svg?style=flat-square)](https://github.com/RichardLitt/standard-readme)
![Issues](https://img.shields.io/github/issues/FatigueDetecting/Fatigue_detecting?style=flat-square)
![Youtube](https://img.shields.io/youtube/channel/views/UCqKJIfqUoB7yAH-tP-y33YQ?style=social)
[![Twitter](https://img.shields.io/twitter/url?style=social&url=https%3A%2F%2Ftwitter.com%2Fdengmin17290037)](https://twitter.com/dengmin17290037)

</div>
<div align="center">
  
![](https://img.shields.io/badge/OS-Debian-informational?style=plastic&color=1f425f)
![](https://img.shields.io/badge/Hardware-RaspberryPi4-informational?style=plastic&color=1f425f)
![](https://img.shields.io/badge/Hardware-OV5647-informational?style=plastic&logo=<LOGO_NAME>&logoColor=white&color=1f425f)
![](https://img.shields.io/badge/Code-C++-informational?style=plastic&logo=<LOGO_NAME>&logoColor=white&color=1f425f)
  
</div>
  <p align="center">
    <a href="https://www.youtube.com/watch?v=6-GsbKH-6_U&t=4s"><strong>View Demo    </strong></a>
    <a href="https://github.com/FatigueDetecting/Fatigue_detecting/wiki"><strong>View Viki    </strong></a>
    </br> 
    <a href="https://github.com/FatigueDetecting/Fatigue_detecting/issues">Report Bug</a>
    <a href="https://github.com/FatigueDetecting/Fatigue_detecting/issues">Request Feature</a>
</p>
  
## Table of Contents

- [Background](#background)
- [Hardware](#hardware)
- [Install](#install)
- [Usage](#usage)
- [Contributors](#contributors)
- [License](#license)


## Background

The increasing number of traffic accidents and the casualties and property losses caused by them have gradually become a common concern of the society.  
How to detect the fatigue state of drivers efficiently and accurately is of great social significance to effectively protect the life and property safety of drivers and other related personnel.
<div align="center">
<img src="https://user-images.githubusercontent.com/92716961/163671711-a983b795-c979-4407-815b-da5e890c7f0b.png" width = "50%">
</div>

This project uses night vision cameras to monitor the driver's status in real time. The system recognises and alerts the driver when they are yawning, nodding and blinking frequently.

## Hardware
  
 <div align="center">
<img src="assets/camera.jpg" width = "20%"> OV5647
<img src="assets/pi.png" width = "20%"> RaspberryPi 4B
</div>
  
## Software

- [Opencv 4.5.1](https://opencv.org/opencv-4-5-1/)
- [Dlib](http://dlib.net/)
- [Qt creator](https://www.qt.io/product/development-tools)
- [libfacedetection](https://github.com/ShiqiYu/libfacedetection)
  
## Install

1. Clone this repository
```
git clone https://github.com/FatigueDetecting/Fatigue_detecting.git
```
2. FatigueDetect path
```
cd FatigueDetect
```
3.  Run
```
./FatigueDetect
```

If you want to edit the source code，you could install Qt creator and open UIproject.pro to edit.

Download and configure QT5
1. Update
```
sudo apt-get update
sudo apt-get upgrade
```
2. Prepare to install
```
sudo apt-get install qt5-default
sudo apt-get install qtcreator
```
3. Configure
 ```
a)Open Qt creator
 ```
 ```
b)Configure the compiler: menu bar->Tools->Options->Build&Run
 Select Compilers
 Click add->GCC Select Add C  
 Click Browser
 Select /usr/bin/gcc in Compiler Path and click Apply
 Similarly, adding c++ is the same operation, the only difference is that c++ needs to add g++ instead of gcc.
```
4. Configure libfacedetection：
 ```
 a)Add the following four files to the header and source respectively: facedetectcnn-model.cpp, facedetectcnn-data.cpp, facedetectcnn.cpp and facedetectcnn.h
 b)Click make to compile. After compiling, open the directory to find the newly generated facedetection_export.h file and import the header.
 c)Add on the fourth line: #define FACEDETECTION_EXPORT and recompile
 ```


## Usage
![image](https://user-images.githubusercontent.com/92716961/164130544-5d8b2118-ff3c-43ec-b3ac-7c350f6932a4.png)

## Contributors
- [Ming Deng(2675614D)](https://github.com/MingDengEason)Project management and coordination, User research, Basic Raspberry Pi configuration debugging
- [Ruixian Hong(2646699H)](https://github.com/smallssnow)Basic Raspberry Pi configuration debugging, Functional testing,Real-time face recognition and feature point extraction
- [Yenuo Liu(2706420L)](https://github.com/pateral)Fatigue detection algorithms, Functional testing
- [Rui Sun(2664218S)](https://github.com/SR9898) Qt interface design, Project integration, Functional testing





## License

[MIT © Richard McRichface.](../LICENSE)
