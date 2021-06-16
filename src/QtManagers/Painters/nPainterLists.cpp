#include <qtmanagers.h>

N::PainterLists:: PainterLists ( QWidget * parent , Plan * p )
                : TreeDock     (           parent ,        p )
{
  WidgetClass   ;
  Configure ( ) ;
}

N::PainterLists::~PainterLists(void)
{
}

void N::PainterLists::Configure(void)
{
  NewTreeWidgetItem            ( head                       ) ;
  head -> setText              ( 0 , tr("Name"    )         ) ;
  head -> setText              ( 1 , tr("Type"    )         ) ;
  head -> setText              ( 2 , tr("Register")         ) ;
  /////////////////////////////////////////////////////////////
  setWindowTitle               ( tr("Painter objects")      ) ;
  setDragDropMode              ( DragOnly                   ) ;
  setRootIsDecorated           ( false                      ) ;
  setAlternatingRowColors      ( true                       ) ;
  setSelectionMode             ( SingleSelection            ) ;
  setColumnCount               ( 3                          ) ;
  setHorizontalScrollBarPolicy ( Qt::ScrollBarAsNeeded      ) ;
  setVerticalScrollBarPolicy   ( Qt::ScrollBarAsNeeded      ) ;
  assignHeaderItems            ( head                       ) ;
  /////////////////////////////////////////////////////////////
  MountClicked                 ( 1                          ) ;
  MountClicked                 ( 2                          ) ;
  plan -> setFont              ( this                       ) ;
}

bool N::PainterLists::FocusIn(void)
{
  nKickOut          ( IsNull(plan) , false           ) ;
  DisableAllActions (                                ) ;
  AssignAction      ( Label        , windowTitle ( ) ) ;
  LinkAction        ( SelectNone   , SelectNone  ( ) ) ;
  LinkAction        ( SelectAll    , SelectAll   ( ) ) ;
  return true                                          ;
}

bool N::PainterLists::ItemRunner(int T,ThreadData * d)
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

void N::PainterLists::run(int T,ThreadData * d)
{
  nDropOut   ( ! IsContinue ( d ) ) ;
  ItemRunner ( T , d              ) ;
}

void N::PainterLists::Load(ThreadData * d)
{
  nDropOut          ( ! IsContinue ( d ) )               ;
  WaitClear         (                    )               ;
  Bustle            (                    )               ;
  SqlConnection  SC ( plan -> sql        )               ;
  if ( SC . open    ( FunctionString     )             ) {
    QString Q                                            ;
    QString name                                         ;
    SUID    uuid                                         ;
    UUIDs   Uuids                                        ;
    Uuids = SC . Uuids                                   (
      PlanTable(Painters)                                ,
      "uuid"                                             ,
      SC.OrderByAsc("id")                              ) ;
    foreach (uuid,Uuids)                                 {
      NewTreeWidgetItem ( IT )                           ;
      name = SC . getName                                (
               PlanTable(Names)                          ,
               "uuid"                                    ,
               vLanguageId                               ,
               uuid                                    ) ;
      Q = SC . sql . SelectFrom                          (
            "type,name"                                  ,
            PlanTable(Painters)                          ,
            QString("where uuid = %1").arg(uuid)       ) ;
      if ( SC . Fetch ( Q ) )                            {
        int     type     = SC . Int    ( 0 )             ;
        QString Register = SC . String ( 1 )             ;
        IT -> setData ( 1 , Qt::UserRole , type        ) ;
        IT -> setText ( 2 , Register                   ) ;
        switch ( type )                                  {
          case Graphics::Font                            :
            IT -> setText ( 1,tr("Font"    )           ) ;
          break                                          ;
          case Graphics::Gradient                        :
            IT -> setText ( 1,tr("Gradient")           ) ;
          break                                          ;
          case Graphics::Pen                             :
            IT -> setText ( 1,tr("Pen"     )           ) ;
          break                                          ;
          case Graphics::Brush                           :
            IT -> setText ( 1,tr("Brush"   )           ) ;
          break                                          ;
        }                                                ;
      }                                                  ;
      IT -> setData   ( 0 , Qt::UserRole , uuid        ) ;
      IT -> setText   ( 0 , name                       ) ;
      addTopLevelItem ( IT                             ) ;
    }                                                    ;
    SC . close        (                                ) ;
  }                                                      ;
  SC   . remove       (                                ) ;
  ////////////////////////////////////////////////////////
  Vacancy             (                                ) ;
  alert               ( "Done" , tr("Painter objects") ) ;
}

bool N::PainterLists::startup(void)
{
  start ( 10001 ) ;
  return true     ;
}

void N::PainterLists::List(void)
{
  startup ( ) ;
}

void N::PainterLists::singleClicked(QTreeWidgetItem * item,int column)
{
  if ( ( ItemEditing == item ) && ( ItemColumn == column ) ) return ;
  RemoveOldItem ( )                                                 ;
}

void N::PainterLists::doubleClicked(QTreeWidgetItem * item,int column)
{
  RemoveOldItem ( )                      ;
  if ( column != 2 ) return              ;
  QLineEdit * le                         ;
  le = setLineEdit                       (
        item                             ,
        column                           ,
        SIGNAL ( editingFinished ( ) )   ,
        SLOT   ( NameFinished    ( ) ) ) ;
  ItemEditing = item                     ;
  ItemWidget  = le                       ;
  ItemColumn  = column                   ;
}

void N::PainterLists::RemoveOldItem(void)
{
  if (IsNull(ItemEditing)) return               ;
  removeItemWidget ( ItemEditing , ItemColumn ) ;
  ItemEditing = NULL                            ;
  ItemWidget  = NULL                            ;
  ItemColumn  = -1                              ;
}

void N::PainterLists::NameFinished(void)
{
  if (IsNull(ItemEditing)) return                        ;
  SUID uuid = nTreeUuid(ItemEditing,0)                   ;
  QLineEdit * le = qobject_cast<QLineEdit *>(ItemWidget) ;
  if ( NotNull ( le ) )                                  {
    QString name = le -> text ( )                        ;
    SqlConnection SC ( plan -> sql )                     ;
    if ( SC . open ( FunctionString ) )                  {
      QString Q                                          ;
      Q = SC . sql . Update                              (
            PlanTable(Painters)                          ,
            QString("where uuid = :UUID"),1,"name"     ) ;
      SC.Prepare  ( Q                                  ) ;
      SC.Bind     ( "uuid" , uuid                      ) ;
      SC.Bind     ( "name" , name . toUtf8 ( )         ) ;
      SC.Exec     (                                    ) ;
      ItemEditing -> setText ( ItemColumn , name       ) ;
      SC . close  (                                    ) ;
    }                                                    ;
    SC   . remove (                                    ) ;
  }                                                      ;
  RemoveOldItem   (                                    ) ;
}

bool N::PainterLists::Menu(QPoint pos)
{
  MenuManager mm (this)                     ;
  QAction   * aa                            ;
  mm . add ( 102 , tr("Refresh") )          ;
  DockingMenu     ( mm      )               ;
  mm . setFont    ( plan    )               ;
  aa = mm . exec  (         )               ;
  if ( IsNull     (      aa ) ) return true ;
  if ( RunDocking ( mm , aa ) ) return true ;
  switch ( mm [ aa ] )                      {
    case 102                                :
      startup ( )                           ;
    break                                   ;
    default                                 :
    break                                   ;
  }                                         ;
  return true                               ;
}
