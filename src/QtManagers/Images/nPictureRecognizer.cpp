#include <qtmanagers.h>

N::PictureRecognizer:: PictureRecognizer ( Plan *              plan )
                     : FilterPlan        (                     plan )
                     , Flip              ( true                     )
                     , FaceDetect        ( QSize(12,12)             )
                     , FaceScale         ( 1.1                      )
                     , FaceNeighbors     ( 2                        )
                     , FaceFlags         ( CV_HAAR_DO_CANNY_PRUNING )
                     , FeatureDetect     ( QSize( 4, 4)             )
                     , FeatureScale      ( 1.1                      )
                     , FeatureNeighbors  ( 1                        )
                     , FeatureFlags      ( CV_HAAR_DO_CANNY_PRUNING )
                     , MinWindowSize     ( QSize(512,512)           )
                     , AdjustWindow      ( false                    )
{
  nIfSafe(FilterPlan) Classifiers = FilterPlan->classifiers ;
}

N::PictureRecognizer::~PictureRecognizer (void)
{
}

N::CvClassifiers N::PictureRecognizer::Load (NAMEs pathes)
{
  CvClassifiers classifiers                                                 ;
  UUIDs Uuids = pathes.keys()                                               ;
  SUID  uuid                                                                ;
  foreach (uuid,Uuids)                                                      {
    int        id       = (int)    uuid                                     ;
    QString    filename = pathes [ uuid ]                                   ;
    QByteArray Local    = File::toNative(filename)                          ;
    classifiers [id] = (CvHaarClassifierCascade *)cvLoad(Local.constData()) ;
  }                                                                         ;
  return classifiers                                                        ;
}

QRectF N::PictureRecognizer::ShiftIn(QRectF Image,QRect Feature)
{
  return QRectF(Feature . left   () + Image . left () ,
                Feature . top    () + Image . top  () ,
                Feature . width  ()                   ,
                Feature . height ()                 ) ;
}

QRectF N::PictureRecognizer::ShiftOut(QRectF Image,QRect Feature)
{
  return QRectF(Feature . left   () - Image . left () ,
                Feature . top    () - Image . top  () ,
                Feature . width  ()                   ,
                Feature . height ()                 ) ;
}

QRect N::PictureRecognizer::GoldenRatio(QRect Face,QSize Size)
{
  QRect     X(0,0,Size.width(),Size.height()) ;
  QRect     R                                 ;
  QPoint    center = Face . center ()         ;
  long long W      = Face . width  ()         ;
  long long H      = Face . height ()         ;
  int       W2                                ;
  int       H2                                ;
  int       HT                                ;
  W *= 112 ; W /= 100                         ;
  H *= 161 ; H /= 100                         ;
  W2 = (int)(W/2)                             ;
  H2 = (int)(H/2)                             ;
  HT = (int)(H/8)                             ;
  center . setY  ( center.y()-HT )            ;
  R  . setLeft   ( center.x()-W2 )            ;
  R  . setTop    ( center.y()-H2 )            ;
  R  . setWidth  ( (int)W        )            ;
  R  . setHeight ( (int)H        )            ;
  return X.intersected(R)                     ;
}

QRect N::PictureRecognizer::FlatRatio(QRect Face,QSize Size)
{
  QRect     X(0,0,Size.width(),Size.height()) ;
  QRect     R                                 ;
  QPoint    center = Face . center ()         ;
  long long W      = Face . width  ()         ;
  long long H      = Face . height ()         ;
  int       W2                                ;
  int       H2                                ;
  int       HT                                ;
  W *= 161 ; W /= 100                         ;
  H *= 161 ; H /= 100                         ;
  W2 = (int)(W/2)                             ;
  H2 = (int)(H/2)                             ;
  HT = (int)(H/8)                             ;
  center . setY  ( center.y()-HT )            ;
  R  . setLeft   ( center.x()-W2 )            ;
  R  . setTop    ( center.y()-H2 )            ;
  R  . setWidth  ( (int)W        )            ;
  R  . setHeight ( (int)H        )            ;
  return X.intersected(R)                     ;
}

bool N::PictureRecognizer::setFeature(int feature,bool enable)
{
  Features[feature] = enable ;
  return Features[feature]   ;
}

bool N::PictureRecognizer::isFeature(int feature)
{
  nKickOut ( !Features.contains(feature) , false ) ;
  return Features[feature]                         ;
}

// Left eye is actually at right side in the image
QRect N::PictureRecognizer::LeftEye(QRect Face)
{
  return QRect                           (
           Face . left   ()              ,
           Face . top    ()              ,
           Face . width  () * 60 / 100   ,
           Face . height ()      / 2   ) ;
}

// Right eye is actually at left side in the image
QRect N::PictureRecognizer::RightEye(QRect Face)
{
  return QRect                                               (
           Face . left   () + ( Face . width () * 40 / 100 ) ,
           Face . top    ()                                  ,
           Face . width  () * 60 / 100                       ,
           Face . height ()      / 2                       ) ;
}

// Mouth normally located at the lower half of face
QRect N::PictureRecognizer::Mouth(QRect Face)
{
  return QRect                                         (
           Face . left   ()                            ,
           Face . top    () + ( Face . height () / 2 ) ,
           Face . width  ()                            ,
           Face . height () / 2                      ) ;
}

// Nose normally located at the center half of face
QRect N::PictureRecognizer::Nose(QRect Face)
{
  return QRect                                        (
           Face . left   () + ( Face . width () / 4 ) ,
           Face . top    ()                           ,
           Face . width  () / 2                       ,
           Face . height ()                         ) ;
}

void N::PictureRecognizer::SetImageROI(IplImage * image,QRect R)
{
  cvSetImageROI                                           (
    image                                                 ,
    cvRect( R.x() , R.y() , R.width() , R.height() )    ) ;
}

CvSeq * N::PictureRecognizer::DetectMajorObject (
          IplImage     * image                ,
          CvMemStorage * storage              )
{
  if (AdjustWindow)                                  {
    // cvSetImagesForHaarClassifierCascade
  }                                                  ;
  return cvHaarDetectObjects                         (
    image                                            , // const CvArr             * image
    Classifiers[Faces::FrontalFace]                  , // CvHaarClassifierCascade * cascade
    storage                                          , // CvMemStorage            * storage
    FaceScale                                        , // double scale_factor
    FaceNeighbors                                    , // int min_neighbors
    FaceFlags                                        , // int flags
    cvSize(FaceDetect.width(),FaceDetect.height()) ) ; // CvSize min_size
}

CvSeq * N::PictureRecognizer::DetectFeatureObject (
          IplImage     * image                  ,
          CvMemStorage * storage                ,
          int            Classifier             )
{
  if (AdjustWindow)                                         {
    // cvSetImagesForHaarClassifierCascade
  }                                                         ;
  return cvHaarDetectObjects                                (
      image                                                 ,
      Classifiers[Classifier]                               ,
      storage                                               ,
      FeatureScale                                          ,
      FeatureNeighbors                                      ,
      FeatureFlags                                          ,
      cvSize(FeatureDetect.width(),FeatureDetect.height())) ;
}

QList<QRect> N::PictureRecognizer::Rectangles(CvSeq * items,QRect S)
{
  QList<QRect> E                                      ;
  for (int j = 0; j < items->total ; j++)             {
    CvRect * r = (CvRect*)cvGetSeqElem(items,j)       ;
    QRect R(r->x+S.x(),r->y+S.y(),r->width,r->height) ;
    E << R                                            ;
  }                                                   ;
  return E                                            ;
}

QList<QRect> N::PictureRecognizer::DetectFeatures(
               IplImage     * image            ,
               CvMemStorage * storage          ,
               int            Classifier       ,
               QRect          Face             )
{
  QList<QRect> Result              ;
  CvSeq      * items               ;
  QRect        S                   ;
  switch (Classifier)              {
    case Faces::LeftEye            :
      S = LeftEye  ( Face )        ;
    break                          ;
    case Faces::RightEye           :
      S = RightEye ( Face )        ;
    break                          ;
    case Faces::Mouth              :
      S = Mouth    ( Face )        ;
    break                          ;
    case Faces::Nose               :
      S = Nose     ( Face )        ;
    break                          ;
  }                                ;
  cvClearMemStorage ( storage    ) ;
  SetImageROI       ( image , S  ) ;
  items = DetectFeatureObject      (
            image,storage          ,
            Classifier           ) ;
  if (NULL!=items)                 {
    Result = Rectangles(items,S)   ;
  }                                ;
  cvResetImageROI(image)           ;
  return Result                    ;
}

N::Phiz N::PictureRecognizer::Faces(IplImage * image)
{
  Phiz faces                                    ;
  nKickOut ( IsNull(image) , faces )            ;
  CvMemStorage * storage                        ;
  storage = cvCreateMemStorage()                ;
  if (IsNull(storage)) return faces             ;
  ///////////////////////////////////////////////
  //             Detect Major Face             //
  ///////////////////////////////////////////////
  CvSeq * face                                  ;
  face = DetectMajorObject(image,storage)       ;
  if (NULL!=face)                               {
    QRect Shift(0,0,1,1)                        ;
    QList<QRect> FS = Rectangles (face,Shift)   ;
    for (int i=0;i<FS.count();i++)              {
      FaceParaments F                           ;
      F.Face = FS[i]                            ;
      faces << F                                ;
    }                                           ;
  }                                             ;
  ///////////////////////////////////////////////
  //             Detect Features               //
  ///////////////////////////////////////////////
  for (int i=0;i<faces.count();i++)             {
    QRect Face = faces[i].Face                  ;
    #define Detect(ITEM,ENUM)                   \
      faces[i].ITEM = DetectFeatures          ( \
                        image                 , \
                        storage               , \
                        Faces::ENUM           , \
                        Face                  ) ;
    #define ifDetect(ITEM,ENUM)                 \
      if (isFeature(Faces::ENUM))             { \
        faces[i].ITEM = DetectFeatures        ( \
                    image                     , \
                    storage                   , \
                    Faces::ENUM               , \
                    Face                    ) ; \
      }
    if (Flip)                                   {
      Detect  ( RightEye  , LeftEye   )         ;
      Detect  ( LeftEye   , RightEye  )         ;
    } else                                      {
      Detect  ( LeftEye   , LeftEye   )         ;
      Detect  ( RightEye  , RightEye  )         ;
    }                                           ;
    Detect    ( Mouth     , Mouth     )         ;
    Detect    ( Noses     , Nose      )         ;
    ifDetect  ( LeftEar   , LeftEar   )         ;
    ifDetect  ( RightEar  , RightEar  )         ;
    ifDetect  ( Glass     , Glass     )         ;
    ifDetect  ( Body      , Body      )         ;
    ifDetect  ( UpperBody , UpperBody )         ;
    ifDetect  ( LowerBody , LowerBody )         ;
    #undef  Detect
  }                                             ;
  cvReleaseImage      ( &image   )              ;
  cvClearMemStorage   (  storage )              ;
  cvReleaseMemStorage ( &storage )              ;
  return faces                                  ;
}
