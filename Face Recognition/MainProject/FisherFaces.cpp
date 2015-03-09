#include "FisherFaces.h"

FisherFaces::FisherFaces() {
	model = createFisherFaceRecognizer();
}

FisherFaces::FisherFaces(int nbComponents, double recognitionThreshold)
{
	model = createFisherFaceRecognizer(nbComponents, recognitionThreshold);
}

FisherFaces::~FisherFaces()
{}

void FisherFaces::training(Images& InputImages)
{
	label2dir = InputImages.getLabel2Dir();
	model->train(InputImages.getGrayImages(),InputImages.getLabels());
	trained = true;
}

string FisherFaces::predict(double *confidence,const Mat& InputImage)
{
	int predictedLabel = -1;
	(*confidence) = 0.0;
	Mat gray_Img;
	if (InputImage.type() == CV_8U)
		gray_Img = InputImage;
	else
		cvtColor(InputImage, gray_Img, CV_BGR2GRAY);
	equalizeHist(gray_Img, gray_Img);
	model->predict(gray_Img, predictedLabel, *confidence);
	std::string predictedDirectoryFisher = label2dir[predictedLabel];
	return predictedDirectoryFisher;
}

void FisherFaces::save(std::string path) const
{
	try{
		std::string extension = ".fis";
		if (path.substr(path.length() - 4) != extension){
			perror(("(saving) false name. must end with" + extension).c_str());
			return;
		}

		saveMap(path, label2dir);
		model->save("./tempModel.yml");
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

void FisherFaces::load(std::string path){
	struct stat buffer;
	std::string extension = ".fis";
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
		model->load("./tempModelFile.yml");

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
