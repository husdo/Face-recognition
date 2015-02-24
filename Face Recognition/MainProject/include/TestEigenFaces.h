#pragma once
#include "Facial_Recognizer.h"
#include <cv.h>

class TestEigenFaces :
	public Facial_Recognizer
{
private:
	cv::Ptr<cv::FaceRecognizer> Recognizer;

public:
	void training(Images& InputImages);
	std::string predict(double* confidence, const cv::Mat& InputImage);
	void save(std::string path = "") const; // if the path is empty you should save the classifier next to the exe with your own file format
	void load(std::string path = ""); // if the path is empty you should load the classifier from the default place.
	TestEigenFaces();
	~TestEigenFaces();
};

TestEigenFaces::TestEigenFaces()
{
	Recognizer = cv::createEigenFaceRecognizer();
}

TestEigenFaces::~TestEigenFaces()
{
}

void TestEigenFaces::training(Images& InputImages){
	label2dir = InputImages.getLabel2Dir();
	Recognizer->train(InputImages.getGrayImages(), InputImages.getLabels());
	this->trained = true;
}

std::string TestEigenFaces::predict(double* confidence, const cv::Mat& InputImage)
{
	int label = -1;
	label = (Recognizer->predict(InputImage));
	return label2dir[label];
}

void TestEigenFaces::save(std::string path /* = "" */) const
{
	Recognizer->save(path + "Classifier.teg");
}

void TestEigenFaces::load(std::string path /* = "" */){
	Recognizer->load(path + "Classifier.teg");
}
