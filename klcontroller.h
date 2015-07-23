#ifndef KLCONTROLLER_H
#define KLCONTROLLER_H

#include <QThread>
#include <QVector2D>
#include <QColor>
#include "Kinect.h"
#include "Kinect.Face.h"
#include <QDebug>
#include <iostream>

#include "klcheckthread.h"
#include "klcommon.h"

struct KLFaceData{
    KLFaceData()
    {
        reset();
    }
    void reset(){
        isValid =  false;
        sourceHD = NULL;
        readerHD = NULL;
        //frameHD = NULL;
        modelVC = 0;
        alignment = NULL;
        model = NULL;
        alignmentInColorSpace = NULL;
        index = 0;
        //trackID = 0;
    }
    int index;
    int trackID;
    bool isValid;
    IHighDefinitionFaceFrameSource* sourceHD;
    IHighDefinitionFaceFrameReader* readerHD;
    IHighDefinitionFaceFrame* frameHD;
    IFaceModel* model;
    UINT32 modelVC;
    IFaceAlignment* alignment;
    ColorSpacePoint* alignmentInColorSpace;
};

class KLController: public QThread
{
    Q_OBJECT
public:
    enum SOURCE_TYPE{
        S_NONE = 0x0,
        S_COLOR = 0x1,
        S_AUDIO = 0x2,
        S_BODY = 0x4,
        S_BODY_INDEX = 0x8,
        S_DEPTH = 0x10,
        S_INFRARED = 0x20,
        S_LONG_EXPOSURE_INFRARED = 0x40,
        S_MULTI = 0x80,
        S_FACE_HD = 0x100,
        S_FACE = 0x400,
        S_ALL = 0xFFF
    };
    enum RESOURCE_TYPE {
        R_NONE = 0x0,
        R_COLOR = 0x1,
        R_AUDIO = 0x2,
        R_BODY = 0x4,
        R_BODY_INDEX = 0x8,
        R_DEPTH = 0x10,
        R_INFRARED = 0x20,
        R_LONG_EXPOSURE_INFRARED = 0x40,
        R_FACE_HD = 0x1000,
    };

    static KLController& getInstance();
    void setPollingRate(int fps);
    void stop();
    void close();
    void open();
    void startStream(int source = -1);
    void stopStream();
    bool isOpened();
    bool isAvailable();
    IKinectSensor* getSensor();
    static const QColor* getBodyColor(int body_index);

    ICoordinateMapper *getCoordMapper();

    const IColorFrameReader* getReader(int source);
    IFrameDescription *getFrameDesc(int source);
    const int getSourceMarker();

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
    void _pulse();

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
    static const QColor* bodyColors[BODY_COUNT];


    ICoordinateMapper* coordMapper;
    unsigned int sourceMarker;

    IColorFrameReader* colorReader;
    IFrameDescription* colorDesc;
    int colorWidth;
    int colorHeight;
    UINT colorBytesPerPixel;
    QVector<BYTE>* colorBuffer;

    IFaceFrameReader* faceReaders[BODY_COUNT];

    IDepthFrameReader* depthReader;

    IBodyFrameReader* bodyReader;
    IBody* bodies[BODY_COUNT];
    /* HD face */
    IHighDefinitionFaceFrameSource* faceHDSources[BODY_COUNT];
    IHighDefinitionFaceFrameReader* faceHDReaders[BODY_COUNT];
    IHighDefinitionFaceFrame* faceHDFrames[BODY_COUNT];
    IFaceAlignment* faceAlignments[BODY_COUNT];
    ColorSpacePoint* faceAlignmentsInColorSpace[BODY_COUNT];
    IFaceModel* faceModels[BODY_COUNT];
    UINT32 faceModelVCs[BODY_COUNT];
    KLFaceData* faceData[BODY_COUNT];

    //IHighDefinitionFaceFrameSource* faceHDSouc

};

#endif // KLCONTROLLER_H
