#include "resultdialogbox.h"
#include <QVBoxLayout>

ResultDialogBox::ResultDialogBox(const Images& img, Facial_Recognizer* recognizer, QWidget* parent) : QDialog(parent), image(0), label(0)
{
    //definition of the window
    setWindowFlags(Qt::WindowTitleHint); //title Bar
    setWindowTitle("Recognition"); //title of the window

    //setModal(true); //modality
    //layout
    QVBoxLayout* layout = new QVBoxLayout();
    layout->addWidget(&image,0,Qt::AlignHCenter);
    double c=0;
	std::string result = recognizer->predict(&c,img.getColorImage(0));
    label = new QLabel(QString::fromStdString(result.c_str())+" | confidence level: "+QString::number(c));
    layout->addWidget(label);
    image.showImage(img.getColorImage(0));
    setLayout(layout); //comment
}
