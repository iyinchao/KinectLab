#include "kinectlab.h"

KinectLab::KinectLab(QWidget *parent) :
    QMainWindow(parent)
{
//    ui->setupUi(this);
//    IKinectSensor* m_kinect;
//    GetDefaultKinectSensor(&m_kinect);
//    m_kinect->Open();
    initUI();
//    BOOLEAN c;
//    c = false;

    //KLController::getInstance().open();
    //KLController::getInstance().start();
    //qDebug()<<KLController::getInstance().getSensor();
}

KinectLab::~KinectLab()
{

}

void KinectLab::initUI()
{
    /* init menu */
    ui_fileMenu = new QMenu(tr("&File"), this);
    menuBar()->addMenu(ui_fileMenu);

    setWindowTitle(tr("Kinect Face Lab"));
    ui_mainWgt = new QWidget(this);
    ui_mainLt = new QGridLayout(ui_mainWgt);
    this->setCentralWidget(ui_mainWgt);

    ui_ctrlPanel = new KLCtrlPanel(this);
    ui_ctrlPanel->show();

}
