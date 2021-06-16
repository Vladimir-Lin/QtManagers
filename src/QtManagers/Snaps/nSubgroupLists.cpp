#include <qtmanagers.h>

N::SubgroupLists:: SubgroupLists (QWidget * parent,Plan * p)
                 : GroupView     (          parent,       p)
                 , Relation      (Groups::Subordination , 1)
                 , OrderBy       (Qt::AscendingOrder       )
                 , OrderItem     ("position"               )
{
  WidgetClass   ;
  Configure ( ) ;
}

N::SubgroupLists::~SubgroupLists(void)
{
}

QSize N::SubgroupLists::sizeHint (void) const
{
  if ( plan  -> Booleans [ "Desktop" ] ) {
    return QSize (  640 ,  480 )         ;
  }                                      ;
  if ( plan  -> Booleans [ "Pad"     ] ) {
    return QSize ( 1024 ,  720 )         ;
  }                                      ;
  if ( plan  -> Booleans [ "Phone"   ] ) {
    return QSize (  320 ,  480 )         ;
  }                                      ;
  return   QSize (  640 ,  480 )         ;
}

void N::SubgroupLists::Configure(void)
{
  icon = QIcon(":/images/box.png") ;
}

int N::SubgroupLists::EachType(void)
{
  return Types::Subgroup ;
}

int N::SubgroupLists::Counting(SqlConnection & SC,SUID u)
{
  QString Q                                                        ;
  QString W                                                        ;
  int     t = 0                                                    ;
  W = QString ( "where first = %1 and t1 = %2 and relation = %3 ;" )
      . arg   ( u                                                  )
      . arg   ( Types  :: Subgroup                                 )
      . arg   ( Groups :: Subordination                          ) ;
  Q = SC . sql . SelectFrom ( "count(*)" , PlanTable(Groups) , W ) ;
  if ( SC . Fetch ( Q ) ) t = SC . Int ( 0 )                       ;
  return t                                                         ;
}

QString N::SubgroupLists::ToolTipTotoal(int total)
{
  return tr("%1 objects in this subgroup").arg(total) ;
}

UUIDs N::SubgroupLists::GroupUuids(SqlConnection & SC)
{
  QString Q                                              ;
  QString W                                              ;
  QString O                                              ;
  QString E                                              ;
  UUIDs   U                                              ;
  E = ( OrderBy == Qt::AscendingOrder ) ? "asc" : "desc" ;
  O = QString("order by %1 %2").arg(OrderItem).arg(E)    ;
  U = GroupItems :: Subordination                        (
        SC                                               ,
        ObjectUuid ( )                                   ,
        ObjectType ( )                                   ,
        Types::Subgroup                                  ,
        relation                                         ,
        O                                              ) ;
  return U                                               ;
}

SUID N::SubgroupLists::AppendGroup(SqlConnection & SC,QString name)
{
  SUID u                                                              ;
  u = SC . Unique ( PlanTable(MajorUuid) , "uuid" , 7236            ) ;
  SC . assureUuid ( PlanTable(MajorUuid) , u      , Types::Subgroup ) ;
  SC . insertUuid ( PlanTable(Subgroups) , u      , "uuid"          ) ;
  SC . assureName ( PlanTable(Names    ) , u   , vLanguageId , name ) ;
  if (ObjectUuid()<=0) return u                                       ;
  if (ObjectType()<=0) return u                                       ;
  UUIDs U                                                             ;
  U << u                                                              ;
  GroupItems :: Join                                                  (
    SC                                                                ,
    ObjectUuid()                                                      ,
    ObjectType()                                                      ,
    Types::Subgroup                                                   ,
    relation                                                          ,
    0                                                                 ,
    U                                                               ) ;
  return u                                                            ;
}

bool N::SubgroupLists::JoinPictures(SUID u,const UUIDs & Uuids)
{
  QListWidgetItem * item  = uuidAt(u)                                             ;
  nKickOut ( IsNull(item) , false )                                               ;
  /////////////////////////////////////////////////////////////////////////////////
  dropAction = true                                                               ;
  /////////////////////////////////////////////////////////////////////////////////
  QString title = item->text()                                                    ;
  QString jmsg                                                                    ;
  jmsg = tr("%1 pictures join subgroup %2").arg(Uuids.count()).arg(title)         ;
  plan -> showMessage   ( jmsg )                                                  ;
  plan -> processEvents (      )                                                  ;
  /////////////////////////////////////////////////////////////////////////////////
  QString fsg                                                                     ;
  UUIDs   U   = Uuids                                                             ;
  VarArgs V                                                                       ;
  Group   G                                                                       ;
  fsg = tr("%1 pictures join subgroup %2 finished").arg(Uuids.count()).arg(title) ;
  /////////////////////////////////////////////////////////////////////////////////
  G  . first      = u                                                             ;
  G  . second     = 0                                                             ;
  G  . t1         = Types  :: Subgroup                                            ;
  G  . t2         = Types  :: Picture                                             ;
  G  . relation   = Groups :: Subordination                                       ;
  G  . position   = 0                                                             ;
  G  . membership = 1.0                                                           ;
  V << 10011                                                                      ;
  V << fsg                                                                        ;
  G  . addParaments ( V     )                                                     ;
  V << U . count    (       )                                                     ;
  toVariants        ( U , V )                                                     ;
  addSequence       ( V     )                                                     ;
  /////////////////////////////////////////////////////////////////////////////////
  dropAction = false                                                              ;
  emit TriggerCommand ( )                                                         ;
  /////////////////////////////////////////////////////////////////////////////////
  return true                                                                     ;
}
