#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "normalization.h"
#include <iostream>
#include <stdio.h>

using namespace std;
using namespace cv;

//EXAMPLE OF USE
/*int main(int argc, const char** argv)
{
	Mat img, normalizedimg;
	std::vector<CascadeClassifier> classifiers;
	if ((img = cv::imread("carole.jpg", CV_LOAD_IMAGE_COLOR)).empty()){ printf("--(!)Error loading\n"); return -1; };
	initiateNormalization(classifiers, "haarcascades/");
	normalize(img, classifiers, normalizedimg, 100);
	cv::imshow("color", normalizedimg);
	cv::waitKey(10);
	return 0;
}*/


bool vectCompRightEye(const Rect &a, const Rect &b) {
	if (abs(a.x - b.x)>max(a.x, b.x)*0.4)
		return (a.x < b.x);
	else
		return (a.y < b.y);
}
bool vectCompLeftEye(const Rect &a, const Rect &b) {
	if (abs(a.x - b.x)>max(a.x, b.x)*0.4)
		return (a.x > b.x);
	else
		return (a.y < b.y);
}
bool vectCompWidth(const Rect &a, const Rect &b) {
	//which rectangle is the widest?
	return (a.width < b.width);
}
bool vectCompHeight(const Rect &a, const Rect &b) {
	//which rectangle is the tallest?
	return (a.height < b.height);
}

Mat rotate(cv::Mat& src, double angle)
{
	//output image
	Mat dst;
	int len = std::max(src.cols, src.rows);
	//center of rotation
	Point2f pt(float(len / 2.), float(len / 2.));
	//get the rotation matrix
	Mat r = cv::getRotationMatrix2D(pt, angle, 1.0);
	//apply the rotation
	warpAffine(src, dst, r, cv::Size(len, len));
	return dst;
}

bool initiateNormalization(std::vector<CascadeClassifier>& classifiers, string pathToHaar){
	CascadeClassifier current;
	//face
	if (!current.load(pathToHaar + "haarcascade_frontalface_alt.xml")){ printf("--(!)Error loading\n"); return false; };
	classifiers.push_back(current);
	//r_eye
	if (!current.load(pathToHaar + "haarcascade_mcs_righteye.xml")){ printf("--(!)Error loading\n"); return false; };
	classifiers.push_back(current);
	//l_eye
	if (!current.load(pathToHaar + "haarcascade_mcs_lefteye.xml")){ printf("--(!)Error loading\n"); return false; };
	classifiers.push_back(current);
	//nose
	if (!current.load(pathToHaar + "haarcascade_mcs_nose.xml")){ printf("--(!)Error loading\n"); return false; };
	classifiers.push_back(current);
	//mouth
	if (!current.load(pathToHaar + "haarcascade_mcs_mouth.xml")){ printf("--(!)Error loading\n"); return false; };
	classifiers.push_back(current);
	return true;
}
bool normalize(Mat& img, std::vector<CascadeClassifier>& classifiers, Mat& rotatedimg,int imageWidth){
	Mat gray_img;
	Mat rotatedgray_img;
	Rect face, l_eye, r_eye, nose, mouth;
	bool detectedFace = false, detectedEyes = false, detectedVertic = false;
//rough angle (step of 10° to detect the face, Haar is sensible to rotation)
	double angle = 0, dx, dy;
//angle added to align the eyes, or the nose and the mouth
	double angleDetail = 0;
//get a grey image
	cvtColor(img, gray_img, CV_BGR2GRAY);
	equalizeHist(gray_img, gray_img);

	if (detectFace(gray_img, face, classifiers[0])){
		detectedFace = true;
		if (!(detectedEyes = detectEyes(gray_img, face, r_eye, l_eye, classifiers[1], classifiers[2])))
			(detectedVertic = detectVertic(gray_img, face, nose, mouth, classifiers[3], classifiers[4]));
	}

// DO rough rotation until a complete face is detected (eyes+face or mouth and nose +face)
	if ((!detectedEyes) && (!detectedVertic)){
		for (double i = 5; i < 30; i += 8){
			//rotate the initial gray image
			rotatedgray_img = rotate(gray_img, i);
			//rotate in one way
			if (detectFace(rotatedgray_img, face, classifiers[0])){
				detectedFace = true;
				angle = i;
				if ((detectedEyes = detectEyes(rotatedgray_img, face, r_eye, l_eye, classifiers[1], classifiers[2])) ||
					(detectedVertic = detectVertic(rotatedgray_img, face, nose, mouth, classifiers[3], classifiers[4])))
					break;
			}
			//rotate the other way
			rotatedgray_img = rotate(gray_img, -i);
			if (detectFace(rotatedgray_img, face, classifiers[0])){
				detectedFace = true;
				angle = -i;
				if ((detectedEyes = detectEyes(rotatedgray_img, face, r_eye, l_eye, classifiers[1], classifiers[2])) ||
					(detectedVertic = detectVertic(rotatedgray_img, face, nose, mouth, classifiers[3], classifiers[4])))
					break;
			}
		}
	}

	if (detectedFace){
		if (detectedEyes){
			//angle between the eyes line and the horizontal line
			dy = ((r_eye.y + double(r_eye.height / 2.0)) - (l_eye.y + double(l_eye.height / 2.0)));
			dx = (r_eye.x + double(r_eye.width / 2.0)) - (l_eye.x + double(l_eye.width / 2.0));
			if (abs(dx) > abs(dy))
				angleDetail = atan(dy / dx);
		}
		else if (detectedVertic){
			//angle between the nose and mouth line and the vertical line
			dy = ((nose.y + double(nose.height / 2.0)) - (mouth.y + double(mouth.height / 2.0)));
			dx = (nose.x + double(nose.width / 2.0)) - (mouth.x + double(mouth.width / 2.0));
			if (abs(dy) > abs(dx))
				angleDetail = -atan(dx / dy);
		}

		rotatedgray_img = rotate(gray_img, angle + angleDetail * 180 / 3.14);
		detectFace(rotatedgray_img, face, classifiers[0]);
		rotatedimg = rotate(img, angle + angleDetail * 180 / 3.14);
		/*face.x = face.x + int(0.1*face.width);
		face.width = int(0.8*face.width);
		face.y = face.y + int(0.15*face.height);
		face.height = int(0.8*face.height);*/
		rotatedimg(face).copyTo(rotatedimg);
		return true;
	}
	else // no face detected
		return false;
}
bool quickNormalize(Mat& img, std::vector<CascadeClassifier>& classifiers, Mat& rotatedimg, int imageWidth){
	Mat gray_img;
	Mat rotatedgray_img;
	Rect face, nose, mouth;
	bool detectedFace = false, detectedVertic = false;
	double angle = 0, dx, dy;
	double angleDetail = 0;

	cvtColor(img, gray_img, CV_BGR2GRAY);
	equalizeHist(gray_img, gray_img);

	if (detectFace(gray_img, face, classifiers[0])){
		detectedFace = true;
		(detectedVertic = detectVertic(gray_img, face, nose, mouth, classifiers[3], classifiers[4]));
	}
	if (!detectedVertic){
		for (double i = 5; i < 30; i += 8){
			rotatedgray_img = rotate(gray_img, i);
			if (detectFace(rotatedgray_img, face, classifiers[0])){
				detectedFace = true;
				angle = i;
				if ((detectedVertic = detectVertic(rotatedgray_img, face, nose, mouth, classifiers[3], classifiers[4])))
					break;
			}
			rotatedgray_img = rotate(gray_img, -i);
			if (detectFace(rotatedgray_img, face, classifiers[0])){
				detectedFace = true;
				angle = -i;
				if ((detectedVertic = detectVertic(rotatedgray_img, face, nose, mouth, classifiers[3], classifiers[4])))
					break;
			}
		}
	}
	if (detectedFace){
		if (detectedVertic){
			dy = ((nose.y + double(nose.height / 2.0)) - (mouth.y + double(mouth.height / 2.0)));
			dx = (nose.x + double(nose.width / 2.0)) - (mouth.x + double(mouth.width / 2.0));
			if (abs(dy) > abs(dx))
				angleDetail = -atan(dx / dy);
		}
		rotatedgray_img = rotate(gray_img, angle + angleDetail * 180 / 3.14);
		rotatedimg = rotate(img, angle + angleDetail * 180 / 3.14);
		detectFace(rotatedgray_img, face, classifiers[0]);
		/*face.x = face.x + int(0.1*face.width);
		face.width = int(0.8*face.width);
		face.y = face.y + int(0.15*face.height);
		face.height = int(0.8*face.height);*/
		rotatedimg(face).copyTo(rotatedimg);
		return true;
	}
	else
		return false;
}
bool detectFace(Mat& frame, Rect& face, CascadeClassifier& face_cascade)
{
	std::vector<Rect> faces;

	//-- Detect faces
	face_cascade.detectMultiScale(frame, faces, 1.1, 2, 0 | CV_HAAR_SCALE_IMAGE, Size(30, 30));

	for (size_t i = 0; i < faces.size(); i++)
	{
		/*Point center(int(faces[i].x + faces[i].width*0.5),int( faces[i].y + faces[i].height*0.5));
		ellipse(frame, center, Size(int(faces[i].width*0.5),int( faces[i].height*0.5)), 0, 0, 360, Scalar(0, 0, 255), 4, 8, 0);*/
		face = faces[0];
		return true;
	}
	return false;
}
bool detectEyes(Mat& frame, Rect& face, Rect& r_eye, Rect& l_eye, CascadeClassifier& right_eye_cascade, CascadeClassifier& left_eye_cascade){
	Rect eyeZone(face);
	eyeZone.height = int(eyeZone.height*0.81);
	Mat eyeROI = frame(eyeZone);
	std::vector<Rect> right_eye;
	std::vector<Rect> left_eye;


	right_eye_cascade.detectMultiScale(eyeROI, right_eye, 1.1, 2, 0 | CV_HAAR_SCALE_IMAGE, Size(30, 30));

	if (right_eye.size() > 0){
		//be sure to get the right eye (not get the left eye
		r_eye = *std::max_element(right_eye.begin(), right_eye.end(), vectCompRightEye);
		/*Point center(int(face.x + r_eye.x + r_eye.width*0.5), int(face.y + r_eye.y + r_eye.height*0.5));
		int radius = cvRound((r_eye.width + r_eye.height)*0.25);
		circle(frame, center, radius, Scalar(0, 0, 255), 4, 8, 0);*/
	}
	else
		return false;

	left_eye_cascade.detectMultiScale(eyeROI, left_eye, 1.1, 2, 0 | CV_HAAR_SCALE_IMAGE, Size(30, 30));

	if (left_eye.size() > 0){
		l_eye = *std::max_element(left_eye.begin(), left_eye.end(), vectCompLeftEye);
		/*Point center(int(face.x + l_eye.x + l_eye.width*0.5), int(face.y + l_eye.y + l_eye.height*0.5));
		int radius = cvRound((l_eye.width + l_eye.height)*0.25);
		circle(frame, center, radius, Scalar(0, 0, 255), 4, 8, 0);*/
		return true;
	}
	return false;
}

bool detectVertic(Mat& frame, Rect& face, Rect& nose, Rect& mouth, CascadeClassifier& nose_cascade, CascadeClassifier& mouth_cascade){
	Mat faceROI = frame(face);
	std::vector<Rect> noses;
	std::vector<Rect> mouths;
	Rect mouthZone(face);
	int skipHeight = int(mouthZone.height*0.5);
	mouthZone.y = mouthZone.y + skipHeight;
	mouthZone.height = mouthZone.height - skipHeight;
	Mat mouthROI = frame(mouthZone);

	nose_cascade.detectMultiScale(faceROI, noses, 1.1, 2, 0 | CV_HAAR_SCALE_IMAGE, Size(30, 30));

	if (noses.size() > 0)
	{
		//keep the tallest element
		nose = *std::max_element(noses.begin(), noses.end(), vectCompHeight);
		/*Point ptA(int(face.x + nose.x + nose.width*0.5), int(face.y + nose.y - nose.height*0.5));
		Point ptB(int(face.x + nose.x + nose.width*0.5), int(face.y + nose.y + nose.height*0.5));
		line(frame, ptA, ptB, Scalar(0, 0, 255), 4, 8, 0);*/
	}
	else
		return false;

	mouth_cascade.detectMultiScale(mouthROI, mouths, 1.1, 2, 0 | CV_HAAR_SCALE_IMAGE, Size(30, 30));

	if (mouths.size() > 0){
		//keep the widest element
		mouth = *std::max_element(mouths.begin(), mouths.end(), vectCompWidth);
		mouth.y = mouth.y + skipHeight;
		/*Point center(int(face.x + mouth.x + mouth.width*0.5), int(face.y + mouth.y + mouth.height*0.5));
		int radius = cvRound((mouth.width + mouth.height)*0.25);
		circle(frame, center, radius, Scalar(0, 0, 255), 4, 8, 0);*/
		return true;
	}
	return false;
}




