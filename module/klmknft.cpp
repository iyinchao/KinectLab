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
    faceData = NULL;
    for(int i = 0; i < BODY_COUNT; i++){
        faceAlignments[i] = NULL;
        faceAlignmentsInColorSpace[i] = NULL;
        faceModels[i] = NULL;
    }
    faceModelVC = 0;
    inValidFrameCount = 0;

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

void KLMKnft::paint2D(int targetID, QPainter *painter, QPaintEvent *event)
{
    switch(targetID){
    case 0:

        if(colorImage.height()){
            //painter->drawPixmap(0,0, QPixmap::fromImage(colorImage.scaledToHeight(ui_2DView->height())));
            painter->drawPixmap(0,0, QPixmap::fromImage(colorImage.scaledToHeight(ui_2DView->height())));
        }


        for(int i = 0; i < BODY_COUNT; i++){
            QPen pen(*KLController::getBodyColor(i));
            pen.setWidth(2);
            pen.setCapStyle(Qt::RoundCap);
            painter->setPen(pen);
            double rate = ui_2DView->height() / 1080.0;

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
                //delete faceAlignmentsInColorSpace[i];
                //faceAlignmentsInColorSpace[i] = NULL;
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

        //ctrler->startStream( KLController::SOURCE_TYPE::S_FACE_HD);
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
//        if(type & KLController::RESOURCE_TYPE::R_FACE_HD){
//            std::cout<<"p2"<<std::endl;
//        }


    }

    if(type & KLController::RESOURCE_TYPE::R_FACE_HD){
        faceData = ((KLFaceData **)data);

        HRESULT hr;
        for(int i = 0; i < BODY_COUNT; i++){
            if(!faceAlignments[i]){
                hr = CreateFaceAlignment(&(faceAlignments[i]));
            }
            if(!faceModels[i]){
                float sd[FaceShapeDeformations_Count] = {0.0f};
                hr = CreateFaceModel(1.0f, FaceShapeDeformations::FaceShapeDeformations_Count, sd, &faceModels[i]);
            }
            GetFaceModelVertexCount(&faceModelVC);

            //QMutex mutex;
            //if(mutex.tryLock()){
                if(faceData[i]->isValid && faceData[i]->frameHD && faceAlignments[i] && faceModels[i] && faceModelVC){
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
                        std::cout<<"p3"<<" "<<std::hex<<hr<<" "<<i<<std::endl;
                    }
                    delete v;
                }
                //mutex.unlock();
            //}

            //if(faceAlignments && faceAlignments[i])delete faceAlignments[i];
            //faceAlignments[i] = NULL;
            //safeRelease(faceAlignments[i]);
            //safeRelease(faceModels[i]);
            //safeRelease(faceData[i]->frameHD);
        }


        //ui_2DView->update();
//        if(!faceAlignmentsInColorSpace){
//            faceAlignmentsInColorSpace = new ColorSpacePoint[faceData->modelVC];
//        }else{
//            memcpy(faceAlignmentsInColorSpace, faceData->alignmentInColorSpace, faceData->modelVC);
//            ui_2DView->update();
//        }
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


