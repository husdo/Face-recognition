//#pragma once
#ifndef IMAGES_H
#define IMAGES_H

#include "cv.h"
#include "dirent.h"
#include <vector>

class Images
{
private:
	std::vector<cv::Mat> ColorImages;
	std::vector<cv::Mat> GrayImages;
	std::vector<int> labels;
	std::vector<std::string> fileNames;
	std::map<std::string, int> directory2label;
	std::map<int, std::string> label2directory;
	cv::Size ImgSize;
	std::vector<std::string> getDirectory(std::string path, bool directory=0);
	std::vector<cv::CascadeClassifier> classifiers;



public:
	Images();
	Images(std::string path,unsigned int row = 100, unsigned int col = 100, bool normalization = true);
	~Images();

	void addImage(cv::Mat& ColorImage, int& label, unsigned int row = 100, unsigned int col = 100 , bool normalization = true );
	unsigned int size() const;

	std::vector<cv::Mat> getColorImages() const;
	std::vector<cv::Mat> getGrayImages();
	std::vector<int> getLabels() const;

	cv::Mat getColorImage(unsigned int i) const;
	cv::Mat getGrayImage(unsigned int i);
	int getLabel(unsigned int i) const;
	std::string getFileName(unsigned int i) const;

	int dir2label(std::string dir);
	std::string label2dir(int label);

	std::map<int, std::string> getLabel2Dir() const;
};

#endif
