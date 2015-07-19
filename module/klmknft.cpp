#include "klmknft.h"

KLMKnft::KLMKnft(QWidget *parentMain, QWidget *parentCtrlPanel):
    KLMBase(parentMain, parentCtrlPanel),
    colorImage(NULL)
{
    initUI();
}

KLMKnft::~KLMKnft()
{

}

void KLMKnft::init()
{
    KLMBase::init();
    emit _setTitle("Kinect Face Lab - Kinect Native Face Tracker", WINDOW::MAIN);
    emit _setUI(ui_mainWgt, WINDOW::MAIN);
    emit _setUI(ui_ctrlPanelWgt, WINDOW::CTRL_PANEL);

    connect(ctrler, SIGNAL(_readerChanged(bool, unsigned int)), this, SLOT(h_kinectReaderChanged(bool,uint)));
    connect(ctrler, SIGNAL(_available(bool)), this, SLOT(h_kinectAvailable(bool)));
    connect(ctrler, SIGNAL(_data(void*,uint)), this, SLOT(h_kinectData(void*,uint)));
    if(ctrler->isAvailable()){
        ctrler->startStream(KLController::SOURCE_TYPE::COLOR);
    }else{
        ctrler->open();
    }
}

void KLMKnft::paint2D(KL2DView *target, QPainter *painter, QPaintEvent *event)
{

}

void KLMKnft::h_kinectReaderChanged(bool exist, uint type)
{

}

void KLMKnft::h_kinectAvailable(bool available)
{
    if(available){

        ctrler->startStream(KLController::SOURCE_TYPE::COLOR);

    }else{

    }
}

void KLMKnft::h_kinectData(void *data, uint type)
{
    if(type & KLController::SOURCE_TYPE::COLOR){
        //qDebug()<<((QVector<BYTE> *)data)->size() / 1920 / 4;
       //colorImage = new
    }
}

void KLMKnft::initUI()
{
    KLMBase::initUI();
    ui_2DView = new KL2DView(ui_mainWgt);
    ui_2DView->setModule(this);
    ui_mainLt->addWidget(ui_2DView, 0,0,1,1);
    //ui_mainLt->addWidget(new QPushButton(tr("sdfgsdfgsdf")), 0,0,1,1);
}


