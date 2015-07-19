#include "kinectlab.h"

KinectLab::KinectLab(QWidget *parent) :
    QMainWindow(parent),
    m_module(NULL),
    ui_comWgt(NULL)
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

void KinectLab::loadModule()
{
    if(sender()){
        if(m_module){
            delete m_module;
        }
        if(sender() == act_m_knft){
            m_module = new KLMKnft(this, ui_ctrlPanel);
        }

        connect(m_module, SIGNAL(_setTitle(const QString&,int)), this, SLOT(h_moduleSetTitle(QString, int)));
        connect(m_module, SIGNAL(_setUI(QWidget*,int)), this, SLOT(h_moduleSetUI(QWidget*,int)));

        m_module->init();
    }
}

void KinectLab::h_moduleSetTitle(const QString & string, int target)
{
    if(target & KLMBase::WINDOW::MAIN){
        setWindowTitle(string);
    }
    if(target & KLMBase::WINDOW::CTRL_PANEL){
        ui_ctrlPanel->setWindowTitle(string);
    }
}

void KinectLab::h_moduleSetUI(QWidget *widget, int target)
{
    if(target & KLMBase::WINDOW::MAIN){
        setUI(widget);
    }
    if(target & KLMBase::WINDOW::CTRL_PANEL){
        ui_ctrlPanel->setUI(widget);
    }
}

void KinectLab::setUI(QWidget *widget)
{
    delete ui_comWgt;
    ui_comWgt = widget;
    ui_mainLt->addWidget(widget, 0, 0, 1, 1);
}

void KinectLab::initUI()
{
    /* init menu */
    ui_fileMenu = new QMenu(tr("&File"), this);
    menuBar()->addMenu(ui_fileMenu);
    ui_moduleMenu = new QMenu(tr("&Module"), this);
    menuBar()->addMenu(ui_moduleMenu);
    ui_aboutMenu = new QMenu(tr("&About"), this);
    menuBar()->addMenu(ui_aboutMenu);

    ico_menuModule = QIcon(":/image/menu_module");

    act_m_knft = new QAction(ico_menuModule, tr("Kinect FaceTrack (Native) Demo"), this);
//    act_m_knft->setCheckable(true);

    ui_moduleMenu->addAction(act_m_knft);

    setWindowTitle(tr("Kinect Face Lab"));
    ui_mainWgt = new QWidget(this);
    ui_mainLt = new QGridLayout(ui_mainWgt);
    ui_mainLt->setContentsMargins(0,0,0,0);
    this->setCentralWidget(ui_mainWgt);

    ui_ctrlPanel = new KLCtrlPanel(this);
    ui_ctrlPanel->show();

    connect(act_m_knft, SIGNAL(triggered()), this, SLOT(loadModule()));

}
