#include <qtmanagers.h>

typedef struct      {
  SUID   first      ;
  SUID   second     ;
  int    t1         ;
  int    t2         ;
  int    relation   ;
  int    position   ;
  int    reversal   ;
  double membership ;
} nGroupData        ;

N::RetrieveCalendar:: RetrieveCalendar ( void )
                    : DataRetriever    (      )
{
  DateTime NX = DateTime::local()    ;
  StartType   = History::PeopleBirth ;
  EndType     = History::PeopleDie   ;
  TimeZone    = NX . timeshift ( )   ;
}

N::RetrieveCalendar:: RetrieveCalendar ( QObject * owner , Plan * p )
                    : DataRetriever    (           owner ,        p )
{
  DateTime NX = DateTime::local()    ;
  GroupTable  = PlanTable(Groups)    ;
  StartType   = History::PeopleBirth ;
  EndType     = History::PeopleDie   ;
  TimeZone    = NX . timeshift ( )   ;
}

N::RetrieveCalendar::~RetrieveCalendar(void)
{
}

int N::RetrieveCalendar::Type(void) const
{
  return 4 ;
}

QValidator * N::RetrieveCalendar::Validator (void)
{
  return NULL ;
}

void N::RetrieveCalendar::setFirst(SUID first ,int t1,int t2,int relation)
{
  Group G                          ;
  G.first    = first               ;
  G.second   = 0                   ;
  G.t1       = t1                  ;
  G.t2       = t2                  ;
  G.relation = relation            ;
  conf       = G . toByteArray ( ) ;
}

void N::RetrieveCalendar::setSecond (SUID second,int t1,int t2,int relation)
{
  Group G                          ;
  G.first    = 0                   ;
  G.second   = second              ;
  G.t1       = t1                  ;
  G.t2       = t2                  ;
  G.relation = relation            ;
  conf       = G . toByteArray ( ) ;
}

bool N::RetrieveCalendar::Obtain (
       SqlConnection & SC        ,
       SUID            uuid      ,
       QString       & string    )
{
  QString V                      ;
  if (ObtainCalendar(SC,uuid,V)) {
    string = V                   ;
    return true                  ;
  }                              ;
  return false                   ;
}

bool N::RetrieveCalendar::Obtain (
       SqlConnection    & SC     ,
       SUID               uuid   ,
       QTreeWidgetItem  * item   )
{
  QString V                        ;
  if (ObtainCalendar(SC,uuid,V))   {
    item -> setText ( column , V ) ;
    return true                    ;
  }                                ;
  return false                     ;
}

bool N::RetrieveCalendar::Obtain (
       SqlConnection    & SC     ,
       SUID               uuid   ,
       QTableWidgetItem * item   )
{
  QString V                      ;
  if (ObtainCalendar(SC,uuid,V)) {
    item -> setText ( V )        ;
    return true                  ;
  }                              ;
  return false                   ;
}

bool N::RetrieveCalendar::Obtain (
       SqlConnection    & SC     ,
       SUID               uuid   ,
       QListWidgetItem  * item   )
{
  QString V                      ;
  if (ObtainCalendar(SC,uuid,V)) {
    item -> setToolTip ( V )     ;
    return true                  ;
  }                              ;
  return false                   ;
}

QWidget * N::RetrieveCalendar::Editor  (
            SqlConnection    & SC     ,
            SUID               uuid   ,
            QString          & string )
{ Q_UNUSED ( SC     ) ;
  Q_UNUSED ( uuid   ) ;
  Q_UNUSED ( string ) ;
  return NULL         ;
}

QWidget * N::RetrieveCalendar::Editor  (
            SqlConnection    & SC     ,
            SUID               uuid   ,
            QTreeWidgetItem  * item   )
{ Q_UNUSED ( SC   ) ;
  Q_UNUSED ( uuid ) ;
  Q_UNUSED ( item ) ;
  return NULL       ;
}

QWidget * N::RetrieveCalendar::Editor  (
            SqlConnection    & SC     ,
            SUID               uuid   ,
            QTableWidgetItem * item   )
{ Q_UNUSED ( SC   ) ;
  Q_UNUSED ( uuid ) ;
  Q_UNUSED ( item ) ;
  return NULL       ;
}

QWidget * N::RetrieveCalendar::Editor  (
            SqlConnection    & SC     ,
            SUID               uuid   ,
            QListWidgetItem  * item   )
{ Q_UNUSED ( SC   ) ;
  Q_UNUSED ( uuid ) ;
  Q_UNUSED ( item ) ;
  return NULL       ;
}

bool N::RetrieveCalendar::ObtainCalendar (
       SqlConnection & SC                ,
       SUID            uuid              ,
       QString       & string            )
{
  if (conf.size()<=0                 ) return false    ;
  if (conf.size()!=sizeof(nGroupData)) return false    ;
  nGroupData * NGD = (nGroupData *)conf.data()         ;
  if ( NGD -> first    != 0                           &&
       NGD -> t1       != 0                           &&
       NGD -> t2       != 0                           &&
       NGD -> relation != 0                          ) {
    GroupItems     GI ( plan )                         ;
    HistoryManager HM ( plan )                         ;
    SUID           xuid  = 0                           ;
    TUID           tuid  = 0                           ;
    UUIDs          TUIDs                               ;
    TUIDs = GI . Subordination                         (
                    SC                                 ,
                    uuid                               ,
                    NGD -> t1                          ,
                    NGD -> t2                          ,
                    NGD -> relation                    ,
                    SC   . OrderByAsc ( "position" ) ) ;
    if (TUIDs.count()>0)                               {
      xuid = HM . GetHistoryType                       (
               SC                                      ,
               TUIDs                                   ,
               StartType                             ) ;
      if (xuid>0)                                      {
        tuid = HM . GetTime ( SC , xuid )              ;
        if (tuid>0) return setTimeString(tuid,string)  ;
      }                                                ;
    }                                                  ;
  }                                                    ;
  return false                                         ;
}

bool N::RetrieveCalendar::setTimeString (
       TUID      tuid                   ,
       QString & string                 )
{
  QString  t                                           ;
  QString  y                                           ;
  QString  M                                           ;
  QString  D                                           ;
  QString  h                                           ;
  QString  m                                           ;
  QString  s                                           ;
  QString  DayFormat  = Keywords [ 13 ]                ;
  QString  TimeFormat = Keywords [ 14 ]                ;
  DateTime NX                                          ;
  int      Y                                           ;
  NX . Stardate = tuid + TimeZone                      ;
  NX . locate ( )                                      ;
  if (NX.year()<0)                                     {
    t =  Keywords [ 12 ]                               ;
    Y = -NX . year (   )                               ;
  } else                                               {
    t =  Keywords [ 11 ]                               ;
    Y =  NX . year (   )                               ;
  }                                                    ;
  y  = QString::number( Y              )               ;
  M  = QString::number( NX . month ( ) )               ;
  D  = QString::number( NX . mday  ( ) )               ;
  h  = QString("%1").arg(NX.hours  (),2,10,QChar('0')) ;
  m  = QString("%1").arg(NX.minutes(),2,10,QChar('0')) ;
  s  = QString("%1").arg(NX.seconds(),2,10,QChar('0')) ;
  t += " "                                             ;
  t += QString(DayFormat ).arg(y).arg(M).arg(D)        ;
  t += " "                                             ;
  t += QString(TimeFormat).arg(h).arg(m).arg(s)        ;
  string = t                                           ;
  return true                                          ;
}
