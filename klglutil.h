#ifndef ALUTIL_H
#define ALUTIL_H

#include <QGLWidget>
#include <QtMath>
#include <GL/GLU.h>
#include <QVector3D>
#include <QMatrix4x4>

class KLGLUtil
{
public:
    KLGLUtil();
    static void drawSphere(double radius, int lats = 128, int longs = 64);
    static void drawAxis(double size);
    static void drawCube(double size);
    static void drawBox(double length, double width, double height);
    static void drawWiredPlane(double length, double width, int divideL = 20, int divideW = 20);
    static void drawCylinder(double baseRadius, double topRadius, double height, int divideR = 64, int divideH = 10);
    static void drawHeavySpring(QVector3D ptStart, QVector3D ptEnd, double Radius = 5, double TubeRadius = 1, double Coils = 10, int Rings = 64, int Sides = 18);
    static void drawCircle(double radius, int slices = 128);
    static void drawParabola(double a, double b, double c, double startX, double endX, int slices = 256);
};

#endif // ALUTIL_H
