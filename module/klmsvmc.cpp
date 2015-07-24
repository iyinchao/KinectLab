#include "klmsvmc.h"

KLMSvmc::KLMSvmc(QWidget *parentMain, QWidget *parentCtrlPanel):
    KLMBase(parentMain, parentCtrlPanel)
{
    initUI();
    svm_problem prob;
    prob.l = 100;
    prob.y = new double[prob.l];
    svm_parameter param;

    // default values
    param.svm_type = C_SVC;
    param.kernel_type = RBF;
    param.degree = 3;
    param.gamma = 0;
    param.coef0 = 0;
    param.nu = 0.5;
    param.cache_size = 100;
    param.C = 1;
    param.eps = 1e-3;
    param.p = 0.1;
    param.shrinking = 1;
    param.probability = 0;
    param.nr_weight = 0;
    param.weight_label = NULL;
    param.weight = NULL;

    //svm_model *model = svm_train(&prob, &param);

}

KLMSvmc::~KLMSvmc()
{

}

void KLMSvmc::init()
{
    KLMBase::init();
}

void KLMSvmc::paint2D(int targetID, QPainter *painter, QPaintEvent *event)
{

}

void KLMSvmc::paintGL2D(int targetID, QPainter *painter, QPaintEvent *event)
{

}

void KLMSvmc::h_kinectReaderInfo(bool exist, uint type)
{

}

void KLMSvmc::h_kinectAvailable(bool available)
{

}

void KLMSvmc::h_kinectData(void *data, uint type)
{

}

void KLMSvmc::initUI()
{
    KLMBase::initUI();
}
