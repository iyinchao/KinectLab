#ifndef KLMKNFT_H
#define KLMKNFT_H

#include "klmbase.h"

class KLMKnft : public KLMBase
{
    Q_OBJECT
public:
    KLMKnft(QWidget *parentMain = 0, QWidget *parentCtrlPanel = 0);
    ~KLMKnft();

public slots:
    void init();
    void paint2D(int targetID, QPainter *painter, QPaintEvent *event);
    void paintGL2D(int targetID, QPainter *painter, QPaintEvent *event);

    void h_kinectReaderInfo(bool exist, uint type);
    void h_kinectAvailable(bool available);
    void h_kinectData(void* data, uint type);

private:
    void initUI();

    KL2DView* ui_2DView;
    KLGLView* ui_RGBView;
    QImage colorImage;

    int colorHeight;
    int colorWidth;

};

#endif // KLMKNFT_H
