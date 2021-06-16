#include <qtmanagers.h>

void N::ListGMT(QComboBox * gmt)
{
  gmt->clear     (                         ) ;
  gmt -> addItem (QObject::tr("GMT-12"),-12) ;
  gmt -> addItem (QObject::tr("GMT-11"),-11) ;
  gmt -> addItem (QObject::tr("GMT-10"),-10) ;
  gmt -> addItem (QObject::tr("GMT-9" ),-9 ) ;
  gmt -> addItem (QObject::tr("GMT-8" ),-8 ) ;
  gmt -> addItem (QObject::tr("GMT-7" ),-7 ) ;
  gmt -> addItem (QObject::tr("GMT-6" ),-6 ) ;
  gmt -> addItem (QObject::tr("GMT-5" ),-5 ) ;
  gmt -> addItem (QObject::tr("GMT-4" ),-4 ) ;
  gmt -> addItem (QObject::tr("GMT-3" ),-3 ) ;
  gmt -> addItem (QObject::tr("GMT-2" ),-2 ) ;
  gmt -> addItem (QObject::tr("GMT-1" ),-1 ) ;
  gmt -> addItem (QObject::tr("GMT-0" ),-0 ) ;
  gmt -> addItem (QObject::tr("GMT+0" ), 0 ) ;
  gmt -> addItem (QObject::tr("GMT+1" ), 1 ) ;
  gmt -> addItem (QObject::tr("GMT+2" ), 2 ) ;
  gmt -> addItem (QObject::tr("GMT+3" ), 3 ) ;
  gmt -> addItem (QObject::tr("GMT+4" ), 4 ) ;
  gmt -> addItem (QObject::tr("GMT+5" ), 5 ) ;
  gmt -> addItem (QObject::tr("GMT+6" ), 6 ) ;
  gmt -> addItem (QObject::tr("GMT+7" ), 7 ) ;
  gmt -> addItem (QObject::tr("GMT+8" ), 8 ) ;
  gmt -> addItem (QObject::tr("GMT+9" ), 9 ) ;
  gmt -> addItem (QObject::tr("GMT+10"),10 ) ;
  gmt -> addItem (QObject::tr("GMT+11"),11 ) ;
  gmt -> addItem (QObject::tr("GMT+12"),12 ) ;
  gmt -> addItem (QObject::tr("GMT+13"),13 ) ;
  gmt -> addItem (QObject::tr("GMT+14"),14 ) ;
}

void N::SetGMT(QComboBox * gmt,int TimeZone)
{
  for (int i=0;i<gmt->count();i++)                       {
    if (gmt->itemData(i,Qt::UserRole).toInt()==TimeZone) {
      gmt->setCurrentIndex(i)                            ;
      return                                             ;
    }                                                    ;
  }                                                      ;
}

int N::localGMT(void)
{
  QDateTime D  = QDateTime::currentDateTime   ()                 ;
  QDateTime U  = QDateTime::currentDateTimeUtc()                 ;
  QString   CD = D.toString("yyyy/MM/dd hh:mm:ss")               ;
  QString   UD = U.toString("yyyy/MM/dd hh:mm:ss")               ;
  D            = QDateTime::fromString(CD,"yyyy/MM/dd hh:mm:ss") ;
  U            = QDateTime::fromString(UD,"yyyy/MM/dd hh:mm:ss") ;
  int       tz = (int)(U.secsTo(D))                              ;
  tz          /= 3600                                            ;
  return tz                                                      ;
}
