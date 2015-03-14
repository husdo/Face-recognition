#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets>
#include <QVBoxLayout>
#include <vector>

#include "Facial_Recognizer.h"
#include "cvwidget.h"
#include "webcam.h"

using namespace std;


class MainWindow : public QMainWindow
{
    Q_OBJECT

    public:
        MainWindow(QWidget* parent=0);
        void printMsg(QString msg);

    private:
        //GUI attributes
        QVBoxLayout* mainLayout;
        QWidget* mainWidget;
        CVWidget* cvWidget;
        QLineEdit* training_path;
        QLineEdit* validation_path;
        QComboBox* methods_list;
		QPushButton* trainingButton;
		QPushButton* validationButton;
		QPushButton* pictureButton;
		QPlainTextEdit* textResults;
        QProgressBar* bar;
        QCheckBox* checkBox;

        //Paths attributes
        QString trainingFolder;
        QString validationFolder;


        vector<Facial_Recognizer*> recognizers; // list of recognizers

        Webcam* webcam; // webcam object

        //GroupBoxes
        QGroupBox* groupPath_creation();
        QGroupBox* groupImage_creation();


    private slots:

        //Menu slots
        void save_classifier();
        void load_classifier();
        void start_imageCapture();

        //Path modification
        void setTrainingPath(); // called when the path is modified
        void setValidationPath(); // called when the path is modified

		void method_changed(int); //called when the method list is modified

        void live_webcam(); //called by the webcam each time a frame is retrieved

        void takePicture(); // called by the pictureButton

        //traing / validation functions
        void training();
        void validation();


};

#endif // MAINWINDOW_H
