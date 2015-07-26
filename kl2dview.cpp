#include "kl2dview.h"
#include "module/klmbase.h"

KL2DView::KL2DView(int id, QWidget *parent):
    QWidget(parent),
    module(NULL),
    id(id)
{
    // this->setFixedSize(200,200);
}

KL2DView::~KL2DView()
{

}

void KL2DView::setModule(KLMBase *module)
{
    if(this->module){
        disconnect(module, SIGNAL(destroyed()), this, SLOT(h_moduleDestroy()));
    }
    this->module = module;
    if(module){
        connect(module, SIGNAL(destroyed()), this, SLOT(h_moduleDestroy()));
    }
    this->repaint();
}

void KL2DView::setId(int id)
{
    this->id = id;
}

const int KL2DView::getId()
{
    return id;
}

void KL2DView::paintEvent(QPaintEvent *event)
{
    //qDebug()<<this;
    QPainter painter;
    painter.begin(this);
    //painter.setRenderHint(QPainter::Antialiasing);
//    painter.setPen(Qt::NoPen);
//    for (int x = 0; x < width(); x += 10) {
//        for (int y = 0; y < height(); y += 10) {
//            if((x / 10) % 2 == (y / 10) % 2){
//                painter.fillRect(x, y, 10, 10, QColor(255,255,255));
//            }else{
//                painter.fillRect(x, y, 10, 10, QColor(225,225,225));
//            }
//        }
//    }

    if(module){
        module->paint2D(id, &painter, event);
    }
    painter.end();
}

void KL2DView::resizeEvent(QResizeEvent *event)
{
    if(module){
        module->viewResize2D(id, event);
    }
}

void KL2DView::h_moduleDestroy()
{
    this->module = NULL;
    this->repaint();
    KL2DView::~KL2DView();
}
