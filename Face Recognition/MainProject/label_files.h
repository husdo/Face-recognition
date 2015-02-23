#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif//for strcpy

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include <dirent.h> /* iterate through a folder*/
#include <sys/types.h>/* for folders*/
#include <sys/stat.h>/*test if it is a folder or not*/
#include <vector>
#include "opencv2/highgui/highgui.hpp"


using namespace std;
using namespace cv;

/**
* for folder named like s1,s2,s3, extract the number after the first chracter
* @param string : string to extract the number from
* @return integer
*/
int extractNb(const string& str);
/**
* compare two string (compare two numbers extracted from the strings)
* @param left : first string
* @param right : second string
* @see int extractNb(const string& str);
* @return bool according to which number is the biggest
*/
bool stringCompare(const string &left, const string &right);
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
void recursSearch(string globalPath, vector<string> *pathImages, vector<int> *labels, int* labelCount);
/**
* test if the folder related to a path is a folder or file
* @param globalPath : string relating to the folder to analyse ex: "../training_set" (no / at the end)
* @see int stat(const char *path, struct stat *buf) system call to have informations about a file
* @see int S_ISDIR (mode_t m) macro to say from the infos of a file if it is a directory
* @return integer : 0 if it is a directory, 1 otherwise
*/
int testDir(string globalPath);
/**
* give a vector of grayscale image from a vector of paths of images
* @param pathImages : adress of a vector of paths
* @param images : adress of a vector of grayscale images (output)
*/
void pathsToGreyImages(vector<string> *pathImages, vector<Mat> *images);