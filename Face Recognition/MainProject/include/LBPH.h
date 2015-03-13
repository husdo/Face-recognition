#pragma once
#include "Facial_Recognizer.h"

using namespace cv;
using namespace std;

class LBPH :
	public Facial_Recognizer
{
private:
	cv::Ptr<cv::FaceRecognizer> Recognizer;

public:
	void training(Images& InputImages);
	std::string predict(double* confidence, const cv::Mat& InputImage);
	void save(std::string path = "") const; // if the path is empty you should save the classifier next to the exe with your own file format
	void load(std::string path = ""); // if the path is empty you should load the classifier from the default place.
	LBPH(QObject* parent=0, int radius = 1, int neighbors = 8, int grid_x = 8, int grid_y = 8, double threshold = DBL_MAX);
	~LBPH();
};
