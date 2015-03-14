#ifndef WEBCAM_H
#define WEBCAM_H

#include <QThread>
#include <QMutex>
#include <QTimer>
#include "cv.h"
#include "highgui.h"


class Webcam : public QThread
{
    Q_OBJECT
    public:
        Webcam();
        //getters
        cv::Mat getImage(bool rect=false); // if rect=true display the green rectangle
        cv::Mat getCroppedImage();
        bool isOpened() const;
        void run(); // to start the thread

    private:
        mutable QMutex mutex; // mutex to avoid conflicts
        cv::Mat img; // image retrieve
        cv::VideoCapture* videoCapture; //webcam stream
        QTimer* timer; // timer to retrieve images continously
        cv::Rect* rectangle; // Rectangle to crop the image

        //settter
        void setImage(cv::Mat image);

    private slots:
        void readImage(); // slot called by the timer to retrieve an image from the camera stream

    signals:
        void imageChanged(); //signal to update the main window
};

#endif // WEBCAM_H
