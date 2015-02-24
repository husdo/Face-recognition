#include "webcam.h"

Webcam::Webcam(): videoCapture(0), timer(0), rectangle(0)
{
    videoCapture = new cv::VideoCapture(0);
    if(videoCapture->isOpened()){
        timer = new QTimer(this);
        rectangle = new cv::Rect((int)videoCapture->get(CV_CAP_PROP_FRAME_WIDTH)/2-100,(int)videoCapture->get(CV_CAP_PROP_FRAME_HEIGHT)/2-100,200,200);
        connect(timer, SIGNAL(timeout()), this, SLOT(readImage()));
    }
}

bool Webcam::isOpened() const{
    return videoCapture->isOpened();
}

void Webcam::setImage(cv::Mat image){
    QMutexLocker locker(&mutex);
    img = image;
}

cv::Mat Webcam::getImage() const{
    QMutexLocker locker(&mutex);
    return img;
}

cv::Mat Webcam::getCroppedImage() const{
    cv::Mat img = getImage();
    cv::Mat croppedImg(img,*rectangle);
    return croppedImg;
}

void Webcam::readImage(){
    cv::Mat src;
    videoCapture->read(src);
    cv::rectangle(src,cvPoint(rectangle->x-1,rectangle->y-1),cvPoint(rectangle->x+rectangle->width+1,rectangle->y+rectangle->height+1),cvScalar(0,255,0));
    setImage(src);
    //imshow("output",src);
    emit imageChanged();
}

void Webcam::run(){
    start();
}

void Webcam::start(){
    timer->start(50);
}

void Webcam::stop(){
    timer->stop();
}
