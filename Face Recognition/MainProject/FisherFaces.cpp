#include "FisherFaces.h"

FisherFaces::FisherFaces() {}

FisherFaces::FisherFaces(int nbComponents, double recognitionThreshold)
{
	model = createFisherFaceRecognizer(nbComponents, recognitionThreshold);
}

void FisherFaces::training(Images& InputImages)
{
	model->train(InputImages.getGrayImages(),InputImages.getLabels());
}

int FisherFaces::predict(double *confidence,const Mat& InputImages)
{
	Mat grayImg;
	cvtColor(InputImages, grayImg, CV_BGR2GRAY);
	int predictedLabelFisher = -1;
	*confidence = 0.0;
	model->predict(grayImg, predictedLabelFisher, *confidence);
	return predictedLabelFisher;
}

void FisherFaces::save(std::string filePath)
{
	model->save(filePath);
}

void FisherFaces::load(std::string filePath)
{
	model->load(filePath);
}

