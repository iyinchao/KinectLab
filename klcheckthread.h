#ifndef KLCHECKTHREAD_H
#define KLCHECKTHREAD_H

#include <QThread>
#include "Kinect.h"
#include "klcommon.h"
#include <QDebug>

class KLCheckThread : public QThread
{
    Q_OBJECT
public:
    KLCheckThread(IKinectSensor *sensor, QObject *parent = 0);
    void setCheckInterval(unsigned long interval);
    void setSensor(IKinectSensor* sensor);
    bool isAvailable();
    void stop();

protected:
    void run();

signals:
    void _connect(bool);
    void _hrError(HRESULT);

public slots:
    void h_started();
    void h_finished();


private:
    bool lastCheck;
    bool isRelease;
    bool isStop;
    unsigned long checkInterval;
    IKinectSensor* sensor;

};

#endif // KLCHECKTHREAD_H
