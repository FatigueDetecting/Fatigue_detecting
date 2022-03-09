# Fatigue_detecting



- 1.HOG compelete
- 2.ERT no use 
- 3.CNN openvino
- 4.split frame
- 5.hyper thread 
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


- Use cirular_buffer to keep realtime 
  Introduction
A Circular Buffer.
  ![](https://github.com/FatigueDetecting/Fatigue_detecting/blob/main/pic2.png)
Description
The term circular buffer (also called a ring or cyclic buffer) refers to an area in memory which is used to store incoming data. When the buffer is filled, new data is written starting at the beginning of the buffer and overwriting the old.

boost::circular_buffer is a STL compliant container.

It is a kind of sequence similar to std::list or std::deque. It supports random access iterators, constant time insert and erase operations at the beginning or the end of the buffer and interoperability with std algorithms.

The circular_buffer is especially designed to provide fixed capacity storage. When its capacity is exhausted, newly inserted elements will cause elements to be overwritten, either at the beginning or end of the buffer (depending on what insert operation is used).

The circular_buffer only allocates memory when created, when the capacity is adjusted explicitly, or as necessary to accommodate resizing or assign operations.



There is also a circular_buffer_space_optimized version available.



circular_buffer_space_optimized is an adaptation of the circular_buffer which does not allocate memory all at once when created, instead it allocates memory as needed.

The predictive memory allocation is similar to typical std::vector implementation. Memory is automatically freed as the size of the container decreases.

The memory allocation process of the space-optimized circular buffer. The min_capacity of the capacity controller represents the minimal guaranteed amount of allocated memory. The allocated memory will never drop under this value. The default value of the min_capacity is set to 0. The min_capacity can be set using the constructor parameter () capacity_control or the function set_capacity.

The space-optimized version is, of course, a little slower.



- Enjoy it!
  ![](https://github.com/FatigueDetecting/Fatigue_detecting/blob/main/pic1.png)