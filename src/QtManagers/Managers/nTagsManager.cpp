#include <qtmanagers.h>

#define plan TagsPlan

N::TagsManager:: TagsManager (Plan * p)
               : TagsPlan    (       p)
{
}

N::TagsManager::~TagsManager (void)
{
}

UUIDs N::TagsManager::Uuids(SqlConnection & SC,int Type)
{
  return SC . Uuids                                                   (
           PlanTable(Tags)                                            ,
           "`uuid`"                                                   ,
           QString("where `type` = %1 order by `id` asc").arg(Type) ) ;
}

bool N::TagsManager::toNames       (
       SqlConnection & SC          ,
       int             vLanguageId ,
       UUIDs         & Uuids       ,
       NAMEs         & names       )
{
  SUID u                              ;
  foreach ( u , Uuids )               {
    QString name = SC . getName       (
                     PlanTable(Names) ,
                     "uuid"           ,
                     vLanguageId      ,
                     u              ) ;
    names [ u ] = name                ;
  }                                   ;
  return ( names . count ( ) > 0 )    ;
}

SUID N::TagsManager::NewTag(SqlConnection & SC,int T)
{
  SUID u                   ;
  int  h = Unify::TagsHead ;
  h *= 10000               ;
  h += T                   ;
  u = SC . Unique          (
    PlanTable(MajorUuid)   ,
    "uuid"                 ,
    h                    ) ;
  SC . assureUuid          (
    PlanTable(MajorUuid)   ,
    u                      ,
    Types::Tag           ) ;
  SC . assureUuid          (
    PlanTable(Tags)        ,
    u                      ,
    T                    ) ;
  return u                 ;
}

bool N::TagsManager::Join(SqlConnection & SC         ,
                          SUID            tag        ,
                          int             ObjectType ,
                          const UUIDs   & Uuids      ,
                          bool            progress   )
{
  QString Q                                                               ;
  int     final = 0                                                       ;
  qint64  total = Uuids . count ( )                                       ;
  qint64  index = 0                                                       ;
  int     id    = -1                                                      ;
  bool    go    = true                                                    ;
  /////////////////////////////////////////////////////////////////////////
  //              Get lastest position                                   //
  /////////////////////////////////////////////////////////////////////////
  Q = SC . sql . SelectFrom                                               (
        "position"                                                        ,
        PlanTable(Groups)                                                 ,
        QString                                                           (
          "where `first` = %1 "
          "and `t1` = %2 "
          "and `t2` = %3 "
          "and `relation` = %4 "
          "order by `position` desc "
          "limit 0,1"
        )
        . arg ( tag                                                       )
        . arg ( Types::Tag                                                )
        . arg ( ObjectType                                                )
        . arg ( Groups::Subordination)                                  ) ;
  if ( SC . Fetch ( Q ) )                                                 {
    final = SC . Int ( 0 )                                                ;
  }                                                                       ;
  final++                                                                 ;
  /////////////////////////////////////////////////////////////////////////
  if ( total < 100 ) progress = false                                     ;
  if ( progress )                                                         {
    id = plan -> Progress ( QString("Join tag %1") . arg(tag) , "%v/%m" ) ;
    plan      -> Start    ( id , &index , &go                           ) ;
    plan      -> setRange ( id , 0      , total                         ) ;
  }                                                                       ;
  /////////////////////////////////////////////////////////////////////////
  SUID u                                                                  ;
  foreach ( u , Uuids )                                                   {
    index++                                                               ;
    Q = SC . sql . InsertInto                                             (
          "first,second,t1,t2,relation,position"                          ,
          PlanTable ( Groups                                            ) ,
          QString   ( "%1,%2,%3,%4,%5,%6"                                 )
          . arg     ( tag                                                 )
          . arg     ( u                                                   )
          . arg     ( Types::Tag                                          )
          . arg     ( ObjectType                                          )
          . arg     ( Groups::Subordination                               )
          . arg     ( final                                               )
        ) + ";"                                                           ;
    if ( SC . Query ( Q ) ) final++                                       ;
  }                                                                       ;
  /////////////////////////////////////////////////////////////////////////
  plan -> Finish ( id )                                                   ;
  return true                                                             ;
}

int N::TagsManager::Count(SqlConnection & Connection,SUID uuid)
{
  int     total = 0                                             ;
  QString Q                                                     ;
  Q = Connection.sql.SelectFrom                                 (
        "count(*)"                                              ,
        PlanTable(Groups)                                       ,
        QString("where first = %1 and t1 = %2 and relation = %3")
          .arg(uuid                                             )
          .arg(Types::Tag                                       )
          .arg(Groups::Subordination                            )
        )                                                       ;
  if (Connection.Fetch(Q)) total = Connection.Value(0).toInt()  ;
  return total                                                  ;
}

SUID N::TagsManager::TagUuid(SqlConnection & SC       ,
                             int             type     ,
                             QString         name     ,
                             int             language )
{
  QString Q                                     ;
  UUIDs   uuids                                 ;
  if (language>0)                               {
      Q = SC.sql.SelectFrom                     (
            "uuid"                              ,
            PlanTable(Names)                    ,
            SC.sql.Where(2,"name","language") ) ;
    SC . Prepare ( Q                          ) ;
    SC . Bind    ( "name"     , name.toUtf8() ) ;
    SC . Bind    ( "language" , language      ) ;
    if (SC.Exec())                              {
      while (SC.Next()) uuids << SC.Uuid(0)     ;
    }                                           ;
  } else                                        {
    Q = SC.sql.SelectFrom                       (
          "uuid"                                ,
          PlanTable(Names)                      ,
          SC.sql.Where(1,"name")              ) ;
    SC . Prepare ( Q                          ) ;
    SC . Bind    ( "name" , name.toUtf8()     ) ;
    if (SC.Exec())                              {
      while (SC.Next()) uuids << SC.Uuid(0)     ;
    }                                           ;
  }                                             ;
  nKickOut ( uuids.count()<=0 , 0 )             ;
  SUID uuid                                     ;
  int  t                                        ;
  foreach (uuid,uuids)                          {
    Q = SC.sql.SelectFrom                       (
          "type"                                ,
          PlanTable(MajorUuid)                  ,
          SC.WhereUuid(uuid)                  ) ;
    if (SC.Fetch(Q))                            {
      t = SC.Int(0)                             ;
      if (t==Types::Tag)                        {
        Q = SC.sql.SelectFrom                   (
              "type"                            ,
              PlanTable(Tags)                   ,
              SC.WhereUuid(uuid)              ) ;
        if (SC.Fetch(Q))                        {
          t = SC.Int(0)                         ;
          if (t==type) return uuid              ;
        }                                       ;
      }                                         ;
    }                                           ;
  }                                             ;
  return 0                                      ;
}

bool N::TagsManager::GetTags(SqlConnection & SC,UUIDs & U,QString name)
{
  QString by = "="                                            ;
  if ( name . contains ( '%' ) ) by = "like"             ; else
  if ( name . contains ( '*' ) ) by = "regexp"                ;
  /////////////////////////////////////////////////////////////
  UUIDs   T                                                   ;
  UUIDs   M                                                   ;
  QString Q                                                   ;
  QString W                                                   ;
  SUID    u                                                   ;
  W = QString ( "where `name` %1 :NAME" ) . arg ( by )        ;
  Q = SC . sql . SelectFrom ( "uuid" , PlanTable(Names) , W ) ;
  /////////////////////////////////////////////////////////////
  SC . Prepare ( Q                          )                 ;
  SC . Bind    ( "name" , name . toUtf8 ( ) )                 ;
  if ( SC . Exec ( ) ) while ( SC . Next ( ) )                {
    SUID x = SC . Uuid ( 0 )                                  ;
    if ( ! T . contains ( x ) ) T << x                        ;
  }                                                           ;
  if ( T . count ( ) <= 0 ) return false                      ;
  T = Uniqueness ( T )                                        ;
  M = T                                                       ;
  /////////////////////////////////////////////////////////////
  foreach ( u , T )                                           {
    Q = SC . sql . SelectFrom                                 (
          "uuid"                                              ,
          PlanTable(NameMaps)                                 ,
          QString ( "where `name` = %1" ) . arg ( u )       ) ;
    SqlLoopNow ( SC , Q )                                     ;
      SUID t = SC . Uuid ( 0 )                                ;
      if ( ! M . contains ( t ) ) M << t                      ;
    SqlLoopErr ( SC , Q )                                     ;
    SqlLoopEnd ( SC , Q )                                     ;
  }                                                           ;
  /////////////////////////////////////////////////////////////
  foreach ( u , M )                                           {
    Q = SC . sql . SelectFrom                                 (
          "type"                                              ,
          PlanTable(MajorUuid)                                ,
          SC . WhereUuid ( u )                              ) ;
    if ( SC . Fetch ( Q ) )                                   {
      SUID z = SC . Uuid ( 0 )                                ;
      if ( Types::Tag == z )                                  {
        if ( ! U . contains ( u ) ) U << u                    ;
      }                                                       ;
    }                                                         ;
  }                                                           ;
  if ( U . count ( ) > 1 )  U = Uniqueness ( U )              ;
  /////////////////////////////////////////////////////////////
  return ( U . count ( ) > 0 )                                ;
}
