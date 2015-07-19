#include "klglview.h"
#include "module/klmbase.h"

KLGLView::KLGLView(int id, QWidget *parent) :
    QGLWidget(parent),
    module(NULL),
    id(id)
{

}

void KLGLView::setModule(KLMBase *module)
{
    this->module = module;
}

void KLGLView::setId(int id)
{
    this->id = id;
}

const int KLGLView::getId()
{
    return id;
}

void KLGLView::initializeGL()
{

}

void KLGLView::paintEvent(QPaintEvent *event)
{
    makeCurrent();

    //3D Context


    //2D Overlay Painting
    QPainter painter(this);
    if(module){
        module->paintGL2D(id, &painter, event);
    }
    painter.end();
}

void KLGLView::resizeGL(int w, int h)
{

}


