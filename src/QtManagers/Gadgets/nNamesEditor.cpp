#include <qtmanagers.h>

N::NamesEditor:: NamesEditor (QWidget * parent,Plan * p)
               : ItemEditor  (          parent,       p)
{
  WidgetClass   ;
}

N::NamesEditor::~NamesEditor (void)
{
}

void N::NamesEditor::LoadNames(SqlConnection & connection,UUIDs & Uuids)
{
  int  index = 0                                  ;
  SUID uuid  = 0                                  ;
  GroupItems GI(plan)                            ;
  foreach (uuid,Uuids)                            {
    QStringList Names = GI.Names(connection,uuid) ;
    if (Names.count()>0) addItem(Names[0],uuid,0) ;
    index++                                       ;
    if (nMod(index,100)) plan->processEvents()    ;
  }                                               ;
}

bool N::NamesEditor::AppendName(SqlConnection & connection,SUID uuid,QString name)
{
  QString Q                                            ;
  GroupItems GI(plan)                                  ;
  SUID nuid = connection.Unique                        (
                PlanTable(MajorUuid) , "uuid"        ) ;
  connection.assureUuid                                (
    PlanTable(MajorUuid) , nuid,Types::Name  )         ;
  Q = connection.sql.ReplaceInto                       (
        PlanTable(Names) , 3,"uuid","language","name") ;
  connection . insertName (Q,nuid,vLanguageId,name   ) ;
  GI.InsertNameMap(connection,uuid,nuid)               ;
  return true                                          ;
}

bool N::NamesEditor::Menu(QPoint pos)
{ Q_UNUSED ( pos ) ;
  return true      ;
}
