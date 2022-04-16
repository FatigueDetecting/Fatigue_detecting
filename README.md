<div align="center">
<img src="assets/logo1.png" width = "20%">
</div>
<h1 align="center">Fatigue_detecting</h1>

<div align="center">
  
![contributors](https://img.shields.io/github/contributors/FatigueDetecting/Fatigue_detecting?style=flat-square)
[![license](https://img.shields.io/github/license/:user/:repo.svg)](LICENSE)
[![standard-readme compliant](https://img.shields.io/badge/readme%20style-standard-brightgreen.svg?style=flat-square)](https://github.com/RichardLitt/standard-readme)
![Issues](https://img.shields.io/github/issues/FatigueDetecting/Fatigue_detecting?style=flat-square)
[![Twitter](https://img.shields.io/twitter/url?style=social&url=https%3A%2F%2Ftwitter.com%2Fdengmin17290037)](https://twitter.com/dengmin17290037)

</div>
<div align="center">
  
![](https://img.shields.io/badge/OS-Debian-informational?style=plastic&color=1f425f)
![](https://img.shields.io/badge/Hardware-RaspberryPi4-informational?style=plastic&color=1f425f)
![](https://img.shields.io/badge/Hardware-OV5647-informational?style=plastic&logo=<LOGO_NAME>&logoColor=white&color=1f425f)
![](https://img.shields.io/badge/Code-C++-informational?style=plastic&logo=<LOGO_NAME>&logoColor=white&color=1f425f)
  
</div>
  <p align="center">
    <a href="https://github.com/FatigueDetecting/Fatigue_detecting/wiki"><strong>View Demo    </strong></a>
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
- [API](#api)
- [Contributors](#contributors)
- [License](#license)


## Background

The increasing number of traffic accidents and the casualties and property losses caused by them have gradually become a common concern of the society.  
How to detect the fatigue state of drivers efficiently and accurately is of great social significance to effectively protect the life and property safety of drivers and other related personnel.
<div align="center">
<img src="https://user-images.githubusercontent.com/92716961/163671711-a983b795-c979-4407-815b-da5e890c7f0b.png" width = "50%">
</div>

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

This module depends upon a knowledge of [Markdown]().

```
```

### Any optional sections

## Usage

  - 1.HOG Finish
- 2.NCNN Finish
- 3.PFLD-NCNN openvino
- 4.hyper thread  Finish

Build Raspberry Pi 4b
- environment
  1. After testing, the fulleye version cannot detect the camera well when using opencv, so the bluster version is used.
  2. opencv4.5.1 can be installed smoothly
  3. Need to install dependencies before installing opencv
  4. Need update python to python3

- dependencies 
   1.  >```$ sudo apt-get install cmake gfortran python3-dev python3-numpy  libjpeg-dev libtiff-dev libgif-dev libgstreamer1.0-dev gstreamer1.0-gtk3 libgstreamer-plugins-base1.0-dev gstreamer1.0-gl  libavcodec-dev libavformat-dev libswscale-dev libgtk2.0-dev libcanberra-gtk* libxvidcore-dev libx264-dev libgtk-3-dev libtbb2 libtbb-dev libdc1394-22-dev libv4l-dev  libopenblas-dev libatlas-base-dev libblas-dev  libjasper-dev liblapack-dev libhdf5-dev protobuf-compiler python-dev python-numpy -y ```
   2. Raspberry Pi update failure handling method
    
      >```$ sudo apt-get -y update --allow-releaseinfo-change ```
- install opencv
1.  step1 
      >```
      > $sudo rpi-eeprom-update 
      > $sudo rpi-eeprom-update -a 
      > $sudo reboot 
      >```
2. step2 
    
     The next step is to increase your swap space. OpenCV needs a lot of memory to compile. The latest versions want to see a minimum of 6.5 GB of memory before building. Your swap space is limited to 2048 MByte by default. To exceed this 2048 MByte limit, you will need to increase this maximum in the /sbin/dphys-swapfile. Everything is demonstrated in the slideshow below.
    >```
    >$ sudo nano /sbin/dphys-swapfile
    >$ sudo nano /etc/dphys-swapfile
    >$ sudo reboot
    >```
3. step3 
    >```
    >$ wget https://github.com/Qengineering/    Install-OpenCV-Raspberry-Pi-32-bits/raw/main/    OpenCV-4-5-1.sh
    >$ sudo chmod 755 ./OpenCV-4-5-1.sh
    >$ ./OpenCV-4-5-1.sh
    >```
- prepare camera hw 
  1. >``` $sudo raspi-config```
  2. >``` $sudo vi /etc/modules-load.d/modules.conf```
  3. Add 'bcm2835-v4l2' after 'I2C'

- How to Drive an LCD Display
  https://github.com/goodtft/LCD-show
  1. >```git clone https://github.com/goodtft/LCD-show.git```
  2. >``` chmod -R 755 LCD-show```
  3. >```cd LCD-show/```
```
```

Note: The `license` badge image link at the top of this file should be updated with the correct `:user` and `:repo`.

### Any optional sections

## API

### Any optional sections

## More optional sections

## Contributors
- [Rui Sun(2664218S)](https://github.com/SR9898)
- [Yenuo Liu(2706420L)](https://github.com/pateral)
- [Ming Deng(2675614D)](https://github.com/MingDengEason)
- [Ruixian Hong(2646699H)](https://github.com/smallssnow)



## License

[MIT © Richard McRichface.](../LICENSE)






