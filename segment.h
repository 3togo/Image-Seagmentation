#ifndef SEGMENT_H
#define SEGMENT_H
#include<opencv2/opencv.hpp>
#include"datastrcture.h"
using namespace cv;
int SegmentImage(Mat &_src3f, Mat &pImgInd, double ts, double tc, int tmin_size);
region *SegGrah(edge &edge, int edgnum, int vecnum, float c);
Mat Label(Mat& label1i, int labelNum, bool showIdx=0);
#endif