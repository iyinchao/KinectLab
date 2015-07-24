#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGridLayout>
#include <QMenu>
#include <QMenuBar>
#include <QLabel>
#include <QIcon>
#include <QAction>
#include <QCloseEvent>

#include "klcontroller.h"
#include "klctrlpanel.h"

#include "module/klmknft.h"
#include "module/klmsvmc.h"

class KinectLab : public QMainWindow
{
    Q_OBJECT

public:
    explicit KinectLab(QWidget *parent = 0);
    ~KinectLab();

public slots:
    void loadModule();
    void h_moduleSetTitle(const QString& string, int target);
    void h_moduleSetUI(QWidget* widget, int target);
    void setUI(QWidget* widget);

protected:
    void closeEvent(QCloseEvent *event);

private:
    /* functions */
    void initUI();

    /* members */
    //ui components
    QGridLayout* ui_mainLt;
    QWidget* ui_mainWgt;
    KLCtrlPanel* ui_ctrlPanel;
    QWidget* ui_comWgt;

    QMenu* ui_fileMenu;
    QMenu* ui_moduleMenu;
    QMenu* ui_aboutMenu;
    QIcon ico_menuModule;
    QAction* act_m_knft;
    QAction* act_m_svmc;

    KLMBase* m_module;
};

#endif // MAINWINDOW_H
