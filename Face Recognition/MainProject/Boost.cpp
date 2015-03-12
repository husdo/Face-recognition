#include "Boost.h"

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

	//label2dir = InputImages.getLabel2Dir();
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

	return 0;
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