#pragma once
#include "Facial_Recognizer.h"
#include <cv.h>
#include "opencv2/core/core.hpp"
#include "opencv2/contrib/contrib.hpp"
#include "opencv2/highgui/highgui.hpp"

using namespace cv;
using namespace std;

class EigenFaces :
	public Facial_Recognizer
{
private:
	cv::Ptr<cv::FaceRecognizer> Recognizer;

public:
	void training(Images& InputImages);
	int predict(double* confidence, cv::Mat* InputImage);
	void save(std::string path = "") const; // if the path is empty you should save the classifier next to the exe with your own file format
	void load(std::string path = ""); // if the path is empty you should load the classifier from the default place.
	EigenFaces(int nbComponents, double recognitionThreshold);
	~EigenFaces();
};
/**
* create a model
* @param nbComponent : number of greatest components from the PCA. 20 is usually enough
* @param recognitionThreshold : 0 => nobody is recognized. inf.=> everybody will be
* recognize as someone. I worked with 1100
*/
EigenFaces::EigenFaces(int nbComponents=20, double recognitionThreshold=1100)
{
	Recognizer = cv::createEigenFaceRecognizer(nbComponents,recognitionThreshold);
}

EigenFaces::~EigenFaces()
{
}

void EigenFaces::training(Images& InputImages){
	std::vector<cv::Mat> TrainImages;
	for (unsigned int ImageIterator = 0; ImageIterator < InputImages.size(); ++ImageIterator)
	{
		TrainImages.push_back(*InputImages.getGrayImage(ImageIterator));
	}
	Recognizer->train(TrainImages, InputImages.getLabels());
}
/**
* give a predicted label to an image according to a model
* @param predictedLabel : label found by the model
* @param confidence : distance between trained images and new image
* @param img : face image to analyse
*/
int EigenFaces::predict(double* confidence, cv::Mat* InputImage)
{
	int predictedLabel = -1;
	(*confidence) = 0.0;
	Recognizer->predict((*InputImage), predictedLabel, (*confidence));
	return predictedLabel;
}

void EigenFaces::save(std::string path /* = "" */) const
{
	Recognizer->save(path + "Classifier.teg");
}

void EigenFaces::load(std::string path /* = "" */){
	Recognizer->load(path + "Classifier.teg");
}
