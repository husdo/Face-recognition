#include "Boost.h"
#include "EigenFaces.h"
#include "FisherFaces.h"
#include "LBPH.h"
#include <stdexcept>
#include <iostream>

using namespace std;

EigenFaces eigenfaces;
FisherFaces fisherfaces;
LBPH lbph;


Boost::Boost(){

	trained = false;
}

Boost::~Boost(){

}

void Boost::training(Images& InputImages){

	label2dir = InputImages.getLabel2Dir();
	std::cout << "Training started, be patient!\n";
	fisherfaces.training(InputImages);
	std::cout << "FisherFaces finished.\n";
	eigenfaces.training(InputImages);
	std::cout << "EigenFaces finished.\n";
	lbph.training(InputImages);
	std::cout << "LBPH finished.\n";
	std::cout << "Training ended!\n";
	//fisherfaces.load();
	//fisherfaces.validation(Imgs2, GrayImg);
	//std::cout<<fisherfaces.predict(Imgs.getGrayImage(0));
}

std::string Boost::predict(double* confidence, const cv::Mat& inputImage){
	vector<int> predictedLabel(3, -1);
	vector<double> conf(3, 0.0);
	vector<string> prediction(3, "");
	prediction[0] = eigenfaces.predict(&conf[0], inputImage);
	prediction[1] = fisherfaces.predict(&conf[1], inputImage);
	prediction[2] = lbph.predict(&conf[2], inputImage);

	if (strcmp(prediction[0].c_str(), prediction[1].c_str()) == 0)
		return prediction[0];

	if (strcmp(prediction[0].c_str(), prediction[2].c_str()) == 0)
		return prediction[0];

	if (strcmp(prediction[1].c_str(), prediction[2].c_str()) == 0)
		return prediction[1];
	return prediction[1];
}

void Boost::save(std::string path) const{
	fisherfaces.save(path);
	eigenfaces.save(path);
	lbph.save(path);
}

void Boost::load(std::string path){
	fisherfaces.load(path);
	eigenfaces.load(path);
	lbph.load(path);
}