#include "mainwindow.h"

#include <QAction>
#include "cv.h"
#include "highgui.h"
#include "dirent.h"

using namespace cv;


MainWindow::MainWindow(QWidget* parent): mainLayout(0), mainWidget(0), cvWidget(0)
{
    //creation of the menu
    QMenu* file = menuBar()->addMenu("&file");
    //creation of the different actions
    QAction* save = new QAction("&save classifier",this);
    file->addAction(save);
    QAction* load = new QAction("&load classifier",this);
    file->addAction(load);
    QAction* exit = new QAction("&exit",this);
    file->addAction(exit);
    //signal/slot linking
    connect(save,SIGNAL(triggered()),this,SLOT(save_classifier())); // save slot
    connect(load,SIGNAL(triggered()),this,SLOT(load_classifier())); // load slot
    connect(exit,SIGNAL(triggered()),qApp,SLOT(quit())); //exit


    mainLayout = new QVBoxLayout();

    //GrouBoxes
    QGroupBox* groupPath = groupPath_creation();
    QGroupBox* groupImage = groupImage_creation();

    mainLayout->addWidget(groupPath);
    mainLayout->addWidget(groupImage);
    mainWidget = new QWidget();
    mainWidget->setLayout(mainLayout);
    setCentralWidget(mainWidget);
}

void MainWindow::save_classifier(){

}

void MainWindow::load_classifier(){

}

QGroupBox* MainWindow::groupPath_creation(){

    QGroupBox* groupPath = new QGroupBox("Directories:");
    groupPath->setMaximumHeight(100);
    training_path = new QLineEdit();
    QLabel* training_label = new QLabel("training directory: ");
    validation_path = new QLineEdit();
    QPushButton* training_pathButton = new QPushButton("...");
    training_pathButton->setMaximumWidth(50);
    QLabel* validation_label = new QLabel("validation directory: ");
    QPushButton* validation_pathButton = new QPushButton("...");
    validation_pathButton->setMaximumWidth(50);
    QPushButton* trainingButton = new QPushButton("Training");
    trainingButton->setMaximumWidth(200);
    QPushButton* validationButton = new QPushButton("Validation");
    validationButton->setMaximumWidth(200);

    //connexions
    connect(training_pathButton,SIGNAL(clicked()),this,SLOT(setTrainingPath()));
    connect(validation_pathButton,SIGNAL(clicked()),this,SLOT(setValidationPath()));
    connect(trainingButton,SIGNAL(clicked()),this,SLOT(training()));
    connect(validationButton,SIGNAL(clicked()),this,SLOT(validation()));

    //Layout
    QGridLayout* pathLayout = new QGridLayout();
    pathLayout->addWidget(training_label,0,0);
    pathLayout->addWidget(validation_label,1,0);
    pathLayout->addWidget(training_path,0,1);
    pathLayout->addWidget(validation_path,1,1);
    pathLayout->addWidget(training_pathButton,0,2);
    pathLayout->addWidget(validation_pathButton,1,2);
    pathLayout->addWidget(trainingButton,0,3);
    pathLayout->addWidget(validationButton,1,3);
    groupPath->setLayout(pathLayout);

    return groupPath;
}

QGroupBox* MainWindow::groupImage_creation(){

    QGroupBox* group = new QGroupBox();
    QHBoxLayout* layout = new QHBoxLayout();

    cvWidget = new CVWidget(this);
    cvWidget->setMinimumSize(640,480);
    textResults = new QPlainTextEdit();
    textResults->setMinimumWidth(400);
    layout->addWidget(cvWidget);
    layout->addWidget(textResults);
    group->setLayout(layout);

    return group;
}

void MainWindow::setTrainingPath(){
    trainingFolder = QFileDialog::getExistingDirectory(this);
    if(trainingFolder!="")
        training_path->setText(trainingFolder);
}

void MainWindow::setValidationPath(){
    validationFolder = QFileDialog::getExistingDirectory(this);
    if(validationFolder!="")
        validation_path->setText(validationFolder);
}

void MainWindow::live_webcam(){
}

void MainWindow::training(){
    if(trainingFolder!=""){
        printMsg("Training with folder: "+trainingFolder);

        DIR *dpdf;
        struct dirent *epdf;
        int nb=0;
        Mat src;

        dpdf = opendir(trainingFolder.toStdString().c_str());
        if (dpdf != NULL){
            while (epdf = readdir(dpdf)){

                ////////////////////////////////////////////////////////////////////////

                QString filename(trainingFolder+"/");

                filename.append(epdf->d_name);
                src = imread(filename.toStdString());

                textResults->appendPlainText(epdf->d_name);
                if(!src.empty()){
                    cvWidget->showImage(src);
                    waitKey(0);
                }

                ///////////////////////////////////////////////////////////////////////
            }
        }
    }
    else
        printMsg("Invalid folder");
}

void MainWindow::validation(){
    if(validationFolder!="")
        printMsg("Validation with folder: "+validationFolder);
    else
        printMsg("Invalid folder");
}

void MainWindow::printMsg(QString msg){
    textResults->appendPlainText(msg);
}
