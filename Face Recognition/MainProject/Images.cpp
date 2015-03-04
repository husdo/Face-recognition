#include "Images.h"
#include "normalization.h"

std::vector<std::string> Images::getDirectory(std::string path, bool directory/* =0 */){
	std::vector<std::string> ret;
	DIR *dir;
	struct dirent *ent;
	if ((dir = opendir(path.c_str())) != NULL) {
		/* print all the files and directories within directory */
		while ((ent = readdir(dir)) != NULL) {
			if (ent->d_type == DT_DIR){
				//printf("Directory: %s\n", ent->d_name);
				ret.push_back(ent->d_name);
			}
			else if (!directory){
				//printf("%s\n", ent->d_name);
				ret.push_back(path + ent->d_name);
			}


		}
		closedir(dir);
	}
	else {
		/* could not open directory */
		perror("Directory error!");
		return ret;
	}
	return std::vector<std::string>(ret.begin()+2,ret.end());
}


Images::Images(){
	ImgSize = cv::Size(100, 100);
}

Images::Images(std::string path, unsigned int row /* = 100 */, unsigned int col /* = 100 */, bool normalization /* = true */){
	std::cout << "Image reading process started!\n";
	ImgSize = cv::Size(col, row);
	std::vector<std::string> Directories;
	std::vector<std::vector<std::string> > files;
	directory2label["Not recognizable face"] = -1;
	label2directory[-1] = "Not recognizable face";
	Directories = getDirectory(path, 1);
	initiateNormalization(classifiers, "haarcascades/");
	for (unsigned int DirectoryIterator = 0; DirectoryIterator < Directories.size(); ++DirectoryIterator){
		directory2label[Directories[DirectoryIterator]] = DirectoryIterator;
		label2directory[DirectoryIterator] = Directories[DirectoryIterator];
		files.push_back(getDirectory(path + Directories[DirectoryIterator] + "/"));
	}

	for (unsigned int DirectoryIterator = 0; DirectoryIterator < files.size(); ++DirectoryIterator)
	for (unsigned int FileIterator = 0; FileIterator < files[DirectoryIterator].size(); ++FileIterator){
		std::cout << files.size() << "/" << DirectoryIterator+1 << " " << files[DirectoryIterator].size() << "/" << FileIterator+1 << std::endl;
		//std::cout << files[DirectoryIterator][FileIterator] << std::endl;
		cv::Mat tmpImg,normalizedimg;
		tmpImg = cv::imread(files[DirectoryIterator][FileIterator],CV_8SC3);
//		imshow("original image", tmpImg);
		normalize(tmpImg, classifiers, normalizedimg, row);
//		imshow("normalized image", normalizedimg);
//		waitKey();
		resize(tmpImg, tmpImg, ImgSize);
		std::cout << tmpImg.size() << " " << ImgSize;
		if (tmpImg.size() == ImgSize){
			ColorImages.push_back(tmpImg);
			cv::cvtColor(tmpImg, tmpImg, CV_RGBA2GRAY);
			GrayImages.push_back(tmpImg);
			fileNames.push_back(files[DirectoryIterator][FileIterator]);
			labels.push_back(DirectoryIterator);
		}
	}
	std::cout << "Image reading process ended!\n";
}

Images::~Images(){

}

void Images::addImage(cv::Mat& ColorImage, int& label){
	cv::Mat tmpImage;
	ColorImage.copyTo(tmpImage);
	ColorImages.push_back(tmpImage);
	GrayImages.push_back(cv::Mat());
	labels.push_back(label);
}

unsigned int Images::size() const{
	return ColorImages.size();
}

std::vector<cv::Mat> Images::getColorImages() const
{
	return(ColorImages);
}

std::vector<cv::Mat> Images::getGrayImages()
{
	if (this->size() == 0)
		return std::vector<cv::Mat>(0);

	return(GrayImages);
}

std::vector<int> Images::getLabels() const
{
	return(labels);
}

cv::Mat Images::getColorImage(unsigned int i) const{
	if (i >= this->size())
		return cv::Mat();
	return(ColorImages[i]);
}

cv::Mat Images::getGrayImage(unsigned int i){
	if (i >= this->size())
		return cv::Mat();

	return(GrayImages[i]);
}

int Images::getLabel(unsigned int i) const{
	if (i >= this->size())
		return NULL;
	return(labels[i]);
}

int Images::dir2label(std::string dir){
	return (directory2label[dir]);
}

std::string Images::label2dir(int label){
	return (label2directory[label]);
}

std::map<int, std::string> Images::getLabel2Dir() const
{
	return label2directory;
}

std::string Images::getFileName(unsigned int i) const
{
	return fileNames[i];
}