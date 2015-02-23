#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif//for strcpy

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <sstream>
#include <dirent.h> /* iterate through a folder*/
#include <sys/types.h>/* for folders*/
#include <sys/stat.h>/*test if it is a folder or not*/
#include <vector>
#include "opencv2/highgui/highgui.hpp"
#include "label_files.h"

#include<iostream>

using namespace std;
using namespace cv;

int extractNb(const string& str)
{
	char c;
	int number;
	istringstream iss(str);
	iss >> c;
	iss >> number;
	return number;
}

bool stringCompare(const string &left, const string &right){
	int nbL = extractNb(left);
	int nbR = extractNb(right);
	return (nbL < nbR);
}


void labelTrainingData(string globalPath, vector<string> *pathImages, vector<int> *labels)
{
	int label = 0;
	// call the recursive function
	recursSearch(globalPath, pathImages, labels, &label);
}

void recursSearch(string cheminDb, vector<string> *pathImages, vector<int> *labels, int* labelCount)/*Cette fonction recursive analyse les fichiers dans le dossier d'adresse fchemin*/
{
	//current directory explored
	DIR* dir = NULL;
	//current file explored
	struct dirent* fileRead = NULL;
	//path of the file explored
	string pathStr;
	vector<string> fileName;
	// image if file managed to be opened as an image in opencv
	Mat img;

	//CHECK IF THE DIRECTORY CAN BE OPENED
	dir = opendir(cheminDb.c_str());
	if (dir == NULL)
	{
		return;
	}

	//ITERATE THROUGH ALL FILES OF THE DIRECTORY
	while ((fileRead = readdir(dir)) != NULL)
	{
		fileName.push_back(string(fileRead->d_name));
	}
	closedir(dir);

	sort(fileName.begin(), fileName.end(), stringCompare);

	for (int unsigned i = 0; i<fileName.size(); i++){
		//SKIP THE . AND .. FILES (CURRENT, ROOT FILE)
		if (fileName[i].compare(".") == 0 || fileName[i].compare("..") == 0) continue;

		//new path = root path + "/" + name of the file/folder
		pathStr = cheminDb + "/" + fileName[i];

		//TEST IF THE FILE EXPLORED IF A FOLDER
		if (testDir(pathStr) == 0)
		{
			//call the recursive function with an updated label
			(*labelCount)++;
			recursSearch(pathStr, pathImages, labels, labelCount);
		}

		//TRY TO OPEN THE FILE AS AN IMAGE
		else if (!(cv::imread(pathStr, CV_LOAD_IMAGE_COLOR)).empty())
		{
			//if it is an image, update the path and label vector
			(*pathImages).push_back(pathStr);
			(*labels).push_back(*labelCount);
		}

	}
}


int testDir(string pathStr)
{
	const char * path = pathStr.c_str();
	//information about the file/folder
	struct stat info;

	//get the infos about the file/folder
	if (stat(path, &info) == -1)
	{
		return 1;
	}
	//test if the file/folder is a folder
	if (S_ISDIR(info.st_mode)) return 0; //folder
	else return 1;//file
}

void pathsToGreyImages(vector<string> *pathImages, vector<Mat> *images){

	for (int unsigned i = 0; i<(*pathImages).size(); i++){
		(*images).push_back(cv::imread((*pathImages)[i], CV_LOAD_IMAGE_GRAYSCALE));
	}
}

/* EXAMPLE OF USE :
int main(int argc, char *argv[])
{
vector<string> pathImages;
vector<int> labels;
labelTrainingData("../training_set", &pathImages,&labels);

for(int unsigned i=0;i<pathImages.size();i++){
std::cout <<pathImages[i]<<"     "<<labels[i]<<"\n";
}

return 0;
}
*/