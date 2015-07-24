#include "klcontroller.h"

KLController::KLController():
    sensor(NULL),
    fps(30),
    isStop(true),
    sourceMarker(SOURCE_TYPE::S_NONE),
    coordMapper(NULL),
    colorReader(NULL),
    colorDesc(NULL),
    colorBuffer(NULL),
    colorBytesPerPixel(0),
    colorWidth(0),
    colorHeight(0)

{
    for(int i = 0; i < BODY_COUNT; i++){
        faceHDReaders[i] = NULL;
        faceHDSources[i] = NULL;
        faceHDFrames[i] = NULL;
        faceData[i] = new KLFaceData();
        faceData[i]->trackID = 0;
        bodies[i] = NULL;
    }


    connect(this, SIGNAL(started()), this, SLOT(h_started()));
    connect(this, SIGNAL(finished()), this, SLOT(h_finished()));
    checkThread = new KLCheckThread(sensor);
    checkThread->setCheckInterval(300);
    connect(checkThread, SIGNAL(_connect(bool)), this, SLOT(h_connect(bool)));
    connect(checkThread, SIGNAL(_hrError(HRESULT)), this, SLOT(__hrError(HRESULT)));
}

KLController &KLController::getInstance()
{
    static KLController instance;
    return instance;
}

const QColor* KLController::bodyColors[BODY_COUNT] =  {
    new QColor("red"),
    new QColor("orange"),
    new QColor("yellow"),
    new QColor("green"),
    new QColor("blue"),
    new QColor("plum")
};

void KLController::setPollingRate(int fps)
{
    if(fps > 0){
        this->fps = fps;
    }
}

void KLController::stop()
{
    isStop = true;
}

void KLController::close()
{
    if(!isOpened()){
        if(sensor){
            safeRelease(sensor);
        }
        if(checkThread){
            checkThread->stop();
            checkThread->wait();
        }
        return;
    }
    HRESULT hr = sensor->Close();
    if(SUCCEEDED(hr)){
        checkThread->stop();
        checkThread->wait();
        safeRelease(sensor);
        qDebug()<<"[KLController] Closed";
        stopStream();
        emit _open(false);
    }else{
        emit _hrError(hr);
    }
}

void KLController::open()
{
    if(isOpened() && checkThread && checkThread->isRunning()){
        return;
    }
    checkThread->stop();
    checkThread->wait();
    safeRelease(sensor);

    HRESULT hr = GetDefaultKinectSensor(&sensor);
    if(SUCCEEDED(hr)){
        hr = sensor->Open();
    }else{
        emit _hrError(hr);
    }
    if(SUCCEEDED(hr)){
        hr = sensor->get_CoordinateMapper(&coordMapper);
    }else{
        emit _hrError(hr);
    }
    if(SUCCEEDED(hr)){
        checkThread->setSensor(sensor);
        checkThread->start();
        qDebug()<<"[KLController] Opened";
        emit _open(true);
        if(isAvailable()){
            qDebug()<<"[KLController] Connected";
            emit _available(true);
        }else{
            qDebug()<<"[KLController] Disconnected";
            emit _available(false);
        }
    }else{
        emit _hrError(hr);
    }
}

void KLController::startStream(int source)
{
    if(isRunning()){
        emit _readerInfo(true, sourceMarker);
        emit _readerInfo(false, SOURCE_TYPE::S_ALL ^ sourceMarker);
    }
    if(source >= 0){
        sourceMarker = source;
    }
    if(!isRunning()){
        this->start();
    }
}

void KLController::stopStream()
{
    this->stop();
    this->wait();
}

bool KLController::isOpened()
{
    if(!sensor){
        return false;
    }
    BOOLEAN result = false;
    HRESULT hr = sensor->get_IsOpen(&result);
    if(SUCCEEDED(hr)){
        return (bool)result;
    }else{
        emit _hrError(hr);
        return false;
    }
}

bool KLController::isAvailable()
{
    if(!isOpened()){
        return false;
    }
    if(checkThread){
        return checkThread->isAvailable();
    }else{
        return false;
    }
}

IKinectSensor *KLController::getSensor()
{
    return sensor;
}

const QColor *KLController::getBodyColor(int body_index)
{
    if(body_index >=0 && body_index <= BODY_COUNT){
        return bodyColors[body_index];
    }
    return bodyColors[0];
}

 ICoordinateMapper *KLController::getCoordMapper()
{
    return coordMapper;
}

const IColorFrameReader *KLController::getReader(int source)
{
    if(source == SOURCE_TYPE::S_COLOR){
        return colorReader;
    }
    return NULL;
}

IFrameDescription *KLController::getFrameDesc(int source)
{
    if(source == SOURCE_TYPE::S_COLOR){
        return colorDesc;
    }
    return NULL;
}

const int KLController::getSourceMarker()
{
    return sourceMarker;
}

/* slots */

void KLController::__hrError(HRESULT hr)
{
    emit _hrError(hr);
}

void KLController::h_connect(bool result)
{
    if(!result){
        stopStream();
    }
    emit _available(result);
}

void KLController::h_started()
{
    isStop = false;
}

void KLController::h_finished()
{
    isStop = true;
}


void KLController::run()
{
    isStop = false;
    while(!isStop){

        if(!isAvailable()){
            break; //Not continue, otherwise it will still use cpu
        }

        HRESULT hr;
        if(sourceMarker == SOURCE_TYPE::S_MULTI){
            continue;
        }

        /* color source */
        if(sourceMarker & SOURCE_TYPE::S_COLOR){
            if(!colorReader){
                hr = sensor->get_ColorFrameSource(&colorSource);
                if(SUCCEEDED(hr)){
                    safeRelease(colorReader);
                    hr = colorSource->OpenReader(&colorReader);
                }else{
                    emit _hrError(hr);
                    colorReader = NULL;
                }
                if(SUCCEEDED(hr)){
                    safeRelease(colorDesc);
                    hr = colorSource->CreateFrameDescription(ColorImageFormat::ColorImageFormat_Rgba, &colorDesc);
                }else{
                    emit _hrError(hr);
                    colorReader = NULL;
                }
                if(SUCCEEDED(hr)){
                    //
                    colorDesc->get_BytesPerPixel(&colorBytesPerPixel);
                    colorDesc->get_Height(&colorHeight);
                    colorDesc->get_Width(&colorWidth);
                    colorBuffer = new QVector<BYTE>(colorHeight * colorWidth * colorBytesPerPixel);

                    emit _readerInfo(true, SOURCE_TYPE::S_COLOR);
                }else{
                    emit _hrError(hr);
                    colorReader = NULL;
                }
            }
            if(colorReader){
                KLComPtr<IColorFrame> colorFrame;
                hr = colorReader->AcquireLatestFrame(&colorFrame);
                if(SUCCEEDED(hr)){
                    colorFrame->CopyConvertedFrameDataToArray(colorBuffer->size(), (colorBuffer->data()), ColorImageFormat::ColorImageFormat_Rgba);
                    emit _data(colorBuffer, RESOURCE_TYPE::R_COLOR);
                }else{
                    emit _hrError(hr);
                }
            }
        }else if(colorReader){
            safeRelease(colorSource);
            safeRelease(colorReader);
            safeRelease(colorDesc);
            qDebug()<<"dfd";
            emit _readerInfo(false, SOURCE_TYPE::S_COLOR);
        }

        /* body source */
        if(sourceMarker & SOURCE_TYPE::S_BODY){
            if(!bodyReader){
                KLComPtr<IBodyFrameSource> bodySource;
                hr = sensor->get_BodyFrameSource(&bodySource);
                if(SUCCEEDED(hr)){
                    safeRelease(bodyReader);
                    hr = bodySource->OpenReader(&bodyReader);
                }else{
                    emit _hrError(hr);
                    safeRelease(bodyReader);
                }

                if(SUCCEEDED(hr)){
                    emit _readerInfo(true, SOURCE_TYPE::S_BODY);
                }else{
                    emit _hrError(hr);
                    safeRelease(bodyReader);
                }
            }
            if(bodyReader){
                KLComPtr<IBodyFrame> bodyFrame;
                hr = bodyReader->AcquireLatestFrame(&bodyFrame);
                if(SUCCEEDED(hr)){
                    hr = bodyFrame->GetAndRefreshBodyData(_countof(bodies), bodies);
                }else{
                    emit _hrError(hr);
                }
            }
        }else if(bodyReader){
            safeRelease(bodyReader);
            for(int i = 0; i < BODY_COUNT; i++){
                safeRelease(bodies[i]);
            }
            emit _readerInfo(false, SOURCE_TYPE::S_BODY);
        }

        /* face source */
        if(sourceMarker & SOURCE_TYPE::S_FACE_HD){
            for(int i = 0; i < BODY_COUNT; i++){
                faceData[i]->reset();
            }
            if(!faceHDReaders[0]){
                bool hasError = false;
                for(int i = 0; i < BODY_COUNT; i++){
                    hr = CreateHighDefinitionFaceFrameSource(sensor, &faceHDSources[i]);
                    if(SUCCEEDED(hr)){
                        safeRelease(faceHDReaders[i]);
                        hr = faceHDSources[i]->OpenReader(&faceHDReaders[i]);
                    }else{
                        hasError = true;
                        break;
                    }
                    if(SUCCEEDED(hr)){
                        continue;
                        //emit _readerInfo(true, SOURCE_TYPE::S_FACE_HD);
                    }else{
                        hasError = true;
                        break;
                    }
                }
                if(hasError){
                    for(int i = 0; i < BODY_COUNT; i++){
                        safeRelease(faceHDReaders[i]);
                        safeRelease(faceHDSources[i]);
                    }
                    emit _hrError(hr);
                }else{
                    emit _readerInfo(true, SOURCE_TYPE::S_FACE_HD);
                }
            }

            if(faceHDReaders[0]){

                bool hasValidFaceTrack = false;

                for(int i = 0; i < BODY_COUNT; i++){
                    BOOLEAN isFaceTracked = false;

                    //faceData[i]->frameHD = NULL;
                    hr = faceHDReaders[i]->AcquireLatestFrame(&faceHDFrames[i]);

                    if(SUCCEEDED(hr)){
                        //qDebug()<<"huuray!!";
                        hr = faceHDFrames[i]->get_IsTrackingIdValid(&isFaceTracked);
                    }
                    if(SUCCEEDED(hr)){
                        faceData[i]->frameHD = faceHDFrames[i];
                        faceData[i]->sourceHD = faceHDSources[i];
                        faceData[i]->readerHD = faceHDReaders[i];
                        faceData[i]->index = i;
                    }

                    if(!isFaceTracked){
                        faceData[i]->isValid = false;
                        safeRelease(faceHDFrames[i]);
                        if(bodyReader){
                            if(bodies[i] != NULL){
                                BOOLEAN isBodyTracked = false;
                                hr = bodies[i]->get_IsTracked(&isBodyTracked);
                                if(SUCCEEDED(hr)){
                                    if(isBodyTracked){
                                        UINT64 bodyID;
                                        hr = bodies[i]->get_TrackingId(&bodyID);
                                        if(SUCCEEDED(hr)){
                                            faceHDSources[i]->put_TrackingId(bodyID);
                                            faceData[i]->trackID = bodyID;
                                        }
                                    }
                                }
                            }
                        }else{
                            // open body source
                            sourceMarker |= SOURCE_TYPE::S_BODY;
                        }
                    }else{
                        hasValidFaceTrack = true;
                        faceData[i]->isValid = true;
                    }

                }

                if(hasValidFaceTrack){
                    //qDebug()<<"p1";
                    emit _data(faceData, RESOURCE_TYPE::R_FACE_HD);
                }
            }
        }else if(faceHDReaders[0]){
            for(int i = 0; i < BODY_COUNT; i++){
                safeRelease(faceHDReaders[i]);
                safeRelease(faceHDFrames[i]);
                safeRelease(faceHDSources[i]);
                faceData[i]->reset();
            }
            emit _readerInfo(false, SOURCE_TYPE::S_FACE_HD);
        }

        if(sourceMarker == SOURCE_TYPE::S_NONE){
            break;
        }

        usleep(floor((1.0 / fps) * 1000 * 1000));

        emit _pulse();
    }
}
