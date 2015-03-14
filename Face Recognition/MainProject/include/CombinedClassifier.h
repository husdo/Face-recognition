#pragma once
#include "Facial_Recognizer.h"
#include "label_files.h"
#include <cv.h>
#include "opencv2/core/core.hpp"
#include "opencv2/contrib/contrib.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "FisherFaces.h"
#include "EigenFaces.h"
#include "LBPH.h"

using namespace cv;
using namespace std;

class CombinedClassifier :
	public Facial_Recognizer
{
private:
	FisherFaces FisherRecognizer;
	EigenFaces EigenRecognizer;
	LBPH LBPHRecognizer;

public:
	void training(Images& InputImages);
	std::string predict(double* confidence, const cv::Mat& InputImage);
	void save(std::string path = "") const; // if the path is empty you should save the classifier next to the exe with your own file format
	void load(std::string path = ""); // if the path is empty you should load the classifier from the default place.
	CombinedClassifier();
	CombinedClassifier(int nbComponents, double recognitionThreshold);
	~CombinedClassifier();
};
