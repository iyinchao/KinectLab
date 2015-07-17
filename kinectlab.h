#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGridLayout>
#include <QMenu>
#include <QMenuBar>
#include <QLabel>

#include "klcontroller.h"
#include "klctrlpanel.h"

class KinectLab : public QMainWindow
{
    Q_OBJECT

public:
    explicit KinectLab(QWidget *parent = 0);
    ~KinectLab();

private:
    /* functions */
    void initUI();

    /* members */
    //ui components
    QMenu* ui_fileMenu;
    QGridLayout* ui_mainLt;
    QWidget* ui_mainWgt;
    QLabel* ui_view2D;
    KLCtrlPanel* ui_ctrlPanel;
};

#endif // MAINWINDOW_H
