#include "kinectlab.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    KinectLab w;
    w.show();

    return a.exec();
}
