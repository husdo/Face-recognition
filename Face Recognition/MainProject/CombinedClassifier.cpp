#include "CombinedClassifier.h"

CombinedClassifier::CombinedClassifier(int nbComponents, double recognitionThreshold)
{
	EigenRecognizer = EigenFaces(nbComponents, recognitionThreshold);
	FisherRecognizer = FisherFaces(nbComponents, recognitionThreshold);
	LBPHRecognizer = LBPH(nbComponents, recognitionThreshold);
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
	map<string, double> mapConfidence;
	prediction[0] = EigenRecognizer.predict(&conf[0], inputImage);
	prediction[1] = FisherRecognizer.predict(&conf[1], inputImage);
	prediction[2] = LBPHRecognizer.predict(&conf[2], inputImage);
	
	//real confidence not distance
	mapConfidence[prediction[0]] += 1 - conf[0] / 4225;
	mapConfidence[prediction[1]] += 1 - conf[1] / 1239;
	mapConfidence[prediction[2]] += 1 - conf[2] / 101;

	string bestPrediction = mapConfidence.begin()->first;
	double maxConfidence = mapConfidence.begin()->second;
	*confidence = maxConfidence;
	for (map<string, double>::iterator it = mapConfidence.begin(); it != mapConfidence.end(); it++)
	{
		if (maxConfidence < it->second)
		{
			maxConfidence = it->second;
			bestPrediction = it->first;
			*confidence = maxConfidence;
		}
	}

	if (maxConfidence>0.6)
		return bestPrediction;
	else
		return "Not recognizable face";


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
