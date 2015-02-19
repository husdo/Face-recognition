#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets>
#include <QVBoxLayout>

#include "cvwidget.h"


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

        QString trainingFolder;
        QString validationFolder;
        QPlainTextEdit* textResults;

        QGroupBox* groupPath_creation();
        QGroupBox* groupImage_creation();


    private slots:
        void save_classifier();
        void load_classifier();
        void setTrainingPath();
        void setValidationPath();

        void live_webcam();
        void training();
        void validation();


};

#endif // MAINWINDOW_H
