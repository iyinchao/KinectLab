#include "klglview.h"
#include "module/klmbase.h"

KLGLView::KLGLView(int id, QWidget *parent) :
    QGLWidget(parent),
    module(NULL),
    id(id)
{
    setAutoFillBackground(false);
}

void KLGLView::setModule(KLMBase *module)
{
    if(this->module){
        disconnect(module, SIGNAL(destroyed()), this, SLOT(h_moduleDestroy()));
    }
    this->module = module;
    if(module){
        connect(module, SIGNAL(destroyed()), this, SLOT(h_moduleDestroy()));
    }
    update();
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

    makeCurrent();

    if(module){
        module->initGL(id);
    }
//    glEnable(GL_LIGHTING);
//    glEnable(GL_LIGHT1);
//    glEnable(GL_COLOR_MATERIAL);

//    static GLfloat lightPosition[4] = { 10, 10, 20.0, 1.0 };
//    GLfloat lightDiffuse[]   = {1.0, 1.0, 1.0, 1.0};
//    GLfloat lightSpecular[] = {0.5, 0.5, 0.5, 1.0};
//    glLightfv(GL_LIGHT1, GL_POSITION, lightPosition);
//    glLightfv(GL_LIGHT1, GL_DIFFUSE, lightDiffuse);
//    glLightfv(GL_LIGHT1, GL_SPECULAR, lightSpecular);

}

void KLGLView::paintEvent(QPaintEvent *event)
{
    makeCurrent();

    //2D Overlay Painting
    QPainter painter(this);
//    painter.setRenderHint(QPainter::Antialiasing);
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

//    //qglClearColor(QColor("blue"));
//    glShadeModel(GL_SMOOTH);
//    glEnable(GL_DEPTH_TEST);
//    glEnable(GL_CULL_FACE);
//    glEnable(GL_LIGHTING);
//    glEnable(GL_LIGHT0);
//    glEnable(GL_MULTISAMPLE);
//    glLoadIdentity();
//    glTranslatef(0.0, 0.0, -10.0);
////! [7]

////! [8]
//    glShadeModel(GL_FLAT);
//    glDisable(GL_CULL_FACE);
//    glDisable(GL_DEPTH_TEST);
//    glDisable(GL_LIGHTING);

    //3D Drawing
    if(module){
        module->paint3D(id);
    }
}

void KLGLView::resizeGL(int w, int h)
{
    makeCurrent();

    if(module){
        module->viewResize3D(id, w, h);
    }
}

void KLGLView::h_moduleDestroy()
{
    this->module = NULL;
    this->repaint();
    KLGLView::~KLGLView();
}


