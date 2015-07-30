//        if(sourceMarker & SOURCE_TYPE::FACE){
//            if(!faceReaders[0]){
//                IFaceFrameSource* faceSources[BODY_COUNT];
//                static const DWORD faceFeatures =
//                    FaceFrameFeatures::FaceFrameFeatures_BoundingBoxInColorSpace
//                    | FaceFrameFeatures::FaceFrameFeatures_PointsInColorSpace
//                    | FaceFrameFeatures::FaceFrameFeatures_RotationOrientation
//                    | FaceFrameFeatures::FaceFrameFeatures_Happy
//                    | FaceFrameFeatures::FaceFrameFeatures_RightEyeClosed
//                    | FaceFrameFeatures::FaceFrameFeatures_LeftEyeClosed
//                    | FaceFrameFeatures::FaceFrameFeatures_MouthOpen
//                    | FaceFrameFeatures::FaceFrameFeatures_MouthMoved
//                    | FaceFrameFeatures::FaceFrameFeatures_LookingAway
//                    | FaceFrameFeatures::FaceFrameFeatures_Glasses
//                    | FaceFrameFeatures::FaceFrameFeatures_FaceEngagement;
//                for(int i = 0; i < BODY_COUNT; i++){
//                    hr = CreateFaceFrameSource(sensor, 0, faceFeatures, &faceSources[i]);
//                    if(SUCCEEDED(hr)){
//                        qDebug()<<"ok";
//                    }else{
//                        std::cout<<std::hex<<hr<<std::endl;
//                        //qDebug()<<hr;
//                    }
//                }

//            }
//            if(faceReaders[0]){

//            }
//        }else if(faceReaders[0]){
//            for(int i = 0; i < BODY_COUNT; i++){
//                safeRelease(faceReaders[i]);
//            }
//            emit _readerInfo(false, SOURCE_TYPE::FACE);

//        }

//                KLComPtr<IColorFrameSource> colorSource;
//                hr = sensor->get_ColorFrameSource(&colorSource);
//                hr = colorSource->OpenReader(&colorReader);

//                KLComPtr<IDepthFrameSource> depthSource;
//                hr = sensor->get_DepthFrameSource(&depthSource);
//                //KLComPtr<IDepthFrameReader> depthReader;
//                hr = depthSource->OpenReader(&depthReader);

//                KLComPtr<IBodyFrameSource> bodySource;
//                hr = sensor->get_BodyFrameSource(&bodySource);
//                //KLComPtr<IBodyFrameReader> bodyReader;
//                hr = bodySource->OpenReader(&bodyReader);

//                KLComPtr<ICoordinateMapper> coord;
//                hr = sensor->get_CoordinateMapper(&coord);
////                if(SUCCEEDED(hr)){
////                    qDebug()<<"ok Body";
////                }


                        //std::cout<<std::hex<<hr<<std::endl;

  //qDebug()<<_countof(faceHDFrames);


        /*for(int i = 0; i < BODY_COUNT; i++){
            if(!faceAlignments[i]){
                hr = CreateFaceAlignment(&(faceAlignments[i]));
            }
            if(!faceModels[i]){
                float sd[FaceShapeDeformations_Count] = {0.0f};
                hr = CreateFaceModel(1.0f, FaceShapeDeformations::FaceShapeDeformations_Count, sd, &faceModels[i]);
            }
            if(!faceModelVCs[i]){
                hr = GetFaceModelVertexCount(&(faceModelVCs[i]));
            }
            if(faceData[i]->isValid && faceAlignments[i] && faceModels[i] && faceModelVCs[i]){
                CameraSpacePoint *v = new CameraSpacePoint[faceModelVCs[i]];
                hr = faceHDFrames[i]->GetAndRefreshFaceAlignmentResult(faceAlignments[i]);
                if(SUCCEEDED(hr)){
                    faceData[i]->alignment = faceAlignments[i];
                    faceData[i]->modelVC = faceModelVCs[i];
                    faceData[i]->model = faceModels[i];
                    faceData[i]->index = i;
                    hr = faceModels[i]->CalculateVerticesForAlignment(faceAlignments[i], faceModelVCs[i], v);
                }
                if(SUCCEEDED(hr)){

                    if(!faceAlignmentsInColorSpace[i]){
                        faceAlignmentsInColorSpace[i] = new ColorSpacePoint[faceModelVCs[i]];
                    }
                    hr = coordMapper->MapCameraPointsToColorSpace(faceModelVCs[i], v, faceModelVCs[i], faceAlignmentsInColorSpace[i]);


//                                if(sharedMemory->isAttached()){
//                                    sharedMemory->detach();
//                                }

//                                if(sharedMemory->size() == 0){
//                                    qDebug()<<"df";
//                                }
                    if(sharedMemory[i]->create(faceModelVCs[i] * sizeof(ColorSpacePoint))){

                    }else{
                        //qDebug()<<sharedMemory->errorString();
                        //qDebug()<<"dfd";
                        sharedMemory[i]->attach();
                    }
                    if(sharedMemory[i]->lock()){
                        ColorSpacePoint* to = static_cast<ColorSpacePoint *>(sharedMemory[i]->data());
                        memcpy(to, faceAlignmentsInColorSpace[i], faceModelVCs[i] * sizeof(ColorSpacePoint));
                        sharedMemory[i]->unlock();
                        emit _data(faceData[i], RESOURCE_TYPE::R_FACE_HD);
                    }

                    //faceAlignmentsInColorSpace[i]
                    //faceData[i]->alignmentInColorSpace = faceAlignmentsInColorSpace[i];

                    delete v;

                    //qDebug()<<faceAlignmentsInColorSpace[i][2].X;
                }
            }else{
                if(sharedMemory[i]->create(faceModelVCs[i] * sizeof(ColorSpacePoint))){

                }else{
                    sharedMemory[i]->attach();
                }
                if(sharedMemory[i]->lock()){
                    ColorSpacePoint* to = static_cast<ColorSpacePoint *>(sharedMemory[i]->data());
                    to[0].X = -1;
                    to[0].Y = -1;
                    sharedMemory[i]->unlock();
                }
            }
        }
    */


//        if(faceData && faceData->isValid && faceData->alignmentInColorSpace){
//            //qDebug()<<sizeof(*(faceData->alignmentInColorSpace));//faceData->alignmentInColorSpace[4].X;
//            //qDebug()<<faceAlignmentsInColorSpace[4].X;
//            QPen pen(QColor("blue"));
//            pen.setWidth(5);
//            pen.setCapStyle(Qt::PenCapStyle::RoundCap);
//            painter->setPen(pen);
//            for(UINT i = 0; i < faceData->modelVC; i++){
//                painter->drawPoint(faceData->alignmentInColorSpace[i].X, faceData->alignmentInColorSpace[i].Y);
//            }
//        }


//        for(int i = 0; i < BODY_COUNT; i++){
//            if(sharedMemory[i]->attach()){
//                QPen pen(QColor("blue"));
//                pen.setWidth(5);
//                pen.setCapStyle(Qt::RoundCap);
//                painter->setPen(pen);
//                double rate = ui_2DView->height() / 1080.0;

//                sharedMemory[i]->lock();

//                const ColorSpacePoint* data_ptr = (static_cast<const ColorSpacePoint *>(sharedMemory[i]->constData()));

//                if(data_ptr[0].X != -1 && data_ptr[0].Y != -1){
//                    for(UINT j = 0; j < faceModelVC; j++){
//                        painter->drawPoint(data_ptr[j].X * rate, data_ptr[j].Y * rate);
//                    }
//                }

//                sharedMemory[i]->unlock();
//                sharedMemory[i]->detach();
//            }
//        }



//        if(faceData && faceData->isValid){
//            int i = faceData->index;

//        }

##TAG　paint2d case 3

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


##TAG paint3D matrix

//                QQuaternion quaternion(faceOrient->w, faceOrient->x, faceOrient->y, faceOrient->z);
//                QMatrix4x4 matrix;
//                matrix.setToIdentity();
//                matrix.rotate(quaternion);
//                GLdouble* glMatrix = new GLdouble[16];
//                for(int j = 0; j < 16; j++){
//                    glMatrix[j] = matrix(j / 4, j % 4);
//                }
//                //qDebug()<<quaternion.x();
//                glPushMatrix();
//                glMultMatrixd(glMatrix);
//                glScalef(3,3,3);
glPopMatrix();


        switch(i){
        case P_MC_1: //positive/mouse closed/smile min
            break;
        case P_MC_2: //positive/mouse closed/smile medium
            break;
        case P_MC_3: //positive/mouse closed/smile max
            break;
        case P_MO1_1: //positive/mouse opened/min/smile min
            break;
        case P_MO1_2: //positive/mouse opened/min/smile med
            break;
        case P_MO1_3: //positive/mouse opened/min/smile max
            break;
        case P_MO2_1: //positive/mouse opened/med/smile min
            break;
        case P_MO2_2: //positive/mouse opened/med/smile med
            break;
        case P_MO2_3: //positive/mouse opened/med/smile max
            break;
        case P_MO3_1: //positive/mouse opened/max/smile min
            break;
        case P_MO3_2: //positive/mouse opened/max/smile med
            break;
        case P_MO3_3: //positive/mouse opened/max/smile max
            break;
        case N_AU27_1: //mouse stretch/min
            break;
        case N_AU27_2: //mouse stretch/med
            break;
        case N_AU27_3: //mouse stretch/max
            break;
        case N_AU18_1: //lip pucker(嘴唇皱起)/min
            break;
        case N_AU18_2: //lip pucker(嘴唇皱起)/max
            break;
        case N_AU22_1: //lip funneler/min
            break;
        case N_AU22_2: //lip funneler/max
            break;
        case N_AU20_1: //lip stretcher/min
            break;
        case N_AU20_2: //lip stretcher/max
            break;
        case N_AU15_1: //lip corner depressor/min
            break;
        case N_AU15_2: //lip corner depressor/max
            break;
        case N_NE: //Neutral face
            break;
