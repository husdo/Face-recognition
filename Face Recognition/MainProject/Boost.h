#include "Facial_Recognizer.h"
//#include <cv.h>

//#include "label_files.h"
#include "opencv2/core/core.hpp"
#include "opencv2/contrib/contrib.hpp"
#include "opencv2/highgui/highgui.hpp"


using namespace cv;

class Boost :
	public Facial_Recognizer
{
private:
	Ptr<FaceRecognizer> Recognizer;


public:

	Boost();
	~Boost();

	//void one();
	void training(Images& InputImages);
	virtual std::string predict(double* confidence, const cv::Mat& InputImage) = 0;
	virtual void save(std::string path = "") const = 0; // if the path is empty you should save the classifier next to the exe with your own file format
	virtual void load(std::string path = "") = 0; // if the path is empty you should load the classifier from the default place.
	
};