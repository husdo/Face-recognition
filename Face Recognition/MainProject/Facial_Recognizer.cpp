#include "Facial_Recognizer.h"

Facial_Recognizer::Facial_Recognizer() : trained(false) {}

Facial_Recognizer::~Facial_Recognizer() {}

std::vector<double> Facial_Recognizer::validation(Images& InputImages, ImgType type /*= ColorImg*/)
{
	int result,label;
	std::map<int,double> CrossValidation;
	std::map<int,double> Counter;
	for (unsigned int Iterator = 0; Iterator < InputImages.size(); ++Iterator){
		label = InputImages.getLabel(Iterator);

		if (type == ColorImg)
			result = predict(InputImages.getColorImage(Iterator));
		else
			result = predict(InputImages.getGrayImage(Iterator));
		

		if (result == label)
		{
			CrossValidation[label]++;
			Counter[label]++;
		}
		else
		{
			Counter[label]++;
		}

	}
	for (unsigned int iter = 0; iter < Counter.size(); ++iter){
		CrossValidation[iter] /= Counter[iter];
	}

	std::printf("*****************************************\n");
	std::printf("** The result of the validation:       **\n");
	std::printf("*****************************************\n");
	for (unsigned int iter = 0; iter < Counter.size(); ++iter)
		std::cout<<InputImages.label2dir(iter)<<": "<<CrossValidation[iter]<<std::endl;
	std::printf("*****************************************\n");

	return std::vector<double>(0);
}

