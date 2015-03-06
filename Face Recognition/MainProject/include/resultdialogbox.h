#ifndef RESULTDIALOGBOX_H
#define RESULTDIALOGBOX_H

#include <QDialog>
#include "cvwidget.h"
#include "cv.h"
#include "Facial_Recognizer.h"

class ResultDialogBox : public QDialog
{
    public:
        ResultDialogBox(const Images& img, Facial_Recognizer* recognizer, QWidget* parent=0);
    private:
        CVWidget image;
        QLabel* label;
};

#endif // RESULTDIALOGBOX_H
