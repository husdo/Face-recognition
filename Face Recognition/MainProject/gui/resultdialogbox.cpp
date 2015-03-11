#include "resultdialogbox.h"
#include <QVBoxLayout>

ResultDialogBox::ResultDialogBox(const Images& img, Facial_Recognizer* recognizer, QWidget* parent) : QDialog(parent), image(0), label(0)
{
    //definition of the window
    //setWindowFlags(Qt::Window); //title Bar
    setWindowTitle("Recognition"); //title of the window

    //setModal(true); //modality
    //layout
    QVBoxLayout* layout = new QVBoxLayout();
    layout->addWidget(&image,0,Qt::AlignHCenter);
    double c=0;
	std::string result = recognizer->predict(&c,img.getColorImage(0));

	//Gareth - Changed the confidence level to output a percent.
	//I have looked up the confidence number and it seems to be a very rough number of how close the image is from the trained images
	//The lower the number the closer it is so when confidence = 0 it is absolutely the same image as a training image
	//There is no upper limit so I have assigned it to be 11000 at this point as a rough number of when it is not confident at all

	double percentMax = 100;
	double cMax = 11000;
	double percent = percentMax - percentMax / cMax*c;
	if (percent < 0.0){
		percent = 0.0;
	}

    label = new QLabel(QString::fromUtf8(result.c_str())+" | confidence level: "+QString::number(percent,'f',1)+"%");
    layout->addWidget(label);
    image.showImage(img.getColorImage(0));
    setLayout(layout); //comment
}
