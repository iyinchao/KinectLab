#ifndef KLGLVIEW_H
#define KLGLVIEW_H

#include <QGLWidget>
#include <QPainter>
#include <GL/GLU.h>

class KLMBase;

class KLGLView : public QGLWidget
{
    Q_OBJECT
public:
    explicit KLGLView(int id = 0, QWidget *parent = 0);
    void setModule(KLMBase* module);
    void setId(int id);
    const int getId();

signals:

public slots:
    void h_moduleDestroy();

protected:
    void initializeGL();
    void paintEvent(QPaintEvent* event);
    void resizeGL(int w, int h);

private:
    KLMBase* module;
    int id;

};

#endif // KLGLVIEW_H
