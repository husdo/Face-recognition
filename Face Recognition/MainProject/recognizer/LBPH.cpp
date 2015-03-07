#include "LBPH.h"
#include <stdexcept>
#include <iostream>
/**
* create a model
* @param nbComponent : number of greatest components from the PCA. 20 is usually enough
* @param recognitionThreshold : 0 => nobody is recognized. inf.=> everybody will be
* recognize as someone. I worked with 1100
*/
LBPH::LBPH(int radius, int neighbors, int grid_x, int grid_y, double threshold)
{
	Recognizer = cv::createLBPHFaceRecognizer(1, 8, 8, 8, 123.0);
	trained = false;
}

LBPH::~LBPH()
{
}

void LBPH::training(Images& InputImages){
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
std::string LBPH::predict(double* confidence, const cv::Mat& inputImage)
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


/*EXAMPLE OF TEST OF SAVING AND LOADING METHOD:
string path = "ATT_ImgDatabase/";
Images Imgs("../Debug/ATT_ImgDatabase/");  //test call
LBPH first, second;
std::cout << "Training started!\n";
first.training(Imgs);
first.save("../Debug/Classifier/model_eigen.lbp");
second.load("../Debug/Classifier/model_eigen.lbp");
std::cout << "Training ended!\n";
second.validation(Imgs);
*/
void LBPH::save(std::string path) const
{
	try{
		std::string extension = ".lbp";
		if (path.substr(path.length() - 4) != extension){
			perror(("(saving) false name. must end with" + extension).c_str());
			return;
		}

		saveMap(path, label2dir);
		Recognizer->save("./tempModel.yml");
		std::ifstream ifile("./tempModel.yml", std::ios::in);
		std::ofstream ofile((path).c_str(), std::ios::out | std::ios::app);

		//check to see that it exists:
		if (!ifile.is_open()) {
			perror("file not found (save method in Eigenfaces)");
		}
		else {
			ofile << ifile.rdbuf();
		}

		ifile.close();
		ofile.close();

		if (remove("./tempModel.yml") != 0)
			perror("Error deleting file");
	}
	catch (std::exception){
		std::cout << "Problem while saving the model";
	}
}

void LBPH::load(std::string path){
	struct stat buffer;
	std::string extension = ".lbp";
	try{
		if (path.substr(path.length() - 4) != extension){
			perror(("(loading) false name.must end with" + extension).c_str());
			return;
		}
		if (!(stat(path.c_str(), &buffer) == 0)){
			perror("model file does not exist (loading)");
			return;
		}
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
			perror("Error deleting temporary file");
		if (remove("./tempModelFile.yml") != 0)
			perror("Error deleting temporary file");
		trained = true;
	}
	catch (std::exception){
		std::cout << "Problem while loading the model";
	}
}
