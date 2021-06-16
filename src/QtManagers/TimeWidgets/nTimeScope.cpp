#include <qtmanagers.h>
#include "ui_nTimeScope.h"

N::TimeScope:: TimeScope ( QWidget * parent , Plan * p )
             : Dialog    (           parent ,        p )
             , ui        ( new Ui::nTimeScope          )
{
  WidgetClass   ;
  Configure ( ) ;
}

N::TimeScope::~TimeScope (void)
{
  delete ui;
}

void N::TimeScope::Configure(void)
{
  TUID    N = StarDate::now       (          ) ;
  QString S = QString::number     ( N        ) ;
  ui   -> setupUi                 ( this     ) ;
  ui   -> StartDT -> blockSignals ( true     ) ;
  ui   -> EndDT   -> blockSignals ( true     ) ;
  ui   -> Start   -> blockSignals ( true     ) ;
  ui   -> End     -> blockSignals ( true     ) ;
  ui   -> StartDT -> setDateTime  ( nTimeNow ) ;
  ui   -> EndDT   -> setDateTime  ( nTimeNow ) ;
  ui   -> Start   -> setText      ( S        ) ;
  ui   -> End     -> setText      ( S        ) ;
  ui   -> StartDT -> blockSignals ( false    ) ;
  ui   -> EndDT   -> blockSignals ( false    ) ;
  ui   -> Start   -> blockSignals ( false    ) ;
  ui   -> End     -> blockSignals ( false    ) ;
  plan -> setFont                 ( this     ) ;
}

void N::TimeScope::setStart(TUID start)
{
  nDropOut ( start <= 0 )       ;
  QString S                     ;
  S = QString::number ( start ) ;
  ui->Start->setText  ( S     ) ;
  doneStart           (       ) ;
}

void N::TimeScope::setEnd(TUID end)
{
  nDropOut ( end <= 0 )       ;
  QString S                   ;
  S = QString::number ( end ) ;
  ui->End->setText    ( S   ) ;
  doneEnd             (     ) ;
}

void N::TimeScope::setGap(TUID gap)
{
  nDropOut ( gap <= 0 )       ;
  ui-> dT -> setValue ( gap ) ;
}

void N::TimeScope::startChanged(void)
{
  QDateTime dt = ui->StartDT->dateTime()      ;
  StarDate  sd                                ;
  QString   S                                 ;
  sd  = dt                                    ;
  S   = QString::number       ( sd.Stardate ) ;
  ui -> Start -> blockSignals ( true        ) ;
  ui -> Start -> setText      ( S           ) ;
  ui -> Start -> blockSignals ( false       ) ;
}

void N::TimeScope::endChanged(void)
{
  QDateTime dt = ui->EndDT->dateTime()      ;
  StarDate  sd                              ;
  QString   S                               ;
  sd  = dt                                  ;
  S   = QString::number     ( sd.Stardate ) ;
  ui -> End -> blockSignals ( true        ) ;
  ui -> End -> setText      ( S           ) ;
  ui -> End -> blockSignals ( false       ) ;
}

void N::TimeScope::doneStart(void)
{
  QDateTime D                                   ;
  QString   S = ui->Start->text (             ) ;
  TUID      T = S.toLongLong    (             ) ;
  StarDate  sd                                  ;
  sd.Stardate = T                               ;
  sd.locate()                                   ;
  D   = sd.toLocalTime          (             ) ;
  ui -> StartDT -> blockSignals ( true        ) ;
  ui -> StartDT -> setDateTime  ( D           ) ;
  ui -> StartDT -> blockSignals ( false       ) ;
}

void N::TimeScope::doneEnd(void)
{
  QDateTime D                                 ;
  QString   S = ui->End->text (             ) ;
  TUID      T = S.toLongLong  (             ) ;
  StarDate  sd                                ;
  sd.Stardate = T                             ;
  sd.locate()                                 ;
  D   = sd.toLocalTime        (             ) ;
  ui -> EndDT -> blockSignals ( true        ) ;
  ui -> EndDT -> setDateTime  ( D           ) ;
  ui -> EndDT -> blockSignals ( false       ) ;
}

TUID N::TimeScope::Start(void)
{
  QString S = ui->Start->text () ;
  TUID    T = S.toLongLong    () ;
  return  T                      ;
}

TUID N::TimeScope::End(void)
{
  QString S = ui->End->text () ;
  TUID    T = S.toLongLong  () ;
  return  T                    ;
}

TUID N::TimeScope::dT(void)
{
  return ui -> dT -> value ( ) ;
}
