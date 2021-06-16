#include <qtmanagers.h>

N::RetrieveSexuality:: RetrieveSexuality ( void )
                     : DataRetriever     (      )
{
}

N::RetrieveSexuality:: RetrieveSexuality ( QObject * owner , Plan * p )
                     : DataRetriever     (           owner ,        p )
{
  GroupTable = PlanTable(Groups) ;
}

N::RetrieveSexuality::~RetrieveSexuality(void)
{
}

int N::RetrieveSexuality::Type(void) const
{
  return 3 ;
}

QValidator * N::RetrieveSexuality::Validator (void)
{
  return NULL ;
}

bool N::RetrieveSexuality::Obtain (
       SqlConnection & SC       ,
       SUID            uuid     ,
       QString       & string   )
{
  QString V                       ;
  if (ObtainSexuality(SC,uuid,V)) {
    string = V                    ;
    return true                   ;
  }                               ;
  return false                    ;
}

bool N::RetrieveSexuality::Obtain (
       SqlConnection    & SC      ,
       SUID               uuid    ,
       QTreeWidgetItem  * item    )
{
  QString V                        ;
  if (ObtainSexuality(SC,uuid,V))  {
    item -> setText ( column , V ) ;
    return true                    ;
  }                                ;
  return false                     ;
}

bool N::RetrieveSexuality::Obtain (
       SqlConnection    & SC      ,
       SUID               uuid    ,
       QTableWidgetItem * item    )
{
  QString V                       ;
  if (ObtainSexuality(SC,uuid,V)) {
    item -> setText ( V )         ;
    return true                   ;
  }                               ;
  return false                    ;
}

bool N::RetrieveSexuality::Obtain (
       SqlConnection    & SC      ,
       SUID               uuid    ,
       QListWidgetItem  * item    )
{
  QString V                       ;
  if (ObtainSexuality(SC,uuid,V)) {
    item -> setToolTip ( V )      ;
    return true                   ;
  }                               ;
  return false                    ;
}

QWidget * N::RetrieveSexuality::Editor  (
            SqlConnection    & SC     ,
            SUID               uuid   ,
            QString          & string )
{ Q_UNUSED ( SC     ) ;
  Q_UNUSED ( uuid   ) ;
  Q_UNUSED ( string ) ;
  return NULL         ;
}

QWidget * N::RetrieveSexuality::Editor  (
            SqlConnection    & SC     ,
            SUID               uuid   ,
            QTreeWidgetItem  * item   )
{ Q_UNUSED ( SC   ) ;
  Q_UNUSED ( uuid ) ;
  Q_UNUSED ( item ) ;
  return NULL       ;
}

QWidget * N::RetrieveSexuality::Editor  (
            SqlConnection    & SC     ,
            SUID               uuid   ,
            QTableWidgetItem * item   )
{ Q_UNUSED ( SC   ) ;
  Q_UNUSED ( uuid ) ;
  Q_UNUSED ( item ) ;
  return NULL       ;
}

QWidget * N::RetrieveSexuality::Editor  (
            SqlConnection    & SC     ,
            SUID               uuid   ,
            QListWidgetItem  * item   )
{ Q_UNUSED ( SC   ) ;
  Q_UNUSED ( uuid ) ;
  Q_UNUSED ( item ) ;
  return NULL       ;
}

bool N::RetrieveSexuality::ObtainList(SqlConnection & SC)
{
  UUIDs   U                   ;
  SUID    u                   ;
  QString n                   ;
  U = SC . Uuids              (
        PlanTable(Gender)     ,
        "uuid"                ,
        SC.OrderByAsc("id") ) ;
  foreach ( u , U )           {
    n = SC . getName          (
          PlanTable(Names)    ,
          "uuid"              ,
          language            ,
          u                 ) ;
    Sexualities [ u ] = n     ;
  }                           ;
  return true                 ;
}

bool N::RetrieveSexuality::ObtainSexuality (
       SqlConnection & SC                  ,
       SUID            uuid                ,
       QString       & name                )
{
  name = ""                                     ;
  if (Sexualities.count()<=0) ObtainList ( SC ) ;
  if (Sexualities.count()<=0) return false      ;
  GroupItems GI ( plan )                        ;
  SUID       xuid = 0                           ;
  QString    n    = ""                          ;
  GI . GroupTable = GroupTable                  ;
  xuid = GI . FindSecond                        (
           SC                                   ,
           uuid                                 ,
           Types  :: People                     ,
           Types  :: Gender                     ,
           Groups :: Sexuality                  ,
           SC.OrderByAsc("position")          ) ;
  if (Sexualities.contains(xuid))               {
    n = Sexualities[xuid]                       ;
    if (Keywords.contains(1))                   {
      QString F = Keywords[1]                   ;
      QString R = QString(F).arg(n)             ;
      n         = R                             ;
    }                                           ;
    if (n.length()>0) name = n                  ;
  }                                             ;
  return ( name.length() > 0 )                  ;
}
