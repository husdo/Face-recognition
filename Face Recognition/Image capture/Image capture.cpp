#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <iostream>
#include <ctime>
#include <stdio.h>

using namespace std;
using namespace cv;


void CallBackFunc(int event, int x, int y, int flags, void* userdata)
{
	
	if (event == EVENT_LBUTTONDOWN) // Left button press
	{
		((Rect*)userdata)->x = x;
		((Rect*)userdata)->y = y;
		((Rect*)userdata)->width = 0;
		((Rect*)userdata)->height = 0;
	}
	else if (flags == EVENT_FLAG_LBUTTON) // left button hold
	{
		if (x > ((Rect*)userdata)->x){
			((Rect*)userdata)->width = x - ((Rect*)userdata)->x;
			((Rect*)userdata)->height = ((Rect*)userdata)->width;
		}
		else{
			((Rect*)userdata)->width = 0;
			((Rect*)userdata)->height = 0;
		}
	}
}

int main(int argc, const char** argv)
{
	VideoCapture cap(0); // open the default camera
	if (!cap.isOpened())  // check if we succeeded
		return -1;
	bool record = false; // if it's turns true it will start to record files.

	Rect* cut= new Rect(0, 0, 0, 0); // the selection rectangle
	namedWindow("Webcam image", 1);
	setMouseCallback("Webcam image", CallBackFunc, (void*)cut); // mouse callback with rectangle parameter

	int imageCounter = 0;

	while (1)
	{
		Mat frame, originalFrame; 
		cap.retrieve(frame); 
		cap >> frame; // get a new frame from camera
		flip(frame, frame, 1); // flip the frame horizontally for better user experience
		frame(*cut).copyTo(originalFrame); // cut the rectangle out
		flip(originalFrame, originalFrame, 1); // flip it back
		
		rectangle(frame,*cut,Scalar(0, 255, 255),1,8); // draw the rectangle to the picture
		if (record)
		{
			putText(frame, "RECORD", Point(50, 50), FONT_HERSHEY_DUPLEX, 1, Scalar(0, 0, 255));
			imwrite("images/"+std::to_string(++imageCounter)+".png",originalFrame);
		}
		imshow("Webcam image", frame);
		int Key = waitKey(30);
		if (Key == 27) // escape key
			break; // exit the code
		if (Key == 32) // space key
			record = !record; // start / stop recording
	}
	// the camera will be closed automatically in VideoCapture destructor
	return 0;
}
