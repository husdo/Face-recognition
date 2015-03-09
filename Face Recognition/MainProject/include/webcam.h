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
        cv::Mat getImage(bool rect=false);
        cv::Mat getCroppedImage();
        bool isOpened() const;
        void run();
        void start();
        void stop();
    protected:
    private:
        mutable QMutex mutex;
        cv::Mat img;
        cv::VideoCapture* videoCapture;
        QTimer* timer;
        cv::Rect* rectangle;

        void setImage(cv::Mat image);

    private slots:
        void readImage();

    signals:
        void imageChanged();
};

#endif // WEBCAM_H
