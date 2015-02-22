#include <mainwindow.h>
#include <QApplication>
#include <Windows.h>
#include "Images.h"
#include "Facial_Recognizer.h"






int main(int argc, char** argv)
{
    QApplication app(argc,argv);

    MainWindow mainWindow;
    mainWindow.show();

//	Images Imgs("ATT_ImgDatabase/");  //test call

    return app.exec();
}
