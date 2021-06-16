#include <qtmanagers.h>

N::ItemNameEditor * N::EditLanguages(QWidget * parent,Plan & plan)
{
  ItemNameEditor * INE = new ItemNameEditor(parent,&plan) ;
  UUIDs       Uuids = plan.languages.LanguageUuids()      ;
  QStringList Names = plan.languages.LanguageNames()      ;
  INE->startup(Names,Uuids,plan.languages.Supports)       ;
  return INE                                              ;
}

N::TreeWidgetItems N :: List                        (
                        Languages   & languages     ,
                        QTreeWidget * Tree          ,
                        int           column        ,
                        bool          processEvents )
{
  TreeWidgetItems Items                         ;
  Tree -> blockSignals ( true  )                ;
  Tree -> clear        (       )                ;
  ForLanguage(LID,languages.languages)          ;
    SUID Uuid = languages.Uuids[LID]            ;
    if (languages.Names.contains(Uuid))         {
      NewTreeWidgetItem(it)                     ;
      it->setText(column,languages.Names[Uuid]) ;
      it->setData(column,Qt::UserRole,LID)      ;
      Tree->addTopLevelItem(it)                 ;
      Items << it                               ;
      if (processEvents)                        {
        qApp->processEvents()                   ;
      }                                         ;
    }                                           ;
  EndLanguage(LID,languages.languages)          ;
  Tree -> blockSignals ( false )                ;
  return Items                                  ;
}

NAMEs N         :: LoadNames (
        PurePlan & plan      ,
        UUIDs    & Uuids     ,
        int        language  )
{
  NAMEs Names                                           ;
  QString       CN = QtUUID::createUuidString ( )       ;
  SqlConnection LC (plan.sql)                           ;
  if (LC.open("LoadNames",CN))                          {
    QString Q                                           ;
    QString name                                        ;
    SUID    uuid                                        ;
    foreach (uuid,Uuids)                                {
      name = ""                                         ;
      Q = LC . sql . SelectFrom                         (
            "name"                                      ,
            plan.Tables[Tables::Names]                  ,
          QString ( "where uuid = %1 and language = %2" )
          .arg    ( uuid                                )
          .arg    ( language                        ) ) ;
      IfSqlQuery(LC,Q) name = LC.String(0)              ;
      Names[uuid] = name                                ;
    }                                                   ;
    LC.close()                                          ;
  }                                                     ;
  LC.remove()                                           ;
  return Names                                          ;
}
