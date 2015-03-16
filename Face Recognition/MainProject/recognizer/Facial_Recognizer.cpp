#include "Facial_Recognizer.h"
#include "mainwindow.h"
#include <fstream>
#include <sstream>

Facial_Recognizer::Facial_Recognizer(QObject* parent) : trained(false), window(parent) {}

Facial_Recognizer::~Facial_Recognizer() {}

std::vector<double> Facial_Recognizer::validation(Images& InputImages, ImgType type /*= ColorImg*/)
{
	std::cout << "General validation process started.\n";
	std::string result,label;
	std::ofstream fileOut("validation.csv");
	std::ofstream resultFileOut("result.csv");
	std::map<std::string, double> CrossValidation;
	std::map<std::string, double> UnsuccesfurRecognization;
	std::map<std::string, double> Counter;
	std::map<std::string, double> NotRecognizableCounter; 
	double confidence = 0.0;
	for (unsigned int Iterator = 0; Iterator < InputImages.size(); ++Iterator){

		label = InputImages.label2dir(InputImages.getLabel(Iterator));

		if (type == ColorImg)
			result = predict(&confidence, InputImages.getColorImage(Iterator));
		else
			//std::cout << Iterator;
			result = predict(&confidence, InputImages.getGrayImage(Iterator));
		fileOut << InputImages.getFileName(Iterator) << "; " << (std::strcmp(result.c_str(), label.c_str()) == 0) << ";" << result << "; " << confidence << "; " << std::endl;
		//std::cout << InputImages.getLabel(Iterator) << " "<<result << std::endl;
		if (std::strcmp(result.c_str(), label.c_str()) == 0)
		{
			CrossValidation[label]++;
			Counter[label]++;
		}
		else if (std::strcmp(result.c_str(), "Not recognizable face") == 0)
		{
			NotRecognizableCounter[label]++;
		}
		else
		{
			Counter[label]++;
		}

	}
	double fullResult = 0;
	for (std::map<std::string, double>::iterator it = Counter.begin(); it != Counter.end(); it++){
		CrossValidation[it->first] /= it->second;
		fullResult += CrossValidation[it->first];
		UnsuccesfurRecognization[it->first] = NotRecognizableCounter[it->first] / (NotRecognizableCounter[it->first] + Counter[it->first]);

	}
	fullResult /= CrossValidation.size();

	print("*****************************************");
	print("** The result of the validation:       **");
	print("*****************************************");
	print("** Classification Rate:                **");
	resultFileOut << "** Classification Rate:                **\n";
	print("*****************************************");
	for (std::map<std::string, double>::iterator it = Counter.begin(); it != Counter.end(); it++){
        stringstream ss;
		ss << (it->first) << ": " << CrossValidation[it->first];
		print(ss.str());
		resultFileOut << (it->first) << ";" << CrossValidation[it->first] << std::endl;
	}
	print("*****************************************");
	stringstream ss;
	ss << "**Full Result: " << fullResult << "\t\t****";
	print(ss.str());
	resultFileOut << "Full Result;" << fullResult << std::endl;
	print("*****************************************");
	print("*****************************************");
	print("** Unsuccesful classification : **");
	resultFileOut << "** Unsuccesful classification:         **\n";
	print("*****************************************");
	for (std::map<std::string, double>::iterator it = Counter.begin(); it != Counter.end(); it++){
		stringstream ss;
		ss << (it->first) << ": " << UnsuccesfurRecognization[it->first] << std::endl;
		print(ss.str());
		resultFileOut << (it->first) << ": " << UnsuccesfurRecognization[it->first] << std::endl;
	}


	std::cout << "General validation process ended.\n";

	return std::vector<double>(0);
}

void Facial_Recognizer::print(std::string message) const{
	    if(window!=0){
            QString msg = QString::fromUtf8(message.c_str());
            static_cast<MainWindow*>(window)->printMsg(msg);
        }
        else
            std::cout << message << std::endl;
	}
