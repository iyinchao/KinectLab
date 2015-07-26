#ifndef KLMSVMC_H
#define KLMSVMC_H

#include "klmbase.h"
#include "svm.h"

#include <QQuaternion>

class KLMSvmc : public KLMBase
{
    Q_OBJECT
public:
    KLMSvmc(QWidget *parentMain = 0, QWidget *parentCtrlPanel = 0);
    ~KLMSvmc();

public slots:
    void init();
    void initGL(int targetID);
    void paint2D(int targetID, QPainter *painter, QPaintEvent *event);
    void paint3D(int targetID);
    void viewResize3D(int targetID, int w, int h);

    void h_kinectReaderInfo(bool exist, uint type);
    void h_kinectAvailable(bool available);
    void h_kinectData(void* data, uint type);

private:
    void initUI();

    KL2DView* ui_frameView;
    KLGLView* ui_pointView;
    KL2DView* ui_userView;
    QPlainTextEdit *ui_console;
    QSize ui_viewSmallSize;
    QSize ui_viewBigSize;

    QBrush brush;
    QPen pen;

    int colorHeight;
    int colorWidth;
    QImage colorImage;
    UINT32 faceModelVC;
    int inValidFrameCount;
    IFaceAlignment* faceAlignment;
    ColorSpacePoint* faceAlignmentColor;
    CameraSpacePoint* faceAlignmentCam;
    IFaceModel* faceModel;
    CameraSpacePoint* headPivot;
    ColorSpacePoint* headPivotColor;
    RectI* faceBound;
    Vector4* faceOrient;

    KLFaceData** faceData;


};

#endif // KLMSVMC_H
