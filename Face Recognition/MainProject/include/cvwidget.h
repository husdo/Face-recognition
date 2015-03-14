/* The code used was found at: http://develnoter.blogspot.co.uk/2012/05/integrating-opencv-in-qt-gui.html
    The class is used to display an OpenCV Image (cv::Mat) into a Qt widget (QWidget).
    It is a link between the OpenCV and Qt librairies

*/

#ifndef CVWIDGET_H
#define CVWIDGET_H

#include <QtWidgets>
#include <opencv2/opencv.hpp>

class CVWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CVWidget(QWidget *parent = 0) : QWidget(parent) {}

    QSize sizeHint() const { return _qimage.size(); }
    QSize minimumSizeHint() const { return _qimage.size(); }

public slots:

    void showImage(const cv::Mat& image) {
        // Convert the image to the RGB888 format
        switch (image.type()) {
        case CV_8UC1:
            cvtColor(image, _tmp, CV_GRAY2RGB);
            break;
        case CV_8UC3:
            cvtColor(image, _tmp, CV_BGR2RGB);
            break;
        }

        // QImage needs the data to be stored continuously in memory
        assert(_tmp.isContinuous());
        // Assign OpenCV's image buffer to the QImage. Note that the bytesPerLine parameter
        // (http://qt-project.org/doc/qt-4.8/qimage.html#QImage-6) is 3*width because each pixel
        // has three bytes.
        _qimage = QImage(_tmp.data, _tmp.cols, _tmp.rows, _tmp.cols*3, QImage::Format_RGB888);

        this->setFixedSize(image.cols, image.rows);

        repaint();
    }

protected:
    void paintEvent(QPaintEvent* /*event*/) {
        // Display the image
        QPainter painter(this);
        painter.drawImage(QPoint(0,0), _qimage);
        painter.end();
    }

    QImage _qimage;
    cv::Mat _tmp;
};


#endif // CVWIDGET_H
