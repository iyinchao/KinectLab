#include "klcontroller.h"

KLController::KLController():
    sensor(NULL),
    fps(30),
    isStop(true),
    sourceMarker(SOURCE_TYPE::NONE),
    colorReader(NULL),
    colorBuffer(NULL)
{
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
    }else{
        emit _hrError(hr);
    }
}

void KLController::startStream(int source)
{
    if(isRunning()){
        emit _readerInfo(true, sourceMarker);
        emit _readerInfo(false, 0xFFF ^ sourceMarker);
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

const IColorFrameReader *KLController::getReader(int source)
{
    if(source == SOURCE_TYPE::COLOR){
        return colorReader;
    }
    return NULL;
}

IFrameDescription *KLController::getFrameDesc(int source)
{
    if(source == SOURCE_TYPE::COLOR){
        return colorDesc;
    }
    return NULL;
}

const int KLController::getSourceMarker()
{
    return sourceMarker;
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
        if(sourceMarker == SOURCE_TYPE::NONE){
            break;
        }
        if(sourceMarker == SOURCE_TYPE::MULTI){
            continue;
        }
        if(sourceMarker & SOURCE_TYPE::COLOR){
            if(!colorReader){
                KLComPtr<IColorFrameSource> colorSource;
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
                    unsigned int bytesPerPixel;
                    int width;
                    int height;
                    colorDesc->get_BytesPerPixel(&bytesPerPixel);
                    colorDesc->get_Height(&height);
                    colorDesc->get_Width(&width);
                    colorBuffer = new QVector<BYTE>(height * width * bytesPerPixel);

                    emit _readerInfo(true, SOURCE_TYPE::COLOR);
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
                    emit _data(colorBuffer, SOURCE_TYPE::COLOR);
                }else{
                    emit _hrError(hr);
                }
            }
        }else if(colorReader){
            emit _readerInfo(false, SOURCE_TYPE::COLOR);
            safeRelease(colorReader);
            safeRelease(colorDesc);
        }

        usleep(floor((1.0 / fps) * 1000 * 1000));
    }
}
