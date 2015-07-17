#ifndef KLCTRLPANEL_H
#define KLCTRLPANEL_H

#include <QWidget>
#include <QGridLayout>
#include <QGroupBox>
#include <QPushButton>
#include <QIcon>

#include "klcontroller.h"

class KLCtrlPanel : public QWidget
{
    Q_OBJECT
public:
    explicit KLCtrlPanel(QWidget *parent = 0);

signals:

public slots:
    void h_openClosePB();
    void h_kinectOpen(bool result);
    void h_kinectAvailable(bool result);
    //void setOpenClosePB()

private:
    QGridLayout* ui_rootLt;
    QWidget* ui_commonWgt;
    QGridLayout* ui_commonLt;
    QGroupBox* ui_ctrlGB;
    QGridLayout* ui_ctrlGBLt;
    QPushButton* ui_openClosePB;
    QIcon* ico_available;
    QIcon* ico_closed;
    QIcon* ico_opened;

    KLController* ctrler;

    void initUI();

};

#endif // KLCTRLPANEL_H
