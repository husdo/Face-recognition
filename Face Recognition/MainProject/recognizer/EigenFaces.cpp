#include "EigenFaces.h"
/**
* create a model
* @param nbComponent : number of greatest components from the PCA. 20 is usually enough
* @param recognitionThreshold : 0 => nobody is recognized. inf.=> everybody will be
* recognize as someone. I worked with 1100
*/
EigenFaces::EigenFaces(int nbComponents, double recognitionThreshold)
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
