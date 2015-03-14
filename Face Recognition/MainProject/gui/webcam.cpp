#include "webcam.h"

Webcam::Webcam(): videoCapture(0), timer(0), rectangle(0)
{
    videoCapture = new cv::VideoCapture(0); //we try the open the default webcam stream with opencv
    if(videoCapture->isOpened()){
        timer = new QTimer(this); // if opened we initialize the attributes of the class
        rectangle = new cv::Rect((int)videoCapture->get(CV_CAP_PROP_FRAME_WIDTH)/2-150,(int)videoCapture->get(CV_CAP_PROP_FRAME_HEIGHT)/2-150,300,300); // we create a 300x300 centered rectangle
        connect(timer, SIGNAL(timeout()), this, SLOT(readImage()));
    }
}

bool Webcam::isOpened() const{
    return videoCapture->isOpened();
}

void Webcam::setImage(cv::Mat image){
    QMutexLocker locker(&mutex); // we lock the mutex to acces the variable (avoid conflicts)
    img = image; // set the image
}

cv::Mat Webcam::getImage(bool rect){
    QMutexLocker locker(&mutex); // lock the mutex
    cv::Mat copy = img.clone(); //get a copy of the image
    if(rect){ // if the rectangle option is true we display the green rectangle
            cv::rectangle(copy,cvPoint(rectangle->x-1,rectangle->y-1),cvPoint(rectangle->x+rectangle->width+1,rectangle->y+rectangle->height+1),cvScalar(0,255,0));
    }
    return copy;
}

cv::Mat Webcam::getCroppedImage(){
    cv::Mat img = getImage(); // we get the image
    cv::Mat croppedImg(img,*rectangle); // we crop it with the rectangle
    return croppedImg;
}

void Webcam::readImage(){
    cv::Mat src;
    videoCapture->read(src); // retrieve a frame from the camera stream
    setImage(src); // set the frame as attribute
    emit imageChanged(); //emit the signal
}

void Webcam::run(){
    timer->start(50); // set the timer timeout at 50 ms
}
