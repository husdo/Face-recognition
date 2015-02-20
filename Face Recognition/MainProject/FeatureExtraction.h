#include <cv.h> // open cv general include file
#include <highgui.h> // open cv GUI include file

#include "Image.h"

using namespace cv;

#pragma once
class FeatureExtraction
{
private:
	Image* image;

protected:
	virtual Mat featureExtraction();
	
	FeatureExtraction();
	~FeatureExtraction();
public:

	
	
	

};

