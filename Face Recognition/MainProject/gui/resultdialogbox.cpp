#include "resultdialogbox.h"
#include <QVBoxLayout>

ResultDialogBox::ResultDialogBox(Webcam* webcam, Facial_Recognizer* recognizer, bool norm, QWidget* parent) : QDialog(parent), image(0), label(0), m_webcam(webcam), normalization(norm), m_recognizer(recognizer)
{
    //definition of the window
    //setWindowFlags(Qt::Window); //title Bar
    setWindowTitle("Recognition"); //title of the window

    //setModal(true); //modality
    //layout
    label = new QLabel("");
    QVBoxLayout* layout = new QVBoxLayout();
    layout->addWidget(label);
    layout->addWidget(&image,0,Qt::AlignHCenter);

    setLayout(layout); //comment
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(takePicture()));
    timer->start(500);
}

void ResultDialogBox::takePicture(){
    double c=0;
    Images imgs;
    int labelImg =0;

   // for(int i=0;i<100;i++){
        cv::Mat img = m_webcam->getCroppedImage();
        imgs.addImage(img,labelImg,100,100,normalization);

        //Gareth - Changed the confidence level to output a percent.
        //I have looked up the confidence number and it seems to be a very rough number of how close the image is from the trained images
        //The lower the number the closer it is so when confidence = 0 it is absolutely the same image as a training image
        //There is no upper limit so I have assigned it to be 11000 at this point as a rough number of when it is not confident at all
        //maths needs work...

        //double percentMax = 100;
        //double cMax = 11000;
        //double percent = percentMax - percentMax / cMax*c;
        //if (percent < 0.0){
        //	percent = 0.0;
        //}

        std::string result = m_recognizer->predict(&c,imgs.getColorImage(0));
        label->setText(QString::fromUtf8(result.c_str())+" | confidence level: "+QString::number(c));
        image.showImage(imgs.getColorImage(0));
    //}
}
