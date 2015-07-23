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
