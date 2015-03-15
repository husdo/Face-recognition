#include "Facial_Recognizer.h"
#include <fstream>

Facial_Recognizer::Facial_Recognizer() : trained(false) {}

Facial_Recognizer::~Facial_Recognizer() {}

std::vector<double> Facial_Recognizer::validation(Images& InputImages, ImgType type /*= ColorImg*/)
{
	std::cout << "General validation process started.\n";
	std::string result,label;
	std::ofstream fileOut("validation.csv");
	std::ofstream resultFileOut("result.csv");
	std::map<std::string,double> CrossValidation;
	std::map<std::string, double> UnsuccesfurRecognization;
	std::map<std::string,double> Counter;
	std::map<std::string, double> NotRecognizableCounter;
	double confidence=0.0;
	for (unsigned int Iterator = 0; Iterator < InputImages.size(); ++Iterator){
		
		label = InputImages.label2dir(InputImages.getLabel(Iterator));
		
		if (type == ColorImg)
			result = predict(&confidence, InputImages.getColorImage(Iterator));
		else
			//std::cout << Iterator;
			result = predict(&confidence,InputImages.getGrayImage(Iterator));
		fileOut << InputImages.getFileName(Iterator) << "; " << (std::strcmp(result.c_str(), label.c_str()) == 0) << ";" << result << "; " << confidence << "; " << std::endl;
		//std::cout << InputImages.getLabel(Iterator) << " "<<result << std::endl;
		if (std::strcmp(result.c_str(),label.c_str()) == 0)
		{
			CrossValidation[label]++;
			Counter[label]++;
		}
		else if (std::strcmp(result.c_str(),"Not recognizable face") == 0)
		{
			NotRecognizableCounter[label]++;
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
		UnsuccesfurRecognization[it->first] = NotRecognizableCounter[it->first] / (NotRecognizableCounter[it->first] + Counter[it->first]);

	}
	fullResult /= CrossValidation.size();

	std::cout << "*****************************************\n";
	std::cout << "** The result of the validation:       **\n";
	std::cout << "*****************************************\n";
	std::cout << "** Classification Rate:                **\n";
	resultFileOut << "** Classification Rate:                **\n";
	std::cout << "*****************************************\n";
	for (std::map<std::string, double>::iterator it = Counter.begin(); it != Counter.end(); it++){
		std::cout << (it->first) << ": " << CrossValidation[it->first] << std::endl;
		resultFileOut << (it->first) << ";" << CrossValidation[it->first] << std::endl;
	}
	std::cout << "*****************************************\n";
	std::cout << "**Full Result: " << fullResult << "\t\t\t****" << std::endl;
	resultFileOut << "Full Result;" << fullResult << std::endl;
	std::cout << "*****************************************\n";

	std::cout << "*****************************************\n";
	std::cout << "** Unsuccesful classification:         **\n";
	resultFileOut << "** Unsuccesful classification:         **\n";
	std::cout << "*****************************************\n";
	for (std::map<std::string, double>::iterator it = Counter.begin(); it != Counter.end(); it++){
		std::cout << (it->first) << ": " << UnsuccesfurRecognization[it->first] << std::endl;
		resultFileOut << (it->first) << ": " << UnsuccesfurRecognization[it->first] << std::endl;
	}
	
	
	std::cout << "General validation process ended.\n";

	return std::vector<double>(0);
}

