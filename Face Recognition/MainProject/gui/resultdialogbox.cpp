#include "resultdialogbox.h"
#include <QVBoxLayout>

ResultDialogBox::ResultDialogBox(cv::Mat img, Facial_Recognizer* recognizer, QWidget* parent) : QDialog(parent), image(0), label(0)
{
    //definition of the window
    setWindowFlags(Qt::WindowTitleHint); //title Bar
    setWindowTitle("Recognition"); //title of the window

    //setModal(true); //modality
    //layout
    QVBoxLayout* layout = new QVBoxLayout();
    layout->addWidget(&image);
    double c;
	std::string result = recognizer->predict(&c,img);
    label = new QLabel(QString::fromStdString(result));
    layout->addWidget(label);
    image.showImage(img);
    setLayout(layout);
}
