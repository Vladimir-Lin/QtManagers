#include <qtmanagers.h>

N::FaceParaments:: FaceParaments (void)
{
}

N::FaceParaments::~FaceParaments (void)
{
}

bool N::FaceParaments::isIntact(void)
{
  nKickOut (Face     . width  () < 15 , false ) ;
  nKickOut (Face     . height () < 15 , false ) ;
  nKickOut (LeftEye  . count  () <= 0 , false ) ;
  nKickOut (RightEye . count  () <= 0 , false ) ;
  nKickOut (Mouth    . count  () <= 0 , false ) ;
  nKickOut (Noses    . count  () <= 0 , false ) ;
  return true                                   ;
}

bool N::FaceParaments::isPartial(void)
{
  nKickOut (Face . width  () < 15 , false ) ;
  nKickOut (Face . height () < 15 , false ) ;
  int total = 0                             ;
  total    += LeftEye  . count ( )          ;
  total    += RightEye . count ( )          ;
  total    += Mouth    . count ( )          ;
  total    += Noses    . count ( )          ;
  return ( total > 0 )                      ;
}

double N::FaceParaments::Possibility(void)
{
  double total = 0                                  ;
  total += ( LeftEye  . count ( ) > 0 ) ? 1.0 : 0.0 ;
  total += ( RightEye . count ( ) > 0 ) ? 1.0 : 0.0 ;
  total += ( Mouth    . count ( ) > 0 ) ? 1.0 : 0.0 ;
  total += ( Noses    . count ( ) > 0 ) ? 1.0 : 0.0 ;
  if ( total > 4.0 ) total = 4.0                    ;
  return ( total / 4 )                              ;
}

double N::FaceParaments::Angle(int rightEyeIndex,int leftEyeIndex)
{
  nKickOut ( rightEyeIndex >= RightEye . count ( ) , 0 ) ;
  nKickOut ( leftEyeIndex  >= LeftEye  . count ( ) , 0 ) ;
  QPointF p1 = LeftEye  [ leftEyeIndex  ] . center ( )   ;
  QPointF p2 = RightEye [ rightEyeIndex ] . center ( )   ;
  QLineF  L ( p2 , p1 )                                  ;
  return  L . angle ( )                                  ;
}
