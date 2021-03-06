#include "klmbase.h"

KLMBase::KLMBase(QWidget *parentMain, QWidget *parentCtrlPanel) :
    QWidget(parentMain),
    p_main(parentMain),
    p_ctrlPanel(parentCtrlPanel)
{
   initUI();

   ctrler = &KLController::getInstance();
}

KLMBase::~KLMBase()
{

}

void KLMBase::init()
{
    //emit _setTitle(tr("Kinect Face Lab - Base"), WINDOW::MAIN);
    //emit _setUI(ui_mainWgt, WINDOW::MAIN);
    //emit _setUI(ui_ctrlPanelWgt, WINDOW::MAIN);
}

void KLMBase::initGL(int targetID)
{

}

void KLMBase::paint2D(int targetID, QPainter *painter, QPaintEvent *event)
{

}


void KLMBase::paint3D(int targetID)
{

}

void KLMBase::init3D(int targetID)
{

}

void KLMBase::viewResize2D(int targetID, QResizeEvent *event)
{

}

void KLMBase::viewResize3D(int targetID, int w, int h)
{

}

void KLMBase::initUI()
{
    ui_mainWgt = new QWidget(p_main);
    ui_mainLt = new QGridLayout(ui_mainWgt);
    ui_ctrlPanelWgt = new QWidget(p_ctrlPanel);
    ui_ctrlPanelLt = new QGridLayout(ui_ctrlPanelWgt);

    ui_mainLt->setContentsMargins(0,0,0,0);
    ui_mainLt->setSpacing(0);
    ui_ctrlPanelLt->setContentsMargins(0,0,0,0);
}

