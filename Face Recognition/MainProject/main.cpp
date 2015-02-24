#include <mainwindow.h>
#include <QApplication>
#include <Windows.h>
#include "Images.h"
#include "Facial_Recognizer.h"
#include "EigenFaces.h"
#include "TestEigenFaces.h"






int main(int argc, char** argv)
{
    QApplication app(argc,argv);

    if(argc == 1){
        MainWindow mainWindow;
        mainWindow.show();
        return app.exec();
    }
    else{
        Images Imgs(argv[1]);  //test call
        EigenFaces first;
        first.training(Imgs);
        //first.load();
        first.validation(Imgs,GrayImg);
        //std::cout<<first.predict(Imgs.getGrayImage(0));
    }
	return 0;

}
