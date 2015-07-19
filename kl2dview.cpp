#include "kl2dview.h"
#include "module/klmbase.h"

KL2DView::KL2DView(QWidget *parent):
    QWidget(parent)
{
   // this->setFixedSize(200,200);
}

void KL2DView::setModule(KLMBase *module)
{
    this->module = module;
}

void KL2DView::paintEvent(QPaintEvent *event)
{
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
    module->paint2D(this, &painter, event);
    painter.end();
}
