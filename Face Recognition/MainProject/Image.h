#include <cv.h> // open cv general include file
#include <highgui.h> // open cv GUI include file

using namespace cv;

#pragma once
class Image
{

private:
	Mat* ColorImage;
	Mat* IntegralImage;
	Mat* GrayImage;


public:
	Mat* getGrayImage();
	Mat* getIntegralImage();
	Mat* getImage();
	Image(Mat& InputImage);
	~Image();
};

