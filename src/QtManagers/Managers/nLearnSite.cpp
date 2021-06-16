#include <qtmanagers.h>

#define TABLES(TableName) NetPlan->Tables[Tables::TableName]
#define TYPE(T)           N::Types::T
#define RELATE(R)         N::Groups::R
#define DebugMsg          NetPlan->Debug
#define ProgressMsg       NetPlan->ProgressText

N::LearnSite:: LearnSite       ( Plan * p )
             : NetworkManager  (        p )
             , DocumentManager (        p )
             , MimeManager     (        p )
             , XmlParser       (          )
             , ProgressId      ( -1       )
             , StepDelay       ( 20       )
             , MiniDelay       ( 10       )
{
}

N::LearnSite::~LearnSite (void)
{
}

QString N::LearnSite::GetAgent(QString debug)
{
  QString agent = N::XML::UserAgents[rand()%40]        ;
  NetPlan -> Debug ( 90 , QString(debug).arg(agent) )  ;
  return agent                                         ;
}

QString N::LearnSite::parseTitle(void)
{
  QString title = QString::fromUtf8(AnalyzeTitle)            ;
  if (NotNull(Codec)) title = Codec->toUnicode(AnalyzeTitle) ;
  return title                                               ;
}

QString N::LearnSite::Temporary(QString filename)
{
  return NetPlan -> Temporary ( filename ) ;
}

bool N::LearnSite::setOptions(Ftp & ftp)
{
  if (useProxy())                                 {
    ftp . Requests [ "Proxy" ] = ProxyAddress ( ) ;
  }                                               ;
  return false                                    ;
}

bool N::LearnSite::useProxy(void)
{
  return false ;
}

QString N::LearnSite::ProxyAddress(void)
{
  return "" ;
}

bool N::LearnSite::isAppendHost(void)
{
  return false ;
}

bool N::LearnSite::TryGuess(void)
{
  return true ;
}

bool N::LearnSite::IgnoreDump(void)
{
  return true ;
}

bool N::LearnSite::TryReferer(void)
{
  return false ;
}

bool N::LearnSite::RemoveImage(void)
{
  return true ;
}

int N::LearnSite::HeaderTime(void)
{
  return 120 ;
}

int N::LearnSite::DownloadTime(void)
{
  return 480 ;
}

int N::LearnSite::ConnectTimeout(void)
{
  return 90 ;
}

void N::LearnSite::shiftDelay(void)
{
  nDropOut     ( StepDelay <= 0     ) ;
  nDropOut     ( MiniDelay <= 0     ) ;
  int  DT = MiniDelay                 ;
  DT     +=    ( rand() % StepDelay ) ;
  Time::msleep ( DT                 ) ;
}

void N::LearnSite::stepDelay(void)
{
  nDropOut     ( StepDelay <= 0 ) ;
  Time::msleep ( StepDelay      ) ;
}

void N::LearnSite::ResetParser(void)
{
  AnalyzeTitle . clear ( ) ;
  Links        . clear ( ) ;
  Images       . clear ( ) ;
}

bool N::LearnSite::setEnabled(int Id,bool enabled)
{
  Enablings [ Id ] = enabled ;
  return enabled             ;
}

bool N::LearnSite::isEnabled(int Id)
{
  if ( ! Enablings . contains ( Id ) ) return false ;
  return Enablings [ Id ]                           ;
}

void N::LearnSite::AppendTitle (QString title,QByteArray origin)
{
  AnalyzeTitle = origin ;
}

void N::LearnSite::AppendLink (
       QString name           ,
       QUrl    origin         ,
       QUrl    url            )
{
  QUrl O = origin             ;
  if (url.host().length()<=0) {
    O = O.resolved(url)       ;
  } else                      {
    O = url                   ;
  }                           ;
  nDropOut ( !O.isValid() )   ;
  Links << O                  ;
}

void N::LearnSite::AppendImage (
       QString name          ,
       QUrl    origin        ,
       QUrl    url           )
{
  QUrl O = origin             ;
  if (url.host().length()<=0) {
    O = O.resolved(url)       ;
  } else                      {
    O = url                   ;
  }                           ;
  nDropOut ( !O.isValid() )   ;
  Images << O                 ;
}

bool N::LearnSite::Initialize (SqlConnection & Connection)
{
  MimeManager :: load ( Connection ) ;
  return true                        ;
}

QString N::LearnSite::LookForMime(QString filename)
{
  QFileInfo FI(filename)                        ;
  QString suffix                                ;
  suffix = FI        . suffix     ( )           ;
  suffix = suffix    . toLower    ( )           ;
  suffix = suffix    . simplified ( )           ;
  int t  = MimeTypes . count      ( )           ;
  for (int i=0;i<t;i++)                         {
    if (MimeTypes[i].suffixes.contains(suffix)) {
      return MimeTypes[i].mime                  ;
    }                                           ;
  }                                             ;
  return ""                                     ;
}

bool N::LearnSite :: isImage  (
       QStringList & Filters  ,
       QString       filename ,
       QString       message  ,
       QString       debug    )
{
  QString F = filename                          ;
  NetPlan->Debug(80,QString(message).arg(F))    ;
  QFileInfo FX(F)                               ;
  QString   suffix = FX.suffix()                ;
  if (suffix.length()<=0) return false          ;
  suffix = suffix.toLower()                     ;
  suffix = "*." + suffix                        ;
  NetPlan->Debug(80,QString(debug).arg(suffix)) ;
  return Filters.contains(suffix)               ;
}

bool N::LearnSite :: isImage  (
       QStringList & Filters  ,
       QString       filename )
{
  nKickOut ( !TryGuess() , false )             ;
  QString message = Translations [ MsgImage  ] ;
  QString debug   = Translations [ MsgSuffix ] ;
  return LearnSite :: isImage                  (
           Filters                             ,
           filename                            ,
           message                             ,
           debug                             ) ;
}

bool N::LearnSite   :: addVariable (
       SqlConnection & SC          ,
       SUID            uuid        ,
       QString         name        ,
       QString         value       )
{
  QString Q                                   ;
  Q = SC.sql.InsertInto                       (
        TABLES(Variables)                     ,
        3                                     ,
        "uuid"                                ,
        "name"                                ,
        "value"                             ) ;
  SC . Prepare ( Q                          ) ;
  SC . Bind    ( "uuid"  , uuid             ) ;
  SC . Bind    ( "name"  , name  . toUtf8() ) ;
  SC . Bind    ( "value" , value . toUtf8() ) ;
  return  SC . Exec    (                    ) ;
}

bool N::LearnSite   :: deleteVariable (
       SqlConnection & SC             ,
       SUID            uuid           ,
       QString         name           )
{
  QString Q                               ;
  Q = SC.sql.DeleteFrom                   (
        TABLES(Variables)                 ,
        SC.sql.Where(2,"uuid","name")   ) ;
  SC . Prepare ( Q                      ) ;
  SC . Bind    ( "uuid" , uuid          ) ;
  SC . Bind    ( "name" , name.toUtf8() ) ;
  return SC . Exec ( )                    ;
}

bool N::LearnSite   :: setReturnCode (
       SqlConnection & SC            ,
       SUID            uuid          ,
       int             retcode       )
{
  return NetworkManager::assureHttpStatus(SC,uuid,retcode) ;
}

bool N::LearnSite   :: assignOriginate (
       SqlConnection & SC              ,
       SUID            uuid            ,
       SUID            from            )
{
  return NetworkManager::assureParent(SC,uuid,from) ;
}

bool N::LearnSite   :: ParseBody (
       SqlConnection & SC        ,
       SUID            uuid      ,
       QUrl          & url       ,
       QByteArray    & Body      )
{
  nKickOut ( Body.size() <= 0 , false )                        ;
  QString HTML    = QString::fromUtf8(Body)                    ;
  int     hndex   = HTML.indexOf("html",0,Qt::CaseInsensitive) ;
  bool    correct = true                                       ;
  if (hndex<0) correct = false                                 ;
  if (correct)                                                 {
    hndex = HTML.indexOf("body",0,Qt::CaseInsensitive)         ;
    if (hndex<0) correct = false                               ;
  }                                                            ;
  if (correct)                                                 {
    QString ux = url.toString()                                ;
    Time::msleep   ( 10  + (rand()%50)           )             ;
    addVariable    ( SC  , uuid , "Parsing" , ux )             ;
    ParseLinks     ( url , Body                  )             ;
    deleteVariable ( SC  , uuid , "Parsing"      )             ;
  }                                                            ;
  return correct                                               ;
}

bool N::LearnSite   :: setPageMime (
       SqlConnection & SC          ,
       SUID            uuid        ,
       QString         title       ,
       QString         mime        )
{
  bool correct = true                    ;
  if (mime.length()>0)                   {
    if (MimeManager::assureDocumentMime  (
      SC                                 ,
      uuid                               ,
      TYPE(URL)                          ,
      0                                  ,
      mime                           ) ) {
    } else correct = false               ;
  } else correct = false                 ;
  if (NetworkManager::assureTitle        (
        SC                               ,
        uuid                             ,
        0                                ,
        title                        ) ) {
  } else correct = false                 ;
  return correct                         ;
}

bool N::LearnSite   :: setBody (
       SqlConnection & SC      ,
       SUID            uuid    ,
       QByteArray    & body    )
{
  int     length = body.size()                             ;
  QString Q                                                ;
  Q = SC.sql.Update                                        (
        TABLES(WebContents)                                ,
        SC.sql.Where(1,"uuid")                             ,
        1                                                  ,
        "length"                                         ) ;
  SC . Prepare ( Q                 )                       ;
  SC . Bind    ( "uuid"   , uuid   )                       ;
  SC . Bind    ( "length" , length )                       ;
  SC . Exec    (                   )                       ;
  if (length<=0) return true                               ;
  return NetworkManager :: assignBody ( SC , uuid , body ) ;
}

bool N::LearnSite::setPageHeader  (
       SqlConnection & SC         ,
       SUID            uuid       ,
       int             retcode    ,
       QByteArray    & header     ,
       QString       & lastupdate )
{
  QString Q                                                  ;
  NetworkManager :: assureHttpStatus ( SC , uuid , retcode ) ;
  NetworkManager :: assignHeader     ( SC , uuid , header  ) ;
  Q = SC.sql.Update                                          (
        TABLES(WebContents)                                  ,
        SC.sql.Where(1,"uuid")                               ,
        1                                                    ,
        "lastupdate"                                       ) ;
  SC . Prepare ( Q                                         ) ;
  SC . Bind    ( "uuid"       , uuid                       ) ;
  SC . Bind    ( "lastupdate" , lastupdate.toUtf8()        ) ;
  return SC . Exec    (                                    ) ;
}

bool N::LearnSite   :: Associate (
       SqlConnection & SC        ,
       SUID            guid      ,
       UUIDs         & Uuids     )
{
  nKickOut ( guid <= 0 , false )                   ;
  QString Q                                        ;
  int     final = 0                                ;
  Q = SC.sql.SelectFrom                            (
        "position"                                 ,
        TABLES(NetGroups)                          ,
        QString("where "
                "`t1` = %1 "
                "and `t2` = %2 "
                "and `relation` = %3 "
                "and `first` = %4 "
                "order by `position` desc "
                "limit 0,1"                        )
        .arg( TYPE(URL)                            )
        .arg( TYPE(URL)                            )
        .arg( RELATE(Contains)                     )
        .arg( guid                                 )
      )                                            ;
  if (SC.Fetch(Q)) final = SC.Int(0)               ;
  final ++                                         ;
  //////////////////////////////////////////////////
  SUID uuid                                        ;
  foreach (uuid,Uuids)                             {
    Q = QString                                    (
          "insert into `%1` "
          "(`first`,`second`,`t1`,`t2`,`relation`,`position`) "
          "values (%2,%3,%4,%5,%6,%7) ;"           )
        . arg ( TABLES(NetGroups)                  )
        . arg ( guid                               )
        . arg ( uuid                               )
        . arg ( TYPE(URL)                          )
        . arg ( TYPE(URL)                          )
        . arg ( RELATE(Contains)                   )
        . arg ( final                            ) ;
    if ( SC . Query ( Q ) ) final++                ;
  }                                                ;
  return true                                      ;
}

bool N::LearnSite::ImportHost(QString host)
{
  if (!Hosts.contains(host)) Hosts << host ;
  return true                              ;
}

bool N::LearnSite   :: ImportUrls (
       SqlConnection & SC         ,
       SUID            uuid       ,
       QString         message    ,
       QString         debug      )
{
  QList<QUrl> Us                                                     ;
  QList<QUrl> Ux                                                     ;
  QStringList UR                                                     ;
  UUIDs       Guids                                                  ;
  Ux << Links                                                        ;
  Ux << Images                                                       ;
  if (Ux.count()<=0) return false                                    ;
  for (int i=0;i<Ux.count();i++) if (Ux[i].isValid())                {
    QString ss = Ux[i].toString()                                    ;
    QString hh = Ux[i].host    ()                                    ;
    if (hh.length()>0 && !UR.contains(ss))                           {
      UR << ss                                                       ;
      Us << Ux[i]                                                    ;
    }                                                                ;
  }                                                                  ;
  UR . clear ( )                                                     ;
  Ux . clear ( )                                                     ;
  if (Us.count()<=0) return false                                    ;
  ////////////////////////////////////////////////////////////////////
  NetPlan->ProgressText(ProgressId,QString(message).arg(Us.count())) ;
  ProgressValue = 0                                                  ;
  NetPlan -> Start    ( ProgressId , &ProgressValue , &ProgressGo )  ;
  NetPlan -> setRange ( ProgressId , 0 , Us.count() )                ;
  for (int i=0;ProgressGo && i<Us.count();i++)                       {
    QUrl u = Us[i]                                                   ;
    ProgressValue++                                                  ;
    if (u.isValid())                                                 {
      QString host = u.host()                                        ;
      SUID zuid = PageUuid ( SC , u )                                ;
      if (zuid<=0)                                                   {
        zuid = insertPage  ( SC , u )                                ;
        if ( ( zuid > 0 ) && ( zuid != uuid ) )                      {
          assignOriginate(SC,zuid,uuid)                              ;
        }                                                            ;
      }                                                              ;
      if ( ( zuid > 0 ) && ( zuid != uuid ) )                        {
        if (!Guids.contains(zuid))                                   {
          Guids << zuid                                              ;
        }                                                            ;
      }                                                              ;
      if (isAppendHost())                                            {
        if (host.length()>0) ImportHost(host)                        ;
      }                                                              ;
    }                                                                ;
  }                                                                  ;
  if (Guids.count()>0) Associate(SC,uuid,Guids)                      ;
  ////////////////////////////////////////////////////////////////////
  return true                                                        ;
}

bool N::LearnSite   :: AttachPicture (
       SqlConnection & SC            ,
       SUID            uuid          ,
       SUID            picture       ,
       QByteArray    & header        ,
       QString       & LastUpdate    ,
       QString       & MimeType      ,
       QString         message       ,
       QString         debug         )
{
  QByteArray Empty                                             ;
  NetPlan -> ProgressText ( ProgressId,message               ) ;
  NetPlan -> Debug        ( 75 , QString(debug).arg(picture) ) ;
  shiftDelay    (                                            ) ;
  setPageHeader ( SC , uuid , 200 , header , LastUpdate      ) ;
  setBody       ( SC , uuid , Empty                          ) ;
  setPageMime   ( SC , uuid , ""  , MimeType                 ) ;
  shiftDelay    (                                            ) ;
  //////////////////////////////////////////////////////////////
  QString Q                                                    ;
  int     final = 0                                            ;
  Q = SC.sql.SelectFrom                                        (
        "position"                                             ,
        TABLES(PictureGroups)                                  ,
        QString("where first = %1 "
                "and t1 = %2 "
                "and t2 = %3 "
                "and relation = %4 "
                "order by position desc "
                "limit 0,1"                                    )
        . arg ( picture                                        )
        . arg ( TYPE   ( Picture       )                       )
        . arg ( TYPE   ( URL           )                       )
        . arg ( RELATE ( Subordination )                       )
      )                                                        ;
  if (SC.Fetch(Q)) final = SC.Int(0)                           ;
  final ++                                                     ;
  //////////////////////////////////////////////////////////////
  Q = QString                                                  (
        "insert into `%1` "
        "(`first`,`second`,`t1`,`t2`,`relation`,`position`) "
        "values (%2,%3,%4,%5,%6,%7) ;"                         )
      . arg ( TABLES(PictureGroups)                            )
      . arg ( picture                                          )
      . arg ( uuid                                             )
      . arg ( TYPE(Picture)                                    )
      . arg ( TYPE(URL)                                        )
      . arg ( RELATE(Subordination)                            )
      . arg ( final                                          ) ;
  SC . Query ( Q )                                             ;
  return true                                                  ;
}

bool N::LearnSite   :: ImportPicture (
       SqlConnection & SC            ,
       QStringList   & ImageF        ,
       SUID            uuid          ,
       QUrl          & url           ,
       QString         filename      ,
       QByteArray    & header        ,
       QString       & lastUpdate    ,
       QString       & MIME          ,
       QString         importPicMsg  ,
       QString         importPicCnt  ,
       QString         msg           ,
       QString         dbg           )
{
  QByteArray BX                                                     ;
  QImage I                                                          ;
  SUID zuid = 0                                                     ;
  QFileInfo FI(filename)                                            ;
  QString suffix = FI.suffix()                                      ;
  QString sufx   = suffix.toLower()                                 ;
  sufx = "*." + sufx                                                ;
  shiftDelay ( )                                                    ;
  File::toByteArray(filename,BX)                                    ;
  if (BX.size()>2)                                                  {
    unsigned char * b = (unsigned char *)BX.data()                  ;
    if ( ( 0x47 == b[0] ) && ( 0x49 == b[1] ) )                     {
      QString nfile = filename                                      ;
      if ( suffix.length() > 0 )                                    {
        nfile = nfile.replace(suffix,"gif")                         ;
      } else nfile += ".gif"                                        ;
      QFile::rename ( filename , nfile )                            ;
      sufx = "*.gif"                                                ;
      filename = nfile                                              ;
      FI . setFile ( filename )                                     ;
    } else
    if ( ( 0x89 == b[0] ) && ( 0x50 == b[1] ) )                     {
      QString nfile = filename                                      ;
      if ( suffix.length() > 0 )                                    {
        nfile = nfile.replace(suffix,"png")                         ;
      } else nfile += ".png"                                        ;
      QFile::rename ( filename , nfile )                            ;
      sufx = "*.png"                                                ;
      filename = nfile                                              ;
      FI . setFile ( filename )                                     ;
    } else
    if ( ( 0x42 == b[0] ) && ( 0x4D == b[1] ) )                     {
      QString nfile = filename                                      ;
      if ( suffix.length() > 0 )                                    {
        nfile = nfile.replace(suffix,"bmp")                         ;
      } else nfile += ".bmp"                                        ;
      QFile::rename ( filename , nfile )                            ;
      sufx = "*.bmp"                                                ;
      filename = nfile                                              ;
      FI . setFile ( filename )                                     ;
    } else
    if ( ( BX.size()>4  )                                          &&
         ( 0xFF == b[0] )                                          &&
         ( 0xD8 == b[1] )                                          &&
         ( 0xFF == b[2] )                                          &&
         ( 0xE0 == b[3] )                                         ) {
      QString nfile = filename                                      ;
      if ( suffix.length() > 0 )                                    {
        nfile = nfile.replace(suffix,"jpg")                         ;
      } else nfile += ".jpg"                                        ;
      QFile::rename ( filename , nfile )                            ;
      sufx = "*.jpg"                                                ;
      filename = nfile                                              ;
      FI . setFile ( filename )                                     ;
    }                                                               ;
  }                                                                 ;
  BX . clear   (          )                                         ;
  if (ImageF.contains(sufx) && I.load(filename))                    {
    PictureManager PM((Plan *)NetPlan)                              ;
    NetPlan->ProgressText ( ProgressId , importPicMsg )             ;
    QString ux = url.toString()                                     ;
    addVariable(SC,uuid,"Importer",ux)                              ;
    shiftDelay ( )                                                  ;
    NetPlan->Debug(75,QString(importPicCnt).arg(filename))          ;
    ImportPicLock . lock   ( )                                      ;
    PM.Import(SC,filename,zuid)                                     ;
    ImportPicLock . unlock ( )                                      ;
    deleteVariable(SC,uuid,"Importer")                              ;
    if (zuid>0)                                                     {
      AttachPicture ( SC,uuid,zuid,header,lastUpdate,MIME,msg,dbg ) ;
    }                                                               ;
  }                                                                 ;
  if (RemoveImage()) QFile::remove(filename)                        ;
  return true                                                       ;
}

bool N::LearnSite   :: Redirect (
       SqlConnection & SC       ,
       SUID            uuid     ,
       QUrl          & source   ,
       QUrl          & location )
{
  shiftDelay()                              ;
  QString host = location . host ( )        ;
  SUID    zuid = PageUuid ( SC , location ) ;
  if (zuid<=0)                              {
    shiftDelay()                            ;
    zuid = insertPage  ( SC , location )    ;
    if (zuid>0)                             {
      shiftDelay()                          ;
      assignOriginate(SC,zuid,uuid)         ;
    }                                       ;
  }                                         ;
  stepDelay ( )                             ;
  if (host.length()>0) ImportHost(host)     ;
  return true                               ;
}

bool N::LearnSite   :: SaveAudio  (
       SqlConnection & SC         ,
       SUID            uuid       ,
       QUrl          & url        ,
       QByteArray    & Header     ,
       QByteArray    & Body       ,
       QString       & LastUpdate ,
       QString       & mimeType   )
{
  QString CT = mimeType                                      ;
  CT . simplified (                                        ) ;
  CT = CT.replace ( " " , ""                               ) ;
  shiftDelay      (                                        ) ;
  setPageHeader   ( SC , uuid , 200  , Header , LastUpdate ) ;
  setBody         ( SC , uuid , Body                       ) ;
  setPageMime     ( SC , uuid , ""   , CT                  ) ;
  return true                                                ;
}

bool N::LearnSite   :: SaveVideo  (
       SqlConnection & SC         ,
       SUID            uuid       ,
       QUrl          & url        ,
       QByteArray    & Header     ,
       QByteArray    & Body       ,
       QString       & LastUpdate ,
       QString       & mimeType   )
{
  QString CT = mimeType                                      ;
  CT . simplified (                                        ) ;
  CT = CT.replace ( " " , ""                               ) ;
  shiftDelay      (                                        ) ;
  setPageHeader   ( SC , uuid , 200  , Header , LastUpdate ) ;
  setBody         ( SC , uuid , Body                       ) ;
  setPageMime     ( SC , uuid , ""   , CT                  ) ;
  return true                                                ;
}

bool N::LearnSite::Download(SqlConnection & SC,SUID uuid,QUrl url)
{ QMutexLocker locker(&DownloadLock)                                          ;
  QString msgParsing = Translations [ MsgParsing ]                            ;
  QString msgUpdate  = Translations [ MsgUpdate  ]                            ;
  QString msgFetch   = Translations [ MsgFetch   ]                            ;
  QString ims        = Translations [ MsgIMS     ]                            ;
  QString imd        = Translations [ MsgIMD     ]                            ;
  QString imb        = Translations [ MsgIMB     ]                            ;
  QString mmb        = Translations [ MsgMMB     ]                            ;
  QString hnh        = Translations [ MsgHNH     ]                            ;
  QString rci        = Translations [ MsgRCI     ]                            ;
  QString urm        = Translations [ MsgURM     ]                            ;
  QString amt        = Translations [ MsgAMT     ]                            ;
  QString dlc        = Translations [ MsgDLC     ]                            ;
  QString dlf        = Translations [ MsgDLF     ]                            ;
  QString msf        = Translations [ MsgMSF     ]                            ;
  QString msg        = Translations [ MsgAPU     ]                            ;
  QString dbg        = Translations [ MsgDBG     ]                            ;
  QString ipm        = Translations [ MsgIPM     ]                            ;
  QString ipc        = Translations [ MsgIPC     ]                            ;
  QString cif        = Translations [ MsgCIF     ]                            ;
  QString wag        = Translations [ MsgWAG     ]                            ;
  QString rdl        = Translations [ MsgRDL     ]                            ;
  /////////////////////////////////////////////////////////////////////////////
  Ftp                 ftp                                                     ;
  QString             Agent    = GetAgent ( wag )                             ;
  QStringList         ImageF   = Images :: ExtensionFilters ( )               ;
  QString             filename = ""                                           ;
  QString             suffix   = ""                                           ;
  QByteArray          Header                                                  ;
  QByteArray          Body                                                    ;
  QString             Q                                                       ;
  HttpParser          HR                                                      ;
  QString             strUrl   = url . toString ( )                           ;
  QString             Referer  = ""                                           ;
  bool                doRefer  = TryReferer     ( )                           ;
  int                 dlTime   = DownloadTime   ( ) * 1000                    ;
  int                 cnTime   = ConnectTimeout ( ) * 1000                    ;
  ftp.connectionTimeout        = cnTime                                       ;
  /////////////////////////////////////////////////////////////////////////////
  ProgressMsg    ( ProgressId , QString(msgFetch).arg(strUrl) )               ;
  ProgressValue = 0                                                           ;
  NetPlan -> Start    ( ProgressId , &ProgressValue , &ProgressGo )           ;
  NetPlan -> setRange ( ProgressId , 0              , 0           )           ;
  Header . clear ( )                                                          ;
  Body   . clear ( )                                                          ;
  /////////////////////////////////////////////////////////////////////////////
  Q = SC.sql.SelectFrom("id",TABLES(WebContents),SC.WhereUuid(uuid))          ;
  if (!SC.Fetch(Q)) SC.insertUuid(TABLES(WebContents),uuid,"uuid")            ;
  /////////////////////////////////////////////////////////////////////////////
  if (doRefer)                                                                {
    SUID ruid = NetworkManager::PageParent(SC,uuid)                           ;
    if (ruid>0)                                                               {
      QUrl ux = PageUrl(SC,ruid)                                              ;
      Referer = ux.toString()                                                 ;
      if (Referer.length()<=0) doRefer = false                                ;
    } else doRefer = false                                                    ;
  }                                                                           ;
  /////////////////////////////////////////////////////////////////////////////
  ftp.Requests["user-agent"] = Agent                                          ;
  if (Referer.length()>0) ftp.Requests["referer"] = Referer                   ;
  setOptions ( ftp )                                                          ;
  Header = ftp . header ( url , HeaderTime() * 1000 )                         ;
  int  sc    = 0                                                              ;
  bool parse = true                                                           ;
  if (Header.size()<=0)                                                       {
    if (IgnoreDump()) sc = 200 ; else                                         {
      sc = 404                                                                ;
      parse = false                                                           ;
    }                                                                         ;
    DebugMsg ( 60 , QString(hnh).arg(strUrl) )                                ;
  } else                                                                      {
    QString EM                                                                ;
    HR . setHeader     ( Header )                                             ;
    sc = HR.statusCode (        )                                             ;
    if (sc!=200) parse = false                                                ;
    EM = QString(rci).arg(strUrl).arg(sc)                                     ;
    DebugMsg ( 60 , EM )                                                      ;
    if (!HR.hasContentType())                                                 {
      QStringList K = HR.Tags.keys()                                          ;
      QString     k                                                           ;
      DebugMsg ( 90 , QString::fromUtf8(Header) )                             ;
      foreach (k,K)                                                           {
        DebugMsg ( 90 , QString("%1 => %2").arg(k).arg(HR.value(k)) )         ;
      }                                                                       ;
      parse = false                                                           ;
    }                                                                         ;
  }                                                                           ;
  if (!parse)                                                                 {
    ProgressMsg ( ProgressId , QString(msf).arg(strUrl) )                     ;
    if (sc==301 || sc==302)                                                   {
      QString LXD = ""                                                        ;
      if (HR.hasKey("Last-Modified")) LXD = HR.value("Last-Modified")         ;
      if (HR.hasKey("Location"))                                              {
        QString NURL = HR.value("Location")                                   ;
        QUrl    nu(NURL)                                                      ;
        QUrl    O = url                                                       ;
        if (nu.host().length()<=0) O = O.resolved(nu) ; else O = nu           ;
        if (O.isValid())                                                      {
          DebugMsg ( 75 , QString(rdl).arg(strUrl).arg(NURL) )                ;
          Redirect ( SC , uuid , url , O )                                    ;
        }                                                                     ;
      }                                                                       ;
      setPageHeader ( SC , uuid , sc , Header , LXD )                         ;
      shiftDelay    (                               )                         ;
      return false                                                            ;
    }                                                                         ;
    if (sc==0   ) sc = 404                                                    ;
    if (sc>=1000) sc = 404                                                    ;
    if (sc< 0   ) sc = 404                                                    ;
    setReturnCode ( SC , uuid , sc )                                          ;
    shiftDelay    (                )                                          ;
    return false                                                              ;
  }                                                                           ;
  /////////////////////////////////////////////////////////////////////////////
  QString XZU = NetworkManager :: PageURI ( SC , uuid )                       ;
  if (XZU.length()>0)                                                         {
    QString Uri= XZU                                                          ;
    QFileInfo FI(Uri)                                                         ;
    suffix = FI.suffix()                                                      ;
    suffix = suffix.toLower()                                                 ;
    DebugMsg ( 75 , QString(urm).arg(Uri) )                                   ;
  }                                                                           ;
  /////////////////////////////////////////////////////////////////////////////
  QString CT = HR.contentType() ; CT = CT.toLower()                           ;
  if (NetPlan->Verbose>=90)                                                   {
    DebugMsg ( 90 , QString("Content-Type : %1").arg(CT) )                    ;
  }                                                                           ;
  if (suffix.length()>0)                                                      {
    suffix = suffix . simplified ( )                                          ;
    if (suffix.length()>6) suffix = ""                                        ;
                      else suffix = "." + suffix                              ;
  }                                                                           ;
  if (suffix.length()<=0 && CT=="text/html") suffix = ".html"                 ;
  filename = QString("%1%2").arg(uuid).arg(suffix)                            ;
  if (suffix.length()> 0 && CT.length()<=0)                                   {
    CT = LookForMime(filename)                                                ;
    DebugMsg ( 75 , QString(amt).arg(CT) )                                    ;
  }                                                                           ;
  /////////////////////////////////////////////////////////////////////////////
  ProgressMsg ( ProgressId , QString(dlc).arg(strUrl) )                       ;
  if (HR.hasContentLength())                                                  {
    ProgressValue = 0                                                         ;
    ftp.Written   = &ProgressValue                                            ;
    NetPlan -> Start    ( ProgressId , &ProgressValue , &ProgressGo )         ;
    NetPlan -> setRange ( ProgressId , 0       , HR.contentLength() )         ;
  } else                                                                      {
    ProgressValue = 0                                                         ;
    NetPlan -> Start    ( ProgressId , &ProgressValue , &ProgressGo )         ;
    NetPlan -> setRange ( ProgressId , 0              , 0           )         ;
  }                                                                           ;
  filename = Temporary(filename)                                              ;
  QFile::remove(filename)                                                     ;
  QFile F(filename)                                                           ;
  Body . clear ( )                                                            ;
  ftp.Requests["user-agent"] = Agent                                          ;
  if (Referer.length()>0) ftp.Requests["referer"] = Referer                   ;
  setOptions ( ftp )                                                          ;
  bool TryLoading  = false                                                    ;
  bool ExamContent = true                                                     ;
  if (CT  . contains ("video"           ) ) dlTime *= 4                       ;
  if (ftp . download ( url , F , dlTime ) )                                   {
    TryLoading = true                                                         ;
  } else                                                                      {
    DebugMsg(60,QString(dlf).arg(strUrl))                                     ;
    QFile :: remove ( filename )                                              ;
  }                                                                           ;
  /////////////////////////////////////////////////////////////////////////////
  if (NetPlan->Verbose>=95 && ftp.dlHeader.size()>0)                          {
    QString M = QString::fromUtf8(ftp.dlHeader)                               ;
    DebugMsg ( 95 , strUrl )                                                  ;
    DebugMsg ( 95 , M      )                                                  ;
  }                                                                           ;
  if (Header.size()<=0 && ftp.dlHeader.size()>0)                              {
    Header = ftp.dlHeader                                                     ;
    HR     = HttpParser(ftp.dlHeader)                                         ;
    CT     = HR.contentType()                                                 ;
    CT     = CT.toLower    ()                                                 ;
  }                                                                           ;
  DownloadedHeader = Header                                                   ;
  /////////////////////////////////////////////////////////////////////////////
  if ( TryLoading )                                                           {
    if (CT.contains("application") && CT.contains("x-rar-compressed"))        {
      QString XMZ                                                             ;
      XMZ = QString("Found %1 is a RAR Archive").arg(strUrl)                  ;
      DebugMsg ( 35 , XMZ )                                                   ;
    }                                                                         ;
    if (ExamContent)                                                          {
      if (F.open(QIODevice::ReadOnly))                                        {
        Body = F.readAll ( )                                                  ;
        F    . close     ( )                                                  ;
      }                                                                       ;
    }                                                                         ;
  }                                                                           ;
  /////////////////////////////////////////////////////////////////////////////
  bool sizeError = false                                                      ;
  if (Body.size()<=0) sizeError = true                                        ;
  if (HR.hasContentLength())                                                  {
    sizeError = (HR.contentLength() != Body.size())                           ;
  } else                                                                      {
    if (TryGuess())                                                           {
      if (Body.contains("<html")) CT = "text/html"                            ;
    }                                                                         ;
  }                                                                           ;
  if (sizeError)                                                              {
    QString EM = QString(msf).arg(strUrl)                                     ;
    ProgressMsg     ( ProgressId , EM  )                                      ;
    DebugMsg        ( 85         , EM  )                                      ;
    sc = 404                                                                  ;
    Time :: msleep  ( 10 + (rand()%50) )                                      ;
    setReturnCode   ( SC , uuid , sc   )                                      ;
    QFile :: remove ( filename         )                                      ;
    return false                                                              ;
  }                                                                           ;
  ProgressValue = 0                                                           ;
  /////////////////////////////////////////////////////////////////////////////
  QString LUD = ""                                                            ;
  if (HR.hasKey("Last-Modified")) LUD = HR.value("Last-Modified")             ;
  /////////////////////////////////////////////////////////////////////////////
  if ( CT . contains ( "image" ) || isImage ( ImageF , filename ) )           {
    if (Body.size()>0)                                                        {
      ProgressMsg  ( ProgressId , cif )                                       ;
      ImportPicture                                                           (
        SC                                                                    ,
        ImageF                                                                ,
        uuid                                                                  ,
        url                                                                   ,
        filename                                                              ,
        Header                                                                ,
        LUD                                                                   ,
        CT                                                                    ,
        ipm                                                                   ,
        ipc                                                                   ,
        msg                                                                   ,
        dbg                                                                 ) ;
    }                                                                         ;
    return true                                                               ;
  }                                                                           ;
  if (CT.contains("video"))                                                   {
    DebugMsg         ( 75,QString(mmb).arg(Body.size()).arg(uuid) )           ;
    ProgressMsg      ( ProgressId,QString(msgUpdate).arg(strUrl)  )           ;
    return SaveVideo ( SC , uuid , url , Header , Body , LUD , CT )           ;
  }                                                                           ;
  if (CT.contains("audio"))                                                   {
    DebugMsg         ( 75,QString(mmb).arg(Body.size()).arg(uuid) )           ;
    ProgressMsg      ( ProgressId,QString(msgUpdate).arg(strUrl)  )           ;
    return SaveAudio ( SC , uuid , url , Header , Body , LUD , CT )           ;
  }                                                                           ;
  /////////////////////////////////////////////////////////////////////////////
  ProgressMsg ( ProgressId , QString(msgParsing).arg(strUrl) )                ;
  ResetParser ( )                                                             ;
  if (CT=="text/html") ParseBody ( SC , uuid , url , Body )                   ;
  QFile::remove(filename)                                                     ;
  /////////////////////////////////////////////////////////////////////////////
  QString title = parseTitle ( )                                              ;
  DebugMsg      ( 75,QString(imb).arg(Body.size()).arg(uuid) )                ;
  ProgressMsg   ( ProgressId,QString(msgUpdate).arg(strUrl)  )                ;
  shiftDelay    (                                            )                ;
  setPageHeader ( SC , uuid , 200   , Header , LUD           )                ;
  setBody       ( SC , uuid , Body                           )                ;
  setPageMime   ( SC , uuid , title , CT                     )                ;
  ImportUrls    ( SC , uuid , ims   , imd                    )                ;
  /////////////////////////////////////////////////////////////////////////////
  Body   . clear ( )                                                          ;
  Header . clear ( )                                                          ;
  return true                                                                 ;
}
