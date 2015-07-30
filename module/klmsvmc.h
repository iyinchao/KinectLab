#ifndef KLMSVMC_H
#define KLMSVMC_H

#include "klmbase.h"
#include "svm.h"

#undef min
#undef max
#include <QTime>
#include <QQuaternion>
#include <QGroupBox>
#include <QPushButton>
#include <QProgressBar>
#include <QFileDialog>
#include <QLabel>


struct KLSvmcTrain{
    KLSvmcTrain()
    {
        reset();
    }
    void reset(){
        id = -1;
        toolTip = "";
        rows = 3;
        label = 0;
        dimension = 30;
        data = new double*[rows];
        for(int i = 0; i < rows; i++){
            data[i] = new double[dimension];
        }
        complete = false;
    }
    QString toolTip;
    int rows;
    int dimension;
    int label;
    int id;
    bool complete;
    double **data;
};

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
    void h_trainBt(bool start);
    void h_trainSave();
    void h_trainLoad();
    void h_modelLoadTrain();
    void h_modelLoad();
    void h_modelSave();
    void h_capture(bool start);

    void trainStart(int id);
    void trainStop(int id);
    void trainDone(int id);
    void trainTestComplete();

    void drawToast(const QString& text, QPainter *painter, QPaintEvent *event);

private:
    enum TRAIN_PHASE{
        P_MC_1, //positive/mouth closed/smile min
        P_MC_2, //positive/mouth closed/smile medium
        P_MC_3, //positive/mouth closed/smile max
        P_MO1_1, //positive/mouth opened/min/smile min
        P_MO1_2, //positive/mouth opened/min/smile med
        P_MO1_3, //positive/mouth opened/min/smile max
        P_MO2_1, //positive/mouth opened/med/smile min
        P_MO2_2, //positive/mouth opened/med/smile med
        P_MO2_3, //positive/mouth opened/med/smile max
        P_MO3_1, //positive/mouth opened/max/smile min
        P_MO3_2, //positive/mouth opened/max/smile med
        P_MO3_3, //positive/mouth opened/max/smile max
        N_AU27_1, //mouth stretch(张嘴)/ narrow / min
        N_AU27_2, //mouth stretch(张嘴)/ narrow / wmax
        N_AU27_3, //mouth stretch(张嘴)/ wide / min
        N_AU27_4, //mouth stretch(张嘴)/ wide / max
        N_AU20_1, //lip stretcher(伸长嘴唇)/min
        N_AU20_2, //lip stretcher(伸长嘴唇)/max
        N_AU15_1, //lip corner depressor(嘴巴闭上，嘴角向下)/min
        N_AU15_2, //lip corner depressor(嘴巴闭上，嘴角向下)/max
        N_AU18, //lip pucker(撅嘴)
        N_AU10, //upper lip raiser(抬上嘴唇)
        N_AU28, //lip suck(吸嘴唇)
        N_NE //Neutral face
    };

    void initUI();

    KL2DView* ui_frameView;
    KLGLView* ui_pointView;
    KL2DView* ui_userView;
    KLGLView* ui_testView;
    QPlainTextEdit *ui_console;
    QSize ui_viewSmallSize;
    QSize ui_viewBigSize;

    QGroupBox* ui_trainGB;
    QGridLayout* ui_trainGBLt;
    QProgressBar* ui_trainPG;
    QPushButton* ui_trainBt[TRAIN_PHASE::N_NE+1];
    QPushButton* ui_trainSaveBt;
    QPushButton* ui_trainLoadBt;

    QGroupBox* ui_modelGB;
    QGridLayout* ui_modelGBLt;
    QPushButton* ui_modelLoadTrainBt;
    QPushButton* ui_modelLoadBt;
    QPushButton* ui_modelSaveBt;
    QLabel* ui_modelStatus;

    QGroupBox* ui_captureGB;
    QGridLayout* ui_captureGBLt;
    QProgressBar* ui_capturePG;
    QPushButton* ui_captureBt;

    QIcon ico_done;
    QIcon ico_ing;
    QIcon ico_no;

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
    KLSvmcTrain* trainSet[TRAIN_PHASE::N_NE+1];
    const int trainDataRows = 3;
    const int trainDataDim = 30;
    const int trainDataRepeat = 10;
    double* trainBuffer;
    QString modelStatus;
    svm_parameter svmTrainParam;
    svm_problem prob;
    svm_model* svmModel;
    int frameSmile;
    const int frameSmileLimit = 35;
    bool captureStart;

    int trainID;
    int trainCursor;

    static const int featurePoints[10];
    bool* featureMap;

    /* ue */
    int cameraFlash;
    bool showToast;


};

#endif // KLMSVMC_H
