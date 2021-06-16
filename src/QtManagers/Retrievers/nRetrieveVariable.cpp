#include <qtmanagers.h>

N::RetrieveVariable:: RetrieveVariable ( void )
                    : DataRetriever    (      )
{
}

N::RetrieveVariable:: RetrieveVariable ( QObject * owner , Plan * p )
                    : DataRetriever    (           owner ,        p )
{
}

N::RetrieveVariable::~RetrieveVariable(void)
{
}

int N::RetrieveVariable::Type(void) const
{
  return 2 ;
}

QValidator * N::RetrieveVariable::Validator (void)
{
  return NULL ;
}

bool N::RetrieveVariable::Obtain (
       SqlConnection & SC       ,
       SUID            uuid     ,
       QString       & string   )
{
  QString V                                  ;
  QString K                                  ;
  if (Keywords.contains(0)) K = Keywords[0]  ;
  if (K.length()<=0) return false            ;
  GroupItems GI ( plan )                     ;
  if ( GI . Variable ( SC , uuid , K , V ) ) {
    if (Keywords.contains(1))                {
      QString F = Keywords[1]                ;
      QString R = QString(F).arg(V)          ;
      V         = R                          ;
    }                                        ;
    string = V                               ;
    return true                              ;
  }                                          ;
  return false                               ;
}

bool N::RetrieveVariable::Obtain (
       SqlConnection    & SC    ,
       SUID               uuid  ,
       QTreeWidgetItem  * item  )
{
  QString V                                  ;
  QString K                                  ;
  if (Keywords.contains(0)) K = Keywords[0]  ;
  if (K.length()<=0) return false            ;
  GroupItems GI ( plan )                     ;
  if ( GI . Variable ( SC , uuid , K , V ) ) {
    if (Keywords.contains(1))                {
      QString F = Keywords[1]                ;
      QString R = QString(F).arg(V)          ;
      V         = R                          ;
    }                                        ;
    item -> setText ( column , V           ) ;
    return true                              ;
  }                                          ;
  return false                               ;
}

bool N::RetrieveVariable::Obtain (
       SqlConnection    & SC    ,
       SUID               uuid  ,
       QTableWidgetItem * item  )
{
  QString V                                  ;
  QString K                                  ;
  if (Keywords.contains(0)) K = Keywords[0]  ;
  if (K.length()<=0) return false            ;
  GroupItems GI ( plan )                     ;
  if ( GI . Variable ( SC , uuid , K , V ) ) {
    if (Keywords.contains(1))                {
      QString F = Keywords[1]                ;
      QString R = QString(F).arg(V)          ;
      V         = R                          ;
    }                                        ;
    item -> setText  ( V                   ) ;
    return true                              ;
  }                                          ;
  return false                               ;
}

bool N::RetrieveVariable::Obtain (
       SqlConnection    & SC    ,
       SUID               uuid  ,
       QListWidgetItem  * item  )
{
  QString V                                  ;
  QString K                                  ;
  if (Keywords.contains(0)) K = Keywords[0]  ;
  if (K.length()<=0) return false            ;
  GroupItems GI ( plan )                     ;
  if ( GI . Variable ( SC , uuid , K , V ) ) {
    if (Keywords.contains(1))                {
      QString F = Keywords[1]                ;
      QString R = QString(F).arg(V)          ;
      V         = R                          ;
    }                                        ;
    item -> setText  ( V                   ) ;
    return true                              ;
  }                                          ;
  return false                               ;
}

QWidget * N::RetrieveVariable::Editor  (
            SqlConnection    & SC     ,
            SUID               uuid   ,
            QString          & string )
{ Q_UNUSED ( SC     ) ;
  Q_UNUSED ( uuid   ) ;
  Q_UNUSED ( string ) ;
  return NULL         ;
}

QWidget * N::RetrieveVariable::Editor  (
            SqlConnection    & SC     ,
            SUID               uuid   ,
            QTreeWidgetItem  * item   )
{ Q_UNUSED ( SC   ) ;
  Q_UNUSED ( uuid ) ;
  Q_UNUSED ( item ) ;
  return NULL       ;
}

QWidget * N::RetrieveVariable::Editor  (
            SqlConnection    & SC     ,
            SUID               uuid   ,
            QTableWidgetItem * item   )
{ Q_UNUSED ( SC   ) ;
  Q_UNUSED ( uuid ) ;
  Q_UNUSED ( item ) ;
  return NULL       ;
}

QWidget * N::RetrieveVariable::Editor  (
            SqlConnection    & SC     ,
            SUID               uuid   ,
            QListWidgetItem  * item   )
{ Q_UNUSED ( SC   ) ;
  Q_UNUSED ( uuid ) ;
  Q_UNUSED ( item ) ;
  return NULL       ;
}
