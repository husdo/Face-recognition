#include "Images.h"
#include "label_files.h"
#include "opencv2/core/core.hpp"
#include "opencv2/contrib/contrib.hpp"
#include "opencv2/highgui/highgui.hpp"


enum ImgType {ColorImg, GrayImg};

#pragma once
class Facial_Recognizer
{

public:
	std::vector<double> validation(Images& InputImages,ImgType type = ColorImg); // The type define the predict Input (ColorImg, GrayImg)
	bool isTrained(){return trained;}
	virtual void training(Images& InputImages) = 0;
	virtual std::string predict(double* confidence, const cv::Mat& InputImage) = 0;
	virtual void save(std::string path = "") const = 0; // if the path is empty you should save the classifier next to the exe with your own file format
	virtual void load(std::string path = "") = 0; // if the path is empty you should load the classifier from the default place.
    void print(std::string message) const;
	Facial_Recognizer(QObject* parent=0);
	~Facial_Recognizer();
protected:
    bool trained;
    QObject* window;
	std::map<int, std::string> label2dir;
};
