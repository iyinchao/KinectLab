#include "klglutil.h"

KLGLUtil::KLGLUtil()
{

}
//adopted here: http://www.cburch.com/cs/490/sched/feb8/index.html
void KLGLUtil::drawSphere(double radius, int lats, int longs)
{

//   int i, j;
//   for(i = 0; i <= lats; i++) {
//       double lat0 = M_PI * (-0.5 + (double) (i - 1) / lats);
//       double z0  =  sin(lat0);
//       double zr0 =  cos(lat0);

//       double lat1 = M_PI * (-0.5 + (double) i / lats);
//       double z1 = sin(lat1);
//       double zr1 = cos(lat1);

//       glBegin(GL_QUAD_STRIP);
//       for(j = 0; j <= longs; j++) {
//           double lng = 2 * M_PI * (double) (j - 1) / longs;
//           double x = cos(lng);
//           double y = sin(lng);

//           glNormal3f(radius * x * zr0, radius * y * zr0, radius * z0);
//           glVertex3f(radius * x * zr0, radius * y * zr0, radius * z0);
//           glNormal3f(radius * x * zr1, radius * y * zr1, radius * z1);
//           glVertex3f(radius * x * zr1, radius * y * zr1, radius * z1);
//       }
//       glEnd();
//   }
    GLUquadricObj* quadratic;
    quadratic = gluNewQuadric();
    gluQuadricNormals(quadratic, GLU_SMOOTH);
    gluQuadricTexture(quadratic, GL_TRUE);
    gluSphere(quadratic, radius, lats, longs);
}

void KLGLUtil::drawAxis(double size)
{
    //x red
    glColor3f(1.0,0,0);
    glBegin(GL_LINES);
    glVertex3f(0,0,0);
    glVertex3f(size,0,0);
    glEnd();
    glPushMatrix();
    glTranslatef(size,0,0);
    glRotatef(90,0,0,-1);
    drawCylinder(size/15, size/128, size/5,16,2);
    glPopMatrix();
    //y green
    glColor3f(0,1.0,0);
    glBegin(GL_LINES);
    glVertex3f(0,0,0);
    glVertex3f(0,size,0);
    glEnd();
    glPushMatrix();
    glTranslatef(0,size,0);
    drawCylinder(size/15, size/128, size/5,16,2);
    //glutSolidCone(size/12, size/4, 16,16);
    glPopMatrix();
    //z blue
    glColor3f(0,0,1.0);
    glBegin(GL_LINES);
    glVertex3f(0,0,0);
    glVertex3f(0,0,size);
    glEnd();
    glPushMatrix();
    glTranslatef(0,0,size);
    glRotatef(90, 1, 0, 0);
    drawCylinder(size/15, size/128, size/5,16,2);
    glPopMatrix();
    //
    glColor3f(1.0,1.0,1.0);
}

void KLGLUtil::drawCube(double size)
{
    static const GLfloat vertex_list[][3] = {
         -0.5f, -0.5f, -0.5f,
          0.5f, -0.5f, -0.5f,
         -0.5f,   0.5f, -0.5f,
          0.5f,   0.5f, -0.5f,
         -0.5f, -0.5f,   0.5f,
          0.5f, -0.5f,   0.5f,
         -0.5f,   0.5f,   0.5f,
          0.5f,   0.5f,   0.5f,
    };

    static const GLint index_list[][4] = {
         0, 2, 3, 1,
         0, 4, 6, 2,
         0, 1, 5, 4,
         4, 5, 7, 6,
         1, 3, 7, 5,
         2, 6, 7, 3,
    };

    glBegin(GL_QUADS);
    for (int i=0; i<6; ++i)
        for (int j=0; j<4; ++j){
             glVertex3f(
                         vertex_list[index_list[i][j]][0] * size,
                     vertex_list[index_list[i][j]][1] * size,
                     vertex_list[index_list[i][j]][2] * size
                     );
        }
    glEnd();
}

void KLGLUtil::drawBox(double length, double width, double height)
{
    glPushMatrix();
    glScalef(length, height, width);
    KLGLUtil::drawCube(1.0);
    glPopMatrix();
}

void KLGLUtil::drawWiredPlane(double length, double width, int divideL, int divideW)
{
    glBegin(GL_LINE_LOOP);
    glVertex3f(-length / 2, 0, -width / 2);
    glVertex3f(length / 2, 0, -width / 2);
    glVertex3f(length / 2, 0, width / 2);
    glVertex3f(-length / 2, 0, width / 2);
    glEnd();

    glBegin(GL_LINES);
    for(int i = 1; i < divideL; i++){
        glVertex3f(-length / 2 + i * length / divideL, 0, -width / 2);
        glVertex3f(-length / 2 + i * length / divideL, 0, width / 2);
    }
    glEnd();

    glBegin(GL_LINES);
    for(int i = 1; i < divideW; i++){
        glVertex3f(-length / 2, 0, -width / 2 + i * width / divideW);
        glVertex3f(length / 2, 0, -width / 2 + i * width / divideW);
    }
    glEnd();
}

void KLGLUtil::drawCylinder(double baseRadius, double topRadius, double height, int divideR, int divideH)
{
    GLUquadricObj   *quadratic;
    quadratic=gluNewQuadric();
    gluQuadricNormals(quadratic,   GLU_SMOOTH);
    gluQuadricTexture(quadratic,   GL_TRUE);
    glPushMatrix();
        glRotatef(-90, 1, 0, 0);
        gluCylinder(quadratic,baseRadius,topRadius,height,divideR,divideH);
        int STEP = divideR;
        GLfloat rbase=baseRadius;
        GLfloat rtop=topRadius;
        glBegin(GL_POLYGON);

        GLfloat n1[]={0.0,0.0,-1.0};
        glNormal3fv(n1);
        for(int i=0;i<=STEP+10;i++){
            glVertex3f(sin(360/STEP*i*M_PI/180)*rbase,cos(360/STEP*i*M_PI/180)*rbase,0.0);
        }
        glEnd();
        glBegin(GL_POLYGON);

        GLfloat n2[]={0.0,0.0,1.0};
        glNormal3fv(n2);
        for(int i=0;i<=STEP+10;i++){
            glVertex3f(sin(360/STEP*i*M_PI/180)*rtop,cos(360/STEP*i*M_PI/180)*rtop,height);
        }
        glEnd();
    glPopMatrix();
}

//adopted here: http://www.cnblogs.com/xpvincent/archive/2013/02/16/2913537.html
void KLGLUtil::drawHeavySpring(QVector3D ptStart, QVector3D ptEnd, double Radius, double TubeRadius, double Coils, int Rings, int Sides)
{
    double sideDelta = 2.0 * M_PI / Sides;
    double ringDelta = 2.0 * M_PI / Rings;
    double ringHeight;
    double theta = 0;
    double cosTheta = 1.0;
    double sinTheta = 0.0;
    double z;
    double phi, sinPhi, cosPhi;
    double dist;
    double springLength;

    QVector3D ptOldFrame, ptNewFrame;
    QVector3D rotationAxis;
    QMatrix4x4 rotationMatrix;
    double rotationAngle;

    ptOldFrame = QVector3D(0, 0, 1);
    ptNewFrame = ptEnd - ptStart;
    springLength = qSqrt(ptNewFrame.x() * ptNewFrame.x()
        + ptNewFrame.y() * ptNewFrame.y()
        + ptNewFrame.z() * ptNewFrame.z());
    ringHeight = springLength / Coils / Rings;

    QVector3D crossOldNew = QVector3D::crossProduct(ptOldFrame, ptNewFrame);
    rotationAngle = qAsin((crossOldNew.length() / (ptOldFrame.length() * ptNewFrame.length())));

    rotationAxis = crossOldNew;
    rotationMatrix.rotate(rotationAngle * 180 / M_PI, rotationAxis);

    glPushMatrix();
    glTranslatef(ptStart.x(), ptStart.y(), ptStart.z());
    float *temp = rotationMatrix.data();
    GLdouble m[16];
    for(int i = 0; i < 16; i++){
        m[i] = temp[i];
    }
    glMultMatrixd(m);

    z = 0;
    for (int i = 0; i < Coils; i++)
    {
        for (int j = 0; j < Rings; j++)
        {
            double theta1 = theta + ringDelta;
            double cosTheta1 = qCos(theta1);
            double sinTheta1 = qSin(theta1);

            glBegin(GL_QUAD_STRIP);
            phi = 0;
            for (int k = 0; k <= Sides; k++)
            {
                phi = phi + sideDelta;
                cosPhi = qCos(phi);
                sinPhi = qSin(phi);
                dist = Radius + (TubeRadius * cosPhi);
                GLfloat n1[]={cosTheta * (dist - Radius), sinTheta * (dist - Radius), TubeRadius * sinPhi};
                glNormal3fv(n1);
                glVertex3d(cosTheta * dist, sinTheta * dist, z + TubeRadius * sinPhi);
                GLfloat n2[]={cosTheta1 * (dist - Radius), sinTheta1 * (dist - Radius), TubeRadius * sinPhi};
                glNormal3fv(n2);
                glVertex3d(cosTheta1 * dist, sinTheta1 * dist, z +ringHeight+ TubeRadius * sinPhi);
            }
            glEnd();
            theta = theta1;
            cosTheta = cosTheta1;
            sinTheta = sinTheta1;

            z += ringHeight;
        }
    }
    glPopMatrix();
}

void KLGLUtil::drawCircle(double radius, int slices)
{
    glBegin(GL_LINE_LOOP);
    for(int i = 0; i < slices; i++){
        glVertex3d(radius * qCos((360.0 / slices) * i / 180 * M_PI), radius * qSin((360.0 / slices) * i / 180 * M_PI), 0);
    }
    glEnd();
}

void KLGLUtil::drawParabola(double a, double b, double c, double startX, double endX, int slices)
{
    glBegin(GL_LINE_STRIP);
    for(int i = 0; i < slices; i++){
        double x = startX + i * (endX - startX) / slices;
        glVertex3d(x, a*x*x + b * x + c, 0);
    }
    glEnd();
}
