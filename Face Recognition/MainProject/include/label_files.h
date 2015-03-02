
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include <dirent.h> /* iterate through a folder*/
#include <sys/types.h>/* for folders*/
#include <sys/stat.h>/*test if it is a folder or not*/
#include <vector>
#include "opencv2/highgui/highgui.hpp"
#include<iomanip>
#include<fstream>

using namespace std;
using namespace cv;

/**
* call the recursive function to iterate through the training images
* change the pathImage vector : relative path from visual studio to the image
* change the label (one label for each folder). 
* ith image has pathImage[i] for path and labels[i] label
* @param globalPath : string relating to the folder to analyse ex: "../training_set" (no / at the end)
* @param pathImages pointer to a vector of string containing the paths of images
* @param labels pointer to a vector of int containing the label : one label for each folder
* @see void recursSearch(string fchemin, vector<string> *pathImages, vector<int> *labels,int* labelCount)
*/
void labelTrainingData(string globalPath, vector<string> *pathImages, vector<int> *labels);
/**
* recursive function used :
* for each path it will for each file of the folder
* - check if the path can be accessed
*      - yes : continue
*      - no : stop the recursion
* - check if the name is . or .. , if so, skip
* - check if the file is a folder
*      - yes : call the recursion function on this new path + increase the label number
*      - no : continue
* - try to open it as an image in opencv
*      - yes : add its path to the vector<string>pathImage, add the current label to vector<int> labels
*      - no : continue
* @param globalPath : string relating to the folder to analyse ex: "../training_set" (no / at the end)
* @param pathImages pointer to a vector of string containing the paths of images
* @param labelcount is the current number of label
* @param labels pointer to a vector of int containing the label : one label for each folder
* @see int testDir(string fchemin);
*/
void recursSearch(string globalPath, vector<string> *pathImages, vector<int> *labels,int* labelCount);
/**
* test if the folder related to a path is a folder or file
* @param globalPath : string relating to the folder to analyse ex: "../training_set" (no / at the end)
* @see int stat(const char *path, struct stat *buf) system call to have informations about a file
* @see int S_ISDIR (mode_t m) macro to say from the infos of a file if it is a directory
* @return integer : 0 if it is a directory, 1 otherwise
*/
int testDir(string globalPath);

/**
* save the map in a .csv file linking labels and files name
* @param path : path to the csv file
* @param map : map to save
*/
void saveMap(std::string path, std::map<int, std::string> map);
/**
* load a map std::map<int, std::string> from a .csv file
* @param path : path to the csv file containing the informations on the map
* @return map
*/
std::map<int, std::string> readMapFile(std::string path);