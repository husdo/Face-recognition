//#include <Windows.h>
#include "Images.h"
#include "Facial_Recognizer.h"
#include "EigenFaces.h"
#include "TestEigenFaces.h"
#include "FisherFaces.h"
#include "LBPH.h"






int main(int argc, char** argv)
{
    
		string path = "FirstDatabase/Train/";
		string path2 = "FirstDatabase/Test/";

        Images Imgs(path,100,100);  //test call
		Images Imgs2(path2,100,100);
        EigenFaces first;
		std::cout << "Training started!\n";
        first.training(Imgs);
		std::cout << "Training ended!\n";
        //first.load();
		first.validation(Imgs2,GrayImg);
        //std::cout<<first.predict(Imgs.getGrayImage(0));

	return 0;

}
