#include "Facial_Recognizer.h"

Facial_Recognizer::Facial_Recognizer() : trained(false) {}

Facial_Recognizer::~Facial_Recognizer() {}

std::vector<double> Facial_Recognizer::validation(Images& InputImages, ImgType type /*= ColorImg*/)
{
	std::cout << "General validation process started.\n";
	std::string result,label;
	std::map<std::string,double> CrossValidation;
	std::map<std::string,double> Counter;
	double confidence=0.0;
	for (unsigned int Iterator = 0; Iterator < InputImages.size(); ++Iterator){
		
		label = InputImages.label2dir(InputImages.getLabel(Iterator));
		
		if (type == ColorImg)
			result = predict(&confidence, InputImages.getColorImage(Iterator));
		else
			result = predict(&confidence,InputImages.getGrayImage(Iterator));

		//std::cout << InputImages.getLabel(Iterator) << " "<<result << std::endl;
		if (std::strcmp(result.c_str(),label.c_str()) == 0)
		{
			CrossValidation[label]++;
			Counter[label]++;
		}
		else
		{
			Counter[label]++;
		}

	}
	double fullResult = 0;
	for (std::map<std::string, double>::iterator it = Counter.begin(); it != Counter.end();it++){
		CrossValidation[it->first] /= it->second;
		fullResult += CrossValidation[it->first];
	}
	fullResult /= CrossValidation.size();

	std::printf("*****************************************\n");
	std::printf("** The result of the validation:       **\n");
	std::printf("*****************************************\n");
	for (std::map<std::string, double>::iterator it = Counter.begin(); it != Counter.end(); it++){
		std::cout << (it->first) << ": " << CrossValidation[it->first] << std::endl;
	}
	std::printf("*****************************************\n");
	std::cout << "**Full Result: " << fullResult << "\t\t****"<< std::endl;
	std::printf("*****************************************\n");
	
	std::cout << "General validation process started.\n";

	return std::vector<double>(0);
}

