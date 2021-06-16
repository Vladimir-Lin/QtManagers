#include <qtmanagers.h>

N::EquipmentList:: EquipmentList (QWidget * parent,Plan * p)
                 : TreeDock      (          parent,       p)
{
  WidgetClass   ;
  Configure ( ) ;
}

N::EquipmentList::~EquipmentList (void)
{
}

void N::EquipmentList::Configure(void)
{
  Scope = ""                                             ;
  setSuggestion                ( QSize ( 360 , 400 )   ) ;
  setWindowTitle               ( tr("Equipments")      ) ;
  setDragDropMode              ( NoDragDrop            ) ;
  setRootIsDecorated           ( false                 ) ;
  setAlternatingRowColors      ( true                  ) ;
  setHorizontalScrollBarPolicy ( Qt::ScrollBarAsNeeded ) ;
  setVerticalScrollBarPolicy   ( Qt::ScrollBarAsNeeded ) ;
  setAccessibleName            ( "N::EquipmentList"    ) ;
  setColumnCount               ( 1                     ) ;
  NewTreeWidgetItem            ( head                  ) ;
  head -> setText              ( 0 , tr("Equipment")   ) ;
  assignHeaderItems            ( head                  ) ;
  MountClicked                 ( 2                     ) ;
  plan -> setFont              ( this                  ) ;
}

bool N::EquipmentList::FocusIn(void)
{
  DisableAllActions (                          ) ;
  AssignAction      ( Label  , windowTitle ( ) ) ;
  LinkAction        ( Insert , Insert      ( ) ) ;
  LinkAction        ( Font   , setFont     ( ) ) ;
  return true                                    ;
}

bool N::EquipmentList::ItemRunner(int T,ThreadData * d)
{
  switch           ( T ) {
    case 10001           :
      startLoading (   ) ;
      Load         ( d ) ;
      stopLoading  (   ) ;
    return true          ;
  }                      ;
  return false           ;
}

bool N::EquipmentList::Load(ThreadData * d)
{
  WaitClear             (                       ) ;
  EnterSQL              ( SC , plan -> sql      ) ;
    QString Q                                     ;
    Q = SC . sql . SelectFrom                     (
          "uuid,identifier"                       ,
          PlanTable(Equipments)                   ,
          SC . OrderByAsc ( "id" )              ) ;
    SqlLoopNow          ( SC , Q                ) ;
      SUID    u = SC . Uuid       ( 0 )           ;
      QString s = SC . String     ( 1 )           ;
      QString t = QString::number ( u )           ;
      NewTreeWidgetItem ( IT                    ) ;
      IT -> setData     ( 0  , Qt::UserRole , u ) ;
      IT -> setText     ( 0  , s                ) ;
      IT -> setToolTip  ( 0  , t                ) ;
      addTopLevelItem   ( IT                    ) ;
    SqlLoopErr          ( SC , Q                ) ;
    SqlLoopEnd          ( SC , Q                ) ;
  ErrorSQL              ( SC , plan -> sql      ) ;
  LeaveSQL              ( SC , plan -> sql      ) ;
  Alert                 ( Done                  ) ;
  return true                                     ;
}

void N::EquipmentList::run(int T,ThreadData * d)
{
  nDropOut   ( ! IsContinue ( d ) ) ;
  ItemRunner ( T , d              ) ;
}

bool N::EquipmentList::startup(void)
{
  start ( 10001 ) ;
  return true     ;
}

void N::EquipmentList::List(void)
{
  start ( 10001 ) ;
}

void N::EquipmentList::doubleClicked(QTreeWidgetItem * item,int column)
{
  if ( column != 0 ) return          ;
  QLineEdit * line                   ;
  removeOldItem ( )                  ;
  line = setLineEdit                 (
         item                        ,
         column                      ,
         SIGNAL(editingFinished())   ,
         SLOT  (editingFinished()) ) ;
  line->setFocus(Qt::TabFocusReason) ;
}

void N::EquipmentList::Insert(void)
{
  NewTreeWidgetItem ( IT                    ) ;
  IT -> setData     ( 0  , Qt::UserRole , 0 ) ;
  addTopLevelItem   ( IT                    ) ;
  scrollToItem      ( IT                    ) ;
  doubleClicked     ( IT , 0                ) ;
}

void N::EquipmentList::editingFinished(void)
{
  if (IsNull(ItemEditing)) return                                      ;
  QLineEdit * line = Casting  (QLineEdit  ,ItemWidget)                 ;
  if ( IsNull(line) ) return                                           ;
  ItemEditing -> setText    ( ItemColumn , line->text() )              ;
  //////////////////////////////////////////////////////////////////////
  SUID    u = nTreeUuid(ItemEditing,0)                                 ;
  QString n = ItemEditing->text(0)                                     ;
  //////////////////////////////////////////////////////////////////////
  EnterSQL ( SC , plan->sql )                                          ;
    QString Q                                                          ;
    if ( u <= 0 )                                                      {
      u  = SC . Unique ( PlanTable(MajorUuid) , "uuid" , 712249      ) ;
      SC . assureUuid  ( PlanTable(MajorUuid) , u , Types::Equipment ) ;
      Q  = SC . sql . InsertInto                                       (
             PlanTable(Equipments)                                     ,
             2                                                         ,
             "uuid"                                                    ,
             "identifier"                                            ) ;
    } else                                                             {
      Q  = SC . sql . Update                                           (
             PlanTable(Equipments)                                     ,
             SC . sql . Where ( 1 , "uuid" )                           ,
             1                                                         ,
             "identifier"                                            ) ;
    }                                                                  ;
    SC . Prepare ( Q                         )                         ;
    SC . Bind    ( "uuid"       , u          )                         ;
    SC . Bind    ( "identifier" , n.toUtf8() )                         ;
    SC . Exec    (                           )                         ;
    Alert  ( Done           )                                          ;
  ErrorSQL ( SC , plan->sql )                                          ;
    Alert  ( Error          )                                          ;
  LeaveSQL ( SC , plan->sql )                                          ;
  removeOldItem ( )                                                    ;
}

bool N::EquipmentList::Menu(QPoint pos)
{ Q_UNUSED       ( pos  )
  MenuManager mm ( this )                   ;
  QAction   * aa                            ;
  DockingMenu    ( mm   )                   ;
  mm . setFont   ( plan )                   ;
  aa = mm . exec (      )                   ;
  if (IsNull(aa)) return true               ;
  if ( RunDocking ( mm , aa ) ) return true ;
  return true                               ;
}
