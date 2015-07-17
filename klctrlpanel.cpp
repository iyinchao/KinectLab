#include "klctrlpanel.h"

KLCtrlPanel::KLCtrlPanel(QWidget *parent) :
    QWidget(parent)
{
    ctrler = &KLController::getInstance();

    initUI();
}

void KLCtrlPanel::h_openClosePB()
{
    if((ui_openClosePB->text()) == tr("Offline")){
        ctrler->open();
    }else{
        ctrler->close();
    }
}

void KLCtrlPanel::h_kinectOpen(bool result)
{
    if(result) {
        ui_openClosePB->setIcon(*ico_opened);
        ui_openClosePB->setText(tr("Waiting for Kinect"));
    }else{
        ui_openClosePB->setIcon(*ico_closed);
        ui_openClosePB->setText(tr("Offline"));
    }
}

void KLCtrlPanel::h_kinectAvailable(bool result)
{
    if(result) {
        ui_openClosePB->setIcon(*ico_available);
        ui_openClosePB->setText(tr("Online"));
        ctrler->startStream(KLController::SOURCE_TYPE::COLOR);
    }else{
        ui_openClosePB->setIcon(*ico_opened);
        ui_openClosePB->setText(tr("Waiting for Kinect"));
    }
}

void KLCtrlPanel::initUI()
{
    setWindowFlags(Qt::Window | Qt::WindowTitleHint | Qt::CustomizeWindowHint);

    ui_commonWgt = new QWidget(this);
    ui_commonLt = new QGridLayout(ui_commonWgt);
    //ui_commonLt->setContentsMargins(0, 0, 0, 0);

    ui_ctrlGB = new QGroupBox(tr("Kinect Control"), ui_commonWgt);
    ui_ctrlGBLt = new QGridLayout(ui_ctrlGB);
    ico_available = new QIcon(":/image/status_available");
    ico_opened = new QIcon(":/image/status_opened");
    ico_closed = new QIcon(":/image/status_closed");
    ui_openClosePB = new QPushButton(*ico_closed, tr("Offline"), ui_ctrlGB);

    ui_commonLt->addWidget(ui_ctrlGB, 0, 0, 1, 1);
    //ui_openClosePB->setStyleSheet ("QPushButton{text-align: right}");
    ui_ctrlGBLt->addWidget(ui_openClosePB, 0,0,1,1);

    ui_rootLt = new QGridLayout(this);
    setLayout(ui_rootLt);
    ui_rootLt->addWidget(ui_commonWgt, 0, 0, 1, 1);
    ui_rootLt->setContentsMargins(0, 0, 0, 0);

    connect(ui_openClosePB, SIGNAL(clicked()), this, SLOT(h_openClosePB()));
    connect(ctrler, SIGNAL(_open(bool)), this, SLOT(h_kinectOpen(bool)));
    connect(ctrler, SIGNAL(_available(bool)), this, SLOT(h_kinectAvailable(bool)));
}
