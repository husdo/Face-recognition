#include "mainwindow.h"

#include <QAction>
#include <QThread>
#include "cv.h"
#include "highgui.h"
#include "dirent.h"
#include "resultdialogbox.h"
#include "EigenFaces.h"
#include "FisherFaces.h"
#include "LBPH.h"
//#include "executionthread.h"

using namespace cv;


MainWindow::MainWindow(QWidget* parent): mainLayout(0), mainWidget(0), cvWidget(0), webcam(0)
{
    // creation of the recognizers
    recognizers.push_back(new EigenFaces());
	recognizers.push_back(new FisherFaces());
	recognizers.push_back(new LBPH());
	recognizers.push_back(new EigenFaces());

    //creation of the menu
    QMenu* file = menuBar()->addMenu("&File");
    //creation of the different actions
    QAction* save = new QAction("&Save Classifier",this);
    file->addAction(save);
    QAction* load = new QAction("&Load Classifier",this);
    file->addAction(load);
    QAction* exit = new QAction("&Exit",this);
    file->addAction(exit);
    //signal/slot linking
    connect(save,SIGNAL(triggered()),this,SLOT(save_classifier())); // save slot
    connect(load,SIGNAL(triggered()),this,SLOT(load_classifier())); // load slot
    connect(exit,SIGNAL(triggered()),qApp,SLOT(quit())); //exit

    webcam = new Webcam();
    connect(webcam,SIGNAL(imageChanged()),this,SLOT(live_webcam()));


    mainLayout = new QVBoxLayout();

    //GrouBoxes
    QGroupBox* groupPath = groupPath_creation();
    QGroupBox* groupImage = groupImage_creation();

    //Trigger button
    pictureButton = new QPushButton("Take Picture");
    pictureButton->setMinimumWidth(500);
	pictureButton->setEnabled(false);

    connect(pictureButton,SIGNAL(clicked()),this,SLOT(takePicture()));

    bar = new QProgressBar(this);
    bar->setRange(0,100);
    checkBox = new QCheckBox("normalization",this);
    checkBox->setChecked(true);
	QLabel* imagesLabel = new QLabel("Loading Images:");
    QHBoxLayout* bottomLayout = new QHBoxLayout();
    bottomLayout->addWidget(checkBox);
    bottomLayout->addWidget(pictureButton);
    bottomLayout->addWidget(imagesLabel);
    bottomLayout->addWidget(bar);
    QWidget* bottomWidget = new QWidget();
    bottomWidget->setLayout(bottomLayout);

    mainLayout->addWidget(groupPath);
    mainLayout->addWidget(groupImage);
    mainLayout->addWidget(bottomWidget);
    mainWidget = new QWidget();
    mainWidget->setLayout(mainLayout);
    setCentralWidget(mainWidget);

    if(webcam->isOpened()){
        webcam->run();
    }
}

void MainWindow::save_classifier(){
	QString filename = QFileDialog::getSaveFileName(this,tr("Save Classifier"));
	if(filename != ""){
		Facial_Recognizer* current_recognizer = recognizers[methods_list->currentIndex()];
		current_recognizer->save(filename.toUtf8().constData());
		printMsg("Recognizer saved successfully!");
	}
	else
		printMsg("Failed to save the recognizer!");
}

void MainWindow::load_classifier(){
	QString filename = QFileDialog::getOpenFileName(this,tr("Save Classifier"));
	if(filename != ""){
		Facial_Recognizer* current_recognizer = recognizers[methods_list->currentIndex()];
		current_recognizer->load(filename.toUtf8().constData());
		validationButton->setEnabled(true);
		pictureButton->setEnabled(true);
		printMsg("Recognizer loaded successfully!");
	}
	else
		printMsg("Failed to load the recognizer!");
}

QGroupBox* MainWindow::groupPath_creation(){

    QGroupBox* groupPath = new QGroupBox("Directories:");
    groupPath->setMaximumHeight(100);
    training_path = new QLineEdit();
    QLabel* training_label = new QLabel("training directory: ");
    validation_path = new QLineEdit();
    QPushButton*  training_pathButton = new QPushButton("...");
    training_pathButton->setObjectName("button");
    training_pathButton->setMaximumWidth(50);
    QLabel* validation_label = new QLabel("validation directory: ");
    QPushButton* validation_pathButton = new QPushButton("...");
    validation_pathButton->setObjectName("button");
    validation_pathButton->setMaximumWidth(50);
    trainingButton = new QPushButton("Training");
    trainingButton->setMaximumWidth(200);
    validationButton = new QPushButton("Validation");
    validationButton->setMaximumWidth(200);
	validationButton->setEnabled(false);
    QLabel* method_label = new QLabel("method: ");
    methods_list = new QComboBox();
    methods_list->addItem("EigenFaces");
    methods_list->addItem("FisherFaces");
    methods_list->addItem("LBPH");
    methods_list->addItem("CNN");
    methods_list->setMaximumWidth(200);
	connect(methods_list,SIGNAL(currentIndexChanged(int)),this,SLOT(method_changed(int)));

    //connexions
    connect(training_pathButton,SIGNAL(clicked()),this,SLOT(setTrainingPath()));
    connect(validation_pathButton,SIGNAL(clicked()),this,SLOT(setValidationPath()));
    connect(training_path,SIGNAL(editingFinished()),this,SLOT(setTrainingPath()));
    connect(validation_path,SIGNAL(editingFinished()),this,SLOT(setValidationPath()));
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
    pathLayout->addWidget(method_label,2,0);
    pathLayout->addWidget(methods_list,2,1);
    groupPath->setLayout(pathLayout);

    return groupPath;
}

QGroupBox* MainWindow::groupImage_creation(){

    QGroupBox* group = new QGroupBox();
    QHBoxLayout* layout = new QHBoxLayout();

    cvWidget = new CVWidget(this);
    cvWidget->setMinimumSize(640,480);
    textResults = new QPlainTextEdit();
    textResults->setReadOnly(true);
    textResults->setMinimumWidth(400);
    layout->addWidget(cvWidget);
    layout->addWidget(textResults);
    group->setLayout(layout);

    return group;
}

void MainWindow::setTrainingPath(){
    if(sender()->objectName() =="button"){
        trainingFolder = QFileDialog::getExistingDirectory(this);
        if(trainingFolder != "")
            training_path->setText(trainingFolder);
    }
    else
        trainingFolder = training_path->text();
}

void MainWindow::setValidationPath(){
    if(sender()->objectName() =="button"){
        validationFolder = QFileDialog::getExistingDirectory(this);
        if(validationFolder != "")
            validation_path->setText(validationFolder);
    }
    else
        validationFolder = validation_path->text();
}

void MainWindow::live_webcam(){

    cvWidget->showImage(webcam->getImage());
}

void MainWindow::training(){
    if(trainingFolder!=""){
        printMsg("Training with folder: "+trainingFolder);
        webcam->stop();
        if(!trainingFolder.endsWith('/'))
            trainingFolder.append('/');
		Facial_Recognizer* current_recognizer = recognizers[methods_list->currentIndex()];
		Images imgs(trainingFolder.toStdString(),100,100,checkBox->isChecked(),bar);
		printMsg("Images loaded");
		printMsg("Learning(Can take a while)...");
		current_recognizer->training(imgs);
		validationButton->setEnabled(true);
		pictureButton->setEnabled(true);
		bar->reset();
		webcam->start();
        printMsg("Done!");
    }
    else
        printMsg("Invalid folder");
}

void MainWindow::validation(){
    if(validationFolder!=""){
        printMsg("Validation with folder: "+validationFolder);
        if(!validationFolder.endsWith('/'))
            validationFolder.append('/');
		Facial_Recognizer* current_recognizer = recognizers[methods_list->currentIndex()];
		Images imgs(validationFolder.toUtf8().constData(),100,100,checkBox->isChecked(),bar);
		printMsg("Images loaded");
		printMsg("Running the test...");
		current_recognizer->validation(imgs);
		bar->reset();
	}
    else
        printMsg("Invalid folder");
}

void MainWindow::printMsg(QString msg){
    textResults->appendPlainText(msg);
    textResults->repaint();
}

void MainWindow::takePicture(){
    cv::Mat img = webcam->getCroppedImage();
    Images imgs;
    int label =0;
    imgs.addImage(img,label,100,100,checkBox->isChecked());
	Facial_Recognizer* current_recognizer = recognizers[methods_list->currentIndex()];
    ResultDialogBox box(imgs,current_recognizer);
    box.exec();
}

void MainWindow::method_changed(int i){
	Facial_Recognizer* current_recognizer = recognizers[i];
	if(current_recognizer->isTrained()){
		validationButton->setEnabled(true);
		pictureButton->setEnabled(true);
	}
	else{
		validationButton->setEnabled(false);
		pictureButton->setEnabled(false);
	}
}
