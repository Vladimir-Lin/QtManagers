#include <qtmanagers.h>

N::CommonSeeker:: CommonSeeker ( QObject * parent )
                : QObject      (           parent )
{
  canRun         = false                                  ;
  downloadTime   = 480                                    ;
  connectTime    =  90                                    ;
  trackingHttp   = true                                   ;
  skipJavaScript = false                                  ;
  ProgressID     = -1                                     ;
  ProgressValue  = 0                                      ;
  ProgressGo     = false                                  ;
  HostProxy      = ""                                     ;
  PortProxy      = 0                                      ;
  /////////////////////////////////////////////////////////
  nConnect ( this , SIGNAL ( privateMessage (QString) )   ,
             this , SLOT   ( emitMessage    (QString) ) ) ;
  nConnect ( this , SIGNAL ( privateUrl     (QString) )   ,
             this , SLOT   ( emitUrl        (QString) ) ) ;
}

N::CommonSeeker::~CommonSeeker (void)
{
}

void N::CommonSeeker::showMessage(QString message)
{
  emit privateMessage ( message ) ;
}

void N::CommonSeeker::emitMessage(QString message)
{
  emit sendMessage ( message ) ;
}

void N::CommonSeeker::LookUrl(QString url)
{
  if (!TrackingHttp()) return ;
  emit privateUrl ( url )     ;
}

void N::CommonSeeker::emitUrl(QString url)
{
  if (!TrackingHttp()) return ;
  emit sendUrl ( url )        ;
}

bool N::CommonSeeker::runnable (void) const
{
  if ( ! ProgressGo ) return false ;
  return canRun                    ;
}

int N::CommonSeeker::DownloadTime(void) const
{
  return downloadTime ;
}

int N::CommonSeeker::ConnectTime(void) const
{
  return connectTime ;
}

bool N::CommonSeeker::skipJava(void) const
{
  return skipJavaScript ;
}

bool N::CommonSeeker::TrackingHttp (void) const
{
  return trackingHttp ;
}

void N::CommonSeeker::setRunnable (bool Run)
{
  canRun = Run ;
}

QString N::CommonSeeker::Temporary(QString filename)
{
  return TemporaryPath . absoluteFilePath ( filename ) ;
}

N::Sql & N::CommonSeeker::HunterSql(void)
{
  return Hunter ;
}

int N::CommonSeeker::ID(void)
{
  return ProgressID ;
}

qint64 & N::CommonSeeker::Value(void)
{
  return ProgressValue ;
}

bool & N::CommonSeeker::Go(void)
{
  return ProgressGo ;
}

QString N::CommonSeeker::ProxyAddress (void) const
{
  return HostProxy ;
}

int N::CommonSeeker::ProxyPort(void) const
{
  return PortProxy ;
}

void N::CommonSeeker::setCommonId(int Id)
{
  ProgressID = Id   ;
  ProgressGo = true ;
}

void N::CommonSeeker::setPath(QString path)
{
  TemporaryPath = QDir ( path ) ;
}

void N::CommonSeeker::setDownloadTime(int dnTime)
{
  downloadTime = dnTime ;
}

void N::CommonSeeker::setConnectTime(int cnTime)
{
  connectTime = cnTime ;
}

void N::CommonSeeker::setTrackingHttp(bool track)
{
  trackingHttp = track ;
}

void N::CommonSeeker::TrackingChanged(int state)
{
  trackingHttp = ( state == Qt::Checked ) ;
}

void N::CommonSeeker::setJavaScript(bool skip)
{
  skipJavaScript = skip ;
}

void N::CommonSeeker::JavaChanged(int state)
{
  skipJavaScript = ( state == Qt::Checked ) ;
}

void N::CommonSeeker::setHunter(Sql & sql)
{
  Hunter = sql ;
}

void N::CommonSeeker::setProxy(const QString & text)
{
  QStringList L = text.split(':') ;
  if ( 2 != L.count() ) return    ;
  bool okay = false               ;
  L[1].toInt(&okay)               ;
  if (!okay) return               ;
  HostProxy = L [ 0 ]             ;
  PortProxy = L [ 1 ] . toInt ( ) ;
}
