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
	int record = 0; // if it's turns true it will start to record files.

	Rect* cut= new Rect(0, 0, 0, 0); // the selection rectangle
	namedWindow("Webcam image", 1);
	setMouseCallback("Webcam image", CallBackFunc, (void*)cut); // mouse callback with rectangle parameter

	int imageCounter = 0;
	bool succesfulSave = false;

	while (1)
	{


		Mat frame, originalFrame; 
		cap.retrieve(frame); 
		cap >> frame; // get a new frame from camera

		if (cut->x + cut->width > frame.size().width || cut->y + cut->height > frame.size().height){
			cut->width = 0;
			cut->height = 0;
		}

		flip(frame, frame, 1); // flip the frame horizontally for better user experience
		frame(*cut).copyTo(originalFrame); // cut the rectangle out
		flip(originalFrame, originalFrame, 1); // flip it back
		
		rectangle(frame,*cut,Scalar(0, 255, 255),1,8); // draw the rectangle to the picture
		line(frame, cv::Point(cut->x + cut->width / 2, cut->y), cv::Point(cut->x + cut->width / 2, cut->y + cut->height),Scalar(0,200,0));
		line(frame, cv::Point(cut->x, cut->y + cut->height*0.4), cv::Point(cut->x + cut->width, cut->y + cut->height*0.4), Scalar(0, 200, 0));
		if (record)
		{
			if (record == 5){
				succesfulSave = imwrite("images/" + std::to_string(++imageCounter) + ".png", originalFrame);
				if (succesfulSave)
					cv::imshow("Last Saved", originalFrame);
			}

			if (succesfulSave)
				putText(frame, "PICTURE RECORDED", Point(50, 100), FONT_HERSHEY_DUPLEX, 1, Scalar(0, 0, 255));
			else
				putText(frame, "SAVE ERROR", Point(50, 100), FONT_HERSHEY_DUPLEX, 1, Scalar(0, 0, 255));			
			record--;

		}
		putText(frame, "Press Space to Capture picture!", Point(20, 20), FONT_HERSHEY_COMPLEX_SMALL, 1, Scalar(0, 255, 255));
		putText(frame, "Press Esc to Exit!", Point(20, 50), FONT_HERSHEY_COMPLEX_SMALL, 1, Scalar(0, 255, 255));
		cv::imshow("Webcam image", frame);
		int Key = waitKey(30);
		if (Key == 27) // escape key
			break; // exit the code
		if (Key == 32) // space key
			record = 5; 
	}
	// the camera will be closed automatically in VideoCapture destructor
	return 0;
}
