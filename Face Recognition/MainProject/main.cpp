#include <mainwindow.h>
#include <QApplication>
#include <Windows.h>
#include "Images.h"
#include "Facial_Recognizer.h"
#include "TestEigenFaces.h"






int main(int argc, char** argv)
{
    /*QApplication app(argc,argv);

    MainWindow mainWindow;
    mainWindow.show();
	return app.exec();*/
	Images Imgs("ATT_ImgDatabase/");  //test call
	TestEigenFaces first;
	first.training(Imgs);
	//first.load();
	first.validation(Imgs,GrayImg);
	//std::cout<<first.predict(Imgs.getGrayImage(0));
	return 0;

}
