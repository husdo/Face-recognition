#include "CombinedClassifier.h"

CombinedClassifier::CombinedClassifier(int nbComponents, double recognitionThreshold, QObject* parent) : Facial_Recognizer(parent)
{
	EigenRecognizer = EigenFaces(nbComponents, recognitionThreshold);
	FisherRecognizer = FisherFaces(nbComponents, recognitionThreshold);
	LBPHRecognizer = LBPH(0,nbComponents, recognitionThreshold);
	trained = false;
}

CombinedClassifier::CombinedClassifier(QObject* parent) : Facial_Recognizer(parent)
{
	EigenRecognizer = EigenFaces();
	FisherRecognizer = FisherFaces();
	LBPHRecognizer = LBPH();
	trained = false;
}

CombinedClassifier::~CombinedClassifier()
{
}

void CombinedClassifier::training(Images& InputImages){
	label2dir = InputImages.getLabel2Dir();
	EigenRecognizer.training(InputImages);
	FisherRecognizer.training(InputImages);
	LBPHRecognizer.training(InputImages);
	trained = true;
}

std::string CombinedClassifier::predict(double* confidence, const cv::Mat& inputImage)
{

	vector<int> predictedLabel(3, -1);
	vector<double> conf(3, 0.0);
	vector<string> prediction(3, "");
	map<string, double> mapConfidence;
	prediction[0] = EigenRecognizer.predict(&conf[0], inputImage);
	prediction[1] = FisherRecognizer.predict(&conf[1], inputImage);
	prediction[2] = LBPHRecognizer.predict(&conf[2], inputImage);

	//real confidence not distance
	mapConfidence[prediction[0]] += 1 - conf[0] / 4225;
	mapConfidence[prediction[1]] += 1 - conf[1] / 1239;
	mapConfidence[prediction[2]] += 1 - conf[2] / 101;

	string bestPrediction = mapConfidence.begin()->first;
	double maxConfidence = mapConfidence.begin()->second;
	*confidence = maxConfidence;
	for (map<string, double>::iterator it = mapConfidence.begin(); it != mapConfidence.end(); it++)
	{
		if (maxConfidence < it->second)
		{
			maxConfidence = it->second;
			bestPrediction = it->first;
			*confidence = maxConfidence;
		}
	}

	if (maxConfidence>0.6)
		return bestPrediction;
	else
		return "Not recognizable face";


	if (strcmp(prediction[0].c_str(), prediction[1].c_str()) == 0)
		return prediction[0];

	if (strcmp(prediction[0].c_str(), prediction[2].c_str()) == 0)
		return prediction[0];

	if (strcmp(prediction[1].c_str(), prediction[2].c_str()) == 0)
		return prediction[1];
	return prediction[1];
}

void CombinedClassifier::save(std::string path) const
{
	try{
		std::string extension = ".com";
		if (path.substr(path.length() - 4) != extension){
			perror(("(saving) false name. must end with" + extension).c_str());
			return;
		}

	EigenRecognizer.save("./tmp.eig");
	FisherRecognizer.save("./tmp.fis");
	LBPHRecognizer.save("./tmp.lbp");

	std::ifstream ifile1a("./tmp.eig", std::ios::in);
	std::ifstream ifile2a("./tmp.fis", std::ios::in);
	std::ifstream ifile3("./tmp.lbp", std::ios::in);
	std::ofstream ofile((path).c_str(), std::ios::out | std::ios::ate);

	//check to see that it exists:
	if (!ifile1a.is_open() || !ifile2a.is_open() || !ifile3.is_open()) {
		perror("file not found (save method in Combined Classifier)");
		return ;
	}
	char c;
	int nb_line_1 = 0;
	int nb_line_2 = 0;
	while (ifile1a.get(c))
		if (c == '\n')
			++nb_line_1;
	while (ifile2a.get(c))
		if (c == '\n')
			++nb_line_2;

	ifile1a.close();
	ifile2a.close();
	std::ifstream ifile1("./tmp.eig", std::ios::in);
	std::ifstream ifile2("./tmp.fis", std::ios::in);

	ofile << nb_line_1 << ',' << nb_line_2<<std::endl;
	ofile << ifile1.rdbuf();
	ofile << ifile2.rdbuf();
	ofile << ifile3.rdbuf();

	ifile1.close();
	ifile2.close();
	ifile3.close();
	ofile.close();

	if (remove("./tmp.eig") != 0)
		perror("Error deleting file");
	if (remove("./tmp.fis") != 0)
		perror("Error deleting file");
	if (remove("./tmp.lbp") != 0)
		perror("Error deleting file");

	print("Recognizer saved successfully!");
}
catch (std::exception){
	print("Problem while saving the model");
}
}

void CombinedClassifier::load(std::string path){
	struct stat buffer;
	std::string extension = ".com";
	try{
		if (path.substr(path.length() - 4) != extension){
			print(("(loading) false name.must end with" + extension).c_str());
			return;
		}
		if (!(stat(path.c_str(), &buffer) == 0)){
			print("model file does not exist (loading)");
			return;
		}
		std::ifstream containerFile(path.c_str());

		std::ofstream file1("./tmp.eig");
		std::ofstream file2("./tmp.fis");
		std::ofstream file3("./tmp.lbp");

		std::string line;

		getline(containerFile, line);
		std::istringstream ss(line);
		char c;
		int nbLines1, nbLines2;
		ss >> nbLines1 >> c >> nbLines2;

		for (int i = 0; i < nbLines1; i++){
			getline(containerFile, line);
			file1 << line << "\n";
		}
		for (int i = nbLines1; i < nbLines1+nbLines2; i++){
			getline(containerFile, line);
			file2 << line << "\n";
		}
		while (getline(containerFile, line))
		{
			file3 << line << "\n";
		}
		containerFile.close();
		file1.close();
		file2.close();
		file3.close();

		EigenRecognizer.load("./tmp.eig");
		FisherRecognizer.load("./tmp.fis");
		LBPHRecognizer.load("./tmp.lbp");


		if (remove("./tmp.eig") != 0)
			print("Error deleting temporary file");
		if (remove("./tmp.fis") != 0)
			print("Error deleting temporary file");
		if (remove("./tmp.lbp") != 0)
			print("Error deleting temporary file");
		trained = true;
		print("Recognizer loaded successfully!");
	}
	catch (std::exception){
		print("Problem while loading the model");
	}
}
