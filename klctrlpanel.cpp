#include "klctrlpanel.h"

KLCtrlPanel::KLCtrlPanel(QWidget *parent) :
    QWidget(parent),
    ui_comWgt(NULL)
{
    ctrler = &KLController::getInstance();

    initUI();
}

void KLCtrlPanel::h_openClosePB()
{
    //qDebug()<<(ui_openClosePB->text()).compare(str_closed);
    if((ui_openClosePB->text()) == str_closed){
        ctrler->open();
    }else{
        ctrler->close();
    }
}

void KLCtrlPanel::h_kinectOpen(bool result)
{
    if(result) {
        ui_openClosePB->setIcon(ico_opened);
        ui_openClosePB->setText(str_opened);
    }else{
        ui_openClosePB->setIcon(ico_closed);
        ui_openClosePB->setText(str_closed);
    }
}

void KLCtrlPanel::h_kinectAvailable(bool result)
{
    if(result) {
        ui_openClosePB->setIcon(ico_available);
        ui_openClosePB->setText(str_available);
        //ctrler->startStream(KLController::SOURCE_TYPE::COLOR); //TODO DELETE
    }else{
        ui_openClosePB->setIcon(ico_opened);
        ui_openClosePB->setText(str_opened);
    }
}

void KLCtrlPanel::h_kinectStream()
{
    if(ctrler->isRunning()){
        ui_openClosePB->setIcon(ico_stream);
        ui_openClosePB->setText(str_stream);
    }else{
        if(ctrler->isAvailable()){
            h_kinectAvailable(true);
        }
    }
}

void KLCtrlPanel::setUI(QWidget *widget)
{
    delete ui_comWgt;
    ui_comWgt = widget;
    ui_commonLt->addWidget(ui_comWgt, 0, 0, 1, 1);
}

void KLCtrlPanel::initUI()
{
    setWindowFlags(Qt::Window | Qt::WindowTitleHint | Qt::CustomizeWindowHint);

    ui_commonWgt = new QWidget(this);
    ui_commonLt = new QGridLayout(ui_commonWgt);
    ui_commonLt->setContentsMargins(0, 0, 0, 0);

    ui_ctrlGB = new QGroupBox(tr("Kinect Control"), ui_commonWgt);
    ui_ctrlGBLt = new QGridLayout(ui_ctrlGB);
    ico_available = QIcon(":/image/status_available");
    ico_opened = QIcon(":/image/status_opened");
    ico_closed = QIcon(":/image/status_closed");
    ico_stream = QIcon(":/image/status_stream");
    str_available = tr("Available");
    str_closed = tr("Closed");
    str_opened = tr("Waiting for Kinect");
    str_stream = tr("Streaming...");
    ui_openClosePB = new QPushButton(ico_closed, str_closed, ui_ctrlGB);


    //ui_openClosePB->setStyleSheet ("QPushButton{text-align: right}");
    ui_ctrlGB->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);
    ui_ctrlGBLt->addWidget(ui_openClosePB, 0,0,1,1);

    ui_rootLt = new QGridLayout(this);
    setLayout(ui_rootLt);
    ui_rootLt->addWidget(ui_ctrlGB, 0, 0, 1, 1);
    ui_rootLt->addWidget(ui_commonWgt, 1, 0, 1, 1);
    //ui_rootLt->setContentsMargins(0, 0, 0, 0);

    connect(ui_openClosePB, SIGNAL(clicked()), this, SLOT(h_openClosePB()));
    connect(ctrler, SIGNAL(_open(bool)), this, SLOT(h_kinectOpen(bool)));
    connect(ctrler, SIGNAL(_available(bool)), this, SLOT(h_kinectAvailable(bool)));
    connect(ctrler, SIGNAL(started()), this, SLOT(h_kinectStream()));
    connect(ctrler, SIGNAL(finished()), this, SLOT(h_kinectStream()));
}
