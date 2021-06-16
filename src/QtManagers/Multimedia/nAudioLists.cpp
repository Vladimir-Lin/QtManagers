#include <qtmanagers.h>

N::AudioLists:: AudioLists ( QWidget * w , Plan * p                  )
              : TreeDock   (           w ,        p                  )
              , Ownership  ( 0 , Types::None , Groups::Subordination )
{
  WidgetClass   ;
  Configure ( ) ;
}

N::AudioLists::~AudioLists(void)
{
}

QMimeData * N::AudioLists::dragMime (void)
{
  QTreeWidgetItem * item = currentItem ()        ;
  UUIDs Uuids                                    ;
  SUID  uuid  = 0                                ;
  if (IsNull(item)) return NULL                  ;
  for (int i=0;i<topLevelItemCount();i++)        {
    QTreeWidgetItem * it = topLevelItem(i)       ;
    SUID auid = nTreeUuid(it,0)                  ;
    if (it->isSelected()) Uuids << auid          ;
  }                                              ;
  if (Uuids.count()<=0) return NULL              ;
  ////////////////////////////////////////////////
  QMimeData * mime = new QMimeData()             ;
  if (Uuids.count()==1)                          {
    uuid = nTreeUuid ( item , 0 )                ;
    setMime ( mime , "audio/uuid"  , uuid  )     ;
  } else                                         {
    setMime ( mime , "audio/uuids" , Uuids )     ;
  }                                              ;
  return mime                                    ;
}

bool N::AudioLists::hasItem(void)
{
  QTreeWidgetItem * item = currentItem () ;
  return NotNull ( item )                 ;
}

bool N::AudioLists::startDrag(QMouseEvent * event)
{
  QTreeWidgetItem * atItem = itemAt(event->pos())            ;
  if (IsNull(atItem)) return false                           ;
  if (!IsMask(event->buttons(),Qt::LeftButton)) return false ;
  dragPoint = event->pos()                                   ;
  if (!atItem->isSelected()) return false                    ;
  if (!PassDragDrop) return true                             ;
  return true                                                ;
}

bool N::AudioLists::fetchDrag(QMouseEvent * event)
{
  if (!IsMask(event->buttons(),Qt::LeftButton)) return false   ;
  QPoint pos = event->pos()                                    ;
  pos -= dragPoint                                             ;
  return ( pos.manhattanLength() > qApp->startDragDistance() ) ;
}

void N::AudioLists::dragDone(Qt::DropAction dropIt,QMimeData * mime)
{
}

bool N::AudioLists::finishDrag(QMouseEvent * event)
{
  return true ;
}

void N::AudioLists::Configure(void)
{
  NewTreeWidgetItem            ( head                  ) ;
  head -> setText              ( 0 , tr("Audio")       ) ;
  ////////////////////////////////////////////////////////
  setWindowTitle               ( tr("Audio lists")     ) ;
  setAccessibleName            ( "N::AudioLists"       ) ;
  setObjectName                ( "N::AudioLists"       ) ;
  setDragDropMode              ( DragOnly              ) ;
  setRootIsDecorated           ( false                 ) ;
  setAlternatingRowColors      ( true                  ) ;
  setSelectionMode             ( ExtendedSelection     ) ;
  setColumnCount               ( 1                     ) ;
  setHorizontalScrollBarPolicy ( Qt::ScrollBarAsNeeded ) ;
  setVerticalScrollBarPolicy   ( Qt::ScrollBarAsNeeded ) ;
  assignHeaderItems            ( head                  ) ;
  MountClicked                 ( 2                     ) ;
  plan -> setFont              ( this                  ) ;
  ////////////////////////////////////////////////////////
  LimitValues     [   0 ] =  0                           ;
  LimitValues     [   1 ] = 42                           ;
  LimitValues     [   2 ] =  0                           ;
  ////////////////////////////////////////////////////////
  Functionalities [ 101 ] = false                        ;
  PassDragDrop            = false                        ;
  dockingOrientation      = Qt::Vertical                 ;
  dockingPlace            = Qt::RightDockWidgetArea      ;
  dockingPlaces           = Qt::LeftDockWidgetArea       |
                            Qt::RightDockWidgetArea      ;
}

bool N::AudioLists::FocusIn(void)
{
  nKickOut          ( IsNull ( plan ) , false               ) ;
  DisableAllActions (                                       ) ;
  AssignAction      ( Label           , windowTitle     ( ) ) ;
  LinkAction        ( Refresh         , startup         ( ) ) ;
  LinkAction        ( Copy            , CopyToClipboard ( ) ) ;
  return true                                                 ;
}

void N::AudioLists::AppendChannel(QString name,SUID uu)
{
  VideoManager VM(plan)                    ;
  QString      filename                    ;
  filename = VM . Location ( *plan , uu )  ;
  if ( filename . length ( ) <= 0 ) return ;
  emit AddChannel ( name , uu , filename ) ;
}

void N::AudioLists::doubleClicked(QTreeWidgetItem * item,int column)
{
  if ( ! dbClicked            ) return    ;
  if ( ! Functionalities[101] ) return    ;
  if ( IsNull(item)           ) return    ;
  SUID    uu = nTreeUuid    ( item , 0  ) ;
  QString nn = item -> text ( 0         ) ;
  if (uu>0) AppendChannel   ( nn , uu   ) ;
}

UUIDs N::AudioLists::AudioUuids(SqlConnection & SC)
{
  int        total   = 0                                      ;
  int        startId = LimitValues [ 0 ]                      ;
  int        amount  = LimitValues [ 1 ]                      ;
  QString    Q                                                ;
  UUIDs      U                                                ;
  GroupItems GI ( plan )                                      ;
  if ( isStandby ( ) )                                        {
    Q = SC . sql . SelectFrom                                 (
          "count(*)"                                          ,
          PlanTable(Audios)                                   ,
          ";"                                               ) ;
    if ( SC . Fetch ( Q ) ) total = SC . Int ( 0 )            ;
    LimitValues [ 2 ] = total                                 ;
    if ( amount <= 0 ) amount = total                         ;
    Q = QString ( "%1 %2"                                     )
          . arg ( SC . OrderByAsc ( "id" )                    )
          . arg ( SC . sql . Limit ( startId , amount )     ) ;
    U = SC . Uuids ( PlanTable(Audios) , "uuid" , Q )         ;
  } else                                                      {
    GI . AutoMap    = true                                    ;
    GI . GroupTable = GI . LookTable                          (
                        ObjectType ( )                        ,
                        Types::Audio                          ,
                        Connexion ( )                       ) ;
    total           = GI . Count                              (
                        SC                                    ,
                        ObjectUuid ( )                        ,
                        ObjectType ( )                        ,
                        Types::Audio                          ,
                        Connexion  ( )                      ) ;
    LimitValues [ 2 ] = total                                 ;
    if ( amount <= 0 ) amount = total                         ;
    U = GI . Subordination                                    (
          SC                                                  ,
          ObjectUuid ( )                                      ,
          ObjectType ( )                                      ,
          Types::Audio                                        ,
          Connexion  ( )                                      ,
          QString ( "%1 %2"                                   )
          . arg   ( SC . OrderByAsc  ( "position" )           )
          . arg   ( SC . sql . Limit ( startId , amount ) ) ) ;
  }                                                           ;
  return U                                                    ;
}

bool N::AudioLists::ItemRunner(int T,ThreadData * d)
{
  switch           ( T ) {
    case 10001           :
      startLoading (   ) ;
      List         (   ) ;
      stopLoading  (   ) ;
    return true          ;
  }                      ;
  return false           ;
}

void N::AudioLists::run(int T,ThreadData * d)
{
  nDropOut   ( ! IsContinue ( d ) ) ;
  ItemRunner ( T , d              ) ;
}

void N::AudioLists::run(void)
{
  startLoading ( ) ;
  List         ( ) ;
  stopLoading  ( ) ;
}

bool N::AudioLists::startup(void)
{
  start ( 10001 ) ;
  return true     ;
}

void N::AudioLists::List(void)
{
  WaitClear               (                       ) ;
  EnterSQL                ( SC , plan -> sql      ) ;
    QString name                                    ;
    SUID    uu                                      ;
    UUIDs   U                                       ;
    U = AudioUuids        ( SC                    ) ;
    foreach               ( uu , U                ) {
      NewTreeWidgetItem   ( IT                    ) ;
      name = SC . getName ( PlanTable(Names)        ,
                            "uuid"                  ,
                            vLanguageId             ,
                            uu                    ) ;
      IT -> setData       ( 0 , Qt::UserRole , uu ) ;
      IT -> setText       ( 0 , name              ) ;
      addTopLevelItem     ( IT                    ) ;
    }                                               ;
  ErrorSQL                ( SC , plan -> sql      ) ;
  LeaveSQL                ( SC , plan -> sql      ) ;
  ///////////////////////////////////////////////////
  int     t = topLevelItemCount ( )                 ;
  QString m                                         ;
  m = tr("%1 audios") . arg ( t )                   ;
  emit assignToolTip      ( m                     ) ;
  Alert                   ( Done                  ) ;
}

bool N::AudioLists::Menu(QPoint pos)
{
  if ( Functionalities[101] ) return true                           ;
  nScopedMenu ( mm , this )                                         ;
  ///////////////////////////////////////////////////////////////////
  QTreeWidgetItem * IT       = currentItem ( )                      ;
  QAction         * aa       = NULL                                 ;
  QMenu           * ms       = NULL                                 ;
  QMenu           * ma       = NULL                                 ;
  QMenu           * mp       = NULL                                 ;
  QMenu           * mpu      = NULL                                 ;
  SpinBox         * spb      = NULL                                 ;
  SpinBox         * spp      = NULL                                 ;
  int               StartId  = 0                                    ;
  int               PageSize = 0                                    ;
  ///////////////////////////////////////////////////////////////////
  if ( LimitValues [ 2 ] > 0 )                                      {
    mpu = PageMenu ( mm                                             ,
                     tr("Total %1 audio files")                     ,
                     StartId                                        ,
                     PageSize                                     ) ;
  }                                                                 ;
  ///////////////////////////////////////////////////////////////////
  if (NotNull(IT  ))                                                {
    mm.add(101,tr("Edit"          ))                                ;
    mm.add(201,tr("Add as channel"))                                ;
  }                                                                 ;
  mm.add(102,tr("Refresh"))                                         ;
  ///////////////////////////////////////////////////////////////////
  mm . addSeparator (                                  )            ;
  ma = mm . addMenu ( tr("Adjustments")                )            ;
  mp = mm . addMenu ( tr("Selection"  )                )            ;
  ms = mm . addMenu ( tr("Sorting"    )                )            ;
  AdjustMenu        ( mm , ma                          )            ;
  SelectionsMenu    ( mm , mp                          )            ;
  SortingMenu       ( mm , ms                          )            ;
  DockingMenu       ( mm                               )            ;
  ///////////////////////////////////////////////////////////////////
  mm . setFont      ( plan                             )            ;
  aa = mm . exec    (                                  )            ;
  if                ( IsNull ( aa )                    )            {
    PageChanged     ( StartId , PageSize               )            ;
    return true                                                     ;
  }                                                                 ;
  ///////////////////////////////////////////////////////////////////
  if ( RunSorting    ( mm , aa ) ) return true                      ;
  if ( RunAdjustment ( mm , aa ) ) return true                      ;
  if ( RunSelections ( mm , aa ) ) return true                      ;
  if ( RunDocking    ( mm , aa ) ) return true                      ;
  ///////////////////////////////////////////////////////////////////
  int Id = mm [ aa ]                                                ;
  ///////////////////////////////////////////////////////////////////
  switch ( Id )                                                     {
    case 101                                                        :
      if (NotNull(IT))                                              {
        SUID uu = nTreeUuid ( IT   , 0  )                           ;
        if (uu>0) emit Edit ( this , uu )                           ;
      }                                                             ;
    return true                                                     ;
    case 102                                                        :
      List ( )                                                      ;
    return true                                                     ;
    case 201                                                        :
      if (NotNull(IT))                                              {
        SUID    uu = nTreeUuid  ( IT   , 0  )                       ;
        QString nn = IT -> text ( 0         )                       ;
        if (uu>0) AppendChannel ( nn , uu   )                       ;
      }                                                             ;
    return true                                                     ;
  }                                                                 ;
  ///////////////////////////////////////////////////////////////////
  RunPageMenu ( mm , aa , StartId , PageSize )                      ;
  PageChanged ( StartId , PageSize           )                      ;
  ///////////////////////////////////////////////////////////////////
  return true                                                       ;
}
