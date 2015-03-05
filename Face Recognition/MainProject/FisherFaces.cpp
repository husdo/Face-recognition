#include "FisherFaces.h"

FisherFaces::FisherFaces() {
	model = createFisherFaceRecognizer();
}

FisherFaces::FisherFaces(int nbComponents, double recognitionThreshold)
{
	model = createFisherFaceRecognizer(nbComponents, recognitionThreshold);
}

FisherFaces::~FisherFaces()
{}

void FisherFaces::training(Images& InputImages)
{
	label2dir = InputImages.getLabel2Dir();
	model->train(InputImages.getGrayImages(),InputImages.getLabels());
	trained = true;
}

string FisherFaces::predict(double *confidence,const Mat& InputImages)
{
	Mat grayImg;
	cvtColor(InputImages, grayImg, CV_BGR2GRAY);
	int predictedLabel = -1;
	*confidence = 0.0;
	model->predict(grayImg, predictedLabel, *confidence);
	std::string predictedDirectoryFisher = label2dir[predictedLabel];
	return predictedDirectoryFisher;
}

void FisherFaces::save(std::string path) const
{
	saveMap(path + "Map.txt", label2dir);
	model->save(path+".yml");
	ifstream ifile((path + ".yml").c_str(), ios::in);
	ofstream ofile((path + "Map.txt").c_str(), ios::out | ios::app);

	ifile.close();
	ofile.close();
}

void FisherFaces::load(std::string path)
{
	model->load(path);
	trained = true;
}

