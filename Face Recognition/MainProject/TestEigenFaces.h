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
	int predict(cv::Mat* InputImage);
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
	std::vector<cv::Mat> TrainImages;
	for (unsigned int ImageIterator = 0; ImageIterator < InputImages.size(); ++ImageIterator)
	{
		TrainImages.push_back(*InputImages.getGrayImage(ImageIterator));
	}
	Recognizer->train(TrainImages, InputImages.getLabels());
}

int TestEigenFaces::predict(cv::Mat* InputImage)
{
	return (Recognizer->predict(*InputImage));
}

void TestEigenFaces::save(std::string path /* = "" */) const
{
	Recognizer->save(path + "Classifier.teg");
}

void TestEigenFaces::load(std::string path /* = "" */){
	Recognizer->load(path + "Classifier.teg");
}
