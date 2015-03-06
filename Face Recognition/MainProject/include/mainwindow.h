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
        QVBoxLayout* mainLayout;
        QWidget* mainWidget;
        CVWidget* cvWidget;
        QLineEdit* training_path;
        QLineEdit* validation_path;
        QComboBox* methods_list;
		QPushButton* trainingButton;
		QPushButton* validationButton;
		QPushButton* pictureButton;

        QString trainingFolder;
        QString validationFolder;
        QPlainTextEdit* textResults;
        QProgressBar* bar;
        QCheckBox* checkBox;

        vector<Facial_Recognizer*> recognizers;

        Webcam* webcam;

        QGroupBox* groupPath_creation();
        QGroupBox* groupImage_creation();


    private slots:
        void save_classifier();
        void load_classifier();
        void setTrainingPath();
        void setValidationPath();
		void method_changed(int);
        void live_webcam();
        void takePicture();
        void training();
        void validation();


};

#endif // MAINWINDOW_H
