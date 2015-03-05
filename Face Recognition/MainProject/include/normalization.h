#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>

using namespace std;
using namespace cv;


/**
* compare two rectangles according to their x coordinates (true if b is the most on the right)
* @param a : first rectangle
* @param b : second rectangle
* @return boolean : true if b is the most on the right
*/
static bool vectCompRightEye(const Rect &a, const Rect &b);
static bool vectCompLeftEye(const Rect &a, const Rect &b);
/**
* compare two rectangles according to their width (true if b has the biggest width)
* @param a : first rectangle
* @param b : second rectangle
* @return boolean : true if b has the biggest width
*/
static bool vectCompWidth(const Rect &a, const Rect &b);
/**
* compare two rectangles according to their height (true if b has the biggest height)
* @param a : first rectangle
* @param b : second rectangle
* @return boolean : true if b has the biggest height
*/
static bool vectCompHeight(const Rect &a, const Rect &b);
/**
* rotate an image around its center
* source : https://opencv-code.com/quick-tips/how-to-rotate-image-in-opencv/
* @param src : Image to rotate
* @param angle : double. angle is in degree
* @see https://opencv-code.com/quick-tips/how-to-rotate-image-in-opencv/
* @return rotated image
*/
Mat rotate(cv::Mat& src, double angle);
/**
* load the classifiers (opencv xml files. Right eye, left eye, nose, mouth and face.) from a path to the folder
* @param path : path to the folder containing xml files
* @param classifiers : classifiers. vector of all classifiers loaded to use in normalize
* @return boolean. True if everything went well, false otherwise.
*/
bool initiateNormalization(std::vector<CascadeClassifier>& classifiers, string pathToHaar);
/**
* normalize an image (scale and rotation)
* test if there is a face (rotate from 0 to 60° : 0,+10,-10,+20,-20...)
* test if it detects 2 eyes => remember the angle to align them
* test if it detects middle of the nose and mouth => remember the angle to align them
* rotate the image (step angle of 10° + the angle to align parts of the face)
* scale the image as a square of the specified length
* @param img : Image to normalize
* @param classifiers : classifiers from xml file (trained Haar analysers)
* @param normalized_img : is the returned normalized image
* @param imageWidth : Width of the square image at the end 
* @see void resize(InputArray src, OutputArray dst, Size dsize, double fx=0, double fy=0, int interpolation=INTER_LINEAR )
* @see bool initiateNormalization(std::vector<CascadeClassifier>& classifiers, string pathToHaar);
* @return true if a face was detected in the image, false otherwise
*/
bool normalize(Mat& img, std::vector<CascadeClassifier>& classifiers, Mat& normalized_img,int imageWidth);
/**
* normalize only with the nose and mouth
* @see bool normalize(Mat& img, std::vector<CascadeClassifier>& classifiers, Mat& normalized_img,int imageWidth);
*/
bool quickNormalize(Mat& img, std::vector<CascadeClassifier>& classifiers, Mat& normalized_img, int imageWidth);
/**
* detect the face in the image
* @param frame : the image to analyze
* @param face : rectangle (output) in which is the face
* @param face_cascade : cascade based on Haar
* @see haarcascade_frontalface_alt.xml 
* @return true if a face was detected in the image, false otherwise
*/
bool detectFace(Mat& frame, Rect& face, CascadeClassifier& face_cascade);
/**
* detect the Eyes in the face with Haar method (training made by opencv)
* @param frame : the image to analyze
* @param face : rectangle (input) in which is the face (where we will search the eyes)
* @param r_eye : rectangle (output) in which is the right eye
* @param l_eye : rectangle (output) in which is the left eye
* @param right_eye_cascade : cascade based on Haar
* @param left_eye_cascade : cascade based on Haar
* @see haarcascade_mcs_righteye.xml
* @see haarcascade_mcs_lefteye.xml
* @return true if 2 eyes were detected in the image, false otherwise
*/
bool detectEyes(Mat& frame, Rect& face, Rect& r_eye, Rect& l_eye, CascadeClassifier& right_eye_cascade, CascadeClassifier& left_eye_cascade);
/**
* detect the nose and mouth of the face with Haar method (training made by opencv)
* @param frame : the image to analyze
* @param classifiers : classifiers from xml file (trained Haar analysers)
* @param face : rectangle (input) in which is the face (where we will search the eyes)
* @param nose : rectangle (output) in which is the nose
* @param mouth : rectangle (output) in which is the mouth
* @see haarcascade_mcs_nose.xml
* @see haarcascade_mcs_mouth.xml
* @return true if a the nose and mouth were detected in the image, false otherwise
*/
bool detectVertic(Mat& frame, Rect& face, Rect& nose, Rect& mouth, CascadeClassifier& nose_cascade, CascadeClassifier& mouth_cascade);

