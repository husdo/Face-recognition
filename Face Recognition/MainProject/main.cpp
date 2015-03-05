#include <mainwindow.h>
#include <QApplication>
//#include <Windows.h>
#include "Images.h"
#include "Facial_Recognizer.h"
#include "EigenFaces.h"
#include "TestEigenFaces.h"
#include "FisherFaces.h"






int main(int argc, char** argv)
{
    

    if(argc == 1){
		QApplication app(argc, argv);
        MainWindow mainWindow;
        mainWindow.show();
        return app.exec();
    }
    else{
		string path = "ATT_ImgDatabase/";
        Images Imgs(argv[1]);  //test call
		Images Imgs2(argv[2]);
        EigenFaces first;
		std::cout << "Training started!\n";
        first.training(Imgs);
		std::cout << "Training ended!\n";
        //first.load();
        first.validation(Imgs2);
        //std::cout<<first.predict(Imgs.getGrayImage(0));
    }
	return 0;

}
