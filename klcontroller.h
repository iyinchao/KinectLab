#ifndef KLCONTROLLER_H
#define KLCONTROLLER_H

#include <QThread>
#include <QVector2D>
#include "Kinect.h"
#include "Kinect.Face.h"
#include <QDebug>

#include "klcheckthread.h"
#include "klcommon.h"

class KLController: public QThread
{
    Q_OBJECT
public:
    enum SOURCE_TYPE{
        NONE = 0x0,
        COLOR = 0x1,
        AUDIO = 0x2,
        BODY = 0x4,
        BODY_INDEX = 0x8,
        DEPTH = 0x10,
        INFRARED = 0x20,
        LONG_EXPOSURE_INFRARED = 0x40,
        MULTI = 0x80,
        FACE_ALIGNMENT = 0x100,
        ALL = 0xFFF
    };

    static KLController& getInstance();
    void setPollingRate(int fps);
    void stop();
    void close();
    void open();
    void startStream(int source = -1);
    void stopStream();
    const IColorFrameReader* getReader(int source);
    IFrameDescription *getFrameDesc(int source);
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
    void _readerInfo(bool, unsigned int);
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
     int fps;
     bool isStop;
     IKinectSensor* sensor;
     KLCheckThread* checkThread;

     unsigned int sourceMarker;
     IColorFrameReader* colorReader;
     IFrameDescription* colorDesc;
     QVector<BYTE>* colorBuffer;
     /* basic face */
     //TODO get to know what is IFaceAlignment
     IFaceFrameReader* faceReader;



};

#endif // KLCONTROLLER_H
