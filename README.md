# Fatigue_detecting



- 1.HOG
- 2.ERT
- 3.CNN

Build Raspberry Pi 4b
- environment
  1. After testing, the fulleye version cannot detect the camera well when using opencv, so the bluster version is used.
  2. opencv4.5.1 can be installed smoothly
  3. Need to install dependencies before installing opencv
  4. The python version needs to be greater than 3.7
   
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