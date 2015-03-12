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
        CVWidget image;
        QLabel* label;
        Webcam* m_webcam;
        Facial_Recognizer* m_recognizer;
        bool normalization;
        QTimer* timer;

    private slots:
        void takePicture();
};

#endif // RESULTDIALOGBOX_H
