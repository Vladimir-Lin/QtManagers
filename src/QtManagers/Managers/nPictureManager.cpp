#include <qtmanagers.h>

#define plan         PicturePlan
#define TYPE(T)      Types::T
#define RELATE(R)    Groups::R
#define PictureDEPOT PlanTable(PictureDepot)
#define PictureINDEX PlanTable(PictureUuid)
#define PictureORDER PlanTable(PictureOrders)
#define ThumbDEPOT   PlanTable(ThumbDepot)
#define ThumbINDEX   PlanTable(ThumbUuid)

N::PictureManager:: PictureManager ( Plan * p )
                  : PicturePlan    (        p )
{
}

N::PictureManager::~PictureManager (void)
{
}

QString N::PictureManager::SafeTable(QString name)
{
  QString     T                             ;
  QStringList L                             ;
  QStringList R                             ;
  if   ( name . contains ( '.' ) )          {
    R = name . split ( '.' )                ;
    foreach ( T , R )                       {
      if ( ! T . contains ( '`' ) )         {
        T = QString ( "`%1`" ) . arg ( T )  ;
      }                                     ;
      L << T                                ;
    }                                       ;
    T = L    . join  ( "." )                ;
  } else                                    {
    if ( name . contains ( '`' ) )          {
      T = name                              ;
    } else                                  {
      T = QString ( "`%1`" ) . arg ( name ) ;
    }                                       ;
  }                                         ;
  return T                                  ;
}

QString N::PictureManager::PictureTable(QString name)
{
  QString PT                                                  ;
  PT = QString ( "create table %1 ("
                 "`id` bigint primary key auto_increment,"
                 "`uuid` bigint unsigned not null,"
                 "`file` longblob not null,"
                 "`ltime` timestamp default CURRENT_TIMESTAMP on update CURRENT_TIMESTAMP,"
                 "unique `uuid` (`uuid`),"
                 "key `ltime` (`ltime`),"
                 "key `file` (`file`(512))"
                 ") Engine = MyISAM default Charset = Utf8 ;" )
       . arg   ( SafeTable ( name )                         ) ;
  return  PT ;
}

QString N::PictureManager::ThumbTable(QString name)
{
  QString PT                                                  ;
  PT = QString ( "create table %1 ("
                 "`id` bigint primary key auto_increment,"
                 "`uuid` bigint unsigned not null,"
                 "`thumb` longblob not null,"
                 "`ltime` timestamp default CURRENT_TIMESTAMP on update CURRENT_TIMESTAMP,"
                 "unique `uuid` (`uuid`),"
                 "key `ltime` (`ltime`),"
                 "key `thumb` (`thumb`(512))"
                 ") Engine = MyISAM default Charset = Utf8 ;" )
       . arg   ( SafeTable ( name )                         ) ;
  return  PT ;
}

QString N::PictureManager::HistogramTable(QString name)
{
  QString PT                                                  ;
  PT = QString ( "create table %1 ("
                 "`id` bigint primary key auto_increment,"
                 "`uuid` bigint unsigned not null,"
                 "`total` double default 0,"
                 "`r` double default 0,"
                 "`g` double default 0,"
                 "`b` double default 0,"
                 "`a` double default 0,"
                 "`histogram` blob,"
                 "`ltime` timestamp default CURRENT_TIMESTAMP on update CURRENT_TIMESTAMP,"
                 "unique `uuid` (`uuid`),"
                 "key `total` (`total`),"
                 "key `r` (`r`),"
                 "key `g` (`g`),"
                 "key `b` (`b`),"
                 "key `a` (`a`)"
                 ") Engine = MyISAM default Charset = Utf8 ;" )
       . arg   ( SafeTable ( name )                         ) ;
  return  PT ;
}

int N::PictureManager::Count(SqlConnection & Connection)
{
  return Connection.count(PictureORDER) ;
}

int N::PictureManager::Count(SqlConnection & Connection,SUID uuid,int type)
{
  QString Where                                 ;
  Where = GroupItems :: FirstItem               (
            uuid                                ,
            type                                ,
            TYPE   ( Picture       )            ,
            RELATE ( Subordination )          ) ;
  return Connection.count(PlanTable(Groups),Where) ;
}

int N::PictureManager::Count(SqlConnection & Connection,SUID uuid,int type,int relation)
{
  QString Where                                 ;
  Where = GroupItems :: FirstItem               (
            uuid                                ,
            type                                ,
            TYPE   ( Picture       )            ,
            relation                          ) ;
  return Connection.count(PlanTable(Groups),Where) ;
}

QByteArray N::PictureManager::toColors(QImage & Image)
{
  QByteArray CG                                     ;
  QList<unsigned int> colors                        ;
  QList<unsigned int> C                             ;
  QImage * V = new QImage()                         ;
  *V = Image.convertToFormat(QImage::Format_ARGB32) ;
  unsigned char * bits   = V->bits ()               ;
  int             total  = V->width() * V->height() ;
  int             dp     = 0                        ;
  if (total<64000000)                               {
    for (int ii=0;ii<total;ii++,dp+=4)              {
      unsigned int c = 0                            ;
      int b = bits [ dp     ]                       ;
      int g = bits [ dp + 1 ]                       ;
      int r = bits [ dp + 2 ]                       ;
      c  = r; c <<= 8                               ;
      c |= g; c <<= 8                               ;
      c |= b                                        ;
      C << c                                        ;
    }                                               ;
    if (C.count()>1)                                {
      qSort(C.begin(),C.end())                      ;
      unsigned int c = C[0]                         ;
      int          t = C.count()                    ;
      unsigned int x                                ;
      colors << c                                   ;
      for (int ii=1;ii<t;ii++)                      {
        x = C[ii]                                   ;
        if (c!=x)                                   {
          c = x                                     ;
          colors << c                               ;
        }                                           ;
      }                                             ;
    } else colors = C                               ;
  }                                                 ;
  delete V                                          ;
  CG.resize(colors.count()*sizeof(unsigned int))    ;
  unsigned int * dd = (unsigned int *)CG.data()     ;
  int t = colors.count()                            ;
  for (int i=0;i<t;i++) dd[i] = colors[i]           ;
  return CG                                         ;
}

QByteArray N::PictureManager::IntoPNG(QImage & image)
{
  QByteArray B                      ;
  QByteArray C                      ;
  QBuffer    A(&B)                  ;
  if (A.open(QIODevice::WriteOnly)) {
    image.save(&A,"PNG")            ;
    C = B                           ;
    A.close()                       ;
  }                                 ;
  B.clear()                         ;
  return C                          ;
}

bool N::PictureManager::IntoPNG(QImage & image,QByteArray & PNG)
{
  nKickOut ( image.isNull() , false ) ;
  PNG . clear  (      )               ;
  QBuffer    A ( &PNG )               ;
  if (A.open(QIODevice::WriteOnly))   {
    image . save  ( &A , "PNG" )      ;
    A     . close (            )      ;
    return ( PNG.size() > 0 )         ;
  }                                   ;
  return false                        ;
}

QImage * N::PictureManager::MimeImage(const QMimeData * mime)
{
  if (IsNull     (mime)) return NULL                      ;
  if (!mime->hasImage()) return NULL                      ;
  QImage image = qvariant_cast<QImage>(mime->imageData()) ;
  if (image.isNull   ()) return NULL                      ;
  QImage * ii = new QImage()                              ;
  *ii = image                                             ;
  return ii                                               ;
}

void N::PictureManager::ColorImage(QImage & image,QColor color)
{
  QRect    R(0,0,image.width(),image.height()) ;
  QPainter p                                   ;
  p . begin     ( &image                     ) ;
  p . setPen    ( QColor(color)              ) ;
  p . setBrush  ( QColor(color)              ) ;
  p . drawRect  ( R                          ) ;
  p . end       (                            ) ;
}

void N::PictureManager::ColorIcon(QIcon & icon,QSize size,QColor color)
{
  QImage image ( size  , QImage::Format_ARGB32 ) ;
  ColorImage   ( image , color                 ) ;
  icon = QIcon ( QPixmap::fromImage ( image )  ) ;
}

bool N::PictureManager::Equal(QImage & image,QSize size)
{
  nKickOut ( image . width  ( ) != size . width  ( ) , false ) ;
  nKickOut ( image . height ( ) != size . height ( ) , false ) ;
  return true                                                  ;
}

bool N::PictureManager::Smaller(QImage & image,QSize size)
{
  nKickOut ( image . width  ( ) > size . width  ( ) , false ) ;
  nKickOut ( image . height ( ) > size . height ( ) , false ) ;
  return true                                                 ;
}

QIcon N::PictureManager::Icon(QImage * image,QSize size)
{
  if (Equal(*image,size))                          {
    return QIcon(QPixmap::fromImage(*image))       ;
  }                                                ;
  QImage I                                         ;
  if (!Smaller(*image,size)) I = Icon(*image,size) ;
                        else I = *image            ;
  //////////////////////////////////////////////////
  QImage W(size,QImage::Format_ARGB32)             ;
  memset(W.bits(),0,W.byteCount())                 ;
  int x = size . width  () - I . width  ()         ;
  int y = size . height () - I . height ()         ;
  //////////////////////////////////////////////////
  QPainter p                                       ;
  p . begin     ( &W            )                  ;
  p . drawImage ( x/2 , y/2 , I )                  ;
  p . end       (               )                  ;
  return QIcon(QPixmap::fromImage(W))              ;
}

QImage N::PictureManager::Icon(QImage & image,QSize size)
{
  if (Smaller(image,size)) return image ;
  return image.scaled                   (
           size                         ,
           Qt::KeepAspectRatio          ,
           Qt::SmoothTransformation   ) ;
}

bool N::PictureManager::Icon (
       QImage & image      ,
       QSize    size       ,
       QImage & target     )
{
  if (Smaller(image,size))                {
    target = image                        ;
  } else                                  {
    target = image.scaled                 (
               size                       ,
               Qt::KeepAspectRatio        ,
               Qt::SmoothTransformation ) ;
  }                                       ;
  return ( ! target.isNull() )            ;
}

QByteArray N::PictureManager::File(QString filename)
{
  QByteArray data                                         ;
  QFile      F ( filename )                               ;
  if ( ! F . exists (                     ) ) return data ;
  if ( ! F . open   ( QIODevice::ReadOnly ) ) return data ;
  data = F . readAll ( )                                  ;
  F . close ( )                                           ;
  return data                                             ;
}

bool N::PictureManager::File(QString filename,QByteArray & Data)
{
  QFile F(filename)                                        ;
  nKickOut ( ! F . exists (                     ) , true ) ;
  nKickOut ( ! F . open   ( QIODevice::ReadOnly ) , true ) ;
  Data = F . readAll ( )                                   ;
  F        . close   ( )                                   ;
  return Data . size ( ) > 0                               ;
}

bool N::PictureManager::Dump(QString filename,QByteArray & Data)
{
  if ( Data . size ( ) <= 0 ) return false ;
  QFile F ( filename )                     ;
  if ( F . open ( QIODevice::WriteOnly ) ) {
    F . write ( Data )                     ;
    F . close (      )                     ;
    return true                            ;
  }                                        ;
  return false                             ;
}

bool N::PictureManager::Dump(QDir & directory,QString filename,QByteArray & Data)
{
  return Dump(directory.absoluteFilePath(filename),Data) ;
}

int N::PictureManager::Checksum(QByteArray & data)
{
  return qChecksum(data.data(),data.size());
}

bool N::PictureManager:: PictureBlob (
       SqlConnection   & Connection  ,
       SUID              uuid        ,
       QByteArray      & blob        )
{
  QString Q                            ;
  int     filesize = 0                 ;
  blob . clear ( )                     ;
  Q = Connection.sql.SelectFrom        (
        "filesize"                     ,
        PictureINDEX                   ,
        Connection.WhereUuid(uuid)   ) ;
  if (Connection.Fetch(Q))             {
    filesize = Connection.Int(0)       ;
  }                                    ;
  if (filesize>0)                      {
    Q = Connection.sql.SelectFrom      (
          "file"                       ,
          PictureDEPOT                 ,
          Connection.WhereUuid(uuid) ) ;
    if (Connection.Fetch(Q))           {
      blob = Connection.ByteArray(0)   ;
      if (blob.size()!=filesize)       {
        blob . resize ( filesize )     ;
      }                                ;
    }                                  ;
  }                                    ;
  return ( blob . size () > 0 )        ;
}

bool N::PictureManager::InsertBlob   (
       SqlConnection & Connection ,
       SUID             uuid       ,
       QByteArray     & blob       )
{
  QString Q                                  ;
  Q = Connection.sql.InsertInto              (
        PictureDEPOT                         ,
        2                                    ,
        "uuid"                               ,
        "file"                             ) ;
  Connection . Prepare     ( Q             ) ;
  Connection . Bind        ( "uuid" , uuid ) ;
  Connection . Bind        ( "file" , blob ) ;
  return Connection . Exec (               ) ;
}

bool N::PictureManager::UpdateBlob   (
       SqlConnection & Connection ,
       SUID             uuid       ,
       QByteArray     & blob       )
{
  QString Q                                  ;
  Q = Connection.sql.Update                  (
        PictureDEPOT                         ,
        Connection.sql.Where(1,"uuid")       ,
        1                                    ,
        "file"                             ) ;
  Connection . Prepare     ( Q             ) ;
  Connection . Bind        ( "uuid" , uuid ) ;
  Connection . Bind        ( "file" , blob ) ;
  return Connection . Exec (               ) ;
}

bool N::PictureManager::AssureBlob   (
       SqlConnection & Connection ,
       SUID             uuid       ,
       QByteArray     & blob       )
{
  if (BlobExists(Connection,uuid))                 {
    return UpdateBlob ( Connection , uuid , blob ) ;
  }                                                ;
  return   InsertBlob ( Connection , uuid , blob ) ;
}

bool N::PictureManager::DeleteBlob   (
       SqlConnection & Connection ,
       SUID             uuid       )
{
  QString Q                          ;
  Q = Connection.sql.DeleteFrom      (
        PictureDEPOT                 ,
        Connection.WhereUuid(uuid) ) ;
  return Connection . Query ( Q    ) ;
}

bool N::PictureManager::BlobExists   (
       SqlConnection & Connection ,
       SUID             uuid       )
{
  QString Q                          ;
  int     Id = 0                     ;
  Q = Connection.sql.SelectFrom      (
        "id"                         ,
        PictureDEPOT                 ,
        Connection.WhereUuid(uuid) ) ;
  if (Connection.Fetch(Q))           {
    Id = Connection . Int ( 0 )      ;
  }                                  ;
  return ( Id > 0 )                  ;
}

SUID N::PictureManager :: Find       (
       SqlConnection    & Connection ,
       QByteArray       & image      )
{
  nKickOut ( image . size ( ) <= 0 , 0 )                                     ;
  ////////////////////////////////////////////////////////////////////////////
  QString Q                                                                  ;
  QString Where                                                              ;
  UUIDs   Uuids                                                              ;
  SUID    uuid                                                               ;
  int     checksum = Checksum(image)                                         ;
  ////////////////////////////////////////////////////////////////////////////
  Where = QString ( "where filesize = %1 and checksum = %2"                  )
          .arg(image.size()).arg(checksum)                                   ;
  Uuids = Connection.Uuids                                                   (
            PictureINDEX                                                     ,
            "uuid"                                                           ,
            Where                                                          ) ;
  if ( Uuids . count ( ) <= 0 ) return 0                                     ;
  ////////////////////////////////////////////////////////////////////////////
  unsigned int adler = ADLER32 ( image )                                     ;
  QByteArray   md5   = QCryptographicHash::hash ( image , QCryptographicHash::Md5  ) ;
  QByteArray   sha1  = QCryptographicHash::hash ( image , QCryptographicHash::Sha1 ) ;
  md5  = md5  . toHex ( )                                                    ;
  sha1 = sha1 . toHex ( )                                                    ;
  QByteArray Blob                                                            ;
  foreach ( uuid , Uuids )                                                   {
    Q = Connection . sql . SelectFrom                                        (
          "`adler32`,`md5`,`sha1`"                                           ,
          PlanTable(PictureSummary)                                             ,
          Connection . WhereUuid ( uuid )                                  ) ;
    if ( Connection . Fetch ( Q ) )                                          {
      unsigned int AD = Connection . Value     ( 0 ) . toUInt ( )            ;
      QByteArray   MD = Connection . ByteArray ( 1 )                         ;
      QByteArray   SH = Connection . ByteArray ( 2 )                         ;
      if ( ( AD == adler ) && ( MD == md5 ) && ( SH == sha1 ) )              {
        if ( PictureBlob ( Connection , uuid , Blob ) )                      {
          if ( Blob == image )                                               {
            Blob . clear ( )                                                 ;
            return uuid                                                      ;
          }                                                                  ;
        }                                                                    ;
      }                                                                      ;
    }                                                                        ;
  }                                                                          ;
  return 0                                                                   ;
}

int N::PictureManager::ColorCount(SqlConnection & SC,SUID u)
{
  int     colors = 0           ;
  QString Q                    ;
  Q = SC . sql . SelectFrom    (
        "colors"               ,
        PlanTable(PictureColors)  ,
        SC . WhereUuid ( u ) ) ;
  if (SC.Fetch(Q))             {
    colors = SC.Int(0)         ;
  }                            ;
  return colors                ;
}

bool N::PictureManager :: InsertPicture (
       SqlConnection    & Connection    ,
       SUID               uuid          ,
       QByteArray       & image         ,
       QString          & Suffix        ,
       int                mimeid        ,
       int                checksum      )
{
  QString Q                                                 ;
  ///////////////////////////////////////////////////////////
  if ( ! AssureBlob ( Connection , uuid , image ) )         {
    return false                                            ;
  }                                                         ;
  Q = Connection . sql . InsertInto                         (
        PictureINDEX                                        ,
        5                                                   ,
        "uuid"                                              ,
        "mimeid"                                            ,
        "suffix"                                            ,
        "filesize"                                          ,
        "checksum"                                        ) ;
  Connection . Prepare ( Q)                                 ;
  Connection . Bind    ( "uuid"     , uuid                ) ;
  Connection . Bind    ( "mimeid"   , mimeid              ) ;
  Connection . Bind    ( "suffix"   , Suffix . toUtf8 ( ) ) ;
  Connection . Bind    ( "filesize" , image  . size   ( ) ) ;
  Connection . Bind    ( "checksum" , checksum            ) ;
  return Connection . Exec (                              ) ;
}

SUID N::PictureManager :: Append     (
       SqlConnection    & Connection ,
       QString            filename   ,
       QByteArray       & image      )
{
  QFileInfo FI(filename)                        ;
  QString   Suffix   = FI.suffix()              ;
  QString   S        = Suffix.toLower()         ;
  SUID      uuid     = 0                        ;
  int       mimeid   = 0                        ;
  int       checksum = Checksum(image)          ;
  ///////////////////////////////////////////////
  if (S.length()>0)                             {
    QString   Q                                 ;
    Q = Connection.sql.SelectFrom               (
          "mimeid"                              ,
          PlanTable(Extensions)                    ,
          QString("where name = '%1'").arg(S) ) ;
    if (Connection.Fetch(Q))                    {
      mimeid = Connection.Int(0)                ;
    }                                           ;
  }                                             ;
  ///////////////////////////////////////////////
  uuid = Connection.Unique                      (
           PlanTable(MajorUuid)                    ,
           "uuid"                               ,
           7920                               ) ;
  Connection.insertUuid                         (
           PlanTable(MajorUuid)                    ,
           uuid                                 ,
           TYPE(Picture)                      ) ;
  if (InsertPicture                             (
        Connection                              ,
        uuid                                    ,
        image                                   ,
        Suffix                                  ,
        mimeid                                  ,
        checksum                            ) ) {
    return uuid                                 ;
  }                                             ;
  ///////////////////////////////////////////////
  return 0                                      ;
}

SUID N::PictureManager :: AppendPNG  (
       SqlConnection & Connection ,
       QByteArray     & image      )
{
  QString   Suffix   = "PNG"           ;
  SUID      uuid     = 0               ;
  int       mimeid   = 951             ;
  int       checksum = Checksum(image) ;
  //////////////////////////////////////
  uuid = Connection.Unique             (
           PlanTable(MajorUuid)           ,
           "uuid"                      ,
           7920                      ) ;
  Connection.insertUuid                (
           PlanTable(MajorUuid)           ,
           uuid                        ,
           TYPE(Picture)             ) ;
  if (InsertPicture                    (
        Connection                     ,
        uuid                           ,
        image                          ,
        Suffix                         ,
        mimeid                         ,
        checksum                   ) ) {
    return uuid                        ;
  }                                    ;
  //////////////////////////////////////
  return 0                             ;
}

bool N::PictureManager::AppendThumb  (
       SqlConnection & Connection ,
       SUID             uuid       ,
       QByteArray     & image      ,
       QByteArray     & thumb      ,
       QImage         & Original   ,
       QImage         & Small      )
{
  QString Q                                           ;
  /////////////////////////////////////////////////////
  Q = Connection.sql.InsertInto                       (
        ThumbDEPOT                                    ,
        2                                             ,
        "uuid"                                        ,
        "thumb"                               ) + ";" ;
  Connection.Prepare(Q                              ) ;
  Connection.Bind   ("uuid"      ,uuid              ) ;
  Connection.Bind   ("thumb"     ,thumb             ) ;
  if (!Connection.Exec()) return false                ;
  /////////////////////////////////////////////////////
  Q = Connection.sql.InsertInto                       (
        ThumbINDEX                                    ,
        7                                             ,
        "uuid"                                        ,
        "filesize"                                    ,
        "iconsize"                                    ,
        "width"                                       ,
        "height"                                      ,
        "iconwidth"                                   ,
        "iconheight"                          ) + ";" ;
  Connection.Prepare(Q                              ) ;
  Connection.Bind   ("uuid"      ,uuid              ) ;
  Connection.Bind   ("filesize"  ,image   .size  () ) ;
  Connection.Bind   ("iconsize"  ,thumb   .size  () ) ;
  Connection.Bind   ("width"     ,Original.width () ) ;
  Connection.Bind   ("height"    ,Original.height() ) ;
  Connection.Bind   ("iconwidth" ,Small   .width () ) ;
  Connection.Bind   ("iconheight",Small   .height() ) ;
  return Connection.Exec()                            ;
}

bool N::PictureManager::Import(QString filename)
{
  QByteArray D = PictureManager::File ( filename )                           ;
  if ( D . size ( ) <= 0 )                                                   {
    #if defined(QT_DEBUG)
    plan -> Debug ( 30                                                       ,
                    QString ( "%1 has a size = 0 problem.").arg(filename)  ) ;
    #endif
    return false                                                             ;
  }                                                                          ;
  SUID PictureUuid = 0                                                       ;
  bool correct = Import ( filename , D , PictureUuid )                       ;
  D . clear ( )                                                              ;
  return correct                                                             ;
}

bool N::PictureManager::Import (
       QString  filename       ,
       SUID   & PictureUuid    )
{
  QByteArray D = PictureManager::File ( filename )                           ;
  if ( D . size ( ) <= 0 )                                                   {
    #if defined(QT_DEBUG)
    plan -> Debug ( 30                                                       ,
                    QString ( "%1 has a size = 0 problem.").arg(filename)  ) ;
    #endif
    return false                                                             ;
  }                                                                          ;
  bool correct = Import ( filename , D , PictureUuid )                       ;
  D . clear             (                            )                       ;
  return correct                                                             ;
}

bool N::PictureManager::Import     (
       SqlConnection & SC          ,
       QString         filename    ,
       SUID          & PictureUuid )
{
  QByteArray D = PictureManager::File ( filename )                           ;
  if ( D . size ( ) <= 0 )                                                   {
    #if defined(QT_DEBUG)
    plan -> Debug ( 30                                                       ,
                    QString ( "%1 has a size = 0 problem.").arg(filename)  ) ;
    #endif
    return false                                                             ;
  }                                                                          ;
  bool correct = Import ( SC , filename , D , PictureUuid )                  ;
  D . clear             (                                 )                  ;
  return correct                                                             ;
}

bool N::PictureManager::Import (QString filename,QByteArray & Data)
{
  SUID PictureUuid = 0                            ;
  return Import ( filename , Data , PictureUuid ) ;
}

bool N::PictureManager::Import     (
       SqlConnection & Connection  ,
       QString         filename    ,
       QByteArray    & Data        ,
       SUID          & PictureUuid )
{
  QSize      V ( 128 , 128 )                             ;
  bool       success = false                             ;
  QImage   * I       = NULL                              ;
  QImage   * S       = NULL                              ;
  bool       cx      = true                              ;
  QByteArray D                                           ;
  ////////////////////////////////////////////////////////
  PictureUuid        = 0                                 ;
  ////////////////////////////////////////////////////////
  I = new QImage ( )                                     ;
  if ( cx && ( ! I -> load   ( filename ) ) ) cx = false ;
  if ( cx && (   I -> isNull (          ) ) ) cx = false ;
  if ( ! cx )                                            {
    #if defined(QT_DEBUG)
    plan -> Debug ( 30 , QString ( "%1 can not be loaded as an image.").arg(filename) ) ;
    #endif
    delete I                                             ;
    return false                                         ;
  }                                                      ;
  ////////////////////////////////////////////////////////
   S = new QImage (        )                             ;
  *S = Icon       ( *I , V )                             ;
  if ( cx && ( S -> isNull ( ) ) ) cx = false            ;
  if ( cx && ( ! IntoPNG(*S,D) || D.size() <= 0 ) )      {
    cx = false                                           ;
  }                                                      ;
  if ( ! cx )                                            {
    #if defined(QT_DEBUG)
    plan -> Debug ( 30 , QString ( "%1 can not be converted into icon.").arg(filename) ) ;
    #endif
    delete I                                             ;
    delete S                                             ;
    return false                                         ;
  }                                                      ;
  ////////////////////////////////////////////////////////
  SUID uuid = Find ( Connection , Data )                 ;
  if ( 0 == uuid )                                       {
    uuid = Append ( Connection , filename , Data )       ;
    if ( uuid > 0 )                                      {
      AppendThumb ( Connection,uuid,Data,D,*I,*S )       ;
      Connection.insertUuid                              (
        PictureORDER                                     ,
        uuid                                             ,
        "uuid"                                         ) ;
      assureVerify    ( Connection , uuid , Data       ) ;
      assurePalette   ( Connection , uuid , *I         ) ;
      assureHistogram ( Connection , uuid , *I         ) ;
      success = true                                     ;
    } else                                               {
      #if defined(QT_DEBUG)
      plan -> Debug ( 30 , QString ( "%1 can not be appended.").arg(filename) ) ;
      #endif
    }                                                    ;
  }                                                      ;
  PictureUuid = uuid                                     ;
  delete I                                               ;
  delete S                                               ;
  I = NULL                                               ;
  S = NULL                                               ;
  D . clear()                                            ;
  return success                                         ;
}

bool N::PictureManager::Import    (
       QString      filename    ,
       QByteArray & Data        ,
       SUID       & PictureUuid )
{
  PictureUuid = 0                                       ;
  bool       success = false                            ;
  QImage   * I                                          ;
  QImage   * S                                          ;
  QSize      V(128,128)                                 ;
  QByteArray D                                          ;
  ///////////////////////////////////////////////////////
  I = new QImage()                                      ;
  if (!I->load  (filename)) { delete I; return false; } ;
  if ( I->isNull(        )) { delete I; return false; } ;
  ///////////////////////////////////////////////////////
   S = new QImage()                                     ;
  *S = Icon(*I,V)                                       ;
  if (S->isNull())                                      {
    delete I; delete S; return false                    ;
  }                                                     ;
  if ( ! IntoPNG(*S,D) || D.size() <= 0 )               {
    delete I; delete S; return false                    ;
  }                                                     ;
  ///////////////////////////////////////////////////////
  SqlConnection Connection(PicturePlan->sql)            ;
  if (Connection . open ( FunctionString ) )            {
    SUID uuid = 0                                       ;
    uuid = Find(Connection,Data)                        ;
    if (uuid==0)                                        {
      uuid = Append(Connection,filename,Data)           ;
      if (uuid>0)                                       {
        AppendThumb(Connection,uuid,Data,D,*I,*S)       ;
        Connection.insertUuid                           (
          PictureORDER                                  ,
          uuid                                          ,
          "uuid"                                      ) ;
        assureVerify    ( Connection , uuid , Data    ) ;
        assurePalette   ( Connection , uuid , *I      ) ;
        assureHistogram ( Connection , uuid , *I      ) ;
        success = true                                  ;
      }                                                 ;
    }                                                   ;
    PictureUuid = uuid                                  ;
    Connection.close()                                  ;
  }                                                     ;
  Connection.remove()                                   ;
  delete I                                              ;
  delete S                                              ;
  I = NULL                                              ;
  S = NULL                                              ;
  D . clear()                                           ;
  return success                                        ;
}

bool N::PictureManager::Import (
       QImage & image        ,
       SUID   & PictureUuid  )
{
  PictureUuid = 0                                     ;
  bool       success = false                          ;
  QImage   * S                                        ;
  QSize      V(128,128)                               ;
  QByteArray D                                        ;
  QByteArray Data                                     ;
  /////////////////////////////////////////////////////
  if ( ! IntoPNG(image,Data) ) return false           ;
  /////////////////////////////////////////////////////
   S = new QImage()                                   ;
  *S = Icon(image,V)                                  ;
  if (S->isNull()) { delete S; return false ;       } ;
  if ( ! IntoPNG(*S,D) || D.size() <= 0 )             {
    delete S; return false                            ;
  }                                                   ;
  /////////////////////////////////////////////////////
  SqlConnection Connection(PicturePlan->sql)          ;
  if ( Connection . open ( FunctionString ) )         {
    SUID uuid = 0                                     ;
    uuid = Find(Connection,Data)                      ;
    if (uuid==0)                                      {
      uuid = AppendPNG(Connection,Data)               ;
      if (uuid>0)                                     {
        AppendThumb(Connection,uuid,Data,D,image,*S)  ;
        Connection.insertUuid                         (
          PictureORDER                                ,
          uuid                                        ,
          "uuid"                                    ) ;
        assureVerify    ( Connection , uuid , Data  ) ;
        assurePalette   ( Connection , uuid , image ) ;
        assureHistogram ( Connection , uuid , image ) ;
        success = true                                ;
      }                                               ;
    }                                                 ;
    PictureUuid = uuid                                ;
    Connection.close()                                ;
  }                                                   ;
  Connection.remove()                                 ;
  delete S                                            ;
  S = NULL                                            ;
  D . clear()                                         ;
  return success                                      ;
}

bool N::PictureManager :: assureVerify (
       SqlConnection & SC              ,
       SUID            u               ,
       QByteArray    & P               )
{
  QString      Q                     ;
  unsigned int adler = ADLER32 ( P ) ;
  QByteArray   md5   = QCryptographicHash::hash ( P , QCryptographicHash::Md5  ) ;
  QByteArray   sha1  = QCryptographicHash::hash ( P , QCryptographicHash::Sha1 ) ;
  md5  = md5  . toHex ( )            ;
  sha1 = sha1 . toHex ( )            ;
  Q = SC . sql . ReplaceInto         (
        PlanTable(PictureSummary)       ,
        4                            ,
        "uuid"                       ,
        "adler32"                    ,
        "md5"                        ,
        "sha1"                     ) ;
  SC . Prepare ( Q                 ) ;
  SC . Bind    ( "uuid"    , u     ) ;
  SC . Bind    ( "adler32" , adler ) ;
  SC . Bind    ( "md5"     , md5   ) ;
  SC . Bind    ( "sha1"    , sha1  ) ;
  SC . Exec    (                   ) ;
  return true                        ;
}

bool N::PictureManager :: PictureSuffix (
       SqlConnection & Connection    ,
       SUID             uuid          ,
       QString        & suffix        )
{
  QString  Q                           ;
  suffix = ""                          ;
  Q = Connection.sql.SelectFrom        (
        "suffix"                       ,
        PictureINDEX                   ,
        Connection.WhereUuid(uuid)   ) ;
  if (Connection.Fetch(Q))             {
    suffix = Connection . String ( 0 ) ;
  }                                    ;
  return ( suffix . length ( ) > 0 )   ;
}

QString N::PictureManager :: Export     (
          SqlConnection & Connection ,
          QDir           & dir        ,
          SUID             uuid       )
{
  QString    filename = ""                          ;
  QString    suffix                                 ;
  QByteArray body                                   ;
  if ( ! PictureSuffix ( Connection,uuid,suffix ) ) {
    return filename                                 ;
  }                                                 ;
  if (!PictureBlob(Connection,uuid,body))           {
    return filename                                 ;
  }                                                 ;
  filename = QString("%1.%2").arg(uuid).arg(suffix) ;
  filename = dir.absoluteFilePath(filename)         ;
  if (!File::toFile(filename,body))                 {
    filename = ""                                   ;
  }                                                 ;
  body . clear ( )                                  ;
  return filename                                   ;
}

bool N::PictureManager :: Export     (
       SqlConnection    & Connection ,
       QString            filename   ,
       SUID               uuid       )
{
  QByteArray body                         ;
  if (!PictureBlob(Connection,uuid,body)) {
    return false                          ;
  }                                       ;
  if (!File::toFile(filename,body))       {
    body . clear ( )                      ;
    return false                          ;
  }                                       ;
  body . clear ( )                        ;
  return true                             ;
}

QSize N::PictureManager::Geometry  (
        SqlConnection & Connection ,
        SUID            uuid       )
{
  QString Q                                  ;
  QSize S(0,0)                               ;
  Q = Connection.sql.SelectFrom              (
        "width,height"                       ,
        ThumbINDEX                           ,
        Connection.WhereUuid(uuid)         ) ;
  if (Connection.Fetch(Q))                   {
    S . setWidth  ( Connection . Int ( 0 ) ) ;
    S . setHeight ( Connection . Int ( 1 ) ) ;
  }                                          ;
  return S                                   ;
}

QImage * N::PictureManager :: SafeImage (
           QByteArray & Body          ,
           QByteArray & Format        )
{
  QImage * I       = new QImage()                      ;
  bool     correct = false                             ;
  if (Format.size()>0)                                 {
    correct = I->loadFromData(Body,Format.constData()) ;
  } else                                               {
    correct = I->loadFromData(Body)                    ;
  }                                                    ;
  //////////////////////////////////////////////////////
  Format . clear     (           )                     ;
  Body   . clear     (           )                     ;
  if (correct && !I->isNull())                         {
    return I                                           ;
  } else                                               {
    delete I                                           ;
  }                                                    ;
  return NULL                                          ;
}

QImage * N::PictureManager :: Picture    (
           SqlConnection & Connection ,
           SUID             uuid       )
{
  QString    Q                         ;
  QByteArray S                         ;
  QByteArray P                         ;
  Q = Connection.sql.SelectFrom        (
        "suffix"                       ,
        PictureINDEX                   ,
        Connection.WhereUuid(uuid)   ) ;
  if (Connection.Fetch(Q))             {
    S = Connection . ByteArray ( 0 )   ;
  }                                    ;
  if (!PictureBlob(Connection,uuid,P)) {
    return NULL                        ;
  }                                    ;
  return SafeImage ( P , S )           ;
}

QImage * N::PictureManager::Picture(SUID uuid)
{
  QImage * image = NULL                     ;
  QString  CN    = QtUUID::createUuidString() ;
  SqlConnection SC(PicturePlan->sql)        ;
  if (SC.open("nPictureManager",CN))        {
    image = Picture(SC,uuid)                ;
    SC.close()                              ;
  }                                         ;
  SC.remove()                               ;
  return image                              ;
}

QImage * N::PictureManager::Thumb        (
           SqlConnection & Connection ,
           SUID             uuid       )
{
  QString    Q                                 ;
  QByteArray S                                 ;
  QByteArray P                                 ;
  //////////////////////////////////////////////
  Q = Connection.sql.SelectFrom                (
        "format"                               ,
        ThumbINDEX                             ,
        Connection.WhereUuid(uuid)           ) ;
  if (Connection.Fetch(Q))                     {
    S = Connection . ByteArray ( 0 )           ;
  }                                            ;
  //////////////////////////////////////////////
  Q = Connection.sql.SelectFrom                (
        "thumb"                                ,
        ThumbDEPOT                             ,
        Connection.WhereUuid(uuid)           ) ;
  if (Connection.Fetch(Q))                     {
    P = Connection . ByteArray ( 0 )           ;
  }                                            ;
  if (P.size()<=0) return NULL                 ;
  return SafeImage ( P , S )                   ;
}

QImage * N::PictureManager::Thumb(SUID uuid)
{
  QImage * image = NULL                     ;
  QString  CN    = QtUUID::createUuidString() ;
  SqlConnection SC(PicturePlan->sql)        ;
  if (SC.open("nPictureManager",CN))        {
    image = Thumb(SC,uuid)                  ;
    SC.close()                              ;
  }                                         ;
  SC.remove()                               ;
  return image                              ;
}

bool N::PictureManager::DeleteThumb(SqlConnection & SC,SUID uuid)
{
  QString Q                                                     ;
  Q = SC . sql . DeleteFrom ( ThumbINDEX , SC.WhereUuid(uuid) ) ;
  if ( ! SC . Query ( Q ) ) return false                        ;
  Q = SC . sql . DeleteFrom ( ThumbDEPOT , SC.WhereUuid(uuid) ) ;
  return SC . Query ( Q )                                       ;
}

bool N::PictureManager::UpdateThumb(SqlConnection & SC,SUID uuid)
{
  QString    Q                                    ;
  QSize      V ( 128 , 128 )                      ;
  QString    Suffix                               ;
  QByteArray S                                    ;
  QByteArray P                                    ;
  /////////////////////////////////////////////////
  DeleteThumb   ( SC , uuid          )            ;
  PictureSuffix ( SC , uuid , Suffix )            ;
  if ( ! PictureBlob ( SC,uuid,P ) ) return false ;
  S = Suffix . toUtf8 ( )                         ;
  /////////////////////////////////////////////////
  QImage * I       = new QImage()                 ;
  bool     correct = false                        ;
  if (S.size()>0)                                 {
    correct = I->loadFromData(P,S.constData())    ;
  } else                                          {
    correct = I->loadFromData(P)                  ;
  }                                               ;
  /////////////////////////////////////////////////
  if (correct)                                    {
    QImage   * T                                  ;
    QByteArray D                                  ;
     T = new QImage()                             ;
    *T = Icon(*I,V)                               ;
    if (T->isNull())                              {
      delete I                                    ;
      delete T                                    ;
      return false                                ;
    }                                             ;
    D = IntoPNG(*T)                               ;
    if (D.size()<=0)                              {
      delete I                                    ;
      delete T                                    ;
      return false                                ;
    }                                             ;
    AppendThumb ( SC , uuid , P , D , *I , *T )   ;
    S    . clear     (           )                ;
    P    . clear     (           )                ;
    delete I                                      ;
  } else                                          {
    S    . clear     (           )                ;
    P    . clear     (           )                ;
    delete I                                      ;
    return false                                  ;
  }                                               ;
  return true                                     ;
}

bool N::PictureManager::Release(IplImage * image)
{
  nKickOut (IsNull(image),false) ;
  ::cvReleaseImage(&image)       ;
  return true                    ;
}

IplImage * N::PictureManager::toIplImage (QImage & image)
{
  CvSize s                                                            ;
  s.width   = image . width  ( )                                      ;
  s.height  = image . height ( )                                      ;
  int depth = 1                                                       ;
  /////////////////////////////////////////////////////////////////////
  switch (image.format())                                             {
    case QImage::Format_Mono                   : depth = 1 ; break    ;
    case QImage::Format_MonoLSB                : depth = 1 ; break    ;
    case QImage::Format_Indexed8               : depth = 1 ; break    ;
    case QImage::Format_RGB32                  : depth = 4 ; break    ;
    case QImage::Format_ARGB32                 : depth = 4 ; break    ;
    case QImage::Format_ARGB32_Premultiplied   : depth = 4 ; break    ;
    case QImage::Format_RGB16                  : depth = 3 ; break    ;
    case QImage::Format_ARGB8565_Premultiplied : depth = 3 ; break    ;
    case QImage::Format_RGB666                 : depth = 3 ; break    ;
    case QImage::Format_ARGB6666_Premultiplied : depth = 3 ; break    ;
    case QImage::Format_RGB555                 : depth = 3 ; break    ;
    case QImage::Format_ARGB8555_Premultiplied : depth = 3 ; break    ;
    case QImage::Format_RGB888                 : depth = 3 ; break    ;
    case QImage::Format_RGB444                 : depth = 3 ; break    ;
    case QImage::Format_ARGB4444_Premultiplied : depth = 3 ; break    ;
    default                                    : return NULL          ;
  }                                                                   ;
  /////////////////////////////////////////////////////////////////////
  IplImage * img = cvCreateImage ( s , IPL_DEPTH_8U , depth )         ;
  switch (image.format())                                             {
    case QImage::Format_RGB32 :                                       {
      /////////////////////////////////////////////////////////////////
      int df = img  -> widthStep                                      ;
      int y  = image . height       ( )                               ;
      int w  = image . width        ( ) * 4                           ;
      /////////////////////////////////////////////////////////////////
      unsigned char * s                                               ;
      unsigned char * f                                               ;
      for (int i=0;i<y;i++)                                           {
        s  = image.scanLine(i)                                        ;
        f  = (unsigned char *)img->imageData                          ;
        f += (i*df)                                                   ;
        memcpy ( f , s , w )                                          ;
      }                                                               ;
    }                                                                 ;
    break                                                             ;
    case QImage::Format_ARGB32_Premultiplied                          :
    case QImage::Format_ARGB32               :                        {
      /////////////////////////////////////////////////////////////////
      int df = img  -> widthStep                                      ;
      int y  = image . height ( )                                     ;
      int w  = image . width  ( ) * 4                                 ;
      /////////////////////////////////////////////////////////////////
      unsigned char * s                                               ;
      unsigned char * f                                               ;
      for (int i=0;i<y;i++)                                           {
        s  = image.scanLine(i)                                        ;
        f  = (unsigned char *)img->imageData                          ;
        f += ( i * df )                                               ;
        memcpy ( f , s , w )                                          ;
      }                                                               ;
    }                                                                 ;
    break                                                             ;
    case QImage::Format_Mono                                          :
    case QImage::Format_MonoLSB                                       :
    case QImage::Format_Indexed8                                      :
    case QImage::Format_RGB16                                         :
    case QImage::Format_ARGB8565_Premultiplied                        :
    case QImage::Format_RGB666                                        :
    case QImage::Format_ARGB6666_Premultiplied                        :
    case QImage::Format_RGB555                                        :
    case QImage::Format_ARGB8555_Premultiplied                        :
    case QImage::Format_RGB888                                        :
    case QImage::Format_RGB444                                        :
    case QImage::Format_ARGB4444_Premultiplied                        :
      for (int x=0;x<img->width;x++)                                  {
        for (int y=0;y<img->height;y++)                               {
          QRgb rgb = image.pixel(x,y)                                 ;
          ::cvSet2D(img,y,x,CV_RGB(qRed(rgb),qGreen(rgb),qBlue(rgb))) ;
        }                                                             ;
      }                                                               ;
    break                                                             ;
  }                                                                   ;
  return img                                                          ;
}

IplImage * N::PictureManager::toIplImage(QImage & image,double angle)
{
  QImage     I                                      ;
  QTransform T                                      ;
  T . reset  (       )                              ;
  T . rotate ( angle )                              ;
  I = image.transformed(T,Qt::SmoothTransformation) ;
  return toIplImage ( I )                           ;
}

QImage * N::PictureManager::toImage (IplImage * image)
{
  QImage::Format format = QImage::Format_Invalid                ;
  QSize  W(image->width,image->height)                          ;
  switch (image->depth)                                         {
    case IPL_DEPTH_8U                                           :
      switch (image->nChannels)                                 {
        case 3:                                                 {
          format = QImage::Format_RGB888                        ;
          QImage * I = new QImage(W,format)                     ;
          unsigned char * s = I->bits()                         ;
          unsigned char * f = (unsigned char *)image->imageData ;
          int sl = image->widthStep                             ;
          int x,y                                               ;
          memset(s,0xFF,I->byteCount())                         ;
          x = W.width ()                                        ;
          y = W.height()                                        ;
          for (int i=0;i<y;i++)                                 {
            s  = I->scanLine(i)                                 ;
            f  = (unsigned char *)image->imageData              ;
            f += (sl*i)                                         ;
            for (int j=0;j<x;j++)                               {
              s [0] = f[2]                                      ;
              s [1] = f[1]                                      ;
              s [2] = f[0]                                      ;
              s    += 3                                         ;
              f    += 3                                         ;
            }                                                   ;
          }                                                     ;
          return I                                              ;
        }                                                       ;
        break                                                   ;
        case 1 :                                                {
          format = QImage::Format_ARGB32                        ;
          QImage * I = new QImage(W,format)                     ;
          for (int y=0;y<image->height;y++)                     {
            for (int x=0;x<image->width;x++)                    {
              CvScalar color = ::cvGet2D(image,y,x)             ;
              int r = color.val[0]                              ;
              I->setPixel(x,y,qRgb(r,r,r))                      ;
            }                                                   ;
          }                                                     ;
          return I                                              ;
        }                                                       ;
        break                                                   ;
        case 2                                                  :
        case 4 :                                                {
          format = QImage::Format_ARGB32                        ;
          QImage * I = new QImage(W,format)                     ;
          for (int y=0;y<image->height;y++)                     {
            for (int x=0;x<image->width;x++)                    {
              CvScalar color = ::cvGet2D(image,y,x)             ;
              int r = color.val[2]                              ;
              int g = color.val[1]                              ;
              int b = color.val[0]                              ;
              I->setPixel(x, y, qRgb(r,g,b))                    ;
            }                                                   ;
          }                                                     ;
          return I                                              ;
        }                                                       ;
        break                                                   ;
      }                                                         ;
    break                                                       ;
    case IPL_DEPTH_8S                                           :
    case IPL_DEPTH_16U                                          :
    case IPL_DEPTH_16S                                          :
    case IPL_DEPTH_32S                                          :
    case IPL_DEPTH_32F                                          :
    case IPL_DEPTH_64F                                        : {
      format = QImage::Format_ARGB32                            ;
      QImage * I = new QImage(W,format)                         ;
      for (int y=0;y<image->height;y++)                         {
        for (int x=0;x<image->width;x++)                        {
          CvScalar color = ::cvGet2D(image,y,x)                 ;
          int r = color.val[2]                                  ;
          int g = color.val[1]                                  ;
          int b = color.val[0]                                  ;
          I->setPixel(x, y, qRgb(r,g,b))                        ;
        }                                                       ;
      }                                                         ;
      return I                                                  ;
    }                                                           ;
    break                                                       ;
  }                                                             ;
  return NULL                                                   ;
}

IplImage * N::PictureManager::toGrey(IplImage * image)
{
  IplImage * grey = ::cvCreateImage(cvGetSize(image),image->depth,1) ;
  ::cvCvtColor ( image , grey , CV_BGR2GRAY )                        ;
  return grey                                                        ;
}

IplImage * N::PictureManager::Smooth(IplImage * image,int Mode,int p1,int p2)
{
  IplImage * smooth = ::cvCreateImage(cvGetSize(image),image->depth,image->nChannels) ;
  ::cvSmooth ( image , smooth , Mode , p1 , p2 )                                      ;
  return smooth                                                                       ;
}

IplImage * N::PictureManager::Median(IplImage * image,int p1,int p2)
{
  return Smooth(image,CV_MEDIAN,p1,p2) ;
}

IplImage * N::PictureManager::Gaussian(IplImage * image,int p1,int p2)
{
  return Smooth(image,CV_GAUSSIAN,p1,p2) ;
}

IplImage * N::PictureManager::Blur(IplImage * image,int p1,int p2)
{
  return Smooth(image,CV_BLUR,p1,p2) ;
}

IplImage * N::PictureManager::Laplace(IplImage * image,int Aperture)
{
  IplImage * laplace = ::cvCreateImage(cvGetSize(image),IPL_DEPTH_32F,1) ;
  ::cvLaplace ( image , laplace , Aperture )                             ;
  return laplace                                                         ;
}

IplImage * N::PictureManager::Threshold(IplImage * image,double threshold,double maxv,int threstype)
{
  IplImage * thres = ::cvCreateImage(cvGetSize(image),image->depth,1) ;
  ::cvThreshold ( image , thres , threshold , maxv , threstype      ) ;
  return thres                                                        ;
}

IplImage * N::PictureManager::Erode(IplImage * image,int Iterations)
{
  IplImage * smooth = ::cvCreateImage(cvGetSize(image),image->depth,image->nChannels) ;
  ::cvErode  ( image , smooth , NULL , Iterations )                                   ;
  return smooth                                                                       ;
}

IplImage * N::PictureManager::Dilate(IplImage * image,int Iterations)
{
  IplImage * smooth = ::cvCreateImage(cvGetSize(image),image->depth,image->nChannels) ;
  ::cvDilate ( image , smooth , NULL , Iterations )                                   ;
  return smooth                                                                       ;
}

// Contrast -255 ~ 255
void N::PictureManager::AdjustContrast(const IplImage * src,IplImage * dst,int contrast)
{
  if (IsNull(src)) return                    ;
  int imagewidth  = src->width               ;
  int imageheight = src->height              ;
  int channel     = src->nChannels           ;
  int wh          = imagewidth * imageheight ;
  ////////////////////////////////////////////
  CvScalar mean = {0,0,0,0}                  ;
  for (int y=0; y<imageheight; y++)          {
    for (int x=0; x<imagewidth; x++)         {
      for (int k=0; k<channel; k++)          {
        CvScalar ori = ::cvGet2D(src, y, x)  ;
        for (int k=0; k<channel; k++)        {
          mean.val[k] += ori.val[k]          ;
        }                                    ;
      }                                      ;
    }                                        ;
  }                                          ;
  ////////////////////////////////////////////
  for (int k=0; k<channel; k++)              {
    mean.val[k] /= wh                        ;
  }                                          ;
  ////////////////////////////////////////////
  if (contrast <= -255)                      {
    for (int y=0; y<imageheight; y++)        {
      for (int x=0; x<imagewidth; x++)       {
        ::cvSet2D(dst, y, x, mean)           ;
      }                                      ;
    }                                        ;
  } else
  if (contrast > -255 && contrast <= 0)      {
    for (int y=0; y<imageheight; y++)        {
      for (int x=0; x<imagewidth; x++)       {
        CvScalar nRGB                        ;
        CvScalar ori = ::cvGet2D(src, y, x)  ;
        for (int k=0; k<channel; k++)        {
          nRGB.val[k] = ori.val[k] + (ori.val[k] - mean.val[k]) * contrast / 255;
        }                                    ;
        ::cvSet2D(dst, y, x, nRGB)           ;
      }                                      ;
    }                                        ;
  } else
  if (contrast >0 && contrast <255)          {
    CvScalar nRGB                            ;
    int nContrast = 255 *255 / (255 - contrast) - 255 ;
    for (int y=0;y<imageheight;y++)          {
      for (int x=0; x<imagewidth; x++)       {
        CvScalar ori = ::cvGet2D(src, y, x)  ;
        for (int k=0; k<channel; k++)        {
          nRGB.val[k] = ori.val[k] + (ori.val[k] - mean.val[k]) *nContrast /255;
        }                                    ;
        ::cvSet2D(dst, y, x, nRGB)           ;
      }                                      ;
    }                                        ;
  } else                                     {
    for (int y=0; y<imageheight; y++)        {
      for (int x=0; x<imagewidth; x++)       {
        CvScalar rgb                         ;
        CvScalar ori = ::cvGet2D(src, y, x)  ;
        for (int k=0; k<channel; k++)        {
          if (ori.val[k] > mean.val[k])      {
            rgb.val[k] = 255                 ;
          } else                             {
            rgb.val[k] = 0                   ;
          }                                  ;
        }                                    ;
        ::cvSet2D(dst, y, x, rgb)            ;
      }                                      ;
    }                                        ;
  }                                          ;
}

void N::PictureManager::UnsharpMask   (
       const IplImage * src       ,
             IplImage * dst       ,
             float      amount    ,
             float      radius    ,
             int        threshold ,
             int        contrast  )
{
  if (IsNull(src)) return                                              ;
  //////////////////////////////////////////////////////////////////////
  int imagewidth  = src->width                                         ;
  int imageheight = src->height                                        ;
  int channel     = src->nChannels                                     ;
  //////////////////////////////////////////////////////////////////////
  IplImage * blurimage    = ::cvCreateImage(cvSize(imagewidth,imageheight),src->depth,channel) ;
  IplImage * DiffImage    = ::cvCreateImage(cvSize(imagewidth,imageheight),         8,channel) ;
  IplImage * highcontrast = ::cvCreateImage(cvSize(imagewidth,imageheight),         8,channel) ;
  //////////////////////////////////////////////////////////////////////
  AdjustContrast ( src , highcontrast , contrast             )         ;
  ::cvSmooth     ( src , blurimage    , CV_GAUSSIAN , radius )         ;
  //////////////////////////////////////////////////////////////////////
  for (int y=0; y<imageheight; y++)                                    {
    for (int x=0; x<imagewidth; x++)                                   {
      CvScalar ori  = ::cvGet2D(      src, y, x)                       ;
      CvScalar blur = ::cvGet2D(blurimage, y, x)                       ;
      CvScalar val                                                     ;
      val.val[0] = abs(ori.val[0] - blur.val[0])                       ;
      val.val[1] = abs(ori.val[1] - blur.val[1])                       ;
      val.val[2] = abs(ori.val[2] - blur.val[2])                       ;
      ::cvSet2D(DiffImage, y, x, val)                                  ;
    }                                                                  ;
  }                                                                    ;
  //////////////////////////////////////////////////////////////////////
  for (int y=0; y<imageheight; y++)                                    {
    for (int x=0; x<imagewidth; x++)                                   {
      CvScalar hc   = ::cvGet2D(highcontrast, y, x)                    ;
      CvScalar diff = ::cvGet2D(   DiffImage, y, x)                    ;
      CvScalar ori  = ::cvGet2D(         src, y, x)                    ;
      CvScalar val                                                     ;
      for (int k=0; k<channel; k++)                                    {
        if (diff.val[k] > threshold)                                   {
          val.val[k]  = ori.val[k] * (100-amount) + hc.val[k] * amount ;
          val.val[k] /= 100                                            ;
        } else                                                         {
          val.val[k] = ori.val[k]                                      ;
        }                                                              ;
      }                                                                ;
      ::cvSet2D ( dst , y , x , val )                                  ;
    }                                                                  ;
  }                                                                    ;
  //////////////////////////////////////////////////////////////////////
  ::cvReleaseImage ( &blurimage )                                      ;
  ::cvReleaseImage ( &DiffImage )                                      ;
}

QImage N::PictureManager::UnsharpMask(QImage & src,float amount,float radius,int threshold,int contrast)
{
  IplImage * simg = toIplImage(src)                       ;
  IplImage * timg = toIplImage(src)                       ;
  UnsharpMask(simg,timg,amount,radius,threshold,contrast) ;
  QImage * I = toImage(timg)                              ;
  Release ( simg )                                        ;
  Release ( timg )                                        ;
  if (IsNull(I)) return src                               ;
  QImage X = *I                                           ;
  delete I                                                ;
  return X                                                ;
}

QImage N::PictureManager::AdjustContrast (QImage & src,int contrast)
{
  IplImage * simg = toIplImage(src)  ;
  IplImage * timg = toIplImage(src)  ;
  AdjustContrast(simg,timg,contrast) ;
  QImage * I = toImage(timg)         ;
  Release ( simg )                   ;
  Release ( timg )                   ;
  if (IsNull(I)) return src          ;
  QImage X = *I                      ;
  delete I                           ;
  return X                           ;
}

SUID N::PictureManager::InsertPoint(SqlConnection & SC,QPoint P)
{
  QString Q                      ;
  SUID    puid = 0               ;
  puid = SC.Unique               (
           PlanTable(MajorUuid)     ,
           "uuid",3912         ) ;
  SC.assureUuid                  (
    PlanTable(MajorUuid)            ,
    puid                         ,
    Types::Point               ) ;
  double x = P . x ( )           ;
  double y = P . y ( )           ;
  Q = SC.sql.InsertInto          (
        PlanTable(Points)           ,
        3                        ,
        "uuid"                   ,
        "x"                      ,
        "y"                    ) ;
  SC . Prepare ( Q             ) ;
  SC . Bind    ( "uuid" , puid ) ;
  SC . Bind    ( "x"    , x    ) ;
  SC . Bind    ( "y"    , y    ) ;
  if (!SC . Exec ( ) ) puid = 0  ;
  return puid                    ;
}

SUID N::PictureManager::InsertRect(SqlConnection & SC,QRect R)
{
  QString Q                      ;
  SUID    fuid = 0               ;
  fuid = SC.Unique               (
           PlanTable(MajorUuid)     ,
           "uuid",3911         ) ;
  SC.assureUuid                  (
    PlanTable(MajorUuid)            ,
    fuid                         ,
    Types::Rectangle           ) ;
  double x = R . left   ( )      ;
  double y = R . top    ( )      ;
  double w = R . width  ( )      ;
  double h = R . height ( )      ;
  Q = SC.sql.InsertInto          (
        PlanTable(Rectangles)       ,
        5                        ,
        "uuid","x","y","w","h" ) ;
  SC . Prepare ( Q             ) ;
  SC . Bind    ( "uuid" , fuid ) ;
  SC . Bind    ( "x"    , x    ) ;
  SC . Bind    ( "y"    , y    ) ;
  SC . Bind    ( "w"    , w    ) ;
  SC . Bind    ( "h"    , h    ) ;
  if (!SC . Exec ( ) ) fuid = 0  ;
  return fuid                    ;
}

bool N::PictureManager::Rectangle(SqlConnection & SC,SUID uuid,QRectF & Rect)
{
  QString Q                           ;
  bool    success = false             ;
  nKickOut ( uuid <= 0 , false )      ;
  Q = SC.sql.SelectFrom               (
        "x,y,w,h"                     ,
        PlanTable(Rectangles)            ,
        SC.WhereUuid(uuid)          ) ;
  if (SC.Fetch(Q))                    {
    double x = SC.Value(0).toDouble() ;
    double y = SC.Value(1).toDouble() ;
    double w = SC.Value(2).toDouble() ;
    double h = SC.Value(3).toDouble() ;
    Rect     . setX      ( x )        ;
    Rect     . setY      ( y )        ;
    Rect     . setWidth  ( w )        ;
    Rect     . setHeight ( h )        ;
    success  = true                   ;
  }                                   ;
  return success                      ;
}

bool N::PictureManager::InsertGroup (
       SqlConnection & SC        ,
       SUID             first     ,
       SUID             second    ,
       int              t1        ,
       int              t2        ,
       int              relation  )
{
  QString Q                              ;
  Q = SC.sql.InsertInto                  (
        PlanTable(Groups)                   ,
        5                                ,
        "first"                          ,
        "second"                         ,
        "t1"                             ,
        "t2"                             ,
        "relation"                     ) ;
  SC . Prepare ( Q                     ) ;
  SC . Bind    ( "first"    , first    ) ;
  SC . Bind    ( "second"   , second   ) ;
  SC . Bind    ( "t1"       , t1       ) ;
  SC . Bind    ( "t2"       , t2       ) ;
  SC . Bind    ( "relation" , relation ) ;
  return SC . Exec (                   ) ;
}

bool N::PictureManager::InsertGroup  (
       SqlConnection & SC         ,
       SUID             first      ,
       SUID             second     ,
       int              t1         ,
       int              t2         ,
       int              relation   ,
       double           membership )
{
  QString Q                                  ;
  Q = SC.sql.InsertInto                      (
        PlanTable(Groups)                       ,
        6                                    ,
        "first"                              ,
        "second"                             ,
        "t1"                                 ,
        "t2"                                 ,
        "relation"                           ,
        "membership"                       ) ;
  SC . Prepare ( Q                         ) ;
  SC . Bind    ( "first"      , first      ) ;
  SC . Bind    ( "second"     , second     ) ;
  SC . Bind    ( "t1"         , t1         ) ;
  SC . Bind    ( "t2"         , t2         ) ;
  SC . Bind    ( "relation"   , relation   ) ;
  SC . Bind    ( "membership" , membership ) ;
  return SC . Exec (                       ) ;
}

bool N::PictureManager::InsertGroup  (
       SqlConnection & Connection ,
       SUID             first      ,
       SUID             second     ,
       int              t1         ,
       int              t2         ,
       int              relation   ,
       int              position   )
{
  QString Q                                    ;
  Q = Connection.sql.InsertInto                (
        "first,second,t1,t2,relation,position" ,
        PlanTable(Groups)                         ,
        QString("%1,%2,%3,%4,%5,%6"            )
        .arg( first                            )
        .arg( second                           )
        .arg( t1                               )
        .arg( t2                               )
        .arg( relation                         )
        .arg( position                     ) ) ;
  return Connection . Query ( Q )              ;
}

UUIDs N::PictureManager::FindOwners (
        SqlConnection & SC       ,
        SUID             uuid     ,
        int              t1       ,
        int              t2       ,
        int              relation )
{
  UUIDs   Uuids                                   ;
  QString Q                                       ;
  QString O                                       ;
  O = QString("where second = %1 "
              "and t1 = %2 "
              "and t2 = %3 "
              "and relation = %4 "
              "order by position asc"             )
          .arg(uuid                               )
          .arg(t1                                 )
          .arg(t2                                 )
          .arg(relation                         ) ;
  Q = SC.sql.SelectFrom("first",PlanTable(Groups),O) ;
  SqlLoopNow(SC,Q)                                ;
    Uuids << SC.Uuid(0)                           ;
  SqlLoopErr(SC,Q)                                ;
  SqlLoopEnd(SC,Q)                                ;
  return Uuids                                    ;
}

SUID N::PictureManager::InsertFace    (
       SqlConnection & SC          ,
       SUID             picture     ,
       SUID             rectangle   ,
       double           possibility )
{
  QString Q                                     ;
  SUID    fuid = 0                              ;
  fuid = SC.Unique                              (
           PlanTable(MajorUuid)                    ,
           "uuid",3920                        ) ;
  SC.assureUuid                                 (
    PlanTable(MajorUuid)                           ,
    fuid                                        ,
    Types::Face                               ) ;
  Q = SC.sql.InsertInto                         (
        PlanTable(Faces)                           ,
        4                                       ,
        "uuid"                                  ,
        "picture"                               ,
        "rectangle"                             ,
        "possibility"                         ) ;
  SC . Prepare ( Q                            ) ;
  SC . Bind    ( "uuid"         , fuid        ) ;
  SC . Bind    ( "picture"      , picture     ) ;
  SC . Bind    ( "rectangle"    , rectangle   ) ;
  SC . Bind    ( "possibility"  , possibility ) ;
  if (!SC . Exec ( ) ) fuid = 0                 ;
  return fuid                                   ;
}

bool N::PictureManager :: InsertFaceParament (
       SqlConnection    & SC                 ,
       SUID               face               ,
       QString            name               ,
       SUID               rectangle          ,
       SUID               point              )
{
  QString Q                                    ;
  Q = SC.sql.InsertInto                        (
        PlanTable(FaceParaments)                  ,
        4                                      ,
        "uuid"                                 ,
        "name"                                 ,
        "rectangle"                            ,
        "point"                              ) ;
  SC . Prepare ( Q                           ) ;
  SC . Bind    ( "uuid"      , face          ) ;
  SC . Bind    ( "name"      , name.toUtf8() ) ;
  SC . Bind    ( "rectangle" , rectangle     ) ;
  SC . Bind    ( "point"     , point         ) ;
  return SC . Exec (                         ) ;
}

UUIDs N::PictureManager::FullFaces(SqlConnection & SC)
{
  UUIDs  Uuids                    ;
  Uuids = SC.Uuids                (
            PlanTable(Faces)         ,
            "uuid"                ,
            SC.OrderByAsc("id") ) ;
  return Uuids                    ;
}

UUIDs N::PictureManager::BadFaces(SqlConnection & SC)
{
  UUIDs  Uuids                    ;
  Uuids = SC.Uuids                (
            PlanTable(BadFaces)      ,
            "uuid"                ,
            SC.OrderByAsc("id") ) ;
  return Uuids                    ;
}

UUIDs N::PictureManager::FaceLimits(SqlConnection & SC,int StartId,int Limit)
{
  UUIDs Uuids                                   ;
  Uuids = SC.Uuids                              (
            PlanTable(Faces)                       ,
            "uuid"                              ,
            QString("%1 %2"                     )
            .arg(SC.OrderByAsc("id")            )
            .arg(SC.sql.Limit(StartId,Limit)) ) ;
  return Uuids                                  ;
}

int N::PictureManager::CountFaceDepot(SqlConnection & SC)
{
  int     total = 0        ;
  QString Q                ;
  Q = SC.sql.SelectFrom    (
          "count(*)"       ,
          PlanTable(Faces)  ) ;
  if (SC.Fetch(Q))         {
    total = SC . Int ( 0 ) ;
  }                        ;
  return total             ;
}

void N::PictureManager::Paint(QImage & image,Palette & palette,QSize D)
{
  QSize I = image.size()                                      ;
  QSize S ( I.width() / D.width() , I.height() / D.height() ) ;
  QRect R ( 0 , 0 , I.width() , I.height()                  ) ;
  int MaxC  = S.width() * S.height()                          ;
  int total = palette.total()                                 ;
  if (total>MaxC) total = MaxC                                ;
  /////////////////////////////////////////////////////////////
  QVector<QRgb> Colors = palette.colors()                     ;
  QPainter      p                                             ;
  p . begin    ( &image              )                        ;
  p . setPen   ( QColor(255,255,255) )                        ;
  p . setBrush ( QColor(255,255,255) )                        ;
  p . drawRect ( R                   )                        ;
  /////////////////////////////////////////////////////////////
  for (int ii=0;ii<total;ii++)                                {
    int   dx = ( ii % S . width ( ) ) * D.width  ( )          ;
    int   dy = ( ii / S . height( ) ) * D.height ( )          ;
    QRect V    ( dx , dy , D . width ( ) , D . height ( )   ) ;
    p . setPen   ( QColor(Colors[ii]) )                       ;
    p . setBrush ( QColor(Colors[ii]) )                       ;
    p . drawRect ( V                  )                       ;
  }                                                           ;
  /////////////////////////////////////////////////////////////
  p . end      (                     )                        ;
}

void N::PictureManager::Paint(QIcon & icon,QSize size,Palette & palette,QSize block)
{
  QImage image ( size  , QImage::Format_ARGB32 ) ;
  Paint        ( image , palette , block       ) ;
  icon = QIcon ( QPixmap::fromImage(image)     ) ;
}

bool N::PictureManager :: GetPalette (
       SqlConnection    & SC         ,
       SUID               uuid       ,
       Palette          & palette    )
{
  QString Q                     ;
  Q = SC.sql.SelectFrom         (
        "colormap"              ,
        PlanTable(Palettes)        ,
        SC.WhereUuid(uuid)    ) ;
  if (SC.Fetch(Q))              {
    QByteArray C                ;
    C = SC.ByteArray(0)         ;
    palette . Uncompress ( C )  ;
  } else return false           ;
  return true                   ;
}

bool N::PictureManager :: AttachPalette (
       SqlConnection    & SC            ,
       SUID               picture       ,
       SUID               palette       )
{
  QString Q                         ;
  Q  = SC.sql.InsertInto            (
         PlanTable(PicturePalettes   ) ,
         2,"picture","colormap"   ) ;
  SC . Prepare (Q                 ) ;
  SC . Bind    ("picture" ,picture) ;
  SC . Bind    ("colormap",palette) ;
  return SC . Exec (              ) ;
}

SUID N::PictureManager::FindPalette(SqlConnection & SC,Palette & palette)
{
  QString    Q                            ;
  QByteArray C = palette . Compress ()    ;
  int        checksum                     ;
  checksum = qChecksum(C.data(),C.size()) ;
  Q = SC.sql.SelectFrom                   (
        "uuid,colormap"                   ,
        PlanTable(Palettes)                  ,
        QString("where compression = %1 "
                "and checksum = %2 "
                "and colors = %3"         )
        .arg(C.size()                     )
        .arg(checksum                     )
        .arg(palette.total()          ) ) ;
  SqlLoopNow(SC,Q)                        ;
    SUID       pcid = SC . Uuid     (0)   ;
    QByteArray D    = SC . ByteArray(1)   ;
    if (D==C) return pcid                 ;
  SqlLoopErr(SC,Q)                        ;
  SqlLoopEnd(SC,Q)                        ;
  return 0                                ;
}

SUID N::PictureManager::AppendPalette(SqlConnection & SC,Palette & palette)
{
  QString            Q                            ;
  int                checksum                     ;
  SUID               uuid  = 0                    ;
  QByteArray         C     = palette.Compress()   ;
  unsigned long long sigma = palette.sigma   ()   ;
  int                Color = palette.total   ()   ;
  int                R     = 0                    ;
  int                G     = 0                    ;
  int                B     = 0                    ;
  int                A     = 0                    ;
  checksum = qChecksum(C.data(),C.size())         ;
  A        = (sigma>>48) & 0xFFFF                 ;
  R        = (sigma>>32) & 0xFFFF                 ;
  G        = (sigma>>16) & 0xFFFF                 ;
  B        =  sigma      & 0xFFFF                 ;
  uuid = SC.Unique(PlanTable(MajorUuid),"uuid",7532) ;
  SC.assureUuid                                   (
    PlanTable(MajorUuid)                             ,
    uuid                                          ,
    Types::Palette                              ) ;
  Q = SC.sql.InsertInto                           (
        PlanTable(Palettes)                          ,
        10                                        ,
        "uuid"                                    ,
        "sigma"                                   ,
        "colors"                                  ,
        "R"                                       ,
        "G"                                       ,
        "B"                                       ,
        "A"                                       ,
        "compression"                             ,
        "checksum"                                ,
        "colormap"                              ) ;
  SC . Prepare ( Q                              ) ;
  SC . Bind    ( "uuid"        , uuid           ) ;
  SC . Bind    ( "sigma"       , sigma          ) ;
  SC . Bind    ( "colors"      , Color          ) ;
  SC . Bind    ( "R"           , R              ) ;
  SC . Bind    ( "G"           , G              ) ;
  SC . Bind    ( "B"           , B              ) ;
  SC . Bind    ( "A"           , A              ) ;
  SC . Bind    ( "compression" , C.size()       ) ;
  SC . Bind    ( "checksum"    , checksum       ) ;
  SC . Bind    ( "colormap"    , C              ) ;
  SC . Exec    (                                ) ;
  return uuid                                     ;
}

bool N::PictureManager :: assurePalette (
       SqlConnection    & SC            ,
       SUID               uuid          ,
       Palette          & palette       )
{
  nKickOut ( uuid <= 0 , false )                ;
  QString Q                                     ;
  bool    exists = false                        ;
  Q = SC.sql.SelectFrom                         (
        "id"                                    ,
        PlanTable(Palettes)                        ,
        SC.WhereUuid(uuid)                    ) ;
  if (SC.Fetch(Q)) exists = true                ;
  ///////////////////////////////////////////////
  int                checksum                   ;
  QByteArray         C     = palette.Compress() ;
  unsigned long long sigma = palette.sigma   () ;
  int                Color = palette.total   () ;
  int                R     = 0                  ;
  int                G     = 0                  ;
  int                B     = 0                  ;
  int                A     = 0                  ;
  checksum = qChecksum(C.data(),C.size())       ;
  A        = (sigma>>48) & 0xFFFF               ;
  R        = (sigma>>32) & 0xFFFF               ;
  G        = (sigma>>16) & 0xFFFF               ;
  B        =  sigma      & 0xFFFF               ;
  if (exists)                                   {
    Q = SC.sql.Update                           (
          PlanTable(Palettes)                      ,
          SC.sql.Where(1,"uuid")                ,
          9                                     ,
          "sigma"                               ,
          "colors"                              ,
          "R"                                   ,
          "G"                                   ,
          "B"                                   ,
          "A"                                   ,
          "compression"                         ,
          "checksum"                            ,
          "colormap"                          ) ;
  } else                                        {
    SC.assureUuid                               (
      PlanTable(MajorUuid)                         ,
      uuid                                      ,
      Types::Palette                          ) ;
    Q = SC.sql.InsertInto                       (
          PlanTable(Palettes)                      ,
          10                                    ,
          "uuid"                                ,
          "sigma"                               ,
          "colors"                              ,
          "R"                                   ,
          "G"                                   ,
          "B"                                   ,
          "A"                                   ,
          "compression"                         ,
          "checksum"                            ,
          "colormap"                          ) ;
  }                                             ;
  SC . Prepare ( Q                            ) ;
  SC . Bind    ( "uuid"        , uuid         ) ;
  SC . Bind    ( "sigma"       , sigma        ) ;
  SC . Bind    ( "colors"      , Color        ) ;
  SC . Bind    ( "R"           , R            ) ;
  SC . Bind    ( "G"           , G            ) ;
  SC . Bind    ( "B"           , B            ) ;
  SC . Bind    ( "A"           , A            ) ;
  SC . Bind    ( "compression" , C.size()     ) ;
  SC . Bind    ( "checksum"    , checksum     ) ;
  SC . Bind    ( "colormap"    , C            ) ;
  SC . Exec    (                              ) ;
  return true                                   ;
}

bool N::PictureManager :: assurePalette (
       SqlConnection    & SC            ,
       SUID               uuid          ,
       QImage           & image         )
{
  QString Q                          ;
  SUID    pcid = 0                   ;
  Palette palette                    ;
  Q = SC.sql.SelectFrom              (
        "colormap"                   ,
        PlanTable(PicturePalettes)      ,
        QString("where picture = %1" )
        .arg(uuid)                 ) ;
  if (SC.Fetch(Q))                   {
    pcid = SC.Uuid(0)                ;
  }                                  ;
  nKickOut ( pcid > 0 , true )       ;
  palette . set ( image )            ;
  pcid = FindPalette(SC,palette)     ;
  if (pcid<=0)                       {
    pcid = AppendPalette(SC,palette) ;
  }                                  ;
  nKickOut ( pcid <= 0 , false )     ;
  return AttachPalette(SC,uuid,pcid) ;
}

bool N::PictureManager :: assureHistogram (
       SqlConnection    & SC              ,
       SUID               uuid            ,
       QImage           & image           )
{
  QImage        * I = &image                              ;
  QImage        * K = NULL                                ;
  /////////////////////////////////////////////////////////
  if (I->format()!=QImage::Format_ARGB32)                 {
     K = new QImage ( )                                   ;
    *K = image.convertToFormat(QImage::Format_ARGB32)     ;
     I = K                                                ;
  }                                                       ;
  /////////////////////////////////////////////////////////
  QString         Q                                       ;
  double          T                                       ;
  double          R,G,B,A                                 ;
  int             C[1024]                                 ;
  double          H[1024]                                 ;
  unsigned char * bits   = I->bits ()                     ;
  int             total  = I->width() * I->height()       ;
  int             dp     = 0                              ;
  int             colors = 0                              ;
  /////////////////////////////////////////////////////////
  QList<unsigned long> Colors                             ;
  /////////////////////////////////////////////////////////
  nKickOut ( IsNull(bits) , false )                       ;
  for (int ii=0;ii<1024;ii++)                             {
    C[ii] = 0                                             ;
    H[ii] = 0                                             ;
  }                                                       ;
  /////////////////////////////////////////////////////////
  if (total<64000000)                                     {
    for (int ii=0;ii<total;ii++,dp+=4)                    {
      unsigned long c = 0                                 ;
      int b = bits [ dp     ]                             ;
      int g = bits [ dp + 1 ]                             ;
      int r = bits [ dp + 2 ]                             ;
      int a = bits [ dp + 3 ]                             ;
      c  = r; c <<= 8                                     ;
      c |= g; c <<= 8                                     ;
      c |= b                                              ;
      g += 256                                            ;
      r += 512                                            ;
      a += 768                                            ;
      C[b]++                                              ;
      C[g]++                                              ;
      C[r]++                                              ;
      C[a]++                                              ;
      Colors << c                                         ;
    }                                                     ;
  } else                                                  {
    for (int ii=0;ii<total;ii++,dp+=4)                    {
      int b = bits [ dp     ]                             ;
      int g = bits [ dp + 1 ]                             ;
      int r = bits [ dp + 2 ]                             ;
      int a = bits [ dp + 3 ]                             ;
      g += 256                                            ;
      r += 512                                            ;
      a += 768                                            ;
      C[b]++                                              ;
      C[g]++                                              ;
      C[r]++                                              ;
      C[a]++                                              ;
    }                                                     ;
    colors = -1                                           ;
  }                                                       ;
  /////////////////////////////////////////////////////////
  if (Colors.count()>0)                                   {
    colors = 1                                            ;
    if (Colors.count()>1)                                 {
      qSort(Colors.begin(),Colors.end())                  ;
      unsigned long c = Colors[0]                         ;
      int           t = Colors.count()                    ;
      unsigned long x                                     ;
      for (int ii=1;ii<t;ii++)                            {
        x = Colors[ii]                                    ;
        if (c!=x)                                         {
          c = x                                           ;
          colors++                                        ;
        }                                                 ;
      }                                                   ;
    }                                                     ;
  }                                                       ;
  /////////////////////////////////////////////////////////
  for (int ii=0;ii<1024;ii++)                             {
    H[ii]  = C[ii]                                        ;
    H[ii] /= total                                        ;
  }                                                       ;
  /////////////////////////////////////////////////////////
  R = G = B = A = T = 0                                   ;
  for (int ii=  0;ii< 256;ii++) B += (H[ii]*H[ii])        ;
  for (int ii=256;ii< 512;ii++) G += (H[ii]*H[ii])        ;
  for (int ii=512;ii< 768;ii++) R += (H[ii]*H[ii])        ;
  for (int ii=768;ii<1024;ii++) A += (H[ii]*H[ii])        ;
  T = R + G + B                                           ;
  /////////////////////////////////////////////////////////
  QByteArray U((const char *)H,sizeof(double)*1024)       ;
  QByteArray M = qCompress(U)                             ;
  /////////////////////////////////////////////////////////
  Q = SC.sql.InsertInto                                   (
        PlanTable(Histogram)                                 ,
        7                                                 ,
        "uuid"                                            ,
        "total"                                           ,
        "r"                                               ,
        "g"                                               ,
        "b"                                               ,
        "a"                                               ,
        "histogram"                                     ) ;
  SC . Prepare ( Q                                      ) ;
  SC . Bind    ( "uuid"      , uuid                     ) ;
  SC . Bind    ( "total"     , T                        ) ;
  SC . Bind    ( "r"         , R                        ) ;
  SC . Bind    ( "g"         , G                        ) ;
  SC . Bind    ( "b"         , B                        ) ;
  SC . Bind    ( "a"         , A                        ) ;
  SC . Bind    ( "histogram" , M                        ) ;
  SC . Exec    (                                        ) ;
  /////////////////////////////////////////////////////////
  Q = SC.sql.InsertInto                                   (
        PlanTable(PictureColors)                             ,
        2                                                 ,
        "uuid"                                            ,
        "colors"                                        ) ;
  SC . Prepare ( Q                                      ) ;
  SC . Bind    ( "uuid"   , uuid                        ) ;
  SC . Bind    ( "colors" , colors                      ) ;
  SC . Exec    (                                        ) ;
  /////////////////////////////////////////////////////////
  Colors . clear ( )                                      ;
  U      . clear ( )                                      ;
  M      . clear ( )                                      ;
  /////////////////////////////////////////////////////////
  if (NotNull(K)) delete K                                ;
  return true                                             ;
}

double N::PictureManager::Similarity(QImage & First,QImage & Second,int Method)
{
  IplImage * I1                              ;
  IplImage * I2                              ;
  IplImage * G1                              ;
  IplImage * G2                              ;
  I1 = toIplImage ( First  )                 ;
  I2 = toIplImage ( Second )                 ;
  G1 = toGrey     ( I1     )                 ;
  G2 = toGrey     ( I2     )                 ;
  double similarity                          ;
  similarity = ::cvMatchShapes(G1,G2,Method) ;
  Release(I1)                                ;
  Release(I2)                                ;
  Release(G1)                                ;
  Release(G2)                                ;
  return similarity                          ;
}

bool N::PictureManager::SkinMask(QImage & Mask,QPolygonF & IF)
{
  QPainterPath path                                ;
  int          total = IF . count ( ) / 2          ;
  path . moveTo ( IF[0] )                          ;
  for (int i=0;i<total;i++)                        {
    int j = i + i + 1                              ;
    path . quadTo ( IF [ j ] , IF [ j + 1 ] )      ;
  }                                                ;
  QBrush brh  ( QColor ( 255 , 255 , 255 , 255 ) ) ;
  Mask . fill ( QColor (   0 ,   0 ,   0 ,   0 ) ) ;
  QPainter pter                                    ;
  pter . begin    ( &Mask      )                   ;
  pter . fillPath ( path , brh )                   ;
  pter . end      (            )                   ;
  return true                                      ;
}

bool N::PictureManager::SurfFeaturePoints (
       QImage        & image              ,
       FeaturePoints & points             ,
       double          hessianThreshold   ,
       int             octaves            ,
       int             octaveLayers       )
{
#pragma message("cv::SurfFeatureDetector was not supported in OpenCV 3.1.0")
#ifdef XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
  IplImage * ipl                                                                   ;
  if (image.format()==QImage::Format_RGB888)                                       {
    ipl  = toIplImage ( image )                                                    ;
  } else                                                                           {
    QImage XXXXX = image.convertToFormat(QImage::Format_RGB888)                    ;
    ipl  = toIplImage ( XXXXX )                                                    ;
  }                                                                                ;
  if (IsNull(ipl )) return false                                                   ;
  IplImage * grey = toGrey ( ipl )                                                 ;
  Release ( ipl )                                                                  ;
  if (IsNull(grey)) return false                                                   ;
  //////////////////////////////////////////////////////////////////////////////////
  cv::SurfFeatureDetector   detector ( hessianThreshold , octaves , octaveLayers ) ;
  std::vector<cv::KeyPoint> keypoints                                              ;
  //////////////////////////////////////////////////////////////////////////////////
  detector . detect ( grey , keypoints )                                           ;
  //////////////////////////////////////////////////////////////////////////////////
  Release ( grey )                                                                 ;
  if (keypoints.size()>0) points = keypoints                                       ;
  //////////////////////////////////////////////////////////////////////////////////
  return ( points . count ( ) > 0 )                                                ;
#endif
  return false ;
}

bool N::PictureManager::SurfFeaturePoints (
       QImage    & image                  ,
       QPolygonF & points                 ,
       double      hessianThreshold       ,
       int         octaves                ,
       int         octaveLayers           )
{
#pragma message("cv::SurfFeatureDetector was not supported in OpenCV 3.1.0")
#ifdef XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
  IplImage * ipl                                                                   ;
  if (image.format()==QImage::Format_RGB888)                                       {
    ipl  = toIplImage ( image )                                                    ;
  } else                                                                           {
    QImage XXXXX = image.convertToFormat(QImage::Format_RGB888)                    ;
    ipl  = toIplImage ( XXXXX )                                                    ;
  }                                                                                ;
  if (IsNull(ipl )) return false                                                   ;
  IplImage * grey = toGrey ( ipl )                                                 ;
  Release ( ipl  )                                                                 ;
  if (IsNull(grey)) return false                                                   ;
  //////////////////////////////////////////////////////////////////////////////////
  cv::SurfFeatureDetector   detector ( hessianThreshold , octaves , octaveLayers ) ;
  std::vector<cv::KeyPoint> keypoints                                              ;
  //////////////////////////////////////////////////////////////////////////////////
  detector . detect( grey , keypoints )                                            ;
  //////////////////////////////////////////////////////////////////////////////////
  Release ( ipl  )                                                                 ;
  Release ( grey )                                                                 ;
  //////////////////////////////////////////////////////////////////////////////////
  for (int i=0;i<keypoints.size();i++)                                             {
    qreal   x = keypoints[i].pt.x                                                  ;
    qreal   y = keypoints[i].pt.y                                                  ;
    QPointF P ( x , y )                                                            ;
    points << P                                                                    ;
  }                                                                                ;
  //////////////////////////////////////////////////////////////////////////////////
  return ( points . count ( ) > 0 )                                                ;
#endif
  return false ;
}

QImage N::PictureManager::toGrey(QImage & source)
{
  QImage I              ;
  toGrey ( source , I ) ;
  return I              ;
}

bool N::PictureManager::toGrey(QImage & source,QImage & grey)
{
  QVector<QRgb> GreyIndex          ;
  for (int i=0;i<256;i++)          {
    GreyIndex << qRgb(i,i,i)       ;
  }                                ;
  grey = source . convertToFormat  (
           QImage::Format_Indexed8 ,
           GreyIndex             ) ;
  return true                      ;
}

bool N::PictureManager::HorizontalSobel(QImage & source,QImage & sobel)
{
  QImage grey                          ;
  toGrey ( source , grey )             ;
  sobel = grey                         ;
  sobel . fill ( 0 )                   ;
  if (sobel . width ()<3) return false ;
  if (sobel . height()<3) return false ;
  //////////////////////////////////////
  unsigned char * s = grey  . bits ( ) ;
  unsigned char * t = sobel . bits ( ) ;
  int w = grey . width        ( ) - 2  ;
  int h = grey . height       ( ) - 2  ;
  int p = grey . bytesPerLine ( )      ;
  int t1,t2,t3,b1,b2,b3,v,r,a          ;
  //////////////////////////////////////
  for (int y=0;y<h;y++)                {
    t1 = y  * p                        ;
    t2 = t1 + p                        ;
    t3 = t2 + p                        ;
    b1 = t1 + 2                        ;
    b2 = t2 + 2                        ;
    b3 = t3 + 2                        ;
    a  = t2 + 1                        ;
    for (int x=0;x<w;x++)              {
      r    = 0                         ;
      v    = s[t2]                     ;
      r   += s[t1]                     ;
      r   += v                         ;
      r   += v                         ;
      r   += s[t3]                     ;
      v    = s[b2]                     ;
      r   -= s[b1]                     ;
      r   -= v                         ;
      r   -= v                         ;
      r   -= s[b3]                     ;
      if (r<0) r = -r                  ;
      t[a] = (unsigned char)r          ;
      t1++                             ;
      t2++                             ;
      t3++                             ;
      b1++                             ;
      b2++                             ;
      b3++                             ;
      a ++                             ;
    }                                  ;
  }                                    ;
  //////////////////////////////////////
  return true                          ;
}

bool N::PictureManager::HorizontalSobel(QImage & source,QImage & sobel,int threshold)
{
  QImage grey                          ;
  toGrey ( source , grey )             ;
  sobel = grey                         ;
  sobel . fill ( 0 )                   ;
  if (sobel . width ()<3) return false ;
  if (sobel . height()<3) return false ;
  //////////////////////////////////////
  unsigned char * s = grey  . bits ( ) ;
  unsigned char * t = sobel . bits ( ) ;
  int w = grey . width        ( ) - 2  ;
  int h = grey . height       ( ) - 2  ;
  int p = grey . bytesPerLine ( )      ;
  int t1,t2,t3,b1,b2,b3,v,r,a          ;
  //////////////////////////////////////
  for (int y=0;y<h;y++)                {
    t1 = y  * p                        ;
    t2 = t1 + p                        ;
    t3 = t2 + p                        ;
    b1 = t1 + 2                        ;
    b2 = t2 + 2                        ;
    b3 = t3 + 2                        ;
    a  = t2 + 1                        ;
    for (int x=0;x<w;x++)              {
      r    = 0                         ;
      v    = s[t2]                     ;
      r   += s[t1]                     ;
      r   += v                         ;
      r   += v                         ;
      r   += s[t3]                     ;
      v    = s[b2]                     ;
      r   -= s[b1]                     ;
      r   -= v                         ;
      r   -= v                         ;
      r   -= s[b3]                     ;
      if (r>=threshold)                {
        if (r>255) r = 255             ;
        t[a] = (unsigned char)r        ;
      }                                ;
      t1++                             ;
      t2++                             ;
      t3++                             ;
      b1++                             ;
      b2++                             ;
      b3++                             ;
      a ++                             ;
    }                                  ;
  }                                    ;
  //////////////////////////////////////
  return true                          ;
}

bool N::PictureManager::VerticalSobel(QImage & source,QImage & sobel)
{
  QImage grey                          ;
  toGrey ( source , grey )             ;
  sobel = grey                         ;
  sobel . fill ( 0 )                   ;
  if (sobel . width ()<3) return false ;
  if (sobel . height()<3) return false ;
  //////////////////////////////////////
  unsigned char * s = grey  . bits ( ) ;
  unsigned char * t = sobel . bits ( ) ;
  int w = grey . width        ( ) - 2  ;
  int h = grey . height       ( ) - 2  ;
  int p = grey . bytesPerLine ( )      ;
  int t1,t2,t3,b1,b2,b3,v,r,a          ;
  //////////////////////////////////////
  for (int y=0;y<h;y++)                {
    t1 = y  * p                        ;
    t2 = t1 + 1                        ;
    t3 = t2 + 1                        ;
    b1 = t1 + p + p                    ;
    b2 = b1 + 1                        ;
    b3 = b2 + 1                        ;
    a  = t1 + p + 1                    ;
    for (int x=0;x<w;x++)              {
      r    = 0                         ;
      v    = s[t2]                     ;
      r   += s[t1]                     ;
      r   += v                         ;
      r   += v                         ;
      r   += s[t3]                     ;
      v    = s[b2]                     ;
      r   -= s[b1]                     ;
      r   -= v                         ;
      r   -= v                         ;
      r   -= s[b3]                     ;
      if (r<0) r = -r                  ;
      t[a] = (unsigned char)r          ;
      t1++                             ;
      t2++                             ;
      t3++                             ;
      b1++                             ;
      b2++                             ;
      b3++                             ;
      a ++                             ;
    }                                  ;
  }                                    ;
  //////////////////////////////////////
  return true                          ;
}

bool N::PictureManager::VerticalSobel(QImage & source,QImage & sobel,int threshold)
{
  QImage grey                          ;
  toGrey ( source , grey )             ;
  sobel = grey                         ;
  sobel . fill ( 0 )                   ;
  if (sobel . width ()<3) return false ;
  if (sobel . height()<3) return false ;
  //////////////////////////////////////
  unsigned char * s = grey  . bits ( ) ;
  unsigned char * t = sobel . bits ( ) ;
  int w = grey . width        ( ) - 2  ;
  int h = grey . height       ( ) - 2  ;
  int p = grey . bytesPerLine ( )      ;
  int t1,t2,t3,b1,b2,b3,v,r,a          ;
  //////////////////////////////////////
  for (int y=0;y<h;y++)                {
    t1 = y  * p                        ;
    t2 = t1 + 1                        ;
    t3 = t2 + 1                        ;
    b1 = t1 + p + p                    ;
    b2 = b1 + 1                        ;
    b3 = b2 + 1                        ;
    a  = t1 + p + 1                    ;
    for (int x=0;x<w;x++)              {
      r    = 0                         ;
      v    = s[t2]                     ;
      r   += s[t1]                     ;
      r   += v                         ;
      r   += v                         ;
      r   += s[t3]                     ;
      v    = s[b2]                     ;
      r   -= s[b1]                     ;
      r   -= v                         ;
      r   -= v                         ;
      r   -= s[b3]                     ;
      if (r>=threshold)                {
        if (r>255) r = 255             ;
        t[a] = (unsigned char)r        ;
      }                                ;
      t1++                             ;
      t2++                             ;
      t3++                             ;
      b1++                             ;
      b2++                             ;
      b3++                             ;
      a ++                             ;
    }                                  ;
  }                                    ;
  //////////////////////////////////////
  return true                          ;
}

bool N::PictureManager::Stitch(QImage & result,QList<QImage> & images,double confidence)
{
#pragma message("cv::Stitcher was not supported in OpenCV 3.1.0")
#ifdef XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
  if (images.count()<=0) return false                                         ;
  /////////////////////////////////////////////////////////////////////////////
  cv  :: Stitcher        stitcher = cv :: Stitcher :: createDefault ( true )  ;
  IplImage             * img                                                  ;
  std :: vector<cv::Mat> vImg                                                 ;
  cv  :: Mat             rImg                                                 ;
  cv::Stitcher::Status   status                                               ;
  QList<IplImage *>      IPLs                                                 ;
  /////////////////////////////////////////////////////////////////////////////
  for (int i=0;i<images.count();i++)                                          {
    QImage     rgb = images [ i ] .convertToFormat(QImage::Format_RGB888)     ;
    IplImage * ipl = toIplImage ( rgb )                                       ;
    if (NotNull(ipl))                                                         {
      cv::Mat MT(ipl)                                                         ;
      vImg . push_back ( MT  )                                                ;
      IPLs << ipl                                                             ;
    }                                                                         ;
  }                                                                           ;
  /////////////////////////////////////////////////////////////////////////////
  stitcher . setPanoConfidenceThresh ( confidence )                           ;
  status = stitcher . stitch ( vImg , rImg )                                  ;
  if ( status == cv::Stitcher::OK)                                            {
    img = ::cvCloneImage ( (IplImage *)&rImg )                                ;
  } else                                                                      {
    img = NULL                                                                ;
  }                                                                           ;
  /////////////////////////////////////////////////////////////////////////////
  for (int i=0;i<IPLs.count();i++)                                            {
    IplImage * ipl = IPLs[i]                                                  ;
    if (NotNull(ipl)) Release ( ipl )                                         ;
  }                                                                           ;
  if (IsNull(img)) return false                                               ;
  /////////////////////////////////////////////////////////////////////////////
  QImage * imx                                                                ;
  imx = toImage(img)                                                          ;
  if (IsNull(imx)) return false                                               ;
  result = *imx                                                               ;
  delete imx                                                                  ;
  /////////////////////////////////////////////////////////////////////////////
  return true                                                                 ;
#endif
  return false ;
}

IplImage * N::PictureManager::RepairMask(IplImage * source,int level)
{
#pragma message("cv::mat can not converted in OpenCV 3.1.0")
#ifdef XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
  IplImage * grey = toGrey ( source )                            ;
  IplImage * mask = NULL                                         ;
  ::cvThreshold(grey,grey,level,255,CV_THRESH_BINARY)            ;
  cv::Mat SRC  ( source )                                        ;
  cv::Mat MASK ( grey   )                                        ;
  std::vector< std::vector<cv::Point> > contours                 ;
  cv::findContours                                               (
    MASK.clone()                                                 ,
    contours                                                     ,
    CV_RETR_EXTERNAL                                             ,
    CV_CHAIN_APPROX_SIMPLE                                     ) ;
  for (int i = 0; i < contours.size(); i++)                      {
    cv::Rect r = cv::boundingRect(contours[i])                   ;
    if (std::abs(1 - (SRC.cols/r.width)) > 0.2)                  {
      cv::drawContours(MASK,contours,i,CV_RGB(0,0,0), CV_FILLED) ;
    }                                                            ;
  }                                                              ;
  mask = ::cvCloneImage ( (IplImage *)&MASK )                    ;
  Release ( grey )                                               ;
  return mask                                                    ;
#endif
  return NULL ;
}

bool N::PictureManager::Repair (
       QImage & source         ,
       QImage & patch          ,
       double   range          ,
       int      flags          ,
       int      level          )
{
  IplImage * src  = NULL                               ;
  IplImage * mask = NULL                               ;
  IplImage * dest = NULL                               ;
  QImage     SRC                                       ;
  SRC  = source.convertToFormat(QImage::Format_RGB888) ;
  src  = toIplImage(SRC)                               ;
  if (IsNull(src)) return false                        ;
  dest = ::cvCloneImage ( src         )                ;
  mask = RepairMask     ( src , level )                ;
  if (IsNull(mask))                                    {
    Release ( src  )                                   ;
    Release ( dest )                                   ;
    return false                                       ;
  }                                                    ;
  //////////////////////////////////////////////////////
  ::cvInpaint(src,mask,dest,range,flags)               ;
  //////////////////////////////////////////////////////
  QImage * R                                           ;
  R = toImage(dest)                                    ;
  if (NotNull(R))                                      {
    patch = *R                                         ;
    delete R                                           ;
  }                                                    ;
  //////////////////////////////////////////////////////
  Release ( src  )                                     ;
  Release ( mask )                                     ;
  Release ( dest )                                     ;
  //////////////////////////////////////////////////////
  return true                                          ;
}

bool N::PictureManager::Repair (
       QImage   & source       ,
       QImage   & patch        ,
       IplImage * mask         ,
       double     range        ,
       int        flags        )
{
  IplImage * src  = NULL                               ;
  IplImage * dest = NULL                               ;
  QImage     SRC                                       ;
  SRC  = source.convertToFormat(QImage::Format_RGB888) ;
  src  = toIplImage(SRC)                               ;
  if (IsNull(src)) return false                        ;
  dest = ::cvCloneImage ( src )                        ;
  //////////////////////////////////////////////////////
  ::cvInpaint(src,mask,dest,range,flags)               ;
  //////////////////////////////////////////////////////
  QImage * R                                           ;
  R = toImage(dest)                                    ;
  if (NotNull(R))                                      {
    patch = *R                                         ;
    delete R                                           ;
  }                                                    ;
  //////////////////////////////////////////////////////
  Release ( src  )                                     ;
  Release ( dest )                                     ;
  //////////////////////////////////////////////////////
  return true                                          ;
}

bool N::PictureManager::ObtainSizes (
       SqlConnection & SC           ,
       UUIDs         & U            ,
       QList<QSize>  & SIZEs        )
{
  SUID u                                 ;
  foreach ( u , U )                      {
    QSize S = Geometry ( SC , u )        ;
    if ( !SIZEs.contains(S) ) SIZEs << S ;
  }                                      ;
  return ( SIZEs.count() > 0 )           ;
}

bool N::PictureManager::ObtainBySizes (
       SqlConnection & SC             ,
       UUIDs         & R              ,
       QList<QSize>  & SIZEs          )
{
  QString Q                                                             ;
  for (int i=0;i<SIZEs.count();i++)                                     {
    int w = SIZEs [ i ] . width  ( )                                    ;
    int h = SIZEs [ i ] . height ( )                                    ;
    Q = SC . sql . SelectFrom                                           (
          "uuid"                                                        ,
          ThumbINDEX                                                    ,
          QString("where `width` = %1 and `height` = %2").arg(w).arg(h) ,
          SC.OrderByAsc("id")                                         ) ;
    SqlLoopNow ( SC , Q )                                               ;
      SUID x = SC . Uuid ( 0 )                                          ;
      if ( ! R . contains ( x ) ) R << x                                ;
    SqlLoopErr ( SC , Q )                                               ;
    SqlLoopEnd ( SC , Q )                                               ;
  }                                                                     ;
  return ( R . count ( ) > 0 )                                          ;
}

bool N::PictureManager::ObtainColors (
       SqlConnection & SC            ,
       UUIDs         & U             ,
       CUIDs         & COLORs        )
{
  SUID u                                   ;
  foreach ( u , U )                        {
    int C = ColorCount ( SC , u )          ;
    if ( !COLORs.contains(C) ) COLORs << C ;
  }                                        ;
  return ( COLORs.count() > 0 )            ;
}

bool N::PictureManager::ObtainByColors (
       SqlConnection & SC              ,
       UUIDs         & R               ,
       CUIDs         & COLORs          )
{
  QString Q                                     ;
  for (int i=0;i<COLORs.count();i++)            {
    int c = COLORs [ i ]                        ;
    Q = SC . sql . SelectFrom                   (
          "uuid"                                ,
          PlanTable(PictureColors)                 ,
          QString("where `colors` = %1").arg(c) ,
          SC.OrderByAsc("id")                 ) ;
    SqlLoopNow ( SC , Q )                       ;
      SUID x = SC . Uuid ( 0 )                  ;
      if ( ! R . contains ( x ) ) R << x        ;
    SqlLoopErr ( SC , Q )                       ;
    SqlLoopEnd ( SC , Q )                       ;
  }                                             ;
  return ( R . count ( ) > 0 )                  ;
}

bool N::PictureManager::ObtainPALs (
       SqlConnection & SC          ,
       UUIDs         & U           ,
       UUIDs         & PALs        )
{
  QString Q                                      ;
  SUID u                                         ;
  foreach ( u , U )                              {
    SUID p = 0                                   ;
    Q = SC.sql.SelectFrom                        (
          "colormap"                             ,
          PlanTable(PicturePalettes)                ,
          QString("where `picture` = %1").arg(u) ,
          SC.OrderByAsc("id")                  ) ;
    SqlLoopNow ( SC , Q )                        ;
      p = SC . Uuid ( 0 )                        ;
      if ( ( p > 0 ) && ( ! PALs.contains(p) ) ) {
        PALs << p                                ;
      }                                          ;
    SqlLoopErr ( SC , Q )                        ;
    SqlLoopEnd ( SC , Q )                        ;
  }                                              ;
  return ( PALs.count() > 0 )                    ;
}

bool N::PictureManager::ObtainByPALs (
       SqlConnection & SC            ,
       UUIDs         & R             ,
       UUIDs         & PALs          )
{
  QString Q                                       ;
  for (int i=0;i<PALs.count();i++)                {
    SUID p = PALs [ i ]                           ;
    Q = SC . sql . SelectFrom                     (
          "picture"                               ,
          PlanTable(PicturePalettes)                 ,
          QString("where `colormap` = %1").arg(p) ,
          SC.OrderByAsc("id")                   ) ;
    SqlLoopNow ( SC , Q )                         ;
      SUID x = SC . Uuid ( 0 )                    ;
      if ( ! R . contains ( x ) ) R << x          ;
    SqlLoopErr ( SC , Q )                         ;
    SqlLoopEnd ( SC , Q )                         ;
  }                                               ;
  return ( R . count ( ) > 0 )                    ;
}

bool N::PictureManager::ObtainHistos (
       SqlConnection & SC            ,
       UUIDs         & U             ,
       ByteArrays    & HISTOs        )
{
  QString Q                                ;
  SUID    u                                ;
  foreach ( u , U )                        {
    QByteArray B                           ;
    Q = SC . sql . SelectFrom              (
          "histogram"                      ,
          PlanTable(Histogram)                ,
          SC.WhereUuid(u)                ) ;
    if ( SC.Fetch(Q) ) B = SC.ByteArray(0) ;
    if ( !HISTOs.contains(B) ) HISTOs << B ;
  }                                        ;
  return ( HISTOs.count() > 0 )            ;
}

bool N::PictureManager::ObtainByHistos (
       SqlConnection & SC              ,
       UUIDs         & R               ,
       ByteArrays    & HISTOs          )
{
  QString Q                           ;
  for (int i=0;i<HISTOs.count();i++)  {
    QByteArray B = HISTOs [ i ]       ;
    Q = SC . sql . SelectFrom         (
          "uuid"                      ,
          PlanTable(Histogram)           ,
          QString("where length(histogram) = %1 "
                  "and histogram = :HISTOGRAM").arg(B.size()) ,
          SC.OrderByAsc("id")       ) ;
    SC . Prepare ( Q                ) ;
    SC . Bind    ( "histogram" , B  ) ;
    IfSqlLoop    ( SC               ) {
      SUID x = SC . Uuid  ( 0 )       ;
      if ( ! R . contains ( x ) )     {
        R << x                        ;
      }                               ;
    }                                 ;
  }                                   ;
  return ( R . count ( ) > 0 )        ;
}


