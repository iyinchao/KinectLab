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
        sharedMemory[i] = new QSharedMemory("alignmentInColorSpace"+i, this);

    }

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
//        if(faceData && faceData->isValid && faceData->alignmentInColorSpace){
//            //qDebug()<<sizeof(*(faceData->alignmentInColorSpace));//faceData->alignmentInColorSpace[4].X;
//            //qDebug()<<faceAlignmentsInColorSpace[4].X;
//            QPen pen(QColor("blue"));
//            pen.setWidth(5);
//            pen.setCapStyle(Qt::PenCapStyle::RoundCap);
//            painter->setPen(pen);
//            for(UINT i = 0; i < faceData->modelVC; i++){
//                painter->drawPoint(faceData->alignmentInColorSpace[i].X, faceData->alignmentInColorSpace[i].Y);
//            }
//        }


//        for(int i = 0; i < BODY_COUNT; i++){
//            if(sharedMemory[i]->attach()){
//                QPen pen(QColor("blue"));
//                pen.setWidth(5);
//                pen.setCapStyle(Qt::RoundCap);
//                painter->setPen(pen);
//                double rate = ui_2DView->height() / 1080.0;

//                sharedMemory[i]->lock();

//                const ColorSpacePoint* data_ptr = (static_cast<const ColorSpacePoint *>(sharedMemory[i]->constData()));
//                if(data_ptr[0].X != -1 && data_ptr[0].Y != -1){
//                    for(UINT j = 0; j < faceData->modelVC; j++){
//                        painter->drawPoint(data_ptr[j].X * rate, data_ptr[j].Y * rate);
//                    }
//                }

//                sharedMemory[i]->unlock();
//                sharedMemory[i]->detach();
//            }
//        }



//        if(faceData && faceData->isValid){
//            int i = faceData->index;

//        }
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
    }
    if(type & KLController::RESOURCE_TYPE::R_FACE_HD){
        faceData = ((KLFaceData *)data);
        qDebug()<<"ok";
        GetFaceModelVertexCount(&faceModelVC);
        HRESULT hr;
        for(int i = 0; i < BODY_COUNT; i++){
            //hr = CreateFaceAlignment()
        }
        /*for(int i = 0; i < BODY_COUNT; i++){
            if(!faceAlignments[i]){
                hr = CreateFaceAlignment(&(faceAlignments[i]));
            }
            if(!faceModels[i]){
                float sd[FaceShapeDeformations_Count] = {0.0f};
                hr = CreateFaceModel(1.0f, FaceShapeDeformations::FaceShapeDeformations_Count, sd, &faceModels[i]);
            }
            if(!faceModelVCs[i]){
                hr = GetFaceModelVertexCount(&(faceModelVCs[i]));
            }
            if(faceData[i]->isValid && faceAlignments[i] && faceModels[i] && faceModelVCs[i]){
                CameraSpacePoint *v = new CameraSpacePoint[faceModelVCs[i]];
                hr = faceHDFrames[i]->GetAndRefreshFaceAlignmentResult(faceAlignments[i]);
                if(SUCCEEDED(hr)){
                    faceData[i]->alignment = faceAlignments[i];
                    faceData[i]->modelVC = faceModelVCs[i];
                    faceData[i]->model = faceModels[i];
                    faceData[i]->index = i;
                    hr = faceModels[i]->CalculateVerticesForAlignment(faceAlignments[i], faceModelVCs[i], v);
                }
                if(SUCCEEDED(hr)){

                    if(!faceAlignmentsInColorSpace[i]){
                        faceAlignmentsInColorSpace[i] = new ColorSpacePoint[faceModelVCs[i]];
                    }
                    hr = coordMapper->MapCameraPointsToColorSpace(faceModelVCs[i], v, faceModelVCs[i], faceAlignmentsInColorSpace[i]);


//                                if(sharedMemory->isAttached()){
//                                    sharedMemory->detach();
//                                }

//                                if(sharedMemory->size() == 0){
//                                    qDebug()<<"df";
//                                }
                    if(sharedMemory[i]->create(faceModelVCs[i] * sizeof(ColorSpacePoint))){

                    }else{
                        //qDebug()<<sharedMemory->errorString();
                        //qDebug()<<"dfd";
                        sharedMemory[i]->attach();
                    }
                    if(sharedMemory[i]->lock()){
                        ColorSpacePoint* to = static_cast<ColorSpacePoint *>(sharedMemory[i]->data());
                        memcpy(to, faceAlignmentsInColorSpace[i], faceModelVCs[i] * sizeof(ColorSpacePoint));
                        sharedMemory[i]->unlock();
                        emit _data(faceData[i], RESOURCE_TYPE::R_FACE_HD);
                    }

                    //faceAlignmentsInColorSpace[i]
                    //faceData[i]->alignmentInColorSpace = faceAlignmentsInColorSpace[i];

                    delete v;

                    //qDebug()<<faceAlignmentsInColorSpace[i][2].X;
                }
            }else{
                if(sharedMemory[i]->create(faceModelVCs[i] * sizeof(ColorSpacePoint))){

                }else{
                    sharedMemory[i]->attach();
                }
                if(sharedMemory[i]->lock()){
                    ColorSpacePoint* to = static_cast<ColorSpacePoint *>(sharedMemory[i]->data());
                    to[0].X = -1;
                    to[0].Y = -1;
                    sharedMemory[i]->unlock();
                }
            }
        }
    */


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


