#include <qtmanagers.h>

#define TABLES(ID) PeoplePlan->Tables[Tables::ID]

N::PeopleManager:: PeopleManager (Plan * p)
                 : PeoplePlan    (       p)
{
}

N::PeopleManager::~PeopleManager (void)
{
}

UUIDs N::PeopleManager::Lists(SqlConnection & Connection,QString options)
{
  return Connection.Uuids(TABLES(PeopleUuid),"uuid",options) ;
}

UUIDs N::PeopleManager::NameList(SqlConnection & Connection,SUID Uuid)
{
  return Connection.Uuids                                            (
           TABLES(NameMaps),"name"                                   ,
           QString("where uuid = %1 order by prefer asc").arg(Uuid)) ;
}

UUIDs N::PeopleManager::Group(SqlConnection & Connection,SUID first,int t1,QString options)
{
  QString Where                                         ;
  Where = QString                                       (
          "where first = %1 "
          "and t1 = %2 "
          "and t2 = %3 "
          "and relation = %4"                           )
          .arg(first                                    )
          .arg(t1                                       )
          .arg(Types::People                            )
          .arg(Groups::Subordination                  ) ;
  if (options.length()>0) Where = Where + " " + options ;
  return Connection.Uuids                               (
           TABLES(PeopleUuid)                           ,
           "second",Where                             ) ;
}

QStringList N::PeopleManager::Names(SqlConnection & Connection,SUID Uuid)
{
  QStringList names                                                   ;
  UUIDs       Uuids                                                   ;
  SUID        uuid                                                    ;
  Uuids = Connection.Uuids                                            (
            TABLES(NameMaps),"name"                                   ,
            QString("where uuid = %1 order by prefer asc").arg(Uuid)) ;
  foreach (uuid,Uuids)                                                {
    QString Q = Connection.sql.SelectFrom("name"                      ,
                  TABLES(Names)                                       ,
                  QString("where uuid = %1").arg(uuid)              ) ;
    if (Connection.Fetch(Q)) names << Connection.String(0)            ;
  }                                                                   ;
  return names                                                        ;
}

int N::PeopleManager::PeopleComboBox(SqlConnection & Connection,QComboBox * Search)
{
  QStringList words                            ;
  SUID        uuid                             ;
  UUIDs       Uuids = Lists(Connection)        ;
  Search -> clear()                            ;
  foreach (uuid,Uuids)                         {
    QStringList names = Names(Connection,uuid) ;
    QString n                                  ;
    foreach (n,names)                          {
      Search->addItem(n,uuid)                  ;
      if (!words.contains(n)) words << n       ;
    }                                          ;
  }                                            ;
  QCompleter * comp = new QCompleter(words)    ;
  Search->lineEdit()->setCompleter(comp)       ;
  Search->lineEdit()->setText     (""  )       ;
  return Search -> count ()                    ;
}

QString N::PeopleManager::PreferName(SqlConnection & Connection,SUID uuid)
{
  QString name = ""                                                         ;
  SUID    nuid = 0                                                          ;
  QString Q                                                                 ;
  Q = Connection.sql.SelectFrom                                             (
        "name"                                                              ,
        TABLES(NameMaps)                                                    ,
        QString("where uuid = %1 order by prefer asc limit 0,1").arg(uuid)) ;
  if (Connection.Fetch(Q)) nuid = Connection.Uuid(0)                        ;
  if (nuid>0)                                                               {
    Q = Connection.sql.SelectFrom                                           (
          "name"                                                            ,
          TABLES(Names)                                                     ,
          QString("where uuid = %1 limit 0,1").arg(nuid))                   ;
    if (Connection.Fetch(Q)) name = Connection.String(0)                    ;
  }                                                                         ;
  return name                                                               ;
}

QString N::PeopleManager::Name(SqlConnection & Connection,SUID Uuid)
{
  QString name = ""                                     ;
  QString Q    = Connection.sql.SelectFrom              (
        "name"                                          ,
        TABLES(Names)                                   ,
        QString("where uuid = %1 limit 0,1").arg(Uuid)) ;
  if (Connection.Fetch(Q)) name = Connection.String(0)  ;
  return name                                           ;
}

QString N::PeopleManager::Name(SqlConnection & Connection,SUID uuid,int language)
{
  QString name = ""                                     ;
  QString Q    = Connection.sql.SelectFrom              (
        "name"                                          ,
        TABLES(Names)                                   ,
        QString("where uuid = %1 and language = %2 limit 0,1").arg(uuid).arg(language)) ;
  if (Connection.Fetch(Q)) name = Connection.String(0)  ;
  return name                                           ;
}

bool N::PeopleManager::NameLanguage(SqlConnection & Connection,SUID Uuid)
{
  QString Q = Connection.sql.SelectFrom   (
    "language,name"                       ,
    TABLES(Names)                         ,
    QString("where uuid = %1").arg(Uuid)) ;
  return Connection . Fetch ( Q )         ;
}

SUID N::PeopleManager::Surname(SqlConnection & Connection,SUID Uuid)
{
  SUID    suid = 0 ;
  QString Q        ;
  Q = Connection.sql.SelectFrom(
        "first",
        TABLES(Groups) ,
        QString("where second = %1 and t1 = %2 and t2 = %3 and relation = %4")
          .arg(Uuid)
          .arg(Types::Surname)
          .arg(Types::Name)
          .arg(Groups::Subordination)
      );
  if (Connection.Fetch(Q)) suid = Connection.Uuid(0) ;
  return suid ;
}

bool N::PeopleManager::PeopleSurname(SqlConnection & Connection,SUID surname,SUID name)
{
  QString Q ;
  Q = Connection.sql.DeleteFrom(
        TABLES(Groups),
        QString("where second = %1 and t1 = %2 and t2 = %3 and relation = %4")
          .arg(name)
          .arg(Types::Surname)
          .arg(Types::Name)
          .arg(Groups::Subordination)
      );
  Connection.Query(Q) ;
  Q = Connection.sql.InsertInto(
        TABLES(Groups),
        5,"first","second","t1","t2","relation");
  Connection.Prepare(Q);
  Connection.Bind("first",surname);
  Connection.Bind("second",name);
  Connection.Bind("t1",Types::Surname);
  Connection.Bind("t2",Types::Name);
  Connection.Bind("relation",Groups::Subordination);
  Connection.Exec();
  return true ;
}

UUIDs N::PeopleManager::Surnames(SqlConnection & Connection,QString options)
{
  return Connection.Uuids(TABLES(Surnames),"uuid",options) ;
}

bool N::PeopleManager::PersonHasSurname(SqlConnection & Connection,SUID surname,UUIDs & people)
{
  QString Q                              ;
  UUIDs   N                              ;
  UUIDs   O                              ;
  SUID    u                              ;
  people . clear ( )                     ;
  Q = Connection.sql.SelectFrom          (
        "second"                         ,
        TABLES(Groups)                   ,
        QString("where first = %1 and t1 = %2 and t2 = %3 and relation = %4 order by position asc")
        .arg(surname).arg(Types::Surname).arg(Types::Name).arg(Groups::Subordination) ) ;
  SqlLoopNow ( Connection , Q )          ;
    N << Connection.Uuid ( 0 )           ;
  SqlLoopErr ( Connection , Q )          ;
  SqlLoopEnd ( Connection , Q )          ;
  if (N.count()<=0) return true          ;
  foreach (u,N)                          {
    Q = Connection.sql.SelectFrom("uuid",TABLES(NameMaps),QString("where name = %1").arg(u)) ;
    SqlLoopNow ( Connection , Q )        ;
      SUID x = Connection.Uuid(0)        ;
      if (!O.contains(x)) O << x         ;
    SqlLoopErr ( Connection , Q )        ;
    SqlLoopEnd ( Connection , Q )        ;
  }                                      ;
  if (O.count()<=0) return true          ;
  foreach (u,O)                          {
    Q = Connection.sql.SelectFrom("type",TABLES(MajorUuid),Connection.WhereUuid(u)) ;
    if (Connection.Fetch(Q))             {
      int t = Connection.Int(0)          ;
      if (nEqual(t,Types::People))       {
        people << u                      ;
      }                                  ;
    }                                    ;
  }                                      ;
  return true                            ;
}

bool N::PeopleManager::SurnameEquivalences(SqlConnection & Connection,SUID surname,UUIDs & equivalences)
{
  QString Q                                                       ;
  Q = Connection.sql.SelectFrom                                   (
        "second"                                                  ,
        TABLES(Groups)                                            ,
        QString("%1 %2"                                           )
        .arg(Connection.sql.Where(4,"first","t1","t2","relation") )
        .arg(Connection.OrderByDesc("position"))                ) ;
  Connection . Prepare ( Q                                 )      ;
  Connection . Bind    ( "first"    , surname              )      ;
  Connection . Bind    ( "t1"       , Types  :: Surname    )      ;
  Connection . Bind    ( "t2"       , Types  :: Surname    )      ;
  Connection . Bind    ( "relation" , Groups :: Equivalent )      ;
  if (Connection.Exec()) while (Connection.Next())                {
    SUID u = Connection.Uuid(0)                                   ;
    if (!equivalences.contains(u)) equivalences << u              ;
  }                                                               ;
  return true                                                     ;
}

bool N::PeopleManager::addEqualSurname(SqlConnection & Connection,SUID surname,SUID equal)
{
  QString Q                                                                 ;
  int     position                                                          ;
  ///////////////////////////////////////////////////////////////////////////
  position = 0                                                              ;
  Q = Connection.sql.SelectFrom                                             (
        "position"                                                          ,
        TABLES(Groups)                                                      ,
        QString ( "%1 %2 %3"                                                )
        . arg ( Connection . sql . Where ( 4,"first","t1","t2","relation" ) )
        . arg ( Connection . OrderByDesc ( "position"                     ) )
        . arg ( Connection . sql . Limit ( 0,1                        ) ) ) ;
  Connection . Prepare ( Q                                 )                ;
  Connection . Bind    ( "first"    , surname              )                ;
  Connection . Bind    ( "t1"       , Types  :: Surname    )                ;
  Connection . Bind    ( "t2"       , Types  :: Surname    )                ;
  Connection . Bind    ( "relation" , Groups :: Equivalent )                ;
  IfSqlExec(Connection) position = Connection.Int(0)                        ;
  ///////////////////////////////////////////////////////////////////////////
  Q = Connection.sql.InsertInto                                             (
        TABLES(Groups)                                                      ,
        6                                                                   ,
        "first"                                                             ,
        "second"                                                            ,
        "t1"                                                                ,
        "t2"                                                                ,
        "relation"                                                          ,
        "position"                                                        ) ;
  Connection . Prepare ( Q                                 )                ;
  Connection . Bind    ( "first"    , surname              )                ;
  Connection . Bind    ( "second"   , equal                )                ;
  Connection . Bind    ( "t1"       , Types  :: Surname    )                ;
  Connection . Bind    ( "t2"       , Types  :: Surname    )                ;
  Connection . Bind    ( "relation" , Groups :: Equivalent )                ;
  Connection . Bind    ( "position" , position             )                ;
  Connection . Exec    (                                   )                ;
  ///////////////////////////////////////////////////////////////////////////
  position = 0                                                              ;
  Q = Connection.sql.SelectFrom                                             (
        "position"                                                          ,
        TABLES(Groups)                                                      ,
        QString ( "%1 %2 %3"                                                )
        . arg ( Connection . sql . Where ( 4,"first","t1","t2","relation" ) )
        . arg ( Connection . OrderByDesc ( "position"                     ) )
        . arg ( Connection . sql . Limit ( 0,1                        ) ) ) ;
  Connection . Prepare ( Q                                 )                ;
  Connection . Bind    ( "first"    , equal                )                ;
  Connection . Bind    ( "t1"       , Types::Surname       )                ;
  Connection . Bind    ( "t2"       , Types::Surname       )                ;
  Connection . Bind    ( "relation" , Groups :: Equivalent )                ;
  IfSqlExec(Connection) position = Connection.Int(0)                        ;
  ///////////////////////////////////////////////////////////////////////////
  Q = Connection.sql.InsertInto                                             (
        TABLES(Groups)                                                      ,
        6                                                                   ,
        "first"                                                             ,
        "second"                                                            ,
        "t1"                                                                ,
        "t2"                                                                ,
        "relation"                                                          ,
        "position"                                                        ) ;
  Connection . Prepare ( Q                                 )                ;
  Connection . Bind    ( "first"    , equal                )                ;
  Connection . Bind    ( "second"   , surname              )                ;
  Connection . Bind    ( "t1"       , Types  :: Surname    )                ;
  Connection . Bind    ( "t2"       , Types  :: Surname    )                ;
  Connection . Bind    ( "relation" , Groups :: Equivalent )                ;
  Connection . Bind    ( "position" , position             )                ;
  Connection . Exec    (                                   )                ;
  ///////////////////////////////////////////////////////////////////////////
  return true                                                               ;
}

bool N::PeopleManager::removeEqualSurname(SqlConnection & Connection,SUID surname,SUID equal)
{
  QString Q                                                             ;
  ///////////////////////////////////////////////////////////////////////
  Q = Connection.sql.DeleteFrom                                         (
        TABLES(Groups)                                                  ,
        Connection.sql.Where(5,"first","second","t1","t2","relation") ) ;
  Connection . Prepare ( Q                                 )            ;
  Connection . Bind    ( "first"    , surname              )            ;
  Connection . Bind    ( "second"   , equal                )            ;
  Connection . Bind    ( "t1"       , Types  :: Surname    )            ;
  Connection . Bind    ( "t2"       , Types  :: Surname    )            ;
  Connection . Bind    ( "relation" , Groups :: Equivalent )            ;
  Connection . Exec    (                                   )            ;
  ///////////////////////////////////////////////////////////////////////
  Q = Connection.sql.DeleteFrom                                         (
        TABLES(Groups)                                                  ,
        Connection.sql.Where(5,"first","second","t1","t2","relation") ) ;
  Connection . Prepare ( Q                                 )            ;
  Connection . Bind    ( "first"    , equal                )            ;
  Connection . Bind    ( "second"   , surname              )            ;
  Connection . Bind    ( "t1"       , Types  :: Surname    )            ;
  Connection . Bind    ( "t2"       , Types  :: Surname    )            ;
  Connection . Bind    ( "relation" , Groups :: Equivalent )            ;
  Connection . Exec    (                                   )            ;
  ///////////////////////////////////////////////////////////////////////
  return true                                                           ;
}

bool N::PeopleManager::detachEqualSurname(SqlConnection & Connection,SUID surname,SUID equal)
{
  QString Q                                                             ;
  ///////////////////////////////////////////////////////////////////////
  Q = Connection.sql.DeleteFrom                                         (
        TABLES(Groups)                                                  ,
        Connection.sql.Where(5,"first","second","t1","t2","relation") ) ;
  Connection . Prepare ( Q                                 )            ;
  Connection . Bind    ( "first"    , surname              )            ;
  Connection . Bind    ( "second"   , equal                )            ;
  Connection . Bind    ( "t1"       , Types  :: Surname    )            ;
  Connection . Bind    ( "t2"       , Types  :: Surname    )            ;
  Connection . Bind    ( "relation" , Groups :: Equivalent )            ;
  Connection . Exec    (                                   )            ;
  ///////////////////////////////////////////////////////////////////////
  return true                                                           ;
}

bool N::PeopleManager::SurnameCombo(QComboBox * combo,SqlConnection & Connection,int language,QString name)
{
  UUIDs       Uuids = Surnames(Connection)   ;
  SUID        uuid                           ;
  QStringList sn                             ;
  QString     pn = ""                        ;
  combo -> clear()                           ;
  foreach (uuid,Uuids)                       {
    if (NameLanguage(Connection,uuid))       {
      int     id = Connection.Int    ( 0 )   ;
      QString nn = Connection.String ( 1 )   ;
      int     nl = nn.length         (   )   ;
      QString nx                             ;
      switch (language)                      {
        case 1819                            :
          nx = name.right (nl )              ;
        break                                ;
        default                              :
          nx = name.left  (nl )              ;
        break                                ;
      }                                      ;
      if (id==language)                      {
        combo -> addItem ( nn , uuid )       ;
        sn    << nn                          ;
        if (nx==nn) pn = nn                  ;
      }                                      ;
    }                                        ;
  }                                          ;
  if (sn.count()<=0)                         {
    combo->lineEdit()->setText(name)         ;
    return true                              ;
  }                                          ;
  QCompleter * qc = new QCompleter(sn,combo) ;
  combo->lineEdit()->setCompleter(qc)        ;
  if (pn.length()>0)                         {
    int index = -1                           ;
    index = combo->findText(pn)              ;
    if (index>=0)                            {
      combo->setCurrentIndex(index)          ;
      combo->lineEdit()->setText(pn)         ;
    }                                        ;
  } else                                     {
    combo->lineEdit()->setText(name)         ;
  }                                          ;
  return true                                ;
}

SUID N::PeopleManager::addSurname(SqlConnection & Connection,int language,QString name)
{
  if (name.length()<=0) return 0;
  SUID suid = Connection.Unique(TABLES(MajorUuid),"uuid",IdSections::HumanSurname) ;
  Connection.assureUuid(TABLES(MajorUuid),suid,Types::Surname) ;
  Connection.insertUuid(TABLES(Surnames),suid,"uuid") ;
  QString Q = Connection.NameSyntax(TABLES(Names),"uuid,language,name");
  Connection.insertName(Q,suid,language,name) ;
  return suid ;
}

SUID N::PeopleManager::FindPeople(SqlConnection & Connection,QString name)
{
  QString Q                                        ;
  UUIDs   Nuids                                    ;
  UUIDs   Uuids                                    ;
  SUID    uuid                                     ;
  SUID    puid                                     ;
  //////////////////////////////////////////////////
  Q = Connection.sql.SelectFrom                    (
        "uuid"                                     ,
        TABLES(Names)                              ,
        "where name = :NAME ;")                    ;
  Connection.Prepare(Q)                            ;
  Connection.Bind("name",name.toUtf8())            ;
  if (Connection.Exec()) while (Connection.Next()) {
    Nuids << Connection . Uuid ( 0 )               ;
  }                                                ;
  //////////////////////////////////////////////////
  if (Nuids.count()<=0) return 0                   ;
  foreach (uuid,Nuids)                             {
    Q = Connection.sql.SelectFrom                  (
          "uuid" , TABLES(NameMaps)                ,
          QString("where name = %1 ;").arg(uuid) ) ;
    Uuids . clear ( )                              ;
    SqlLoopNow ( Connection , Q )                  ;
      Uuids << Connection . Uuid ( 0 )             ;
    SqlLoopErr ( Connection , Q )                  ;
    SqlLoopEnd ( Connection , Q )                  ;
    if (Uuids.count()>0)                           {
      foreach (puid,Uuids)                         {
        Q = Connection.sql.SelectFrom              (
              "id"                                 ,
              TABLES(PeopleUuid)                   ,
              Connection.WhereUuid(puid)         ) ;
        if (Connection.Fetch(Q)) return puid       ;
      }                                            ;
    }                                              ;
  }                                                ;
  return 0                                         ;
}

SUID N::PeopleManager::addPeople(SqlConnection & Connection,int language,QString name)
{
  QString Q                                      ;
  SUID uuid = 0                                  ;
  SUID nuid = 0                                  ;
  uuid = Connection.Unique                       (
    TABLES(MajorUuid)                            ,
    "uuid"                                       ,
    IdSections::Human                          ) ;
  Connection . assureUuid                        (
    TABLES(MajorUuid)                            ,
    uuid,Types::People                         ) ;
  Connection . insertUuid                        (
    TABLES(PeopleUuid)                           ,
    uuid,"uuid"                                ) ;
  nuid = Connection.Unique                       (
    TABLES(MajorUuid)                            ,
    "uuid"                                     ) ;
  Connection.assureUuid                          (
    TABLES(MajorUuid)                            ,
    nuid,Types::Name                           ) ;
  Q = Connection . sql . ReplaceInto             (
    TABLES(Names)                                ,
    3,"uuid","language","name"                 ) ;
  Connection . insertName (Q,nuid,language,name) ;
  InsertNameMap(Connection,uuid,nuid)            ;
  return uuid                                    ;
}

bool N::PeopleManager::InsertNameMap(SqlConnection & Connection,SUID Uuid,SUID Nuid)
{
  QString Q                                                         ;
  int     Prefer = -1                                               ;
  Q = Connection.sql.SelectFrom                                     (
        "prefer",TABLES(NameMaps)                                   ,
        QString("where uuid = %1 order by prefer desc limit 0,1") ) ;
  if (Connection.Fetch(Q)) Prefer = Connection.Value(0).toInt()     ;
  Prefer ++                                                         ;
  Q = Connection.sql.InsertInto                                     (
        TABLES(NameMaps)                                            ,
        3,"uuid","name","prefer"                                  ) ;
  Connection . Prepare ( Q )                                        ;
  Connection . Bind    ( "uuid"   , Uuid   )                        ;
  Connection . Bind    ( "name"   , Nuid   )                        ;
  Connection . Bind    ( "prefer" , Prefer )                        ;
  Connection . Exec    (   )                                        ;
  return true                                                       ;
}

bool N::PeopleManager::AssignSexuality(SqlConnection & Connection,SUID People,SUID Sexuality)
{
  QString Q                                             ;
  Q = Connection.sql.DeleteFrom                         (
        TABLES(Groups)                                  ,
          QString("where first = %1 "
                  "and t1 = %2 "
                  "and t2 = %3 "
                  "and relation = %4"                   )
          .arg(People                                   )
          .arg(Types::People                            )
          .arg(Types::Gender                            )
          .arg(Groups::Sexuality                        )
      )                                                 ;
  Connection . Query (Q)                                ;
  Q = Connection.sql.InsertInto                         (
        TABLES(Groups)                                  ,
          5,"first","second","t1","t2","relation"     ) ;
  Connection . Prepare ( Q                            ) ;
  Connection . Bind    ( "first"   ,People            ) ;
  Connection . Bind    ( "second"  ,Sexuality         ) ;
  Connection . Bind    ( "t1"      ,Types::People     ) ;
  Connection . Bind    ( "t2"      ,Types::Gender     ) ;
  Connection . Bind    ( "relation",Groups::Sexuality ) ;
  Connection . Exec    (                              ) ;
  return true                                           ;
}

SUID N::PeopleManager::PeopleGender(SqlConnection & Connection,SUID People)
{
  QString Q                                                    ;
  SUID    uuid = 0                                             ;
  Q = Connection.sql.SelectFrom                                (
        "second" , TABLES(Groups)                              ,
        Connection.sql.Where(4,"first","t1","t2","relation") ) ;
  Connection . Prepare ( Q                                   ) ;
  Connection . Bind    ( "first"   ,People                   ) ;
  Connection . Bind    ( "t1"      ,Types::People            ) ;
  Connection . Bind    ( "t2"      ,Types::Gender            ) ;
  Connection . Bind    ( "relation",Groups::Sexuality        ) ;
  if (Connection.Exec() && Connection.Next())                  {
    uuid = Connection . Uuid ( 0 )                             ;
  }                                                            ;
  return uuid                                                  ;
}

UUIDs N::PeopleManager::SexualityCrowd(SqlConnection & SC,SUID Sexuality)
{
  QString Q                                             ;
  UUIDs   Uuids                                         ;
  Q = SC.sql.SelectFrom                                 (
        "first" , TABLES(Groups)                        ,
        SC.sql.Where( 4,"second","t1","t2","relation" ) +
        " order by id asc"                            ) ;
  SC . Prepare (Q                                     ) ;
  SC . Bind    ("second"  ,Sexuality                  ) ;
  SC . Bind    ("t1"      ,Types::People              ) ;
  SC . Bind    ("t2"      ,Types::Gender              ) ;
  SC . Bind    ("relation",Groups::Sexuality          ) ;
  if (SC.Exec()) while (SC.Next()) Uuids << SC.Uuid(0)  ;
  return Uuids                                          ;
}

UUIDs N::PeopleManager::HumanSexualities(SqlConnection & Connection)
{
  return Connection.Uuids(TABLES(Gender),"uuid",Connection.OrderByAsc("id") ) ;
}

bool N::PeopleManager::HumanSexualities(SqlConnection & Connection,int language,NAMEs & names)
{
  UUIDs Uuids = HumanSexualities ( Connection )      ;
  SUID  u                                            ;
  names . clear ( )                                  ;
  foreach (u,Uuids)                                  {
    names [ u ] = Name ( Connection , u , language ) ;
  }                                                  ;
  return names . count ( )                           ;
}
