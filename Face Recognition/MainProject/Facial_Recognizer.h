#include "Images.h"
#include "cv.h"

enum ImgType {ColorImg, GrayImg};

#pragma once
class Facial_Recognizer
{

public:
	std::vector<double> validation(Images& InputImages,ImgType type = ColorImg); // The type define the predict Input (ColorImg, GrayImg)
	virtual void training(Images& InputImages) = 0;
	virtual int predict(double* confidence, cv::Mat* InputImage) = 0;
	virtual void save(std::string path = "") const = 0; // if the path is empty you should save the classifier next to the exe with your own file format
	virtual void load(std::string path = "") = 0; // if the path is empty you should load the classifier from the default place.

	Facial_Recognizer();
	~Facial_Recognizer();
private:
    bool trained;
};
