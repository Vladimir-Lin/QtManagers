#include <qtmanagers.h>
#include "ui_nContourController.h"

N::ContourController:: ContourController (QWidget * parent,Plan *  p)
                     : StackedWidget     (          parent,        p)
                     , GraphicsManager   (                         p)
                     , ui                (new Ui::nContourController)
                     , canvas            (NULL                      )
                     , contour           (NULL                      )
                     , PenColor          (QColor (  0,  0,255)      )
                     , BrushColor        (QColor (255,255,255)      )
{
  WidgetClass            ;
  ui -> setupUi ( this ) ;
}

N::ContourController::~ContourController (void)
{
  delete ui;
}

QImage N::ContourController::CreateSnap(QSize size,QPointF center,Contour & c)
{
  QImage   I(size,QImage::Format_ARGB32)     ;
  QPointF  C(size.width()/2,size.height()/2) ;
  QRectF   r = c . boundingRect ( )          ;
  double   f = r . width()                   ;
  double   m = size.width()                  ;
  QPolygon P                                 ;
  if (r.height()>f) f = r.height()           ;
  m /= f                                     ;
  for (int i=0;i<c.count();i++)              {
    double  x = c.points[i].x                ;
    double  y = c.points[i].y                ;
    QPointF z(x,y)                           ;
    z -= center                              ;
    z *= m                                   ;
    z += C                                   ;
    int X = z . x ( )                        ;
    int Y = z . y ( )                        ;
    if (X<0) X = 0                           ;
    if (Y<0) Y = 0                           ;
    if (X>=size.width ()) X=size.width ()-1  ;
    if (Y>=size.height()) Y=size.height()-1  ;
    P << QPoint ( X , Y )                    ;
  }                                          ;
  memset(I.bits(),0,I.byteCount())           ;
  QPainter p                                 ;
  p . begin       ( &I                   )   ;
  p . setPen      ( PenColor             )   ;
  if (ui->Fill->isChecked())                 {
    p . setBrush  ( BrushColor           )   ;
  }                                          ;
  p . drawPolygon ( P , Qt::WindingFill  )   ;
  p . end         (                      )   ;
  return I                                   ;
}

QIcon N::ContourController::IconColor(QSize size,QColor color)
{
  QImage I(size,QImage::Format_ARGB32)     ;
  QRect  R(0,0,size.width(),size.height()) ;
  QPainter p                               ;
  p . begin    ( &I    )                   ;
  p . setPen   ( color )                   ;
  p . setBrush ( color )                   ;
  p . drawRect ( R     )                   ;
  p . end      (       )                   ;
  return QIcon(QPixmap::fromImage(I))      ;
}

void N::ContourController::Display(void)
{
  ui->Name->setFocus(Qt::TabFocusReason)                         ;
  if (IsNull(contour)) return                                    ;
  Contour c = contour->contour                                   ;
  QRectF  r = c . boundingRect ( )                               ;
  QPointF o = r . center       ( )                               ;
  QSize   s = QSize(128,128)                                     ;
  QImage  i = CreateSnap(s,o,contour->contour)                   ;
  ui -> Icon -> setPixmap(QPixmap::fromImage(i))                 ;
  ui -> Name -> blockSignals ( true  )                           ;
  ui -> Name -> setText (contour->contour.name)                  ;
  ui -> Name -> blockSignals ( false )                           ;
  ui -> LineColor -> setIcon(IconColor(QSize(56,56),PenColor  )) ;
  ui -> FillColor -> setIcon(IconColor(QSize(56,56),BrushColor)) ;
}

void N::ContourController::Save(void)
{
  if (IsNull(contour))                            {
    Alert ( Error )                               ;
    emit Finish ( )                               ;
    return                                        ;
  }                                               ;
  /////////////////////////////////////////////////
  Contour  c ; c = contour->contour               ;
  QRectF   r = c . boundingRect ( )               ;
  QPointF  o = r . center       ( )               ;
  QSize    s = QSize(128,128)                     ;
  QImage   i = CreateSnap(s,o,contour->contour)   ;
  c -= o                                          ;
  SqlConnection SC(plan->sql)                     ;
  if (SC.open("nContourController","Save"))       {
    SaveContour ( SC , c , i     )                ;
    SC . close ( )                                ;
  }                                               ;
  SC . remove ( )                                 ;
  contour->contour.uuid = c.uuid                  ;
  Alert ( Done )                                  ;
  emit Finish ( )                                 ;
}

void N::ContourController::Cancel(void)
{
  emit Finish ( ) ;
}

void N::ContourController::NameChanged(QString text)
{
  contour->contour.name = text ;
}

void N::ContourController::Fill(int state)
{ Q_UNUSED ( state )                             ;
  Contour c = contour->contour                   ;
  QRectF  r = c . boundingRect ( )               ;
  QPointF o = r . center       ( )               ;
  QSize   s = QSize(128,128)                     ;
  QImage  i = CreateSnap(s,o,contour->contour)   ;
  ui -> Icon -> setPixmap(QPixmap::fromImage(i)) ;
}

void N::ContourController::Bounding(int state)
{
}

void N::ContourController::LineColor(void)
{
  PenColor  = QColorDialog::getColor(PenColor  ,this)          ;
  Contour c = contour->contour                                 ;
  QRectF  r = c . boundingRect ( )                             ;
  QPointF o = r . center       ( )                             ;
  QSize   s = QSize(128,128)                                   ;
  QImage  i = CreateSnap(s,o,contour->contour)                 ;
  ui -> Icon -> setPixmap(QPixmap::fromImage(i))               ;
  ui -> LineColor -> setIcon(IconColor(QSize(56,56),PenColor)) ;
}

void N::ContourController::FillColor(void)
{
  BrushColor = QColorDialog::getColor(BrushColor,this)           ;
  Contour  c = contour->contour                                  ;
  QRectF   r = c . boundingRect ( )                              ;
  QPointF  o = r . center       ( )                              ;
  QSize    s = QSize(128,128)                                    ;
  QImage   i = CreateSnap(s,o,contour->contour)                  ;
  ui -> Icon -> setPixmap(QPixmap::fromImage(i))                 ;
  ui -> FillColor -> setIcon(IconColor(QSize(56,56),BrushColor)) ;
}
