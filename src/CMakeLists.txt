cmake_minimum_required (VERSION 3.10)

project (pfld_ncnn)
# set_target_properties(detect-video-demo PROPERTIES CXX_STANDARD 11)
SET(CMAKE_CXX_STANDARD 11)
SET(CMAKE_CXX_STANDARD_REQUIRED ON)

set(OpenCV_DIR /usr/local/Cellar/opencv/4.5.5)
find_package(OpenCV REQUIRED)

include(./dlibAndModel/dlib/dlib/cmake)

# include_directories(./ncnn/include)
# add_library(ncnn STATIC IMPORTED) 
# set_property(TARGET ncnn PROPERTY IMPORTED_LOCATION ../ncnn/libncnn.a)

include_directories(./libfacedetection)
add_library(libfacedetection STATIC IMPORTED)
set_property(TARGET libfacedetection PROPERTY IMPORTED_LOCATION ../libfacedetection/libfacedetection.a)


INCLUDE_DIRECTORIES(${OpenCV_INCLUDE_DIRS} ${PROJECT_SOURCE_DIR}/include)
# target_link_libraries(detect-image-demo ncnn libfacedetection ${OpenCV_LIBS})
add_executable(pfld_ncnn test.cpp)
# target_link_libraries(pfld_ncnn ncnn libfacedetection  dlib::dlib ${OpenCV_LIBS}   )
target_link_libraries(pfld_ncnn  libfacedetection dlib::dlib ${OpenCV_LIBS}   )
 