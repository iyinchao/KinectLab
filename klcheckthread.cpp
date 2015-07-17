#include "klcheckthread.h"

KLCheckThread::KLCheckThread(IKinectSensor *sensor, QObject *parent) :
    QThread(parent),
    isStop(true),
    lastCheck(false),
    checkInterval(300),
    sensor(sensor)
{
//    if(this->sensor){
//        sensor->Open();
//        HRESULT hr = this->sensor->SubscribeIsAvailableChanged(&isAvailableEvent);
//        if(FAILED(hr)){
//            qDebug()<<'Err';
//        }
//        qDebug()<<sensor;
    //    }
    connect(this, SIGNAL(started()), this, SLOT(h_started()));
    connect(this, SIGNAL(finished()), this, SLOT(h_finished()));
}

void KLCheckThread::setCheckInterval(unsigned long interval)
{
    checkInterval = interval;
}

void KLCheckThread::setSensor(IKinectSensor *sensor)
{
    this->sensor = sensor;
}

bool KLCheckThread::isAvailable()
{
    BOOLEAN available = false;
    HRESULT hr = sensor->get_IsAvailable(&available);
    if(SUCCEEDED(hr)){
        return (bool) available;
    }else{
        emit _hrError(hr);
        return false;
    }
}

void KLCheckThread::stop()
{
    isStop = true;
}

void KLCheckThread::run()
{
    isStop = false;
    while(!isStop){
//        KLComPtr<IIsAvailableChangedEventArgs> args;
//        auto ret = sensor->GetIsAvailableChangedEventData(isAvailableEvent, &args);
//        if(ret != S_OK){
//            continue;
//        }
//        BOOLEAN available = false;
//        args->get_IsAvailable( &available );
//        qDebug()<<available;
        if(!sensor) break;

        BOOLEAN available = false;
        HRESULT hr = sensor->get_IsAvailable(&available);

        if(SUCCEEDED(hr)){
            if(!lastCheck && available){
                emit _connect(true);
                qDebug()<<"[KLCheckThread] Connected";
            }
            if(lastCheck && !available){
                emit _connect(false);
                qDebug()<<"[KLCheckThread] Disconnected";
            }
            lastCheck = available;
        }else{
            emit _hrError(hr);
        }

        usleep(checkInterval * 1000);
    }
}

void KLCheckThread::h_started()
{
    isStop = false;
}

void KLCheckThread::h_finished()
{
    isStop = true;
}

