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

N::RetrieveMembers:: RetrieveMembers ( void )
                   : DataRetriever   (      )
{
}

N::RetrieveMembers:: RetrieveMembers ( QObject * owner , Plan * p )
                   : DataRetriever   (           owner ,        p )
{
  GroupTable = PlanTable ( Groups ) ;
}

N::RetrieveMembers::~RetrieveMembers(void)
{
}

int N::RetrieveMembers::Type(void) const
{
  return 1 ;
}

QValidator * N::RetrieveMembers::Validator (void)
{
  return NULL ;
}

void N::RetrieveMembers::setFirst(SUID first ,int t1,int t2,int relation)
{
  Group G                          ;
  G.first    = first               ;
  G.second   = 0                   ;
  G.t1       = t1                  ;
  G.t2       = t2                  ;
  G.relation = relation            ;
  conf       = G . toByteArray ( ) ;
}

void N::RetrieveMembers::setSecond (SUID second,int t1,int t2,int relation)
{
  Group G                          ;
  G.first    = 0                   ;
  G.second   = second              ;
  G.t1       = t1                  ;
  G.t2       = t2                  ;
  G.relation = relation            ;
  conf       = G . toByteArray ( ) ;
}

bool N::RetrieveMembers::Obtain (
       SqlConnection & SC       ,
       SUID            uuid     ,
       QString       & string   )
{
  int total = Counting(SC,uuid)        ;
  QString V                            ;
  if (Keywords.contains(1))            {
    QString F = Keywords[1]            ;
    QString R = QString(F).arg(total)  ;
    V         = R                      ;
  } else                               {
    V         = QString::number(total) ;
  }                                    ;
  string = V                           ;
  return true                          ;
}

bool N::RetrieveMembers::Obtain (
       SqlConnection    & SC    ,
       SUID               uuid  ,
       QTreeWidgetItem  * item  )
{
  int total = Counting(SC,uuid)          ;
  if (NotNull(item))                     {
    QString V                            ;
    if (Keywords.contains(1))            {
      QString F = Keywords[1]            ;
      QString R = QString(F).arg(total)  ;
      V         = R                      ;
    } else                               {
      V         = QString::number(total) ;
    }                                    ;
    item -> setText ( column , V )       ;
  }                                      ;
  return true                            ;
}

bool N::RetrieveMembers::Obtain (
       SqlConnection    & SC    ,
       SUID               uuid  ,
       QTableWidgetItem * item  )
{
  int total = Counting(SC,uuid)          ;
  if (NotNull(item))                     {
    QString V                            ;
    if (Keywords.contains(1))            {
      QString F = Keywords[1]            ;
      QString R = QString(F).arg(total)  ;
      V         = R                      ;
    } else                               {
      V         = QString::number(total) ;
    }                                    ;
    item -> setText ( V )                ;
  }                                      ;
  return true                            ;
}

bool N::RetrieveMembers::Obtain (
       SqlConnection    & SC    ,
       SUID               uuid  ,
       QListWidgetItem  * item  )
{
  int total = Counting(SC,uuid)          ;
  if (NotNull(item))                     {
    QString V                            ;
    if (Keywords.contains(1))            {
      QString F = Keywords[1]            ;
      QString R = QString(F).arg(total)  ;
      V         = R                      ;
    } else                               {
      V         = QString::number(total) ;
    }                                    ;
    item -> setToolTip ( V )             ;
  }                                      ;
  return true                            ;
}

QWidget * N::RetrieveMembers::Editor  (
            SqlConnection    & SC     ,
            SUID               uuid   ,
            QString          & string )
{ Q_UNUSED ( SC     ) ;
  Q_UNUSED ( uuid   ) ;
  Q_UNUSED ( string ) ;
  return NULL         ;
}

QWidget * N::RetrieveMembers::Editor  (
            SqlConnection    & SC     ,
            SUID               uuid   ,
            QTreeWidgetItem  * item   )
{ Q_UNUSED ( SC   ) ;
  Q_UNUSED ( uuid ) ;
  Q_UNUSED ( item ) ;
  return NULL       ;
}

QWidget * N::RetrieveMembers::Editor  (
            SqlConnection    & SC     ,
            SUID               uuid   ,
            QTableWidgetItem * item   )
{ Q_UNUSED ( SC   ) ;
  Q_UNUSED ( uuid ) ;
  Q_UNUSED ( item ) ;
  return NULL       ;
}

QWidget * N::RetrieveMembers::Editor  (
            SqlConnection    & SC     ,
            SUID               uuid   ,
            QListWidgetItem  * item   )
{ Q_UNUSED ( SC   ) ;
  Q_UNUSED ( uuid ) ;
  Q_UNUSED ( item ) ;
  return NULL       ;
}

int N::RetrieveMembers::Counting(SqlConnection & SC,SUID uuid)
{
  if (conf.size()<=0                 ) return 0                               ;
  if (conf.size()!=sizeof(nGroupData)) return 0                               ;
  int total = 0                                                               ;
  nGroupData * NGD = (nGroupData *)conf.data()                                ;
  GroupItems   GI ( plan )                                                    ;
  QString      Q                                                              ;
  if (NGD->first  != 0 && NGD->t1 != 0 && NGD->t2 != 0 && NGD->relation != 0) {
    Q = SC . sql . SelectFrom                                                 (
          "count(*)"                                                          ,
          GroupTable                                                          ,
          GI . FirstItem (uuid,NGD->t1,NGD->t2,NGD->relation)               ) ;
    if (SC.Fetch(Q)) total = SC.Int(0)                                        ;
  } else
  if (NGD->second != 0 && NGD->t1 != 0 && NGD->t2 != 0 && NGD->relation != 0) {
    Q = SC . sql . SelectFrom                                                 (
          "count(*)"                                                          ,
          GroupTable                                                          ,
          GI . SecondItem(uuid,NGD->t1,NGD->t2,NGD->relation)               ) ;
    if (SC.Fetch(Q)) total = SC.Int(0)                                        ;
  }                                                                           ;
  return total                                                                ;
}
