#ifndef RESULTDIALOGBOX_H
#define RESULTDIALOGBOX_H

#include <QDialog>
#include <QTimer>
#include "cvwidget.h"
#include "webcam.h"
#include "cv.h"
#include "Facial_Recognizer.h"

class ResultDialogBox : public QDialog
{
    Q_OBJECT

    public:
        ResultDialogBox(Webcam* webcam, Facial_Recognizer* recognizer, bool norm, QWidget* parent=0);
    private:
        CVWidget image; // image taken
        QLabel* label; // text to be displayed (name of the person and confidence level)
        Webcam* m_webcam; //webcam object to take a picture
        Facial_Recognizer* m_recognizer; //recognizer to predict
        bool normalization;  //normalization enabled or not
        QTimer* timer; // timer to make a live

    private slots:
        void takePictures(); // function called by the timer
};

#endif // RESULTDIALOGBOX_H
