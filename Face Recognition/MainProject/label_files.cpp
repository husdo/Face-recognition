
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include <dirent.h> /* iterate through a folder*/
#include <sys/types.h>/* for folders*/
#include <sys/stat.h>/*test if it is a folder or not*/
#include <vector>
#include "opencv2/highgui/highgui.hpp"
#include "label_files.h"

using namespace std;
using namespace cv;


void labelTrainingData(string globalPath, vector<string> *pathImages, vector<int> *labels)
{
	int label=0;
	// call the recursive function
	recursSearch(globalPath, pathImages,labels,&label);
}

void recursSearch(string cheminDb, vector<string> *pathImages, vector<int> *labels,int* labelCount)/*Cette fonction recursive analyse les fichiers dans le dossier d'adresse fchemin*/
{
	//current directory explored
	DIR* dir = NULL;
	//current file explored
	struct dirent* fichierLu = NULL;
	//path of the file explored
	string pathStr;
	// image if file managed to be opened as an image in opencv
	Mat img;

	//CHECK IF THE DIRECTORY CAN BE OPENED
	dir = opendir(cheminDb.c_str());
	if (dir == NULL)
	{
		return;
	}

	//ITERATE THROUGH ALL FILES OF THE DIRECTORY
	while ((fichierLu = readdir(dir)) !=NULL)
	{
	//SKIP THE . AND .. FILES (CURRENT, ROOT FILE)
	if (strcmp(fichierLu->d_name,".") == 0 || strcmp(fichierLu->d_name, "..") == 0) continue;

	//new path = root path + "/" + name of the file/folder
	pathStr=cheminDb+"/"+string(fichierLu->d_name);

	//TEST IF THE FILE EXPLORED IF A FOLDER
	if (testDir(pathStr) == 0)
	{
		//call the recursive function with an updated label
		(*labelCount)++;
		recursSearch (pathStr,pathImages,labels,labelCount);
	}

	//TRY TO OPEN THE FILE AS AN IMAGE
	else if (!(cv::imread(pathStr, CV_LOAD_IMAGE_COLOR)).empty())
			{
			//if it is an image, update the path and label vector
				(*pathImages).push_back(pathStr);
				(*labels).push_back(*labelCount);
			}

	}

	closedir(dir);
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

void saveMap(std::string path, std::map<int, std::string> map){
	//file in which we will insert all informations
	std::ofstream mapFile;
	//itrator which will go through each pair of label+name of file
	std::map<int, std::string>::iterator iter;
	//we'll write in classifier folder in a csv file (easier to read)
	mapFile.open(path.c_str());

	//for each couple we'll write
	for (iter = map.begin(); iter != map.end(); ++iter) {
		//label number
		mapFile << iter->first;
		//end of cell
		mapFile << ";";
		//name of file
		mapFile << iter->second;
		mapFile << "\n";
	}
	mapFile.close();
}

std::map<int, std::string> readMapFile(std::string path){
	//try to read the concerned file
	struct stat buffer;
	if (!(stat(path.c_str(), &buffer) == 0))
		perror("map file does not exist (readMapFile in label_files.cpp)");

	//map linking label to file name
	std::map<int, std::string> label2directory;
	//file to read
	std::ifstream map(path.c_str());
	//line buffer to read the file
	std::string line;

	while (getline(map, line))
	{
		std::istringstream ss(line);
		std::string dir;
		char c;
		int label;
		//line = label +';' + name_of_file
		ss >> label >> c >> dir;
		//update of the map
		label2directory[label] = dir;
	}
	return label2directory;
}