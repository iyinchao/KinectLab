#ifndef KLMSVMC_H
#define KLMSVMC_H

#include "klmbase.h"
#include "svm.h"

class KLMSvmc : public KLMBase
{
    Q_OBJECT
public:
    KLMSvmc(QWidget *parentMain = 0, QWidget *parentCtrlPanel = 0);
    ~KLMSvmc();

public slots:
    void init();
    void paint2D(int targetID, QPainter *painter, QPaintEvent *event);
    void paintGL2D(int targetID, QPainter *painter, QPaintEvent *event);

    void h_kinectReaderInfo(bool exist, uint type);
    void h_kinectAvailable(bool available);
    void h_kinectData(void* data, uint type);

private:
    void initUI();

};

#endif // KLMSVMC_H
