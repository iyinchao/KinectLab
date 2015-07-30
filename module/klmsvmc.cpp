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
    trainID = -1;
    trainCursor = 0;
    featureMap = NULL;
    trainBuffer = NULL;
    modelStatus = "No model";
    svmModel = NULL;
    frameSmile = 0;
    captureStart = false;
    cameraFlash = 0;
    showToast = 0;

    {
        svmTrainParam.svm_type = C_SVC;
        svmTrainParam.kernel_type = RBF;
        svmTrainParam.degree = 3;
        svmTrainParam.gamma = 1/30.0; //
        svmTrainParam.coef0 = 0;
        svmTrainParam.nu = 0.5;
        svmTrainParam.cache_size = 100;
        svmTrainParam.C = 100000000;
        svmTrainParam.eps = 0.001;
        svmTrainParam.p = 0.1;
        svmTrainParam.shrinking = 1;
        svmTrainParam.probability = 0;
        svmTrainParam.nr_weight = 0;
        svmTrainParam.weight_label = NULL;
        svmTrainParam.weight = NULL;
    }

    for(int i = 0; i <= TRAIN_PHASE::N_NE; i++){
        trainSet[i] = new KLSvmcTrain();
        trainSet[i]->id = i;
        if(i <= P_MO3_3){
            trainSet[i]->label = 1;
        }else{
            trainSet[i]->label = -1;
        }
        switch(i){
        case P_MC_1: //positive/mouth closed/smile min
            trainSet[i]->toolTip = "Positive / Mouth closed / Smile min";
            break;
        case P_MC_2: //positive/mouth closed/smile medium
            trainSet[i]->toolTip = "Positive / Mouth closed / Smile medium";
            break;
        case P_MC_3: //positive/mouth closed/smile max
            trainSet[i]->toolTip = "Positive / Mouth closed / Smile max";
            break;
        case P_MO1_1: //positive/mouth opened/min/smile min
            trainSet[i]->toolTip = "Positive / Mouth open min / Smile min";
            break;
        case P_MO1_2: //positive/mouth opened/min/smile med
            trainSet[i]->toolTip = "Positive / Mouth open min / Smile medium";
            break;
        case P_MO1_3: //positive/mouth opened/min/smile max
            trainSet[i]->toolTip = "Positive / Mouth open min / Smile max";
            break;
        case P_MO2_1: //positive/mouth opened/med/smile min
            trainSet[i]->toolTip = "Positive / Mouth open medium / Smile min";
            break;
        case P_MO2_2: //positive/mouth opened/med/smile med
            trainSet[i]->toolTip = "Positive / Mouth open medium / Smile medium";
            break;
        case P_MO2_3: //positive/mouth opened/med/smile max
            trainSet[i]->toolTip = "Positive / Mouth open medium / Smile max";
            break;
        case P_MO3_1: //positive/mouth opened/max/smile min
            trainSet[i]->toolTip = "Positive / Mouth open max / Smile min";
            break;
        case P_MO3_2: //positive/mouth opened/max/smile med
            trainSet[i]->toolTip = "Positive / Mouth open max / Smile medium";
            break;
        case P_MO3_3: //positive/mouth opened/max/smile max
            trainSet[i]->toolTip = "Positive / Mouth open max / Smile max";
            break;
        case N_AU27_1: //mouth stretch(张嘴)/ narrow / min
            trainSet[i]->toolTip = "Negative / Mouth stretch / narrow / min";
            break;
        case N_AU27_2: //mouth stretch(张嘴)/ narrow / wmax
            trainSet[i]->toolTip = "Negative / Mouth stretch / narrow / max";
            break;
        case N_AU27_3: //mouth stretch(张嘴)/ wide / min
            trainSet[i]->toolTip = "Negative / Mouth stretch / wide / min";
            break;
        case N_AU27_4: //mouth stretch(张嘴)/ wide / max
            trainSet[i]->toolTip = "Negative / Mouth stretch / wide / max";
            break;
        case N_AU20_1: //lip stretcher(伸长嘴唇)/min
            trainSet[i]->toolTip = "Negative / lip stretcher / min";
            break;
        case N_AU20_2: //lip stretcher(伸长嘴唇)/max
            trainSet[i]->toolTip = "Negative / lip stretcher / max";
            break;
        case N_AU15_1: //lip corner depressor(嘴巴闭上，嘴角向下)/min
            trainSet[i]->toolTip = "Negative / lip corner depressor / min";
            break;
        case N_AU15_2: //lip corner depressor(嘴巴闭上，嘴角向下)/max
            trainSet[i]->toolTip = "Negative / lip corner depressor / max";
            break;
        case N_AU18: //lip pucker(撅嘴)
            trainSet[i]->toolTip = "Negative / lip pucker";
            break;
        case N_AU10: //upper lip raiser(抬上嘴唇)
            trainSet[i]->toolTip = "Negative / upper lip raiser";
            break;
        case N_AU28: //lip suck(吸嘴唇)
            trainSet[i]->toolTip = "Negative / lip suck";
            break;
        case N_NE: //Neutral face
            trainSet[i]->toolTip = "Negative / Neutral expression";
            break;
        }
    }

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

const int KLMSvmc::featurePoints[10] = {91, 687, 1072, 10, 412, 933, 458, 674, 156, 783};

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
    case 3:
        glEnable(GL_DEPTH_TEST);
        glShadeModel(GL_SMOOTH);
        glEnable(GL_POINT_SMOOTH);
        glEnable(GL_MULTISAMPLE);
        glCullFace (GL_BACK);
        glEnable (GL_CULL_FACE);
        glEnable( GL_PROGRAM_POINT_SIZE );
        glEnable(GL_LIGHTING);
        glEnable(GL_LIGHT0);
//        static GLfloat lightPosition[4] = { 10, 10, 10.0, 1.0 };
//        GLfloat lightDiffuse[]   = {1.0, 1.0, 1.0, 1.0};
//        GLfloat lightSpecular[] = {0.5, 0.5, 0.5, 1.0};
//        glLightfv(GL_LIGHT1, GL_POSITION, lightPosition);
//        glLightfv(GL_LIGHT1, GL_DIFFUSE, lightDiffuse);
//        glLightfv(GL_LIGHT1, GL_SPECULAR, lightSpecular);

        glEnable(GL_COLOR_MATERIAL);
        glClearColor(0, 0.22f, 0.4f, 1);
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
        if(cameraFlash > 0){

            //brush.setColor(QColor(255, 255, 255, double(cameraFlash) / 100 * 255));
            //painter->setBrush(brush);
            //qDebug()<<double(cameraFlash) / 100 * 255;
            painter->fillRect(0,0,ui_userView->width(), ui_userView->height(), QBrush(QColor(255, 255, 255, double(cameraFlash) / 100 * 255)));
            cameraFlash -= 10;
        }
        if(showToast){
            painter->setRenderHint(QPainter::Antialiasing);
            QPainterPath path;
            path.addRoundedRect(QRectF(ui_viewBigSize.width() / 2.0 - 100, 300, 100 * 2, 50), 25, 25);
            painter->fillPath(path, QBrush(QColor(0,0,0,128)));
            painter->setRenderHint(QPainter::Antialiasing, false);
            painter->setPen(Qt::white);
            painter->setFont(QFont(tr("Microsoft YaHei"),11));
            painter->drawText(310,330, tr("Smile face detected"));
            showToast = false;
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
                if(featureMap[j]){
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
    case 3:

        glMatrixMode(GL_MODELVIEW);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glLoadIdentity();
        gluLookAt(0, 0, 1, 0,0,0, 0,1,0);
        //KLGLUtil::drawSphere(0.1, 4, 4);
        if(trainID != -1){
            for(int i = 0; i < trainDataDim; i+=3){
                glPushMatrix();
                glTranslatef((trainSet[trainID]->data[0])[i] / 100, (trainSet[trainID]->data[0])[i+1] / 100, (trainSet[trainID]->data[0])[i+2] / 100);
                KLGLUtil::drawSphere(0.01, 4, 4);
                glPopMatrix();
            }
        }
        KLGLUtil::drawAxis(3);
        break;
    case 2:
        glMatrixMode(GL_MODELVIEW);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glLoadIdentity();
        gluLookAt(0, 0, 1, 0,0,0, 0,1,0);

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
                if(!featureMap){
                    featureMap = new bool[faceModelVC];
                    for(UINT j = 0; j < faceModelVC; j++){
                        featureMap[j] = false;
                    }
                    for(int j = 0; j < sizeof(featurePoints)/sizeof(int); j++){
                        featureMap[featurePoints[j]] = true;
                    }
                }
                if(!trainBuffer){
                    trainBuffer = new double[trainDataRepeat * trainDataDim];
                }
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
                    if(qAbs(faceAlignmentCam[0].X) > 1e-5 && qAbs(faceAlignmentCam[0].X) < 1e20){
                        if(!faceAlignmentColor){
                            faceAlignmentColor = new ColorSpacePoint[faceModelVC];
                        }
                        if(!headPivotColor){
                            headPivotColor = new ColorSpacePoint;
                        }
                        hr = ctrler->getCoordMapper()->MapCameraPointsToColorSpace(faceModelVC, faceAlignmentCam, faceModelVC, faceAlignmentColor);
                        hr = ctrler->getCoordMapper()->MapCameraPointToColorSpace(*headPivot, headPivotColor);
                    }else{
                        faceAlignmentColor = NULL;
                    }
                }
            }

            if(faceAlignmentColor){
                //glBegin(GL_POINTS);
                //qDebug()<<"sdf";
                svm_node* capture = new svm_node[trainDataDim + 1];
                int capture_cursor = 0;
                for(UINT j = 0; j < faceModelVC; j++){
                    QVector3D vector(faceAlignmentCam[j].X - headPivot->X, faceAlignmentCam[j].Y - headPivot->Y, faceAlignmentCam[j].Z - headPivot->Z);
                    QQuaternion quaternion(faceOrient->w, faceOrient->x, faceOrient->y, faceOrient->z);
                    QMatrix4x4 matrix;
                    matrix.rotate(quaternion);
                    vector = vector * matrix;

                    double radius = 0.002;

                    if(featureMap[j]){
                        //glPointSize(4);
                        radius = 0.005;
                        glColor3d(1, 1, 1);

                        if(trainID != -1){
                            int total = trainDataRows * trainDataRepeat * trainDataDim;
                            //trainBuffer[trainCursor % trainDataRepeat]
                            //if(trainCursor)
        //                    for(int i = 0; i < sizeof(featurePoints)/sizeof(int); i++){
        //                        trainSet[trainID]->data[trainCursor / trainDataDim][trainCursor % trainDataDim] = 0;

        //                    }


                            //trainSet[trainID]->data[trainCursor / (trainDataRepeat * trainDataDim)][trainCursor % (trainDataRows * trainDataRepeat)]

                            //trainCursor += 3;

                            trainBuffer[trainCursor % (trainDataRepeat * trainDataDim)] = vector.x() * 100;
                            trainBuffer[(trainCursor % (trainDataRepeat * trainDataDim)) + 1] = vector.y() * 100;
                            trainBuffer[(trainCursor % (trainDataRepeat * trainDataDim)) + 2] = vector.z() * 100;
                            //qDebug()<<trainCursor % (trainDataRepeat * trainDataDim)<<vector.x();

                            trainCursor += 3;
                            ui_trainPG->setValue(double(trainCursor) / total * 100);

                            if(trainCursor % (trainDataRepeat * trainDataDim) == 0){

                                for(int l = 0; l < trainDataDim; l++){
                                    double val = 0;
                                    for(int k = 0; k < trainDataRepeat; k++){
                                        val += trainBuffer[k*trainDataDim + l];
                                    }
                                    val /= trainDataRepeat;
                                    (trainSet[trainID]->data[trainCursor / (trainDataRepeat * trainDataDim) - 1])[l] = val;
                                    //qDebug()<<val;
                                }
                            }

                            if(trainCursor >= total){
                                //train done
                                trainSet[trainID]->complete = true;
                                //qDebug() << (trainSet[trainID]->data[0])[3] <<(trainSet[trainID]->data[0])[9];

                                ui_testView->update();

                                trainDone(trainID);
                            }
                        }

                        if(captureStart && svmModel){
                            capture[capture_cursor].index = capture_cursor + 1;
                            capture[capture_cursor].value = vector.x() * 100;
                            capture[capture_cursor + 1].index = capture_cursor + 2;
                            capture[capture_cursor + 1].value = vector.y() * 100;
                            capture[capture_cursor + 2].index = capture_cursor + 3;
                            capture[capture_cursor + 2].value = vector.z() * 100;
                            capture_cursor += 3;
                        }

                    } else {
                        //glPointSize(4);
                        glColor3d(0.5, 0, 0);
                    }

                    glPushMatrix();
                    glTranslatef(vector.x(), vector.y(), vector.z());
                    KLGLUtil::drawSphere(radius, 4, 4);
                    glPopMatrix();

                    //glVertex3d(vector.x(), vector.y(), vector.z());
                    //store data
                    //TRAIN_PHASE::POSITIVE;
                }



                if(captureStart && svmModel){
                    capture[trainDataDim].index = -1;
//                    for(int i = 0; i < trainDataDim + 1; i ++){
//                        std::cout<<capture[i].index<<":"<<capture[i].value<<" ";
//                    }
//                    std::cout<<std::endl;
                    double result = svm_predict(svmModel, capture);
                    if(result > 0){
                        if(frameSmile < frameSmileLimit){
                            showToast = true;
                            frameSmile++;
                        }else{
                            if(!colorImage.isNull()){
//                                (QTime::currentTime()).toString("hh:mm:ss.zzz")
                                QString path = tr(".//captured//smile-img-%1.png").arg((QTime::currentTime()).toString("hh-mm-ss-zzz"));
                                QImage tempImg;
                                tempImg = colorImage.copy(0,0,1920,1080).scaledToHeight(320);
                                tempImg.save(path);
                                //qDebug()<<path;
                                qDebug()<<"[KLMSvmc] Captured image:"<<path;
                            }
                            frameSmile = 0;
                            cameraFlash = 90;
                        }
                    }else{
                        if(frameSmile > 0){
                            frameSmile--;
                        }
                    }
                    ui_capturePG->setValue(double(frameSmile) / frameSmileLimit * 100);
                }
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
    case 3:
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
        //ui_testView->update();
        ui_userView->update();
    }

    if(type & KLController::RESOURCE_TYPE::R_FACE_HD){
        faceData = ((KLFaceData **)data);
    }
}

void KLMSvmc::h_trainBt(bool start)
{
    int id = -1;
    for(int i = 0; i <= TRAIN_PHASE::N_NE; i++){
        if(sender() == ui_trainBt[i]){
            id = i;
            break;
        }
    }
    if(id == -1) return;

    if(start){
        trainStart(id);
        for(int i = 0; i <= TRAIN_PHASE::N_NE; i++){
            if(ui_trainBt[i]->isChecked() && i != id){
                //other traning is doing
                trainStop(i);
            }
        }
    }else{
        trainStop(id);
    }
}

void KLMSvmc::h_trainSave()
{

//    for(int i = 0; i < TRAIN_PHASE::N_NE; i++){
//        //ui_trainBt[i]
//    }

    QFileDialog dialog(p_main,
                       tr("Save train data"),
                       "./");
    QStringList mimeTypeFilters;
    mimeTypeFilters << "text/plain";
    dialog.setMimeTypeFilters(mimeTypeFilters);
    dialog.selectMimeTypeFilter("text/plain");
    dialog.setAcceptMode(QFileDialog::AcceptSave);

    QString path;

    if (dialog.exec() == QDialog::Accepted) {
        path = dialog.selectedFiles().value(0);
    }

    if(path.isEmpty()){
        return;
    }

    QFile file(path);
    if(file.open(QIODevice::ReadWrite)){
        //qDebug()<<path;
        QTextStream out(&file);
        out.setRealNumberNotation(QTextStream::FixedNotation);
        out.setRealNumberPrecision(5);

        for(int i = 0; i <= N_NE; i++){
            for(int j = 0; j < trainDataRows; j++){
                out<<trainSet[i]->label<<" ";
                for(int k = 0; k < trainDataDim; k++){
                    out<<(k+1)<<":"<<(trainSet[i]->data[j])[k]<<" ";
                }
                out<<endl;
            }
        }
    }
}

void KLMSvmc::h_trainLoad()
{
    QFileDialog dialog(p_main,
                       tr("Load train data"),
                       "./");
    QStringList mimeTypeFilters;
    mimeTypeFilters << "text/plain";
    dialog.setMimeTypeFilters(mimeTypeFilters);
    dialog.selectMimeTypeFilter("text/plain");
    dialog.setAcceptMode(QFileDialog::AcceptOpen);

    QString path;

    if (dialog.exec() == QDialog::Accepted) {
        path = dialog.selectedFiles().value(0);
    }

    if(path.isEmpty()){
        return;
    }

    QFile file(path);
    if(file.open(QIODevice::ReadOnly)){
        int cursor = 0;

        for(int i = 0; i <= N_NE; i++){
            trainSet[i]->complete = false;
        }

        while(!file.atEnd()) {
            QString line = file.readLine();
            QStringList fields = line.split(" ");
            if(fields.length() == 1 + trainDataDim + 1){
                if(cursor / trainDataRows <= N_NE){
                    for(int i = 1; i < 1+ trainDataDim; i++){
                        QStringList data = fields[i].split(":");
                        if(data.length() == 2){
                            bool isValid = false;
                            data[1].toDouble(&isValid);
                            if(isValid){
                                (trainSet[cursor / trainDataRows]->data[cursor % trainDataRows])[i-1] = data[1].toDouble();
                            }
                        }
                    }
                }else{
                    break;
                }

                cursor++;

                if(cursor % trainDataRows == 0){
                    trainSet[cursor / trainDataRows - 1]->complete = true;
                    trainTestComplete();
                }
            }
        }
    }
}

void KLMSvmc::h_modelLoadTrain()
{
    QFileDialog dialog(p_main,
                       tr("Load train data"),
                       "./");
    QStringList mimeTypeFilters;
    mimeTypeFilters << "text/plain";
    dialog.setMimeTypeFilters(mimeTypeFilters);
    dialog.selectMimeTypeFilter("text/plain");
    dialog.setAcceptMode(QFileDialog::AcceptOpen);

    QString path;

    if (dialog.exec() == QDialog::Accepted) {
        path = dialog.selectedFiles().value(0);
    }

    if(path.isEmpty()){
        return;
    }

    QFile file(path);
    if(file.open(QIODevice::ReadOnly)){

        int linesCount = 0;
        QList<double> y;
        QList<svm_node*> x;

        while(!file.atEnd()){
            QString line = file.readLine();
            QStringList fields = line.split(" ");
            if(fields.length() == 1 + trainDataDim + 1){
                linesCount++;
            }
        }
        file.reset();
        svm_node *x_space = new svm_node[(trainDataDim + 1) * linesCount];

        int rowCursor = 0;
        while(!file.atEnd()) {
            QString line = file.readLine();
            QStringList fields = line.split(" ");
            if(fields.length() == 1 + trainDataDim + 1){
                bool hasError = false;
                for(int i = 1; i < 1+ trainDataDim; i++){
                    QStringList data = fields[i].split(":");
                    if(data.length() == 2){
                        x_space[(trainDataDim + 1) * rowCursor +(i - 1)].index = data[0].toInt();
                        x_space[(trainDataDim + 1) * rowCursor +(i - 1)].value = data[1].toDouble();
                    }else{
                        hasError = true;
                        break;
                    }
                }
                x_space[(trainDataDim + 1) * rowCursor + trainDataDim].index = -1;
                x_space[(trainDataDim + 1) * rowCursor + trainDataDim].value = 0;
                if(hasError) break;
                //qDebug()<<fields[0].toDouble();
                y.push_back(fields[0].toDouble());
                rowCursor++;
            }
        }

//        for(int i = 0; i < (trainDataDim + 1) * linesCount; i++){
//            //qDebug()<<i<<x_space[i].value;
//        }

        if(linesCount > 0){
            prob.l = linesCount;
            prob.x = new svm_node*[prob.l];
            prob.y = new double[prob.l];
            //qDebug()<<y.length()<<linesCount;
            for(int i = 0; i < prob.l; i++){
                //qDebug()<<i * (trainDataDim + 1);
                prob.x[i] = &(x_space[i * (trainDataDim + 1)]);
                prob.y[i] = y[i];
                //qDebug()<<i<<y[i];
                //qDebug()<<prob.x[i]->index<<prob.x[i]->value;
            }
            //qDebug()<<"start";
            //qDebug()<<svmTrainParam.C;
            svmModel = svm_train(&prob, &svmTrainParam);
            ui_modelSaveBt->setEnabled(true);
            ui_captureBt->setEnabled(true);
            modelStatus = "ready";
            ui_modelStatus->setText(tr("<b>Model status:</b> %1").arg(modelStatus));
            //qDebug()<<"end";
        }
    }
}

void KLMSvmc::h_modelLoad()
{
    QFileDialog dialog(p_main,
                       tr("Load train data"),
                       "./");
    QStringList mimeTypeFilters;
    mimeTypeFilters << "text/plain";
    dialog.setMimeTypeFilters(mimeTypeFilters);
    dialog.selectMimeTypeFilter("text/plain");
    dialog.setAcceptMode(QFileDialog::AcceptOpen);

    QString path;

    if (dialog.exec() == QDialog::Accepted) {
        path = dialog.selectedFiles().value(0);
    }

    if(path.isEmpty()){
        return;
    }

    QFile file(path);
    if(file.open(QIODevice::ReadOnly)){
        svmModel = svm_load_model(path.toUtf8().constData());
        if(svmModel){
            ui_modelSaveBt->setEnabled(true);
            ui_captureBt->setEnabled(true);
            modelStatus = "ready";
            ui_modelStatus->setText(tr("<b>Model status:</b> %1").arg(modelStatus));
        }
    }

    file.close();
}

void KLMSvmc::h_modelSave()
{
    QFileDialog dialog(p_main,
                       tr("Save train data"),
                       "./");
    QStringList mimeTypeFilters;
    mimeTypeFilters << "text/plain";
    dialog.setMimeTypeFilters(mimeTypeFilters);
    dialog.selectMimeTypeFilter("text/plain");
    dialog.setAcceptMode(QFileDialog::AcceptSave);

    QString path;

    if (dialog.exec() == QDialog::Accepted) {
        path = dialog.selectedFiles().value(0);
    }

    if(path.isEmpty()){
        return;
    }

    QFile file(path);
    if(file.open(QIODevice::ReadWrite)){
        if(svmModel){
            svm_save_model(path.toUtf8().constData(), svmModel);
        }
    }
    file.close();
}

void KLMSvmc::h_capture(bool start)
{
    if(!svmModel) return;
    if(start){
        captureStart = true;
    }else{
        captureStart = false;
    }
}

void KLMSvmc::trainStart(int id)
{
    ui_trainBt[id]->setIcon(ico_ing);
    trainID = id;
    trainSet[id]->complete = false;
    trainCursor = 0;
    ui_trainSaveBt->setEnabled(false);
}

void KLMSvmc::trainStop(int id)
{
    ui_trainBt[id]->setChecked(false);
    ui_trainBt[id]->setIcon(ico_no);
    ui_trainPG->setValue(0);
    trainID = -1;
    trainCursor = 0;
}

void KLMSvmc::trainDone(int id)
{
    trainID = -1;
    trainCursor = 0;
    ui_trainBt[id]->setChecked(false);
    ui_trainBt[id]->setIcon(ico_done);
    ui_trainPG->setValue(0);
    trainTestComplete();
}

void KLMSvmc::trainTestComplete()
{
    bool trainComplete = true;
    for(int i = 0; i <= N_NE; i++){
        if(trainSet[i]->complete == false){
            ui_trainBt[i]->setIcon(ico_no);
            trainComplete = false;
        }else{
            ui_trainBt[i]->setIcon(ico_done);
        }
    }
    if(trainComplete){
        ui_trainSaveBt->setEnabled(true);
    }else{
        ui_trainSaveBt->setEnabled(false);
    }
}

void KLMSvmc::drawToast(const QString &text, QPainter *painter, QPaintEvent *event)
{
    //ui_viewBigSize
//    QFontMetrics metrics = QFontMetrics(font());
//    metrics.boundingRect(text);
//    painter->setRenderHint(QPainter::Antialiasing);
//    QPainterPath path;
//    path.addRoundedRect(QRectF(10, 10, 100, 50), 10, 10);
//    painter->fillPath(path, QBrush(QColor(0,0,0,128)));
//    painter->setRenderHint(QPainter::Antialiasing, false);
}

void KLMSvmc::initUI()
{
    KLMBase::initUI();

    ui_frameView = new KL2DView(1, ui_mainWgt);
    ui_pointView = new KLGLView(2, NULL);
    ui_testView = new KLGLView(3, NULL);
    ui_userView = new KL2DView(0, ui_mainWgt);
    ui_console = new QPlainTextEdit(ui_mainWgt);
    ui_frameView->setModule(this);
    ui_userView->setModule(this);
    ui_pointView->setModule(this);
    ui_testView->setModule(this);
    ui_frameView->setFixedSize(ui_viewSmallSize);
    ui_pointView->setFixedSize(ui_viewSmallSize);
    ui_testView->setFixedSize(ui_viewSmallSize);
    ui_userView->setFixedSize(ui_viewBigSize);
    ui_console->setReadOnly(true);
    ui_console->setMinimumHeight(28);
    ui_console->setUndoRedoEnabled(true);
    ui_console->setLineWrapMode(QPlainTextEdit::WidgetWidth);
    ui_console->appendPlainText(tr("Ready"));

    ui_mainLt->addWidget(ui_frameView, 0, 0, 1, 1);
    //ui_mainLt->addWidget(ui_testView, 0, 0, 1, 1);
    ui_mainLt->addWidget(ui_pointView, 1, 0, 1, 1);
    ui_mainLt->addWidget(ui_userView, 0, 1, 2, 1);
    ui_mainLt->addWidget(ui_console, 2, 0, 1, 2);

    ico_done = QIcon(":/image/status_available");
    ico_ing = QIcon(":/image/status_opened");
    ico_no = QIcon(":/image/status_closed");
    ui_trainGB = new QGroupBox(tr("SVM train"), ui_ctrlPanelWgt);
    ui_trainGBLt = new QGridLayout(ui_trainGB);
    ui_trainGBLt->setSpacing(0);

    //ui_trainGBLt->setContentsMargins(0,0,0,0);
    int buttonsOneRow = 6;

    ui_trainPG = new QProgressBar(ui_trainGB);
    ui_trainPG->setValue(0);
    //ui_trainPG->setValue(20);
    ui_trainGBLt->addWidget(ui_trainPG, 0, 0, 1, buttonsOneRow);

    ui_trainPG->setStyleSheet("width:100%;margin-bottom: 10px");
    ui_trainPG->setTextVisible(false);

    for(int i = 0; i <= TRAIN_PHASE::N_NE; i++){
        ui_trainBt[i] = new QPushButton(ui_trainGB);
        ui_trainBt[i]->setCheckable(true);
        ui_trainBt[i]->setIcon(ico_no);
        ui_trainBt[i]->setToolTip(trainSet[i]->toolTip);
        ui_trainGBLt->addWidget(ui_trainBt[i], i / buttonsOneRow + 1, i % buttonsOneRow, 1, 1);
        connect(ui_trainBt[i], SIGNAL(clicked(bool)), this, SLOT(h_trainBt(bool)));
    }

    ui_trainSaveBt = new QPushButton(tr("Save"), ui_trainGB);
    ui_trainSaveBt->setEnabled(false);
    connect(ui_trainSaveBt, SIGNAL(clicked()), this, SLOT(h_trainSave()));
    ui_trainGBLt->addWidget(ui_trainSaveBt, TRAIN_PHASE::N_NE / buttonsOneRow + 2, 0, 1, buttonsOneRow);

    ui_trainLoadBt = new QPushButton(tr("Load"), ui_trainGB);
    connect(ui_trainLoadBt, SIGNAL(clicked()), this, SLOT(h_trainLoad()));
    ui_trainGBLt->addWidget(ui_trainLoadBt, TRAIN_PHASE::N_NE / buttonsOneRow + 3, 0, 1, buttonsOneRow);

    ui_ctrlPanelLt->addWidget(ui_trainGB, 1, 0, 1, 1);

    ui_modelGB = new QGroupBox(tr("SVM model"), ui_ctrlPanelWgt);
    ui_modelGBLt = new QGridLayout(ui_modelGB);
    ui_modelGBLt->setSpacing(0);

    ui_modelLoadTrainBt = new QPushButton(tr("Load train data"), ui_modelGB);
    ui_modelLoadBt = new QPushButton(tr("Load model file"), ui_modelGB);
    ui_modelSaveBt = new QPushButton(tr("Save model"), ui_modelGB);
    ui_modelSaveBt->setEnabled(false);
    ui_modelStatus = new QLabel(tr("<b>Model status:</b> %1").arg(modelStatus), ui_modelGB);
    ui_modelStatus->setStyleSheet("qproperty-alignment: AlignCenter;margin-bottom:10px");
    connect(ui_modelLoadTrainBt, SIGNAL(clicked()), this, SLOT(h_modelLoadTrain()));
    connect(ui_modelLoadBt, SIGNAL(clicked()), this, SLOT(h_modelLoad()));
    connect(ui_modelSaveBt, SIGNAL(clicked()), this, SLOT(h_modelSave()));

    ui_modelGBLt->addWidget(ui_modelStatus, 0, 0, 1, 1);
    ui_modelGBLt->addWidget(ui_modelLoadTrainBt, 1, 0, 1, 1);
    ui_modelGBLt->addWidget(ui_modelLoadBt, 2, 0, 1, 1);
    ui_modelGBLt->addWidget(ui_modelSaveBt, 3, 0, 1, 1);

    ui_ctrlPanelLt->addWidget(ui_modelGB, 2, 0, 1, 1);

    ui_captureGB = new QGroupBox(tr("Camera"), ui_ctrlPanelWgt);
    ui_captureGBLt = new QGridLayout(ui_captureGB);
    ui_captureGBLt->setSpacing(0);
    ui_captureBt = new QPushButton(tr("Capture"), ui_captureGB);
    ui_captureBt->setEnabled(false);
    ui_captureBt->setCheckable(true);
    ui_capturePG = new QProgressBar(ui_captureGB);
    ui_capturePG->setValue(0);
    ui_capturePG->setStyleSheet("width:100%;margin-bottom: 10px");
    ui_capturePG->setTextVisible(false);
    ui_captureGBLt->addWidget(ui_capturePG, 0, 0, 1, 1);
    ui_captureGBLt->addWidget(ui_captureBt, 1, 0, 1, 1);
    ui_ctrlPanelLt->addWidget(ui_captureGB, 3, 0, 1, 1);

    connect(ui_captureBt, SIGNAL(clicked(bool)), this, SLOT(h_capture(bool)));

}
