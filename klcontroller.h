#ifndef KLCONTROLLER_H
#define KLCONTROLLER_H

#include <QThread>
#include "Kinect.h"
#include "klcheckthread.h"
#include "klcommon.h"
#include <QDebug>
#include <QVector2D>

class KLController: public QThread
{
    Q_OBJECT
public:
    enum SOURCE_TYPE{
        NONE = 0x00,
        COLOR = 0x01,
        AUDIO = 0x02,
        BODY = 0x04,
        BODY_INDEX = 0x08,
        DEPTH = 0x10,
        INFRARED = 0x20,
        LONG_EXPOSURE_INFRARED = 0x40,
        MULTI = 0x80
    };
    static KLController& getInstance();
    void stop();
    void close();
    void open();
    void startStream(int source = -1);
    void stopStream();
    const IColorFrameReader* getReader(int source);
    const IFrameDescription* getFrameDesc(int source);
    const int getSourceMarker();
    bool isOpened();
    bool isAvailable();
    IKinectSensor* getSensor();

public slots:
    /* adapter slots */
    void __hrError(HRESULT hr);

    void h_connect(bool result);
    void h_started();
    void h_finished();

signals:
    void _open(bool);
    void _available(bool);
    void _hrError(HRESULT);
    void _readerChanged(bool, unsigned int);
    void _stream(bool);
    void _data(void*, unsigned int);

protected:
     void run();

private:
     /* constructors */
     KLController();
     KLController(KLController const&);
     KLController& operator=(KLController const&);

     /* members */
     bool isStop;
     IKinectSensor* sensor;
     KLCheckThread* checkThread;

     unsigned int sourceMarker;
     IColorFrameReader* colorReader;
     IFrameDescription* colorDesc;
     QVector<BYTE>* colorBuffer;


};

#endif // KLCONTROLLER_H
