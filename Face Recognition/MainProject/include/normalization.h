#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>

using namespace std;
using namespace cv;

/** Function Headers */

static bool vectCompX(const Rect &a, const Rect &b);
static bool vectCompWidth(const Rect &a, const Rect &b);
static bool vectCompHeight(const Rect &a, const Rect &b);

Mat rotate(cv::Mat& src, double angle);

bool initiateNormalization(std::vector<CascadeClassifier>& classifiers, string pathToHaar);
bool normalize(Mat& img, std::vector<CascadeClassifier>& classifiers, Mat& normalized_img,int imageWidth);
bool detectFace(Mat& frame, Rect& face, CascadeClassifier& face_cascade);
bool detectEyes(Mat& frame, Rect& face, Rect& r_eye, Rect& l_eye, CascadeClassifier& right_eye_cascade, CascadeClassifier& left_eye_cascade);
bool detectVertic(Mat& frame, Rect& face, Rect& nose, Rect& mouth, CascadeClassifier& nose_cascade, CascadeClassifier& mouth_cascade);

