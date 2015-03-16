#include "resultdialogbox.h"
#include <QVBoxLayout>

ResultDialogBox::ResultDialogBox(Webcam* webcam, Facial_Recognizer* recognizer, bool norm, QWidget* parent) : QDialog(parent), image(0), label(0), m_webcam(webcam), normalization(norm), m_recognizer(recognizer)
{
    setWindowTitle("Recognition"); //title of the window
	setMinimumSize(400,150);
    label = new QLabel("");
    // layout of the window
    QVBoxLayout* layout = new QVBoxLayout();
	layout->addWidget(label, 0, Qt::AlignHCenter);
    layout->addWidget(&image,0,Qt::AlignHCenter);
    setLayout(layout);

    // timer to retrieve images every 50 ms
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(takePictures()));
    timer->start(50);
}

void ResultDialogBox::takePictures(){

    double c=0; // confidence level
    Images imgs; // Image
    int labelImg =0; // output label

	if (m_webcam->isOpened()){
		cv::Mat img = m_webcam->getCroppedImage(); // we get only the inside of the rectangle (cropped image)
		imgs.addImage(img, labelImg, 100, 100, normalization); //add the image to the Images object for preprocessing

		std::string result = m_recognizer->predict(&c, imgs.getColorImage(0)); //predicting the output
		label->setText(QString::fromUtf8(result.c_str()) + " | confidence level: " + QString::number(c)); // creating the sentence to be displayed
		image.showImage(imgs.getColorImage(0)); //displaying the image taken
	}
	else{
		label->setText("No webcam detected!");
	}
}
