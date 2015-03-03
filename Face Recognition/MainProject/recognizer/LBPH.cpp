#include "LBPH.h"
#include <stdexcept>
#include <iostream>
/**
* create a model
* @param nbComponent : number of greatest components from the PCA. 20 is usually enough
* @param recognitionThreshold : 0 => nobody is recognized. inf.=> everybody will be
* recognize as someone. I worked with 1100
*/
LBPH::LBPH(int radius, int neighbors, int grid_x, int grid_y, double threshold)
{
	Recognizer = cv::createLBPHFaceRecognizer(1, 8, 8, 8, 123.0);
	trained = false;
}

LBPH::~LBPH()
{
}

void LBPH::training(Images& InputImages){
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
std::string LBPH::predict(double* confidence, const cv::Mat& inputImage)
{
	int predictedLabel = -1;
	(*confidence) = 0.0;
	Mat gray_Img;
	if (inputImage.type() == CV_8U)
		gray_Img = inputImage;
	else
		cvtColor(inputImage, gray_Img, CV_BGR2GRAY);

	Recognizer->predict(gray_Img, predictedLabel, (*confidence));
	std::string predictedDirectory = label2dir[predictedLabel];
	return predictedDirectory;
}

void LBPH::save(std::string path) const
{
	saveMap(path+"mapFile_LBPH.csv", label2dir);
	Recognizer->save(path + "Classifier_LBPH.yml");
}

void LBPH::load(std::string path){
	label2dir = readMapFile(path + "mapFile_LBPH.csv");
	Recognizer->load(path + "Classifier_LBPH.yml");
	trained = true;
}
