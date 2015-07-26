#include "kinectlab.h"

KinectLab::KinectLab(QWidget *parent) :
    QMainWindow(parent),
    m_module(NULL),
    ui_comWgt(NULL)
{

    initUI();

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
        if(sender() == act_m_svmc){
            m_module = new KLMSvmc(this, ui_ctrlPanel);
        }

        connect(m_module, SIGNAL(_setTitle(const QString&,int)), this, SLOT(h_moduleSetTitle(QString, int)));
        connect(m_module, SIGNAL(_setUI(QWidget*,int)), this, SLOT(h_moduleSetUI(QWidget*,int)));
        //ui_mainWgt->repaint();

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

void KinectLab::closeEvent(QCloseEvent *event)
{
    KLController::getInstance().stop();
    KLController::getInstance().wait();
    event->accept();
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
    act_m_svmc = new QAction(ico_menuModule, tr("SVM Smile Cam"), this);

    ui_moduleMenu->addAction(act_m_knft);
    ui_moduleMenu->addAction(act_m_svmc);

    setWindowTitle(tr("Kinect Face Lab"));
    ui_mainWgt = new QWidget(this);
    ui_mainLt = new QGridLayout(ui_mainWgt);
    ui_mainLt->setContentsMargins(0,0,0,0);
//    ui_mainLt->setHorizontalSpacing(0);
//    ui_mainLt->setVerticalSpacing();

    this->setCentralWidget(ui_mainWgt);

    ui_ctrlPanel = new KLCtrlPanel(this);
    ui_ctrlPanel->show();

    connect(act_m_knft, SIGNAL(triggered()), this, SLOT(loadModule()));
    connect(act_m_svmc, SIGNAL(triggered()), this, SLOT(loadModule()));

}
