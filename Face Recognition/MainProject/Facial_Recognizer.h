#include "Images.h"
#include "cv.h"

#pragma once
class Facial_Recognizer
{

private:
	std::vector<double> validation(Images& InputImages);
public:
	virtual void training(Images& InputImages) = 0;
	virtual int predict(cv::Mat* InputImage) = 0;
	virtual void save(std::string path = "") const = 0; // if the path is empty you should save the classifier next to the exe with your own file format
	virtual void load(std::string path = "") = 0; // if the path is empty you should load the classifier from the default place.

	Facial_Recognizer();
	~Facial_Recognizer();
};

std::vector<double> Facial_Recognizer::validation(Images& InputImages)
{
	int result;
	double TruePositive(0), TrueNegative(0), FalsePositive(0), FalseNegative(0), MissedClass(0);
	for (unsigned int Iterator = 0; Iterator < InputImages.size(); ++Iterator){
		result = predict(InputImages.getColorImage(Iterator));
		if (result == InputImages.getLabel(Iterator) && result >= 0)
			++TruePositive;
		else if (result == InputImages.getLabel(Iterator) && result == -1)
			++TrueNegative;
		else if (result >= 0 && InputImages.getLabel(Iterator) == -1)
			++FalsePositive;
		else if (result == -1 && InputImages.getLabel(Iterator) >= 0)
			++FalseNegative;
		else if (result != InputImages.getLabel(Iterator) && result >= 0 && InputImages.getLabel(Iterator) >= 0)
			++MissedClass;
	}
	TruePositive /= InputImages.size();
	TrueNegative /= InputImages.size();
	FalsePositive /= InputImages.size();
	FalseNegative /= InputImages.size();
	MissedClass /= InputImages.size();

	std::printf("*****************************************\n");
	std::printf("** The result of the validation:       **\n");
	std::printf("*****************************************\n");
	std::printf("** True Positive:  %1.3f               **\n",TruePositive);
	std::printf("** True Negative:  %1.3f               **\n",TrueNegative);
	std::printf("** False Positive: %1.3f               **\n",FalsePositive);
	std::printf("** False Negative: %1.3f               **\n",FalseNegative);
	std::printf("** Missed Class:   %1.3f               **\n",MissedClass);
	std::printf("*****************************************\n");


	std::vector<double> ret = {TruePositive, TrueNegative, FalsePositive, FalseNegative, MissedClass};
	return ret;
}

