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
	label2dir = InputImages.getLabel2Dir();
	Recognizer->train(InputImages.getGrayImages(), InputImages.getLabels());
	trained = true;
}
/**
* give a predicted label to an image according to a model
* @param predictedLabel : label found by the model
* @param confidence : distance between trained images and new image
* @param img : face image to analyse
*/
std::string EigenFaces::predict(double* confidence, const cv::Mat& InputImage)
{
    Mat grayImg;
    cvtColor(InputImage,grayImg,CV_BGR2GRAY);
	int predictedLabel = -1;
	(*confidence) = 0.0;
	Recognizer->predict(grayImg, predictedLabel, (*confidence));
	std::string predictedDirectory = label2dir[predictedLabel];
	return predictedDirectory;
}

void EigenFaces::save(std::string path /* = "" */) const
{
	Recognizer->save(path + "Classifier.teg");
}

void EigenFaces::load(std::string path /* = "" */){
	Recognizer->load(path + "Classifier.teg");
}
