#include <qtmanagers.h>

#define TABLES(ID) ScenePlan->Tables[Tables::ID]

N::SceneManager:: SceneManager ( Plan * p )
                : ScenePlan    (        p )
{
}

N::SceneManager::~SceneManager(void)
{
}

QString N::SceneManager::Name(SqlConnection & Connection,SUID uuid,int language)
{
  return Connection.getName(TABLES(Names),"uuid",language,uuid) ;
}

bool N::SceneManager::assureName(SqlConnection & Connection,SUID uuid,int language,QString name)
{
  return Connection.assureName(TABLES(Names),uuid,language,name) ;
}

UUIDs N::SceneManager::Matrices(SqlConnection & Connection)
{
  return Connection.Uuids(TABLES(Matrices),"uuid","order by id desc") ;
}

SUID N::SceneManager::updateMatrix(SqlConnection & Connection,SUID uuid,Matrix & matrix)
{
  if (uuid<=0)                                                            {
    uuid = Connection.Unique(TABLES(MajorUuid),"uuid",7011)               ;
    Connection.assureUuid(TABLES(MajorUuid),uuid,Types::Matrix) ;
  }                                                                       ;
  QString Q                                                               ;
  int checksum = qChecksum((const char *)matrix.data(),matrix.size())     ;
  Q = Connection.sql.SelectFrom                                           (
        "type",TABLES(Matrices)                                           ,
        Connection.WhereUuid(uuid)                                      ) ;
  if (Connection.Fetch(Q))                                                {
    Q = Connection.sql.Update(TABLES(Matrices),"where uuid = :UUID",6     ,
          "type","rows","columns","size","checksum","matrix"            ) ;
  } else                                                                  {
    Q = Connection.sql.InsertInto(TABLES(Matrices),7                      ,
          "uuid","type","rows","columns","size","checksum","matrix"     ) ;
  }                                                                       ;
  Connection . Prepare ( Q )                                              ;
  Connection . Bind    ( "uuid"     , uuid                )               ;
  Connection . Bind    ( "type"     , matrix . Type    () )               ;
  Connection . Bind    ( "rows"     , matrix . Rows    () )               ;
  Connection . Bind    ( "columns"  , matrix . Columns () )               ;
  Connection . Bind    ( "size"     , matrix . size    () )               ;
  Connection . Bind    ( "checksum" , checksum            )               ;
  Connection . Bind    ( "matrix"   , matrix              )               ;
  Connection . Exec    (   )                                              ;
  return uuid                                                             ;
}

SUID N::SceneManager::updateMatrix(SqlConnection & Connection,SUID uuid,QString name,QString identifier,Matrix & matrix)
{
  if (uuid<=0)                                                            {
    uuid = Connection.Unique(TABLES(MajorUuid),"uuid",7011)               ;
    Connection.assureUuid(TABLES(MajorUuid),uuid,Types::Matrix) ;
  }                                                                       ;
  QString Q                                                               ;
  int checksum = qChecksum((const char *)matrix.data(),matrix.size())     ;
  Q = Connection.sql.SelectFrom                                           (
        "type",TABLES(Matrices)                                           ,
        Connection.WhereUuid(uuid)                                      ) ;
  if (Connection.Fetch(Q))                                                {
    Q = Connection.sql.Update(TABLES(Matrices),"where uuid = :UUID",7     ,
          "name","type","rows","columns","size","checksum","matrix"     ) ;
  } else                                                                  {
    Q = Connection.sql.InsertInto(TABLES(Matrices),8                      ,
        "uuid","name","type","rows","columns","size","checksum","matrix") ;
  }                                                                       ;
  Connection . Prepare ( Q )                                              ;
  Connection . Bind    ( "uuid"     , uuid                )               ;
  Connection . Bind    ( "name"     , identifier.toUtf8() )               ;
  Connection . Bind    ( "type"     , matrix . Type    () )               ;
  Connection . Bind    ( "rows"     , matrix . Rows    () )               ;
  Connection . Bind    ( "columns"  , matrix . Columns () )               ;
  Connection . Bind    ( "size"     , matrix . size    () )               ;
  Connection . Bind    ( "checksum" , checksum            )               ;
  Connection . Bind    ( "matrix"   , matrix              )               ;
  Connection . Exec    (   )                                              ;
  assureName(Connection,uuid,ScenePlan->LanguageId,name)                  ;
  return uuid                                                             ;
}

N::Matrix N::SceneManager::getMatrix(SqlConnection & Connection,SUID uuid)
{
  Matrix matrix                      ;
  QString Q                          ;
  Q = Connection.sql.SelectFrom      (
        "matrix"                     ,
        TABLES(Matrices)             ,
        Connection.WhereUuid(uuid) ) ;
  if (Connection.Fetch(Q))           {
    QByteArray B                     ;
    B = Connection.ByteArray(0)      ;
    matrix.resize(B.size())          ;
    char * d = (char *)B.data()      ;
    char * m = (char *)matrix.data() ;
    memcpy ( m , d , B.size() )      ;
  }                                  ;
  return  matrix                     ;
}

UUIDs N::SceneManager::Cameras(SqlConnection &Connection)
{
  return Connection.Uuids(TABLES(Cameras),"uuid","order by id desc ;") ;
}

N::Camera N::SceneManager::GetCamera(SqlConnection & Connection,SUID uuid)
{
  Camera camera                                             ;
  QString Q                                                 ;
  camera.node = uuid                                        ;
  camera.name = Name(Connection,uuid,ScenePlan->LanguageId) ;
  Q = Connection.sql.SelectFrom                             (
        "suid"                                              ,
        TABLES(CameraParaments)                             ,
        "where uuid = :UUID and name = :NAME ;"           ) ;
  Connection . Prepare ( Q                 )                ;
  Connection . Bind    ( "uuid" , uuid     )                ;
  Connection . Bind    ( "name" , "Matrix" )                ;
  if (Connection.Exec() && Connection.Next())               {
    camera.muid = Connection . Uuid ( 0 )                   ;
  }                                                         ;
  Q = Connection.sql.SelectFrom                             (
        "flags"                                             ,
        TABLES(CameraParaments)                             ,
        "where uuid = :UUID and name = :NAME ;"           ) ;
  Connection . Prepare ( Q                 )                ;
  Connection . Bind    ( "uuid" , uuid     )                ;
  Connection . Bind    ( "name" , "Mode"   )                ;
  if (Connection.Exec() && Connection.Next())               {
    camera.mode = Connection . Int ( 0 )                    ;
  }                                                         ;
  Q = Connection.sql.SelectFrom                             (
        "f"                                                 ,
        TABLES(CameraParaments)                             ,
        "where uuid = :UUID and name = :NAME ;"           ) ;
  Connection . Prepare ( Q                 )                ;
  Connection . Bind    ( "uuid" , uuid     )                ;
  Connection . Bind    ( "name" , "FOV"    )                ;
  if (Connection.Exec() && Connection.Next())               {
    camera.FOV = Connection . Value ( 0 ) . toDouble ( )    ;
  }                                                         ;
  Q = Connection.sql.SelectFrom                             (
        "x,y,z,r,t,f"                                       ,
        TABLES(CameraParaments)                             ,
        "where uuid = :UUID and name = :NAME ;"           ) ;
  Connection . Prepare ( Q                  )               ;
  Connection . Bind    ( "uuid" , uuid      )               ;
  Connection . Bind    ( "name" , "Minimum" )               ;
  if (Connection.Exec() && Connection.Next())               {
    camera.minimum.x = Connection.Value(0).toDouble()       ;
    camera.minimum.y = Connection.Value(1).toDouble()       ;
    camera.minimum.z = Connection.Value(2).toDouble()       ;
    camera.minimum.r = Connection.Value(3).toDouble()       ;
    camera.minimum.t = Connection.Value(4).toDouble()       ;
    camera.minimum.f = Connection.Value(5).toDouble()       ;
  }                                                         ;
  Q = Connection.sql.SelectFrom                             (
        "x,y,z,r,t,f"                                       ,
        TABLES(CameraParaments)                             ,
        "where uuid = :UUID and name = :NAME ;"           ) ;
  Connection . Prepare ( Q                  )               ;
  Connection . Bind    ( "uuid" , uuid      )               ;
  Connection . Bind    ( "name" , "Maximum" )               ;
  if (Connection.Exec() && Connection.Next())               {
    camera.maximum.x = Connection.Value(0).toDouble()       ;
    camera.maximum.y = Connection.Value(1).toDouble()       ;
    camera.maximum.z = Connection.Value(2).toDouble()       ;
    camera.maximum.r = Connection.Value(3).toDouble()       ;
    camera.maximum.t = Connection.Value(4).toDouble()       ;
    camera.maximum.f = Connection.Value(5).toDouble()       ;
  }                                                         ;
  Q = Connection.sql.SelectFrom                             (
        "x,y,z,r,t,f"                                       ,
        TABLES(CameraParaments)                             ,
        "where uuid = :UUID and name = :NAME ;"           ) ;
  Connection . Prepare ( Q                  )               ;
  Connection . Bind    ( "uuid" , uuid      )               ;
  Connection . Bind    ( "name" , "Eye"     )               ;
  if (Connection.Exec() && Connection.Next())               {
    camera.eye.x = Connection.Value(0).toDouble()           ;
    camera.eye.y = Connection.Value(1).toDouble()           ;
    camera.eye.z = Connection.Value(2).toDouble()           ;
    camera.eye.r = Connection.Value(3).toDouble()           ;
    camera.eye.t = Connection.Value(4).toDouble()           ;
    camera.eye.f = Connection.Value(5).toDouble()           ;
  }                                                         ;
  Q = Connection.sql.SelectFrom                             (
        "x,y,z,r,t,f"                                       ,
        TABLES(CameraParaments)                             ,
        "where uuid = :UUID and name = :NAME ;"           ) ;
  Connection . Prepare ( Q                  )               ;
  Connection . Bind    ( "uuid" , uuid      )               ;
  Connection . Bind    ( "name" , "Center"  )               ;
  if (Connection.Exec() && Connection.Next())               {
    camera.center.x = Connection.Value(0).toDouble()        ;
    camera.center.y = Connection.Value(1).toDouble()        ;
    camera.center.z = Connection.Value(2).toDouble()        ;
    camera.center.r = Connection.Value(3).toDouble()        ;
    camera.center.t = Connection.Value(4).toDouble()        ;
    camera.center.f = Connection.Value(5).toDouble()        ;
  }                                                         ;
  Q = Connection.sql.SelectFrom                             (
        "x,y,z,r,t,f"                                       ,
        TABLES(CameraParaments)                             ,
        "where uuid = :UUID and name = :NAME ;"           ) ;
  Connection . Prepare ( Q                  )               ;
  Connection . Bind    ( "uuid" , uuid      )               ;
  Connection . Bind    ( "name" , "Up"      )               ;
  if (Connection.Exec() && Connection.Next())               {
    camera.up.x = Connection.Value(0).toDouble()            ;
    camera.up.y = Connection.Value(1).toDouble()            ;
    camera.up.z = Connection.Value(2).toDouble()            ;
    camera.up.r = Connection.Value(3).toDouble()            ;
    camera.up.t = Connection.Value(4).toDouble()            ;
    camera.up.f = Connection.Value(5).toDouble()            ;
  }                                                         ;
  if (camera.muid>0)                                        {
    camera.matrix = getMatrix(Connection,camera.muid)       ;
  }                                                         ;
  return camera                                             ;
}

bool N::SceneManager::updateCamera (
       SqlConnection & SC          ,
       Camera        & camera      ,
       QImage        & snap        ,
       QString         identifier  )
{
  QString          Q                                                     ;
  GraphicsManager  GM(ScenePlan)                                         ;
  PictureManager   PM(ScenePlan)                                         ;
  QImage           icon = PM . Icon        ( snap , QSize(128,128) )     ;
  QByteArray       CPNG = GM . CompressPNG ( icon                  )     ;
  if (camera.node<=0)                                                    {
    camera.node = SC.Unique(TABLES(MajorUuid),"uuid",7111)               ;
    SC.assureUuid(TABLES(MajorUuid),camera.node,Types::Camera)           ;
    Q  = SC.sql.InsertInto(TABLES(Cameras),1,"uuid")                     ;
    SC . Prepare ( Q                    )                                ;
    SC . Bind    ( "uuid" , camera.node )                                ;
    SC . Exec    (                      )                                ;
    Q  = SC.sql.InsertInto(TABLES(CameraParaments),2,"uuid","name")      ;
    SC . Prepare ( Q                    )                                ;
    SC . Bind    ( "uuid" , camera.node )                                ;
    SC . Bind    ( "name" , "Matrix"    )                                ;
    SC . Exec    (                      )                                ;
    SC . Prepare ( Q                    )                                ;
    SC . Bind    ( "uuid" , camera.node )                                ;
    SC . Bind    ( "name" , "Minimum"   )                                ;
    SC . Exec    (                      )                                ;
    SC . Prepare ( Q                    )                                ;
    SC . Bind    ( "uuid" , camera.node )                                ;
    SC . Bind    ( "name" , "Maximum"   )                                ;
    SC . Exec    (                      )                                ;
    SC . Prepare ( Q                    )                                ;
    SC . Bind    ( "uuid" , camera.node )                                ;
    SC . Bind    ( "name" , "Mode"      )                                ;
    SC . Exec    (                      )                                ;
    SC . Prepare ( Q                    )                                ;
    SC . Bind    ( "uuid" , camera.node )                                ;
    SC . Bind    ( "name" , "FOV"       )                                ;
    SC . Exec    (                      )                                ;
    SC . Prepare ( Q                    )                                ;
    SC . Bind    ( "uuid" , camera.node )                                ;
    SC . Bind    ( "name" , "Eye"       )                                ;
    SC . Exec    (                      )                                ;
    SC . Prepare ( Q                    )                                ;
    SC . Bind    ( "uuid" , camera.node )                                ;
    SC . Bind    ( "name" , "Center"    )                                ;
    SC . Exec    (                      )                                ;
    SC . Prepare ( Q                    )                                ;
    SC . Bind    ( "uuid" , camera.node )                                ;
    SC . Bind    ( "name" , "Up"        )                                ;
    SC . Exec    (                      )                                ;
  }                                                                      ;
  camera.muid = updateMatrix(SC,camera.muid,camera.matrix)               ;
  assureName(SC,camera.node,ScenePlan->LanguageId,camera.name)           ;
  assureName(SC,camera.muid,ScenePlan->LanguageId,camera.name)           ;
  ////////////////////////////////////////////////////////////////////////
  Q = SC.sql.Update                                                      (
        TABLES(Cameras)                                                  ,
        "where uuid = :UUID"                                             ,
        3,"name","flags","snap"                                        ) ;
  SC . Prepare ( Q                             )                         ;
  SC . Bind    ( "uuid"  , camera.node         )                         ;
  SC . Bind    ( "name"  , identifier.toUtf8() )                         ;
  SC . Bind    ( "flags" , 0                   )                         ;
  SC . Bind    ( "snap"  , CPNG                )                         ;
  SC . Exec    (                               )                         ;
  ////////////////////////////////////////////////////////////////////////
  Q = SC.sql.Update                                                      (
        TABLES(CameraParaments)                                          ,
        "where uuid = :UUID and name = :NAME"                            ,
        1,"suid"                                                       ) ;
  SC . Prepare ( Q                    )                                  ;
  SC . Bind    ( "uuid" , camera.node )                                  ;
  SC . Bind    ( "name" , "Matrix"    )                                  ;
  SC . Bind    ( "suid" , camera.muid )                                  ;
  SC . Exec    (                      )                                  ;
  Q = SC.sql.Update                                                      (
        TABLES(CameraParaments)                                          ,
        "where uuid = :UUID and name = :NAME"                            ,
        6,"x","y","z","r","t","f"                                      ) ;
  SC . Prepare ( Q                    )                                  ;
  SC . Bind    ( "uuid" , camera.node )                                  ;
  SC . Bind    ( "name" , "Minimum"   )                                  ;
  SC . Bind    ( "x"    , camera.minimum.x )                             ;
  SC . Bind    ( "y"    , camera.minimum.y )                             ;
  SC . Bind    ( "z"    , camera.minimum.z )                             ;
  SC . Bind    ( "r"    , camera.minimum.r )                             ;
  SC . Bind    ( "t"    , camera.minimum.t )                             ;
  SC . Bind    ( "f"    , camera.minimum.f )                             ;
  SC . Exec    (                      )                                  ;
  Q = SC.sql.Update                                                      (
        TABLES(CameraParaments)                                          ,
        "where uuid = :UUID and name = :NAME"                            ,
        6,"x","y","z","r","t","f"                                      ) ;
  SC . Prepare ( Q                         )                             ;
  SC . Bind    ( "uuid" , camera.node      )                             ;
  SC . Bind    ( "name" , "Maximum"        )                             ;
  SC . Bind    ( "x"    , camera.maximum.x )                             ;
  SC . Bind    ( "y"    , camera.maximum.y )                             ;
  SC . Bind    ( "z"    , camera.maximum.z )                             ;
  SC . Bind    ( "r"    , camera.maximum.r )                             ;
  SC . Bind    ( "t"    , camera.maximum.t )                             ;
  SC . Bind    ( "f"    , camera.maximum.f )                             ;
  SC . Exec    (                           )                             ;
  Q = SC.sql.Update                                                      (
        TABLES(CameraParaments)                                          ,
        "where uuid = :UUID and name = :NAME"                            ,
        1,"flags"                                                      ) ;
  SC . Prepare ( Q                     )                                 ;
  SC . Bind    ( "uuid"  , camera.node )                                 ;
  SC . Bind    ( "name"  , "Mode"      )                                 ;
  SC . Bind    ( "flags" , camera.mode )                                 ;
  SC . Exec    (                       )                                 ;
  Q = SC.sql.Update                                                      (
        TABLES(CameraParaments)                                          ,
        "where uuid = :UUID and name = :NAME"                            ,
        1,"f"                                                          ) ;
  SC . Prepare ( Q                    )                                  ;
  SC . Bind    ( "uuid" , camera.node )                                  ;
  SC . Bind    ( "name" , "FOV"       )                                  ;
  SC . Bind    ( "f"    , camera.FOV  )                                  ;
  SC . Exec    (                      )                                  ;
  Q = SC.sql.Update                                                      (
        TABLES(CameraParaments)                                          ,
        "where uuid = :UUID and name = :NAME"                            ,
        6,"x","y","z","r","t","f"                                      ) ;
  SC . Prepare ( Q                    )                                  ;
  SC . Bind    ( "uuid" , camera.node )                                  ;
  SC . Bind    ( "name" , "Eye"       )                                  ;
  SC . Bind    ( "x"    , camera.eye.x )                                 ;
  SC . Bind    ( "y"    , camera.eye.y )                                 ;
  SC . Bind    ( "z"    , camera.eye.z )                                 ;
  SC . Bind    ( "r"    , camera.eye.r )                                 ;
  SC . Bind    ( "t"    , camera.eye.t )                                 ;
  SC . Bind    ( "f"    , camera.eye.f )                                 ;
  SC . Exec    (                      )                                  ;
  Q = SC.sql.Update                                                      (
        TABLES(CameraParaments)                                          ,
        "where uuid = :UUID and name = :NAME"                            ,
        6,"x","y","z","r","t","f"                                      ) ;
  SC . Prepare ( Q                        )                              ;
  SC . Bind    ( "uuid" , camera.node     )                              ;
  SC . Bind    ( "name" , "Center"        )                              ;
  SC . Bind    ( "x"    , camera.center.x )                              ;
  SC . Bind    ( "y"    , camera.center.y )                              ;
  SC . Bind    ( "z"    , camera.center.z )                              ;
  SC . Bind    ( "r"    , camera.center.r )                              ;
  SC . Bind    ( "t"    , camera.center.t )                              ;
  SC . Bind    ( "f"    , camera.center.f )                              ;
  SC . Exec    (                          )                              ;
  Q = SC.sql.Update                                                      (
        TABLES(CameraParaments)                                          ,
        "where uuid = :UUID and name = :NAME"                            ,
        6,"x","y","z","r","t","f"                                      ) ;
  SC . Prepare ( Q                    )                                  ;
  SC . Bind    ( "uuid" , camera.node )                                  ;
  SC . Bind    ( "name" , "Up"        )                                  ;
  SC . Bind    ( "x"    , camera.up.x )                                  ;
  SC . Bind    ( "y"    , camera.up.y )                                  ;
  SC . Bind    ( "z"    , camera.up.z )                                  ;
  SC . Bind    ( "r"    , camera.up.r )                                  ;
  SC . Bind    ( "t"    , camera.up.t )                                  ;
  SC . Bind    ( "f"    , camera.up.f )                                  ;
  SC . Exec    (                      )                                  ;
  return true                                                            ;
}
