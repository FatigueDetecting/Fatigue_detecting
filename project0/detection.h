#ifndef DETECTION_H
#define DETECTION_H
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/core/mat.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/core/types_c.h>
#include <opencv2/imgproc/imgproc_c.h>
#include <opencv2/videoio.hpp>
#include <QDebug>

void detection_yawn(cv::Mat &image);
void detection_closedEye(cv::Mat &image);
void detection_nod(cv::Mat &image);
void detection_all(cv::Mat &image);

#endif // DETECTION_H
