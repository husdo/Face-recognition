#include "EigenFaces.h"
#include <stdexcept>
#include <iostream>
/**
* create a model
* @param nbComponent : number of greatest components from the PCA. 20 is usually enough
* @param recognitionThreshold : 0 => nobody is recognized. inf.=> everybody will be
* recognize as someone. I worked with 1100
*/
EigenFaces::EigenFaces(int nbComponents, double recognitionThreshold)
{
	Recognizer = cv::createEigenFaceRecognizer(nbComponents,recognitionThreshold);
	trained = false;
}

EigenFaces::EigenFaces()
{
	Recognizer = cv::createEigenFaceRecognizer();
	trained = false;
}

EigenFaces::~EigenFaces()
{
}

void EigenFaces::training(Images& InputImages){
	label2dir = InputImages.getLabel2Dir();
	Recognizer->train(InputImages.getGrayImages(), InputImages.getLabels());
	trained = true;
}
/**
* give a predicted label to an image according to a model
* @param predictedLabel : label found by the model
* @param confidence : distance between trained images and new image
* @param Input images : must be gray images
*/
std::string EigenFaces::predict(double* confidence, const cv::Mat& inputImage)
{
	int predictedLabel = -1;
	(*confidence) = 0.0;
	Mat gray_Img;
	if (inputImage.type() == CV_8U)
		gray_Img = inputImage;
	else
		cvtColor(inputImage, gray_Img, CV_BGR2GRAY);
	
	Recognizer->predict(gray_Img, predictedLabel, (*confidence));
	std::string predictedDirectory = label2dir[predictedLabel];
	return predictedDirectory;
}

void EigenFaces::save(std::string path) const
{
	saveMap(path+"model_EigenFace.txt", label2dir);
	Recognizer->save(path + "tempEigenFace.yml");
	std::ifstream ifile((path + "tempEigenFace.yml").c_str(), std::ios::in);
	std::ofstream ofile((path + "model_EigenFace.txt").c_str(), std::ios::out | std::ios::app);

	//check to see that it exists:
	if (!ifile.is_open()) {
		perror("file not found (save method in Eigenfaces)");
	}
	else {
		ofile << ifile.rdbuf();
	}

	ifile.close();
	ofile.close();

	if (remove((path + "tempEigenFace.yml").c_str()) != 0)
		perror("Error deleting file");
}

void EigenFaces::load(std::string path){
	struct stat buffer;
	if (!(stat(path.c_str(), &buffer) == 0))
		perror("model file does not exist load in Eigenface.cpp)");
	std::ifstream containerFile(path.c_str());

	std::ofstream labelFile("./tempLabelFile.csv");
	std::ofstream modelFile("./tempModelFile.yml");

	std::string line;
	
	getline(containerFile, line);
	std::istringstream ss(line);
	int nbLines;
	ss >> nbLines;

	for (int i = 0; i < nbLines; i++){
		getline(containerFile, line);
		labelFile << line << "\n";
	}
	while (getline(containerFile, line))
	{
		modelFile << line << "\n";
	}
	containerFile.close();
	labelFile.close();
	modelFile.close();

	label2dir = readMapFile("./tempLabelFile.csv");
	Recognizer->load("./tempModelFile.yml");

	if (remove("./tempLabelFile.csv") != 0)
		perror("Error deleting file");
	if (remove("./tempModelFile.yml") != 0)
		perror("Error deleting file");
	trained = true;
}
