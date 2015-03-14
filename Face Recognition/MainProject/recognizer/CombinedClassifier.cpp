#include "CombinedClassifier.h"

CombinedClassifier::CombinedClassifier(int nbComponents, double recognitionThreshold)
{
	EigenRecognizer = EigenFaces(nbComponents, recognitionThreshold);
	FisherRecognizer = FisherFaces(nbComponents, recognitionThreshold);
	LBPHRecognizer = LBPH(0,nbComponents, recognitionThreshold);
	trained = false;
}

CombinedClassifier::CombinedClassifier()
{
	EigenRecognizer = EigenFaces();
	FisherRecognizer = FisherFaces();
	LBPHRecognizer = LBPH();
	trained = false;
}

CombinedClassifier::~CombinedClassifier()
{
}

void CombinedClassifier::training(Images& InputImages){
	label2dir = InputImages.getLabel2Dir();
	EigenRecognizer.training(InputImages);
	FisherRecognizer.training(InputImages);
	LBPHRecognizer.training(InputImages);
	trained = true;
}

std::string CombinedClassifier::predict(double* confidence, const cv::Mat& inputImage)
{

	vector<int> predictedLabel(3,-1);
	vector<double> conf(3,0.0);
	vector<string> prediction(3, "");
	prediction[0] = EigenRecognizer.predict(&conf[0], inputImage);
	prediction[1] = FisherRecognizer.predict(&conf[1], inputImage);
	prediction[2] = LBPHRecognizer.predict(&conf[2], inputImage);

	if (strcmp(prediction[0].c_str(), prediction[1].c_str()) == 0)
		return prediction[0];

	if (strcmp(prediction[0].c_str(), prediction[2].c_str()) == 0)
		return prediction[0];

	if (strcmp(prediction[1].c_str(), prediction[2].c_str()) == 0)
		return prediction[1];
	return prediction[1];
}

void CombinedClassifier::save(std::string path) const
{
	EigenRecognizer.save(path);
	FisherRecognizer.save(path);
	LBPHRecognizer.save(path);
}

void CombinedClassifier::load(std::string path){
	EigenRecognizer.load(path);
	FisherRecognizer.load(path);
	LBPHRecognizer.load(path);
}
