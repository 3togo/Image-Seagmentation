// EffGraphCut.cpp : 
//

#include<opencv2/opencv.hpp>
#include<opencv/cv.h>
#include<opencv2/core/core.hpp>
#include"segment.h"
using namespace cv;
#define sigma  0.8
#define min_size 200
#define k 1 //300除以255
//For instance, in the 128£128 images of the COIL database
//of objects we use k = 150. In the 320£240 or larger images, such as the street scene
//and the baseball player, we use k = 300.
//void label(Mat &imgIdx, int num);

int main(int argc, char* argv[])
{
	//IplImage *orgImge;
	//orgImge = cvLoadImage("./test.jpg");
	//imgMat = Mat(orgImge);
	Mat imgMat, img3f, imgInd,imgRes;
	int regnum = 0;	
	imgMat = imread("./test.jpg");
	cvNamedWindow("image", CV_WINDOW_AUTOSIZE);
	imshow("image", imgMat);
	imgMat.convertTo(img3f, CV_32FC3, 1.0 / 255);
	regnum = SegmentImage(img3f, imgInd, sigma, k, min_size);
	imgRes=Label(imgInd, regnum);	
	imwrite("./result.jpg",imgRes );
	cvNamedWindow("result", CV_WINDOW_AUTOSIZE);
        imshow("result", imgRes);
	waitKey(0);
	return 0;
}

