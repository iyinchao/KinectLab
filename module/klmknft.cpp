#include "klmknft.h"

KLMKnft::KLMKnft(QWidget *parentMain, QWidget *parentCtrlPanel):
    KLMBase(parentMain, parentCtrlPanel)
{
    colorImage = QImage();
    faceData = NULL;
    ui_2DView = NULL;
    ui_RGBView = NULL;
    pen = QPen();
    for(int i = 0; i < BODY_COUNT; i++){
        faceAlignments[i] = NULL;
        faceAlignmentsInColorSpace[i] = NULL;
        faceModels[i] = NULL;
    }
    faceModelVC = 0;
    inValidFrameCount = 0;
    colorWidth = 0;
    colorHeight = 0;

    initUI();
}

KLMKnft::~KLMKnft()
{
    //ctrler->startStream(KLController::SOURCE_TYPE::S_NONE);
    //ui_2DView->setModule(NULL);
    //qDebug()<<"delete mo";
    for(int i = 0; i < BODY_COUNT; i++){
        safeRelease(faceAlignments[i]);
        safeDelete(faceAlignmentsInColorSpace[i]);
        safeRelease(faceModels[i]);
    }
}

void KLMKnft::init()
{
    KLMBase::init();

    emit _setTitle("Kinect Face Lab - Kinect Native Face Tracker", WINDOW::MAIN);
    emit _setUI(ui_mainWgt, WINDOW::MAIN);
    emit _setUI(ui_ctrlPanelWgt, WINDOW::CTRL_PANEL);

    connect(ctrler, SIGNAL(_readerInfo(bool, unsigned int)), this, SLOT(h_kinectReaderInfo(bool,uint)));
    connect(ctrler, SIGNAL(_available(bool)), this, SLOT(h_kinectAvailable(bool)));
    connect(ctrler, SIGNAL(_data(void*,uint)), this, SLOT(h_kinectData(void*,uint)));

    if(ctrler->isAvailable()){
        ctrler->startStream(KLController::SOURCE_TYPE::S_COLOR | KLController::SOURCE_TYPE::S_FACE_HD);
    }else{
        ctrler->open();
    }
}

void KLMKnft::paint2D(int targetID, QPainter *painter, QPaintEvent *)
{
    switch(targetID){
    case 0:

        if(colorImage.height()){
            painter->drawPixmap(0,0, QPixmap::fromImage(colorImage.scaledToHeight(ui_2DView->height())));
        }

        HRESULT hr = E_PENDING;

        for(int i = 0; i < BODY_COUNT; i++){

            if(!faceAlignments[i]){
                hr = CreateFaceAlignment(&(faceAlignments[i]));
            }
            if(SUCCEEDED(hr) && !faceModels[i]){
                float sd[FaceShapeDeformations_Count] = {0.0f};
                hr = CreateFaceModel(1.0f, FaceShapeDeformations::FaceShapeDeformations_Count, sd, &faceModels[i]);
            }
            if(SUCCEEDED(hr) && !faceModelVC){
                hr = GetFaceModelVertexCount(&faceModelVC);
                qDebug()<<faceModelVC;
            }

            if(faceData && faceData[i]->isValid && faceAlignments[i] && faceModels[i] && faceModelVC){
                //qDebug()<<"p2";
                CameraSpacePoint *v = new CameraSpacePoint[faceModelVC];
                hr = faceData[i]->frameHD->GetAndRefreshFaceAlignmentResult(faceAlignments[i]);

                if(SUCCEEDED(hr)){
                    hr = faceModels[i]->CalculateVerticesForAlignment(faceAlignments[i], faceModelVC, v);
                }
                if(SUCCEEDED(hr)){
                    if(!faceAlignmentsInColorSpace[i]){
                        faceAlignmentsInColorSpace[i] = new ColorSpacePoint[faceModelVC];
                    }
                    hr = ctrler->getCoordMapper()->MapCameraPointsToColorSpace(faceModelVC, v, faceModelVC, faceAlignmentsInColorSpace[i]);
                    //std::cout<<"p3"<<" "<<std::hex<<hr<<" "<<i<<std::endl;
                }
                delete v;
            }

            pen.setWidth(2);
            pen.setColor(*KLController::getBodyColor(i));
            painter->setPen(pen);

            double rate = ui_2DView->height() / (colorHeight?colorHeight:1.0);

            if(faceAlignmentsInColorSpace[i]){
                for(UINT j = 0; j < faceModelVC; j++){
                    painter->drawPoint(faceAlignmentsInColorSpace[i][j].X * rate, faceAlignmentsInColorSpace[i][j].Y * rate);
                }
                if(faceData && faceData[i] && !faceData[i]->isValid){
                    inValidFrameCount++;
                    if(inValidFrameCount > 2){
                        delete faceAlignmentsInColorSpace[i];
                        faceAlignmentsInColorSpace[i] = NULL;
                        inValidFrameCount = 0;
                    }
                }
            }
        }
        break;
    }
}

void KLMKnft::paintGL2D(int targetID, QPainter *painter, QPaintEvent *event)
{
    switch(targetID){
    case 0:
//        if(colorImage.height()){
//            painter->drawPixmap(0,0, QPixmap::fromImage(colorImage));
//        }
        break;
    }
}

void KLMKnft::h_kinectReaderInfo(bool exist, uint type)
{
    if(exist){
        if(type & KLController::SOURCE_TYPE::S_COLOR){
            IFrameDescription* frameDesc;
            frameDesc = ctrler->getFrameDesc(KLController::SOURCE_TYPE::S_COLOR);
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

        ctrler->startStream(KLController::SOURCE_TYPE::S_COLOR | KLController::SOURCE_TYPE::S_FACE_HD);

    }else{

    }
}

void KLMKnft::h_kinectData(void *data, uint type)
{
    if(type & KLController::RESOURCE_TYPE::R_COLOR){
        const QVector<BYTE>* data_ptr = ((QVector<BYTE> *)data);
        colorImage = QImage(data_ptr->data(), 1920, 1080, QImage::Format_RGBA8888_Premultiplied);
        ui_2DView->update();
    }

    if(type & KLController::RESOURCE_TYPE::R_FACE_HD){
        faceData = ((KLFaceData **)data);
    }
}

void KLMKnft::initUI()
{
    KLMBase::initUI();
    ui_2DView = new KL2DView(0, ui_mainWgt);
    ui_2DView->setModule(this);
    ui_mainLt->addWidget(ui_2DView, 0,0,1,1);
//    ui_RGBView = new KLGLView(0, ui_mainWgt);
//    ui_RGBView->setModule(this);
//    ui_mainLt->addWidget(ui_RGBView, 0,0,1,1);
}


