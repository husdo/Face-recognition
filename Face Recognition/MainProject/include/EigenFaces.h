#pragma once
#include "Facial_Recognizer.h"

using namespace cv;
using namespace std;

class EigenFaces :
	public Facial_Recognizer
{
private:
	cv::Ptr<cv::FaceRecognizer> Recognizer;

public:
	void training(Images& InputImages);
	std::string predict(double* confidence, const cv::Mat& InputImage);
	void save(std::string path = "") const; // if the path is empty you should save the classifier next to the exe with your own file format
	void load(std::string path = ""); // if the path is empty you should load the classifier from the default place.
	EigenFaces(QObject* parent=0);
	EigenFaces(int nbComponents, double recognitionThreshold,QObject* parent=0);
	~EigenFaces();
};
