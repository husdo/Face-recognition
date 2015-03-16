#include "mainwindow.h"

#include <QAction>
#include <QThread>
#include <typeinfo>
#include "cv.h"
#include "highgui.h"
#include "dirent.h"
#include "resultdialogbox.h"
#include "EigenFaces.h"
#include "FisherFaces.h"
#include "CombinedClassifier.h"
#include "LBPH.h"

using namespace cv;

void imageCaptureLoop(Webcam& webcam,QString foldername);


MainWindow::MainWindow(QWidget* parent): mainLayout(0), mainWidget(0), cvWidget(0), webcam(0)
{
    // creation of the recognizers
    recognizers.push_back(new EigenFaces(this));
	recognizers.push_back(new FisherFaces(this));
	recognizers.push_back(new LBPH(this));
	recognizers.push_back(new CombinedClassifier(this));

    //creation of the menu
    QMenu* file = menuBar()->addMenu("&File");
    //creation of the different actions
    QAction* save = new QAction("&Save Classifier",this);
    file->addAction(save);
    QAction* load = new QAction("&Load Classifier",this);
    file->addAction(load);
    QAction* captureImgs = new QAction("&Capture Images",this);
    file->addAction(captureImgs);
    QAction* exit = new QAction("&Exit",this);
    file->addAction(exit);
    //signal/slot linking
    connect(save,SIGNAL(triggered()),this,SLOT(save_classifier())); // save slot
    connect(load,SIGNAL(triggered()),this,SLOT(load_classifier())); // load slot
    connect(captureImgs,SIGNAL(triggered()),this,SLOT(start_imageCapture())); // load slot
    connect(exit,SIGNAL(triggered()),qApp,SLOT(quit())); //exit

    //webcam object to retrieve images from the camera
    webcam = new Webcam();
    connect(webcam,SIGNAL(imageChanged()),this,SLOT(live_webcam()));

    //Layout to display the different parts of the window
    mainLayout = new QVBoxLayout();

    //GrouBoxes
    QGroupBox* groupPath = groupPath_creation();
    QGroupBox* groupImage = groupImage_creation();

    //Trigger button to start taking picturs
    pictureButton = new QPushButton("Take Pictures");
    pictureButton->setMinimumWidth(500);
	pictureButton->setEnabled(false); // at the initialisation it is not enabled because no classifier is trained

    connect(pictureButton,SIGNAL(clicked()),this,SLOT(takePicture()));

    bar = new QProgressBar(this); //progress bar for loading images
    bar->setRange(0,100); //percentage
    checkBox = new QCheckBox("normalization",this); // checkbox for the normalization
    checkBox->setChecked(true);
	QLabel* imagesLabel = new QLabel("Partial progress:");
    QHBoxLayout* bottomLayout = new QHBoxLayout();
    bottomLayout->addWidget(checkBox);
    bottomLayout->addWidget(pictureButton);
    bottomLayout->addWidget(imagesLabel);
    bottomLayout->addWidget(bar);
    QWidget* bottomWidget = new QWidget(); // we create a widget at the bottom of the window, where the checkbox, the button and the progress bar will be displayed
    bottomWidget->setLayout(bottomLayout);

    //adding the groupboxes and the bottom widget to the main window
    mainLayout->addWidget(groupPath);
    mainLayout->addWidget(groupImage);
    mainLayout->addWidget(bottomWidget);
    mainWidget = new QWidget();
    mainWidget->setLayout(mainLayout);
    setCentralWidget(mainWidget);

    if(webcam->isOpened()){ // If we can open the webcam stream we start retrieving the images
        webcam->run();
    }
}

void MainWindow::save_classifier(){
    Facial_Recognizer* current_recognizer = recognizers[methods_list->currentIndex()]; // get the selected classifier
    if(!current_recognizer->isTrained()){ // if not trained cannot be saved
        printMsg("Classifier not trained. Cannot be saved!");
        return;
    }
    QString filter("Classifier (*.eig *.fis *.lbp)");
    string type = typeid(*current_recognizer).name(); // depending on the type of the recognizer we want a particular extension
    if(type == "10EigenFaces")
        filter = "EigenFaces classifier (*.eig)";
     if(type == "11FisherFaces")
        filter = "FisherFaces classifier (*.fis)";
     if(type == "4LBPH")
        filter = "LBPH classifier (*.lbp)";
	QString filename = QFileDialog::getSaveFileName(this,tr("Save Classifier"),QString(),filter); // DialogBox for saving
	if(filename != ""){
		current_recognizer->save(filename.toUtf8().constData()); //calling the save function of the recognizer
	}
	else
		printMsg("Failed to save the recognizer!");
}

void MainWindow::load_classifier(){
	QString filename = QFileDialog::getOpenFileName(this,"load Classifier",QString(),"Classifier (*.eig *.fis *.lbp)"); //DialogBox for loading
	if(filename != ""){
		Facial_Recognizer* current_recognizer = recognizers[methods_list->currentIndex()]; // get the recognizer selected
		current_recognizer->load(filename.toUtf8().constData()); //calling the load function
		validationButton->setEnabled(true); // because we have loaded a recognizer we can now test it
		pictureButton->setEnabled(true); // and predict pictures
	}
	else
		printMsg("Failed to load the recognizer!");
}

void MainWindow::start_imageCapture(){
    imageCaptureLoop(*webcam,QFileDialog::getExistingDirectory(this)); // slot function calling the ImageCapture program
}

QGroupBox* MainWindow::groupPath_creation(){

    //creating widgets used to get the training and validation folders
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
	methods_list->addItem("Boost");
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

    // The groupImage is composed of an image widget and a GUI console
    QGroupBox* group = new QGroupBox();
    QHBoxLayout* layout = new QHBoxLayout();

    cvWidget = new CVWidget(this); // Image widget
    cvWidget->setMinimumSize(640,480);
    textResults = new QPlainTextEdit(); // GUI console
    textResults->setReadOnly(true);
    textResults->setMinimumWidth(400);
    layout->addWidget(cvWidget);
    layout->addWidget(textResults);
    group->setLayout(layout);

    return group;
}

void MainWindow::setTrainingPath(){
    if(sender()->objectName() =="button"){ //test if the function is called from the '...' button
        trainingFolder = QFileDialog::getExistingDirectory(this); // if yes, use the dialogbox
        if(trainingFolder != "")
            training_path->setText(trainingFolder); // an set the new path as attribute
    }
    else // if the button is not used it's because tha path has been modified with the text widget
        trainingFolder = training_path->text();
}

void MainWindow::setValidationPath(){
    // same as the training path but with different variables
    if(sender()->objectName() =="button"){
        validationFolder = QFileDialog::getExistingDirectory(this);
        if(validationFolder != "")
            validation_path->setText(validationFolder);
    }
    else
        validationFolder = validation_path->text();
}

void MainWindow::live_webcam(){
    //everytime the Webcam object retrieve an image this function is called and update the Image Widget of the window
    cvWidget->showImage(webcam->getImage(true));
}

void MainWindow::training(){
    if(trainingFolder!=""){
        printMsg("Training with folder: "+trainingFolder); // display which folder is used
        if(!trainingFolder.endsWith('/')) // is not valid make the ath valid
            trainingFolder.append('/');
		Facial_Recognizer* current_recognizer = recognizers[methods_list->currentIndex()];
		Images imgs(trainingFolder.toStdString(),100,100,checkBox->isChecked(),bar); //creates the Images from the path
		printMsg("Images loaded");
		printMsg("Learning(Can take a while)...");
		current_recognizer->training(imgs); // train the recognizer
		validationButton->setEnabled(true);
		pictureButton->setEnabled(true);
		bar->reset(); // at the end of the process the progress bar is 100% so we need to reset it
        printMsg("Done!");
    }
    else
        printMsg("Invalid folder");
}

void MainWindow::validation(){
    if(validationFolder!=""){
        printMsg("Validation with folder: "+validationFolder); // display the selected folder
        if(!validationFolder.endsWith('/')) // make the path valid
            validationFolder.append('/');
		Facial_Recognizer* current_recognizer = recognizers[methods_list->currentIndex()];
		Images imgs(validationFolder.toUtf8().constData(),100,100,checkBox->isChecked(),bar); // create the Images
		printMsg("Images loaded");
		printMsg("Running the test...");
		current_recognizer->validation(imgs); // run the validation process
		bar->reset();// reset the progress bar
	}
    else
        printMsg("Invalid folder");
}

void MainWindow::printMsg(QString msg){
    textResults->appendPlainText(msg); // print a message to the GUI console
    textResults->repaint(); // refresh the GUI console
}

void MainWindow::takePicture(){
	Facial_Recognizer* current_recognizer = recognizers[methods_list->currentIndex()]; // get the selected recognizer
    ResultDialogBox box(webcam,current_recognizer,checkBox->isChecked()); // create the ResultDialogBox
    box.exec(); //execute it
}

void MainWindow::method_changed(int i){
	Facial_Recognizer* current_recognizer = recognizers[i]; // get the new selected recognizer
	if(current_recognizer->isTrained()){ // if trained we can enable the validation and taking picture
		validationButton->setEnabled(true);
		pictureButton->setEnabled(true);
	}
	else{ // if not trained we disable the buttons
		validationButton->setEnabled(false);
		pictureButton->setEnabled(false);
	}
}
