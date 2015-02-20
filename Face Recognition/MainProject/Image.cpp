#include "Image.h"


Image::Image(Mat& InputImage)
{
	ColorImage = new Mat(InputImage);
}


Image::~Image()
{
	delete(ColorImage);
	delete(GrayImage);
	delete(IntegralImage);
}
