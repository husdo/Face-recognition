#include "Facial_Recognizer.h"
#include "opencv2/core/core.hpp"
#include "opencv2/contrib/contrib.hpp"
#include "opencv2/highgui/highgui.hpp"

using namespace std;
using namespace cv;

class FisherFaces : public Facial_Recognizer
{
private:
	Ptr<FaceRecognizer> model;

public:
	FisherFaces();
	FisherFaces(int nbComponents, double recognitionThreshold);
	~FisherFaces();

	void training(Images& InputImages);
	string predict(double *confidence, const Mat& InputImages);
	void save(std::string path = "") const;
	void load(std::string path = "");
};