#ifndef KLMBASE_H
#define KLMBASE_H

#include <QWidget>
#include <QGridLayout>
#include <QPainter>
#include <QBrush>
#include <QFont>
#include <QPen>

#include "../klcontroller.h"
#include "../kl2dview.h"

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
    virtual void paint2D(KL2DView* target, QPainter *painter, QPaintEvent *event);

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
