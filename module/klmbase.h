#ifndef KLMBASE_H
#define KLMBASE_H

#include <QWidget>
#include <QGridLayout>
#include <QPainter>
#include <QBrush>
#include <QFont>
#include <QPen>
#include <QResizeEvent>
#include <QPlainTextEdit>
#include <QtMath>


#include "../klcontroller.h"
#include "../kl2dview.h"
#include "../klglview.h"
#include "../klglutil.h"

class KLMBase : public QWidget
{
    Q_OBJECT
public:
    enum WINDOW{
        MAIN = 0x1,
        CTRL_PANEL = 0X2
    };
    explicit KLMBase(QWidget *parentMain = 0, QWidget *parentCtrlPanel = 0);
    ~KLMBase();

signals:
    void _setTitle(const QString&, int);
    void _setUI(QWidget*, int);

public slots:
    virtual void init();
    virtual void initGL(int targetID);
    virtual void paint2D(int targetID, QPainter *painter, QPaintEvent *event);
    virtual void paint3D(int targetID);
    virtual void init3D(int targetID);
    virtual void viewResize2D(int targetID, QResizeEvent* event);
    virtual void viewResize3D(int targetID, int w, int h);

protected:

    QWidget* p_main;
    QWidget* p_ctrlPanel;

    QWidget* ui_mainWgt;
    QGridLayout* ui_mainLt;
    QWidget* ui_ctrlPanelWgt;
    QGridLayout* ui_ctrlPanelLt;

    KLController *ctrler;

    void initUI();

};

#endif // KLMBASE_H
