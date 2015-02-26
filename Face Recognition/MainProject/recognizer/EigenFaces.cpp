#include "EigenFaces.h"
#include <stdexcept>
#include <iostream>
/**
* create a model
* @param nbComponent : number of greatest components from the PCA. 20 is usually enough
* @param recognitionThreshold : 0 => nobody is recognized. inf.=> everybody will be
* recognize as someone. I worked with 1100
*/
EigenFaces::EigenFaces(int nbComponents, double recognitionThreshold)
{
	Recognizer = cv::createEigenFaceRecognizer(nbComponents,recognitionThreshold);
	trained = false;
}

EigenFaces::~EigenFaces()
{
}

void EigenFaces::training(Images& InputImages){
	label2dir = InputImages.getLabel2Dir();
	Recognizer->train(InputImages.getGrayImages(), InputImages.getLabels());
	trained = true;
}
/**
* give a predicted label to an image according to a model
* @param predictedLabel : label found by the model
* @param confidence : distance between trained images and new image
* @param Input images : must be gray images
*/
std::string EigenFaces::predict(double* confidence, const cv::Mat& InputImage)
{
	CV_Assert(InputImage.type() == CV_8U);
	int predictedLabel = -1;
	(*confidence) = 0.0;
	Recognizer->predict(InputImage, predictedLabel, (*confidence));
	std::string predictedDirectory = label2dir[predictedLabel];
	return predictedDirectory;
}

void EigenFaces::save(std::string path) const
{
	saveMap("../Debug/Classifier/", label2dir);
	Recognizer->save(path + "Classifier_EigenFace.yml");
}

void EigenFaces::load(std::string path){
	label2dir=readMapFile("../Debug/Classifier/");
	Recognizer->load(path + "Classifier_EigenFace.yml");
	trained = true;
}
