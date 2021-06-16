#include <qtmanagers.h>

N::MemberLists:: MemberLists ( QWidget * parent , Plan * p )
               : TreeDock    (           parent ,        p )
{
  WidgetClass   ;
  Configure ( ) ;
}

N::MemberLists::~MemberLists (void)
{
  emit Leave ( this ) ;
}

void N::MemberLists::Configure(void)
{
  setAccessibleName             ( "N::MemberLists"                ) ;
  setObjectName                 ( "N::MemberLists"                ) ;
  setWindowTitle                ( tr("Members")                   ) ;
  setSuggestion                 ( QSize ( 240 , 600 )             ) ;
  setDragDropMode               ( NoDragDrop                      ) ;
  setRootIsDecorated            ( false                           ) ;
  setAlternatingRowColors       ( true                            ) ;
  setSelectionMode              ( ExtendedSelection               ) ;
  setHorizontalScrollBarPolicy  ( Qt::ScrollBarAsNeeded           ) ;
  setVerticalScrollBarPolicy    ( Qt::ScrollBarAsNeeded           ) ;
  setColumnCount                ( 3                               ) ;
  ///////////////////////////////////////////////////////////////////
  NewTreeWidgetItem             ( head                            ) ;
  head -> setText               ( 0 , ""                          ) ;
  head -> setText               ( 1 , tr("Member"  )              ) ;
  head -> setText               ( 2 , ""                          ) ;
  assignHeaderItems             ( head                            ) ;
  setColumnWidth                ( 2 , 3                           ) ;
  ///////////////////////////////////////////////////////////////////
  MountClicked                  ( 1                               ) ;
  MountClicked                  ( 2                               ) ;
  setLimitValue                 ( HavingMenu   , 1                ) ;
  setLimitValue                 ( EnablingItem , 1                ) ;
  plan -> setFont               ( this                            ) ;
  ///////////////////////////////////////////////////////////////////
  LocalIcons [ "Plus" ] = QIcon ( ":/images/plus.png"             ) ;
  LocalIcons [ "Hide" ] = QIcon ( ":/images/close.png"            ) ;
  LocalIcons [ "Okay" ] = QIcon ( ":/images/yes.png"              ) ;
  LocalIcons [ "Idle" ] = QIcon ( ":/icons/empty.png"             ) ;
  ///////////////////////////////////////////////////////////////////
  LocalMsgs  [ AddMsg    ] = tr ( "Add"                           ) ;
  LocalMsgs  [ DeleteMsg ] = tr ( "Delete"                        ) ;
  ///////////////////////////////////////////////////////////////////
  emit AutoFit                  (                                 ) ;
}

bool N::MemberLists::FocusIn(void)
{
  nKickOut          ( IsNull(plan) , true            ) ;
  DisableAllActions (                                ) ;
  AssignAction      ( Label        , windowTitle ( ) ) ;
  LinkAction        ( Insert       , Add         ( ) ) ;
  LinkAction        ( Delete       , Delete      ( ) ) ;
  return true                                          ;
}

bool N::MemberLists::isEnabling(SUID u)
{
  nKickOut ( topLevelItemCount ( ) <= 0 , false )     ;
  for (int i = 0 ; i < topLevelItemCount ( ) ; i++  ) {
    QTreeWidgetItem * it = topLevelItem ( i         ) ;
    SUID              uu = nTreeUuid    ( it , 1    ) ;
    if ( uu == u ) return ( nTreeInt ( it , 0 ) > 0 ) ;
  }                                                   ;
  return false                                        ;
}

void N::MemberLists::PrepareItem(QTreeWidgetItem * item)
{
  int  c = nTreeInt   ( item , 0                  ) ;
  switch              ( c                         ) {
    case 0                                          :
      item -> setIcon ( 0 , LocalIcons [ "Idle" ] ) ;
    break                                           ;
    case 1                                          :
      item -> setIcon ( 0 , LocalIcons [ "Okay" ] ) ;
    break                                           ;
  }
}

void N::MemberLists::Add(void)
{
  emit AddMember ( ) ;
}

void N::MemberLists::Delete(void)
{
  QTreeWidgetItem * it = currentItem ( ) ;
  nDropOut           ( IsNull ( it )   ) ;
  SUID u = nTreeUuid ( it , 1          ) ;
  emit DeleteMember  ( u               ) ;
  takeItem           ( it              ) ;
}

void N::MemberLists::Delete(SUID u)
{
  QTreeWidgetItem * it = uuidAtItem ( u , 1         ) ;
  nDropOut                          ( IsNull ( it ) ) ;
  takeItem                          ( it            ) ;
}

void N::MemberLists::Append(SUID u,QString n)
{
  NewTreeWidgetItem ( it                   ) ;
  it -> setData     ( 0 , Qt::UserRole , 1 ) ;
  it -> setData     ( 1 , Qt::UserRole , u ) ;
  it -> setText     ( 1 , n                ) ;
  PrepareItem       ( it                   ) ;
  addTopLevelItem   ( it                   ) ;
}

void N::MemberLists::Append(SUID u,QString n,bool e)
{
  NewTreeWidgetItem ( it                           ) ;
  it -> setData     ( 0 , Qt::UserRole , e ? 1 : 0 ) ;
  it -> setData     ( 1 , Qt::UserRole , u         ) ;
  it -> setText     ( 1 , n                        ) ;
  PrepareItem       ( it                           ) ;
  addTopLevelItem   ( it                           ) ;
}

void N::MemberLists::singleClicked(QTreeWidgetItem * item,int column)
{
  nDropOut  ( IsNull ( item )   ) ;
  SUID u = nTreeUuid ( item , 1 ) ;
  emit MemberPressed ( u        ) ;
}

void N::MemberLists::doubleClicked(QTreeWidgetItem * item,int column)
{
  nDropOut  ( IsNull    ( item )               ) ;
  int  c = nTreeInt     ( item , 0             ) ;
  SUID u = nTreeUuid    ( item , 1             ) ;
  switch                ( column               ) {
    case 0                                       :
      if ( LimitValue ( EnablingItem ) > 0 )     {
        c = ( c > 0 ) ? 0 : 1                    ;
        item -> setData ( 0 , Qt::UserRole , c ) ;
        PrepareItem     ( item                 ) ;
      }                                          ;
    break                                        ;
  }                                              ;
  emit MemberClicked    ( u                    ) ;
}

bool N::MemberLists::Menu(QPoint pos)
{
  nKickOut    ( LimitValue(HavingMenu) <= 0 , true )                         ;
  nScopedMenu ( mm                          , this )                         ;
  QAction         * aa = NULL                                                ;
  QTreeWidgetItem * it = itemAt ( pos )                                      ;
  ////////////////////////////////////////////////////////////////////////////
  mm   . add ( 101 , LocalIcons["Plus"] , LocalMsgs  [ AddMsg    ] )         ;
  if ( NotNull(it) )                                                         {
    mm . add ( 102 , LocalIcons["Hide"] , LocalMsgs  [ DeleteMsg ] )         ;
  }                                                                          ;
  ////////////////////////////////////////////////////////////////////////////
  DockingMenu ( mm        )                                                  ;
  ////////////////////////////////////////////////////////////////////////////
  aa = mm . exec ( )                                                         ;
  ////////////////////////////////////////////////////////////////////////////
  if ( IsNull        ( aa      ) ) return true                               ;
  if ( RunDocking    ( mm , aa ) ) return true                               ;
  ////////////////////////////////////////////////////////////////////////////
  switch ( mm [ aa ] )                                                       {
    case 101                                                                 :
      Add    ( )                                                             ;
    break                                                                    ;
    case 102                                                                 :
      Delete ( )                                                             ;
    break                                                                    ;
    default                                                                  :
    break                                                                    ;
  }                                                                          ;
  return true                                                                ;
}
