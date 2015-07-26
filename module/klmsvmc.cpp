#include "klmsvmc.h"

KLMSvmc::KLMSvmc(QWidget *parentMain, QWidget *parentCtrlPanel):
    KLMBase(parentMain, parentCtrlPanel)
{
    ui_viewBigSize = QSize(768, 432);
    ui_viewSmallSize = QSize(384, 216);
    brush = QBrush();
    pen = QPen();

    colorWidth = 0;
    colorHeight = 0;
    colorImage = QImage();
    faceData = NULL;
    faceAlignment = NULL;
    faceAlignmentColor = NULL;
    faceAlignmentCam = NULL;
    faceModel = NULL;
    headPivot = NULL;
    headPivotColor = NULL;
    faceBound = NULL;
    faceOrient = NULL;
    faceModelVC = 0;
    inValidFrameCount = 0;

    initUI();

//    svm_problem prob;
//    prob.l = 100;
//    prob.y = new double[prob.l];
//    svm_parameter param;

//    // default values
//    param.svm_type = C_SVC;
//    param.kernel_type = RBF;
//    param.degree = 3;
//    param.gamma = 0;
//    param.coef0 = 0;
//    param.nu = 0.5;
//    param.cache_size = 100;
//    param.C = 1;
//    param.eps = 1e-3;
//    param.p = 0.1;
//    param.shrinking = 1;
//    param.probability = 0;
//    param.nr_weight = 0;
//    param.weight_label = NULL;
//    param.weight = NULL;

    //svm_model *model = svm_train(&prob, &param);

}

KLMSvmc::~KLMSvmc()
{

}

void KLMSvmc::init()
{
    KLMBase::init();

    emit _setTitle("Kinect Face Lab - SVM Smile Cam", WINDOW::MAIN);
    emit _setUI(ui_mainWgt, WINDOW::MAIN);
    emit _setUI(ui_ctrlPanelWgt, WINDOW::CTRL_PANEL);

    connect(ctrler, SIGNAL(_readerInfo(bool, unsigned int)), this, SLOT(h_kinectReaderInfo(bool,uint)));
    connect(ctrler, SIGNAL(_available(bool)), this, SLOT(h_kinectAvailable(bool)));
    connect(ctrler, SIGNAL(_data(void*,uint)), this, SLOT(h_kinectData(void*,uint)));

    if(ctrler->isAvailable()){
        ctrler->startStream(KLController::SOURCE_TYPE::S_COLOR | KLController::SOURCE_TYPE::S_FACE_HD);
    }else{
        ctrler->open();
    }
}

void KLMSvmc::initGL(int targetID)
{
    switch(targetID){
    case 2:
        glEnable(GL_DEPTH_TEST);
        glShadeModel(GL_SMOOTH);
        glEnable(GL_MULTISAMPLE);

        glEnable(GL_LIGHTING);
        glEnable(GL_LIGHT0);
//        static GLfloat lightPosition[4] = { 10, 10, 10.0, 1.0 };
//        GLfloat lightDiffuse[]   = {1.0, 1.0, 1.0, 1.0};
//        GLfloat lightSpecular[] = {0.5, 0.5, 0.5, 1.0};
//        glLightfv(GL_LIGHT1, GL_POSITION, lightPosition);
//        glLightfv(GL_LIGHT1, GL_DIFFUSE, lightDiffuse);
//        glLightfv(GL_LIGHT1, GL_SPECULAR, lightSpecular);

        glEnable(GL_COLOR_MATERIAL);
        glClearColor(0, 0.22,0.4,1);
        //qglClearColor(QColor(0,72,102));
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glLoadIdentity();
        viewResize3D(2, ui_pointView->width(), ui_pointView->height());
        break;
    }
}

void KLMSvmc::paint2D(int targetID, QPainter *painter, QPaintEvent *event)
{
    switch(targetID){
    case 0:
        if(colorImage.height()){
            painter->drawPixmap(0,0, QPixmap::fromImage(colorImage.scaledToHeight(ui_userView->height())));
        }
        break;
    case 1:

        pen.setWidth(2);
        pen.setColor(QColor("blue"));
        painter->setPen(pen);



        if(colorImage.height()){
            painter->drawPixmap(0,0, QPixmap::fromImage(colorImage.scaledToHeight(ui_frameView->height())));
        }
        if(faceBound && faceBound->Left){
            double rate = ui_frameView->height() / (colorHeight?colorHeight:1.0);
            painter->drawRect(
                        faceBound->Left * rate,
                        faceBound->Top * rate,
                        (faceBound->Right - faceBound->Left) * rate,
                        (faceBound->Bottom - faceBound->Top) * rate);
        }



        if(faceAlignmentColor){
            painter->setRenderHint(QPainter::Antialiasing);

            double rate = ui_pointView->height() / (colorHeight?colorHeight:1.0);
            double centerX = ui_pointView->width() / 2.0;
            double centerY = ui_pointView->height() / 2.0;

            for(UINT j = 0; j < faceModelVC; j++){
                if(j == 91 ||
                   j == 687 ||
                   j == 1072||
                        j == 10 ||
                        j == 412 ||
                        j == 933 ||
                        j == 458 ||
                        j == 674 ||
                        j == 156 ||
                        j == 783){
                    pen.setWidth(3);
                    pen.setColor(QColor("white"));
                    painter->setPen(pen);
                }else{
                    pen.setWidth(1);
                    pen.setColor(QColor("red"));
                    painter->setPen(pen);
                }

                painter->drawPoint((faceAlignmentColor[j].X) * rate,
                                   (faceAlignmentColor[j].Y) * rate );
            }
//            if(faceData && faceData[i] && !faceData[i]->isValid){
//                inValidFrameCount++;
//                if(inValidFrameCount > 2){
//                    delete faceAlignmentColor;
//                    faceAlignmentColor = NULL;
//                    faceBound->Left = 0;

//                    inValidFrameCount = 0;
//                }
//            }
        }
        break;
    case 3:

        painter->fillRect(0,0,ui_viewSmallSize.width(), ui_viewSmallSize.height(), QColor("black"));

        HRESULT hr = E_PENDING;

        for(int i = 0; i < BODY_COUNT; i++){

            if(!faceAlignment){
                hr = CreateFaceAlignment(&(faceAlignment));
            }
            if(SUCCEEDED(hr) && !faceModel){
                float sd[FaceShapeDeformations_Count] = {0.0f};
                hr = CreateFaceModel(1.0f, FaceShapeDeformations::FaceShapeDeformations_Count, sd, &faceModel);
            }
            if(SUCCEEDED(hr) && !faceModelVC){
                hr = GetFaceModelVertexCount(&faceModelVC);
            }
            if(faceData && faceData[i]->isValid){
                qDebug()<<"P3"<<faceData[i]->isValid<<i;
            }

            if(faceData && faceData[i]->isValid && faceAlignment && faceModel && faceModelVC){

                hr = faceData[i]->frameHD->GetAndRefreshFaceAlignmentResult(faceAlignment);

                if(SUCCEEDED(hr)){
                    if(!headPivot){
                        headPivot = new CameraSpacePoint;
                    }
                    hr = faceAlignment->get_HeadPivotPoint(headPivot);
                }
                //std::cout<<std::hex<<hr<<std::endl;
                if(SUCCEEDED(hr)){
                    if(!faceBound){
                        faceBound = new RectI;
                    }
                    hr = faceAlignment->get_FaceBoundingBox(faceBound);
                }
                if(SUCCEEDED(hr)){
                    if(!faceOrient){
                        faceOrient = new Vector4;
                    }
                    hr = faceAlignment->get_FaceOrientation(faceOrient);
                }

                if(SUCCEEDED(hr)){
                    if(!faceAlignmentCam){
                        faceAlignmentCam = new CameraSpacePoint[faceModelVC];
                    }
                    hr = faceModel->CalculateVerticesForAlignment(faceAlignment, faceModelVC, faceAlignmentCam);
                }
                if(SUCCEEDED(hr)){
//                    for(UINT j = 0; j < faceModelVC; j++){
//                        v[j].X -= headPivot->X;
//                        v[j].Y -= headPivot->Y;
//                        v[j].Z -= headPivot->Z;
//                    }
                    if(!faceAlignmentColor){
                        faceAlignmentColor = new ColorSpacePoint[faceModelVC];
                    }
                    if(!headPivotColor){
                        headPivotColor = new ColorSpacePoint;
                    }
                    hr = ctrler->getCoordMapper()->MapCameraPointsToColorSpace(faceModelVC, faceAlignmentCam, faceModelVC, faceAlignmentColor);
                    hr = ctrler->getCoordMapper()->MapCameraPointToColorSpace(*headPivot, headPivotColor);
                }
            }



            painter->setRenderHint(QPainter::Antialiasing);

            double rate = ui_pointView->height() / (colorHeight?colorHeight:1.0);
            double centerX = ui_pointView->width() / 2.0;
            double centerY = ui_pointView->height() / 2.0;

            if(faceAlignmentColor){
                for(UINT j = 0; j < faceModelVC; j++){
                    if(j == 91 ||
                       j == 687 ||
                       j == 1072||
                            j == 10 ||
                            j == 412 ||
                            j == 933 ||
                            j == 458 ||
                            j == 674 ||
                            j == 156 ||
                            j == 783){
                        pen.setWidth(3);
                        pen.setColor(QColor("white"));
                        painter->setPen(pen);
                    }else{
                        pen.setWidth(1);
                        pen.setColor(QColor("red"));
                        painter->setPen(pen);
                    }

                    painter->drawPoint((faceAlignmentColor[j].X - headPivotColor->X ) * rate * 2 + centerX,
                                       (faceAlignmentColor[j].Y - headPivotColor->Y) * rate * 2 + centerY);
                }
                if(faceData && faceData[i] && !faceData[i]->isValid){
                    inValidFrameCount++;
                    if(inValidFrameCount > 2){
                        delete faceAlignmentColor;
                        faceAlignmentColor = NULL;
                        faceBound->Left = 0;

                        inValidFrameCount = 0;
                    }
                }
            }
            if(faceData && faceData[i] && faceData[i]->isValid){
                break;
            }
        }

        break;
    }

}

void KLMSvmc::paint3D(int targetID)
{
//    ui_pointView->makeCurrent();
//    int w = ui_pointView->width();
//    int h = ui_pointView->height();
//    glViewport(0,0,(GLsizei)w,(GLsizei)h);
//    glMatrixMode(GL_PROJECTION);
//    glLoadIdentity();
//    gluPerspective(90, w / double(h), 0.1, 15000);
//    glMatrixMode(GL_MODELVIEW);
    switch(targetID){
    case 2:
        glMatrixMode(GL_MODELVIEW);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable( GL_PROGRAM_POINT_SIZE );
        glPointSize(1);
        glLoadIdentity();
        gluLookAt(0,0, 3, 0,0,0, 0,1,0);



        HRESULT hr = E_PENDING;

        for(int i = 0; i < BODY_COUNT; i++){

            if(!faceAlignment){
                hr = CreateFaceAlignment(&(faceAlignment));
            }
            if(SUCCEEDED(hr) && !faceModel){
                float sd[FaceShapeDeformations_Count] = {0.0f};
                hr = CreateFaceModel(1.0f, FaceShapeDeformations::FaceShapeDeformations_Count, sd, &faceModel);
            }
            if(SUCCEEDED(hr) && !faceModelVC){
                hr = GetFaceModelVertexCount(&faceModelVC);
            }

            if(faceData && faceData[i]->isValid && faceAlignment && faceModel && faceModelVC){

                hr = faceData[i]->frameHD->GetAndRefreshFaceAlignmentResult(faceAlignment);

                if(SUCCEEDED(hr)){
                    if(!headPivot){
                        headPivot = new CameraSpacePoint;
                    }
                    hr = faceAlignment->get_HeadPivotPoint(headPivot);
                }
                //std::cout<<std::hex<<hr<<std::endl;
                if(SUCCEEDED(hr)){
                    if(!faceBound){
                        faceBound = new RectI;
                    }
                    hr = faceAlignment->get_FaceBoundingBox(faceBound);
                }
                if(SUCCEEDED(hr)){
                    if(!faceOrient){
                        faceOrient = new Vector4;
                    }
                    hr = faceAlignment->get_FaceOrientation(faceOrient);
                }

                if(SUCCEEDED(hr)){

                    if(!faceAlignmentCam){
                        faceAlignmentCam = new CameraSpacePoint[faceModelVC];
                    }
                    hr = faceModel->CalculateVerticesForAlignment(faceAlignment, faceModelVC, faceAlignmentCam);
                }
                if(SUCCEEDED(hr)){
//                    for(UINT j = 0; j < faceModelVC; j++){
//                        v[j].X -= headPivot->X;
//                        v[j].Y -= headPivot->Y;
//                        v[j].Z -= headPivot->Z;
//                    }
                    if(!faceAlignmentColor){
                        faceAlignmentColor = new ColorSpacePoint[faceModelVC];
                    }
                    if(!headPivotColor){
                        headPivotColor = new ColorSpacePoint;
                    }
                    hr = ctrler->getCoordMapper()->MapCameraPointsToColorSpace(faceModelVC, faceAlignmentCam, faceModelVC, faceAlignmentColor);
                    hr = ctrler->getCoordMapper()->MapCameraPointToColorSpace(*headPivot, headPivotColor);
                }
            }

            //if(faceAlignmentColor && faceAlignmentCam[0].X > 1e-4 && faceAlignmentCam[0].X < 1e9){
            if(faceAlignmentColor){
                qDebug()<<"sdf";
                //qDebug()<<faceAlignmentCam[0].Z;
                QQuaternion quaternion(faceOrient->w, faceOrient->x, faceOrient->y, faceOrient->z);
                //quaternion.normalize();
                //quaternion = quaternion.conjugate();
                QMatrix4x4 matrix;
                matrix.setToIdentity();
                matrix.rotate(quaternion);
                //qDebug()<<matrix;
                GLdouble* glMatrix = new GLdouble[16];
                for(int j = 0; j < 16; j++){
                    glMatrix[j] = matrix(j / 4, j % 4);
                }
                //qDebug()<<quaternion.x();
                glPushMatrix();
                glMultMatrixd(glMatrix);
                glScalef(3,3,3);
                glBegin(GL_POINTS);
                for(UINT j = 0; j < faceModelVC; j++){
                    glVertex3d(faceAlignmentCam[j].X - headPivot->X, faceAlignmentCam[j].Y - headPivot->Y, faceAlignmentCam[j].Z - headPivot->Z);
                }
                glEnd();
                glPopMatrix();
            }


            if(faceData && faceData[i] && faceData[i]->isValid){
                break;
            }
        }

        KLGLUtil::drawAxis(3);
//        glPushMatrix();
//        //glTranslatef(2,0,0);
//        glBegin(GL_POINTS);
//        glVertex3f(1,0,0);
//        glEnd();
//        glPopMatrix();

        //KLGLUtil::drawSphere(1);
        //KLGLUtil::drawHeavySpring(QVector3D(0,0.5,0), QVector3D(0,0,0), 0.05, 0.01);
        break;
    }

}

void KLMSvmc::viewResize3D(int targetID, int w, int h)
{
    switch(targetID){
    case 2:
        glViewport(0,0,(GLsizei)w ,(GLsizei)h );
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluPerspective(20, w / double(h), 0.1, 15000);
        glMatrixMode(GL_MODELVIEW);
    break;
    }
}

void KLMSvmc::h_kinectReaderInfo(bool exist, uint type)
{
    if(exist){
        if(type & KLController::SOURCE_TYPE::S_COLOR){
            IFrameDescription* frameDesc;
            frameDesc = ctrler->getFrameDesc(KLController::SOURCE_TYPE::S_COLOR);
            if(frameDesc){
                frameDesc->get_Height(&colorHeight);
                frameDesc->get_Height(&colorWidth);
            }
        }
    }
}

void KLMSvmc::h_kinectAvailable(bool available)
{
    if(available){

        ctrler->startStream(KLController::SOURCE_TYPE::S_COLOR | KLController::SOURCE_TYPE::S_FACE_HD);

    }else{

    }
}

void KLMSvmc::h_kinectData(void *data, uint type)
{
    if(type & KLController::RESOURCE_TYPE::R_COLOR){
        const QVector<BYTE>* data_ptr = ((QVector<BYTE> *)data);
        colorImage = QImage(data_ptr->data(), 1920, 1080, QImage::Format_RGBA8888_Premultiplied);
        ui_frameView->update();
        ui_pointView->update();
        ui_userView->update();
    }

    if(type & KLController::RESOURCE_TYPE::R_FACE_HD){
        faceData = ((KLFaceData **)data);
    }
}

void KLMSvmc::initUI()
{
    KLMBase::initUI();

    ui_frameView = new KL2DView(1, ui_mainWgt);
    ui_pointView = new KLGLView(2, NULL);
    ui_userView = new KL2DView(0, ui_mainWgt);
    ui_console = new QPlainTextEdit(ui_mainWgt);
    ui_frameView->setModule(this);
    ui_userView->setModule(this);
    ui_pointView->setModule(this);
    ui_frameView->setFixedSize(ui_viewSmallSize);
    ui_pointView->setFixedSize(ui_viewSmallSize);
    ui_userView->setFixedSize(ui_viewBigSize);
    ui_console->setReadOnly(true);
    ui_console->setMinimumHeight(28);
    ui_console->setUndoRedoEnabled(true);
    ui_console->setLineWrapMode(QPlainTextEdit::WidgetWidth);
    ui_console->appendPlainText(tr("Ready"));

//    ui_userView->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
//    ui_frameView->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
//    ui_pointView->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    ui_mainLt->addWidget(ui_frameView, 0, 0, 1, 1);
    ui_mainLt->addWidget(ui_pointView, 1, 0, 1, 1);
    ui_mainLt->addWidget(ui_userView, 0, 1, 2, 1);
    ui_mainLt->addWidget(ui_console, 2, 0, 1, 2);

    //ui_mainWgt->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);

//    ui_frameView->setSizeIncrement(0, 0);
//    ui_pointView->setSizeIncrement(0, 0);

    //connect(ui_frameView, SIGNAL(_resize(QResizeEvent*)), this, SLOT(h_viewResize(QResizeEvent*)));


}
