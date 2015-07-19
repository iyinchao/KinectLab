#include "klmknft.h"

KLMKnft::KLMKnft(QWidget *parentMain, QWidget *parentCtrlPanel):
    KLMBase(parentMain, parentCtrlPanel)
{
    initUI();
}

KLMKnft::~KLMKnft()
{

}

void KLMKnft::init()
{
    KLMBase::init();

    colorImage = QImage();

    emit _setTitle("Kinect Face Lab - Kinect Native Face Tracker", WINDOW::MAIN);
    emit _setUI(ui_mainWgt, WINDOW::MAIN);
    emit _setUI(ui_ctrlPanelWgt, WINDOW::CTRL_PANEL);

    connect(ctrler, SIGNAL(_readerInfo(bool, unsigned int)), this, SLOT(h_kinectReaderInfo(bool,uint)));
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
    if(colorImage.height()){
        painter->drawPixmap(0,0, QPixmap::fromImage(colorImage));
    }
}

void KLMKnft::h_kinectReaderInfo(bool exist, uint type)
{
    if(exist){
        if(type & KLController::SOURCE_TYPE::COLOR){
            IFrameDescription* frameDesc;
            frameDesc = ctrler->getFrameDesc(KLController::SOURCE_TYPE::COLOR);
            if(frameDesc){
                frameDesc->get_Height(&colorHeight);
                frameDesc->get_Height(&colorWidth);
            }
        }
    }
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
        const QVector<BYTE>* data_ptr = ((QVector<BYTE> *)data);
        colorImage = QImage(data_ptr->data(), 1920, 1080, QImage::Format_RGBA8888_Premultiplied).scaledToHeight(ui_2DView->height());

        //colorImage->fromData((const uchar *)(data_ptr->data()), data_ptr->size(), QImage::Format_RGBA8888);
        ui_2DView->update();
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


