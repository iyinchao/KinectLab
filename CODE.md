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
