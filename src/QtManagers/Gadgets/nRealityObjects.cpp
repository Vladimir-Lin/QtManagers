#include <qtmanagers.h>

N::RealityLists:: RealityLists ( QWidget * parent , Plan * p )
                : TreeDock     (           parent ,        p )
                , Group        (                             )
                , sorting      ( Qt::AscendingOrder          )
{
  WidgetClass   ;
  Configure ( ) ;
}

N::RealityLists::~RealityLists(void)
{
}

void N::RealityLists::Configure(void)
{
  PassDragDrop       = false                            ;
  t2                 = Types::Reality                   ;
  relation           = Groups::Subordination            ;
  dockingOrientation = Qt::Vertical                     ;
  ///////////////////////////////////////////////////////
  LimitValues [ 0 ]  =  0                               ;
  LimitValues [ 1 ]  = 48                               ;
  LimitValues [ 2 ]  =  0                               ;
  ///////////////////////////////////////////////////////
  setAccessibleName        ( "N::RealityLists"        ) ;
  setObjectName            ( "N::RealityLists"        ) ;
  setWindowTitle           ( tr("Reality objects")    ) ;
  setFocusPolicy           ( Qt::WheelFocus           ) ;
  setDragDropMode          ( DragDrop                 ) ;
  setAlternatingRowColors  ( true                     ) ;
  setRootIsDecorated       ( false                    ) ;
  setSelectionMode         ( ExtendedSelection        ) ;
  setColumnCount           ( 2                        ) ;
  ///////////////////////////////////////////////////////
  NewTreeWidgetItem        ( head                     ) ;
  head -> setText          ( 0 , tr("Name")           ) ;
  head -> setText          ( 1 , ""                   ) ;
  head -> setTextAlignment ( 0 , Qt::AlignCenter      ) ;
  setHeaderItem            ( head                     ) ;
  setColumnWidth           ( 1 , 2                    ) ;
  setColumnHidden          ( 1 , true                 ) ;
  ///////////////////////////////////////////////////////
  MountClicked             ( 1                        ) ;
  MountClicked             ( 2                        ) ;
  plan -> setFont          ( this                     ) ;
  ///////////////////////////////////////////////////////
  setDropFlag              ( DropText , true          ) ;
  ///////////////////////////////////////////////////////
  setFunction              ( InterfaceHasMenu , true  ) ;
  setFunction              ( DockingMenuID    , true  ) ;
  setFunction              ( AlternateMenuID  , false ) ;
  ///////////////////////////////////////////////////////
  InstallMsgs              (                          ) ;
  InstalIcons              (                          ) ;
  ///////////////////////////////////////////////////////
  if ( ! plan -> Booleans [ "Desktop" ] )               {
    allowGesture = true                                 ;
    grabGesture            ( Qt::TapAndHoldGesture    ) ;
    grabGesture            ( Qt::PanGesture           ) ;
  }                                                     ;
}

bool N::RealityLists::FocusIn(void)
{
  nKickOut          ( IsNull ( plan ) , false               ) ;
  DisableAllActions (                                       ) ;
  AssignAction      ( Label           , windowTitle     ( ) ) ;
  /////////////////////////////////////////////////////////////
  LinkAction        ( Refresh         , startup         ( ) ) ;
  LinkAction        ( Insert          , Insert          ( ) ) ;
  LinkAction        ( Rename          , Rename          ( ) ) ;
  LinkAction        ( Paste           , Paste           ( ) ) ;
  LinkAction        ( Export          , Export          ( ) ) ;
  LinkAction        ( Copy            , CopyToClipboard ( ) ) ;
  LinkAction        ( SelectNone      , SelectNone      ( ) ) ;
  LinkAction        ( SelectAll       , SelectAll       ( ) ) ;
  LinkAction        ( Language        , Language        ( ) ) ;
  LinkAction        ( Font            , setFont         ( ) ) ;
  /////////////////////////////////////////////////////////////
  if                ( isFirst ( ) || isSecond ( )           ) {
    LinkAction      ( Delete          , Delete          ( ) ) ;
  }                                                           ;
  /////////////////////////////////////////////////////////////
  return true                                                 ;
}

QMimeData * N::RealityLists::dragMime (void)
{
  UUIDs Uuids                                       ;
  SUID  uuid  = 0                                   ;
  ///////////////////////////////////////////////////
  for (int i=0;i<topLevelItemCount();i++)           {
    QTreeWidgetItem * it = topLevelItem(i)          ;
    if (it->isSelected())                           {
      Uuids << nTreeUuid(it,0)                      ;
    }                                               ;
  }                                                 ;
  if (Uuids.count()<=0)                             {
    QTreeWidgetItem * it = currentItem()            ;
    if (NotNull(it))                                {
      uuid = nTreeUuid(it,0)                        ;
    }                                               ;
  }                                                 ;
  ///////////////////////////////////////////////////
  nKickOut(Uuids.count()<=0 && uuid == 0,NULL)      ;
  if (Uuids.count()==1)                             {
    uuid  = Uuids [0]                               ;
    Uuids . clear ( )                               ;
  }                                                 ;
  ///////////////////////////////////////////////////
  QMimeData * mime = new QMimeData()                ;
  if (Uuids.count()==0)                             {
    setMime ( mime , "reality/uuid"  , uuid  )      ;
  } else                                            {
    setMime ( mime , "reality/uuids" , Uuids )      ;
  }                                                 ;
  ///////////////////////////////////////////////////
  QImage image = QImage ( ":/images/graphics.png" ) ;
  mime -> setImageData(image)                       ;
  return mime                                       ;
}

bool N::RealityLists::hasItem(void)
{
  QTreeWidgetItem * item = currentItem ( ) ;
  return NotNull ( item )                  ;
}

bool N::RealityLists::startDrag(QMouseEvent * event)
{
  QTreeWidgetItem * atItem = itemAt(event->pos())           ;
  nKickOut ( IsNull(atItem) , false )                       ;
  nKickOut (!IsMask(event->buttons(),Qt::LeftButton),false) ;
  dragPoint = event->pos()                                  ;
  nKickOut (!atItem->isSelected(),false)                    ;
  nKickOut (!PassDragDrop,true)                             ;
  return true                                               ;
}

bool N::RealityLists::fetchDrag(QMouseEvent * event)
{
  nKickOut ( !IsMask(event->buttons(),Qt::LeftButton) , false) ;
  QPoint pos = event->pos()                                    ;
  pos -= dragPoint                                             ;
  return ( pos.manhattanLength() > qApp->startDragDistance() ) ;
}

void N::RealityLists::dragDone(Qt::DropAction dropIt,QMimeData * mime)
{
}

bool N::RealityLists::finishDrag(QMouseEvent * event)
{
  return true ;
}

bool N::RealityLists::acceptDrop(QWidget * source,const QMimeData * mime)
{
  nKickOut           ( nEqual ( source , this ) , false ) ;
  return dropHandler ( mime                             ) ;
}

bool N::RealityLists::dropNew(QWidget * source,const QMimeData * mime,QPoint pos)
{
  Alert ( Action ) ;
  return true      ;
}

bool N::RealityLists::dropText(QWidget * source,QPointF pos,const QString & text)
{ Q_UNUSED ( source ) ;
  Q_UNUSED ( pos    ) ;
  Paste    ( text   ) ;
  return true         ;
}

bool N::RealityLists::dropPeople(QWidget * source,QPointF pos,const UUIDs & Uuids)
{
  nKickOut           ( nEqual ( source , this ) , false ) ;
  return dropObjects ( Types::People , pos , Uuids      ) ;
}

bool N::RealityLists::dropPictures(QWidget * source,QPointF pos,const UUIDs & Uuids)
{
  nKickOut           ( nEqual ( source , this ) , false ) ;
  return dropObjects ( Types::Picture , pos , Uuids     ) ;
}

bool N::RealityLists::dropAlbums(QWidget * source,QPointF pos,const UUIDs & Uuids)
{
  nKickOut           ( nEqual ( source , this ) , false ) ;
  return dropObjects ( Types::Album , pos , Uuids       ) ;
}

bool N::RealityLists::dropVideos(QWidget * source,QPointF pos,const UUIDs & Uuids)
{
  nKickOut           ( nEqual ( source , this ) , false ) ;
  return dropObjects ( Types::Video , pos , Uuids       ) ;
}

bool N::RealityLists::dropModels(QWidget * source,QPointF pos,const UUIDs & Uuids)
{
  nKickOut           ( nEqual ( source , this ) , false ) ;
  return dropObjects ( Types::Model , pos , Uuids       ) ;
}

bool N::RealityLists::dropReality(QWidget * source,QPointF pos,const UUIDs & Uuids)
{
  VarArgs args                                               ;
  if             ( nEqual ( source , this ) )                {
    if ( isFirst ( ) || isSecond ( ) )                       {
      QTreeWidgetItem * it = itemAt    ( pos . toPoint ( ) ) ;
      SUID              uu = nTreeUuid ( it , 0            ) ;
      if ( uu <= 0 ) return false                            ;
      args << uu                                             ;
      toVariants ( Uuids           , args   )                ;
      start      ( 10006           , args   )                ;
      return true                                            ;
    }                                                        ;
    Alert        ( Error                    )                ;
    return false                                             ;
  }                                                          ;
  toVariants     ( Uuids           , args   )                ;
  start          ( 10004           , args   )                ;
  return true                                                ;
}

bool N::RealityLists::dropObjects(int T2,QPointF pos,const UUIDs & Uuids)
{
  QPoint            pp = pos . toPoint ( ) ;
  QTreeWidgetItem * it = itemAt ( pp     ) ;
  nKickOut ( IsNull ( it ) , false       ) ;
  SUID u = nTreeUuid ( it , 0 )            ;
  nKickOut ( ( u <= 0 )    , false       ) ;
  VarArgs args                             ;
  args << Types::Reality                   ;
  args << T2                               ;
  args << u                                ;
  toVariants ( Uuids , args )              ;
  start      ( 10005 , args )              ;
  return true                              ;
}

void N::RealityLists::InstallMsgs(void)
{
  LocalMsgs [ RealityListMsgExport ] = tr ( "Export reality objects" ) ;
  LocalMsgs [ RealityListMsgFilter ] = tr ( "Plain text (*.txt)"     ) ;
  LocalMsgs [ RealityListMsgPage   ] = tr ( "Total %1 objects"       ) ;
}

void N::RealityLists::InstalIcons(void)
{
  LocalIcons [ "Update" ] = QIcon ( ":/images/update.png" ) ;
  LocalIcons [ "Plus"   ] = QIcon ( ":/images/plus.png"   ) ;
}

void N::RealityLists::run(int T,ThreadData * d)
{
  nDropOut         ( ! IsContinue ( d ) ) ;
  switch           ( T                  ) {
    case 10001                            :
      startLoading (                    ) ;
      List         ( d                  ) ;
      stopLoading  (                    ) ;
    break                                 ;
    case 10002                            :
      startLoading (                    ) ;
      Append       ( d                  ) ;
      stopLoading  (                    ) ;
    break                                 ;
    case 10003                            :
      startLoading (                    ) ;
      Delete       ( d                  ) ;
      stopLoading  (                    ) ;
    break                                 ;
    case 10004                            :
      startLoading (                    ) ;
      Join         ( d                  ) ;
      stopLoading  (                    ) ;
    break                                 ;
    case 10005                            :
      startLoading (                    ) ;
      DropIn       ( d                  ) ;
      stopLoading  (                    ) ;
    break                                 ;
    case 10006                            :
      startLoading (                    ) ;
      MoveTo       ( d                  ) ;
      stopLoading  (                    ) ;
    break                                 ;
    case 10007                            :
      startLoading (                    ) ;
      SetPositions ( d                  ) ;
      stopLoading  (                    ) ;
    break                                 ;
  }                                       ;
}

bool N::RealityLists::startup(void)
{
  clear (       ) ;
  start ( 10001 ) ;
  return true     ;
}

UUIDs N::RealityLists::LoadUuids(SqlConnection & SC)
{
  UUIDs U                                                   ;
  if ( isFirst  ( ) )                                       {
    GroupItems GI ( plan )                                  ;
    GI . AutoMap    = true                                  ;
    GI . GroupTable = GI . LookTable ( t1 , t2 , relation ) ;
    U  = GI . Subordination                                 (
          SC                                                ,
          first                                             ,
          t1                                                ,
          t2                                                ,
          relation                                          ,
          SC . OrderBy ( "position" , sorting )           ) ;
  } else
  if ( isSecond ( ) )                                       {
    GroupItems GI ( plan )                                  ;
    GI . AutoMap    = true                                  ;
    GI . GroupTable = GI . LookTable ( t1 , t2 , relation ) ;
    U  = GI . GetOwners                                     (
          SC                                                ,
          second                                            ,
          t1                                                ,
          t2                                                ,
          relation                                          ,
          SC . OrderBy ( "reversal" , sorting )           ) ;
  } else                                                    {
    U = SC . Uuids                                          (
          PlanTable(Reality)                                ,
          "uuid"                                            ,
          SC . OrderBy ( "id" , sorting )                 ) ;
  }                                                         ;
  FixUuids ( U )                                            ;
  return U                                                  ;
}

void N::RealityLists::LoadNames (
       ThreadData    * d        ,
       SqlConnection & SC       ,
       UUIDs         & U        )
{
  SUID              u  = 0                            ;
  QString           name                              ;
  QTreeWidgetItem * it                                ;
  /////////////////////////////////////////////////////
  for (int i = 0                                      ;
       IsContinue ( d ) && i < U . count ( )          ;
       i++                                          ) {
    u = U [ i ]                                       ;
    name = SC . getName                               (
             PlanTable(Names)                         ,
             "uuid"                                   ,
             vLanguageId                              ,
             u                                      ) ;
    it = new QTreeWidgetItem (                      ) ;
    it -> setText            ( 0 , name             ) ;
    it -> setData            ( 0 , Qt::UserRole , u ) ;
    PrepareItem              ( it                   ) ;
    addTopLevelItem          ( it                   ) ;
  }                                                   ;
}

bool N::RealityLists::JoinGroup(SqlConnection & SC,SUID u)
{
  GroupItems GI ( plan )             ;
  UUIDs      U                       ;
  ////////////////////////////////////
  if ( isFirst  ( ) )                {
    GI . AutoMap    = true           ;
    GI . GroupTable = GI . LookTable (
                        t1           ,
                        t2           ,
                        relation   ) ;
    U << u                           ;
    GI . Join         ( SC           ,
                        first        ,
                        t1           ,
                        t2           ,
                        relation     ,
                        0            ,
                        U            ,
                        false      ) ;
    return true                      ;
  }                                  ;
  ////////////////////////////////////
  if ( isSecond ( ) )                {
    GI . AutoMap    = true           ;
    GI . GroupTable = GI . LookTable (
                        t1           ,
                        t2           ,
                        relation   ) ;
    U << u                           ;
    GI . JoinOwner    ( SC           ,
                        second       ,
                        t1           ,
                        t2           ,
                        relation     ,
                        U            ,
                        false      ) ;
    return true                      ;
  }                                  ;
  ////////////////////////////////////
  return false                       ;
}

void N::RealityLists::Join(ThreadData * d)
{
  nDropOut              ( ! IsContinue ( d ) ) ;
  UUIDs U                                      ;
  FromVariants          ( d -> Arguments , U ) ;
  nDropOut              ( U . count ( ) <= 0 ) ;
  Bustle                (                    ) ;
  //////////////////////////////////////////////
  GroupItems    GI      ( plan               ) ;
  SqlConnection SC      ( plan -> sql        ) ;
  //////////////////////////////////////////////
  if ( SC . open        ( FunctionString )   ) {
    if                  ( isFirst  ( )       ) {
      GI . AutoMap    = true                   ;
      GI . GroupTable = GI . LookTable         (
                          t1                   ,
                          t2                   ,
                          relation           ) ;
      GI . Join         ( SC                   ,
                          first                ,
                          t1                   ,
                          t2                   ,
                          relation             ,
                          0                    ,
                          U                    ,
                          true               ) ;
    }                                          ;
    ////////////////////////////////////////////
    if                  ( isSecond ( )       ) {
      GI . AutoMap    = true                   ;
      GI . GroupTable = GI . LookTable         (
                          t1                   ,
                          t2                   ,
                          relation           ) ;
      GI . JoinOwner    ( SC                   ,
                          second               ,
                          t1                   ,
                          t2                   ,
                          relation             ,
                          U                    ,
                          true               ) ;
    }                                          ;
  }                                            ;
  SC   . remove         (                    ) ;
  Vacancy               (                    ) ;
  //////////////////////////////////////////////
  emit clearItems       (                    ) ;
  Time::msleep          ( 500                ) ;
  start                 ( 10001              ) ;
}

void N::RealityLists::Delete(ThreadData * d)
{
  nDropOut              ( ! IsContinue ( d ) ) ;
  UUIDs U                                      ;
  FromVariants          ( d -> Arguments , U ) ;
  nDropOut              ( U . count ( ) <= 0 ) ;
  Bustle                (                    ) ;
  //////////////////////////////////////////////
  GroupItems    GI      ( plan               ) ;
  SqlConnection SC      ( plan -> sql        ) ;
  //////////////////////////////////////////////
  if ( SC . open        ( FunctionString )   ) {
    if                  ( isFirst  ( )       ) {
      GI . AutoMap    = true                   ;
      GI . GroupTable = GI . LookTable         (
                          t1                   ,
                          t2                   ,
                          relation           ) ;
      GI . Detach       ( SC                   ,
                          first                ,
                          t1                   ,
                          t2                   ,
                          relation             ,
                          U                  ) ;
    }                                          ;
    ////////////////////////////////////////////
    if ( isSecond ( ) )                        {
      QString Q                                ;
      SUID    u                                ;
      GI . AutoMap    = true                   ;
      GI . GroupTable = GI . LookTable         (
                          t1                   ,
                          t2                   ,
                          relation           ) ;
      foreach ( u , U )                        {
        Q = SC . sql . DeleteFrom              (
              GI . GroupTable                  ,
              GI . ExactItem                   (
                u                              ,
                second                         ,
                t1                             ,
                t2                             ,
                relation                   ) ) ;
        SC . Query ( Q )                       ;
      }                                        ;
    }                                          ;
  }                                            ;
  SC   . remove         (                    ) ;
  Vacancy               (                    ) ;
  //////////////////////////////////////////////
  emit clearItems       (                    ) ;
  Time::msleep          ( 500                ) ;
  start                 ( 10001              ) ;
}

void N::RealityLists::DropIn(ThreadData * d)
{
  nDropOut              ( ! IsContinue ( d ) )            ;
  /////////////////////////////////////////////////////////
  if ( d -> Arguments . count ( ) < 4 ) return            ;
  int   T1 = d -> Arguments [ 0 ] . toInt       ( )       ;
  int   T2 = d -> Arguments [ 1 ] . toInt       ( )       ;
  SUID  u  = d -> Arguments [ 2 ] . toULongLong ( )       ;
  UUIDs U                                                 ;
  /////////////////////////////////////////////////////////
  for (int i = 3 ; i < d -> Arguments . count ( ) ; i++ ) {
    U << d -> Arguments [ i ] . toULongLong ( )           ;
  }                                                       ;
  /////////////////////////////////////////////////////////
  Bustle                (                               ) ;
  /////////////////////////////////////////////////////////
  GroupItems    GI      ( plan                          ) ;
  SqlConnection SC      ( plan -> sql                   ) ;
  /////////////////////////////////////////////////////////
  if ( SC . open        ( FunctionString )              ) {
    GI . AutoMap    = true                                ;
    GI . GroupTable = GI . LookTable                      (
                        T1                                ,
                        T2                                ,
                        Groups::Subordination           ) ;
    GI . Join         ( SC                                ,
                        u                                 ,
                        T1                                ,
                        T2                                ,
                        Groups::Subordination             ,
                        0                                 ,
                        U                                 ,
                        true                            ) ;
  }                                                       ;
  SC   . remove         (                               ) ;
  Vacancy               (                               ) ;
  /////////////////////////////////////////////////////////
  alert                 ( "Done" , ""                   ) ;
}

void N::RealityLists::MoveTo(ThreadData * d)
{
  nDropOut ( ! IsContinue ( d ) ) ;
}

void N::RealityLists::SetPositions(ThreadData * d)
{
  nDropOut ( ! IsContinue ( d ) ) ;
}

void N::RealityLists::List(ThreadData * d)
{
  nDropOut              ( ! IsContinue ( d ) ) ;
  emit assignEnabling   ( false              ) ;
  Bustle                (                    ) ;
  SqlConnection SC      ( plan -> sql        ) ;
  if ( SC . open        ( FunctionString )   ) {
    UUIDs U = LoadUuids (     SC             ) ;
    LoadNames           ( d , SC , U         ) ;
    SC . close          (                    ) ;
  }                                            ;
  SC   . remove         (                    ) ;
  Vacancy               (                    ) ;
  emit assignEnabling   ( true               ) ;
  Alert                 ( Done               ) ;
}

void N::RealityLists::Append(ThreadData * d)
{
  nDropOut ( ! IsContinue ( d )                       ) ;
  nDropOut ( d -> Arguments . count ( )          <= 0 ) ;
  QString T = d -> Arguments . first ( ) . toString ( ) ;
  nDropOut ( T  . length ( )                     <= 0 ) ;
  QStringList L = T . split ( "\n" )                    ;
  L = File::PurifyLines ( L )                           ;
  nDropOut ( L  . count  ( )                     <= 0 ) ;
  ///////////////////////////////////////////////////////
  SqlConnection SC     ( plan -> sql                  ) ;
  if                   ( SC . open ( FunctionString ) ) {
    for (int i = 0                                      ;
         IsContinue ( d ) && ( i < L . count ( ) )      ;
         i++                                          ) {
      QString s = L [ i ]                               ;
      s = s . replace ( "\r" , "" )                     ;
      s = s . replace ( "\n" , "" )                     ;
      if ( s . length ( ) > 0 )                         {
        SUID u                                          ;
        u = SC . Unique                                 (
              PlanTable(MajorUuid)                      ,
              "uuid"                                    ,
              Unify::RealityHead                      ) ;
        SC . assureUuid                                 (
              PlanTable(MajorUuid)                      ,
              u                                         ,
              Types::Reality                          ) ;
        SC . insertUuid                                 (
              PlanTable(Reality)                        ,
              u                                         ,
              "uuid"                                  ) ;
        JoinGroup ( SC , u                            ) ;
        /////////////////////////////////////////////////
        SC . assureName                                 (
          PlanTable(Names)                              ,
          u                                             ,
          vLanguageId                                   ,
          s                                           ) ;
        /////////////////////////////////////////////////
        NewTreeWidgetItem ( IT                        ) ;
        IT -> setText     ( 0 , s                     ) ;
        IT -> setData     ( 0 , Qt::UserRole , u      ) ;
        addTopLevelItem   ( IT                        ) ;
      }                                                 ;
    }                                                   ;
    SC . close  (                                     ) ;
  }                                                     ;
  SC   . remove (                                     ) ;
  ///////////////////////////////////////////////////////
  alert         ( "Done" , "" ) ;
}

void N::RealityLists::singleClicked(QTreeWidgetItem * item,int column)
{
  Alert ( Action ) ;
}

void N::RealityLists::doubleClicked(QTreeWidgetItem * item,int column)
{
  QLineEdit * le                               ;
  removeOldItem ( true , 0 )                   ;
  switch        ( column   )                   {
    case 0                                     :
      le  = setLineEdit                        (
              item                             ,
              column                           ,
              SIGNAL ( editingFinished ( ) )   ,
              SLOT   ( editingFinished ( ) ) ) ;
      le -> setFocus ( Qt::TabFocusReason    ) ;
    break                                      ;
  }                                            ;
}

void N::RealityLists::Insert(void)
{
  removeOldItem     (                      ) ;
  NewTreeWidgetItem ( IT                   ) ;
  IT -> setData     ( 0 , Qt::UserRole , 0 ) ;
  addTopLevelItem   ( IT                   ) ;
  scrollToItem      ( IT                   ) ;
  doubleClicked     ( IT , 0               ) ;
}

void N::RealityLists::Delete(void)
{
  nDropOut   ( ( ! isFirst ( ) ) && ( ! isSecond ( ) ) ) ;
  UUIDs U = selectedUuids ( 0 )                          ;
  nDropOut   ( U . count ( ) <= 0                      ) ;
  VarArgs args                                           ;
  toVariants ( U     , args                            ) ;
  start      ( 10003 , args                            ) ;
}

void N::RealityLists::Rename(void)
{
  QTreeWidgetItem * it               ;
  it = currentItem (               ) ;
  nDropOut         ( IsNull ( it ) ) ;
  doubleClicked    ( it , 0        ) ;
}

void N::RealityLists::editingFinished(void)
{
  SUID        u = nTreeUuid ( ItemEditing , ItemColumn     ) ;
  QLineEdit * l = Casting   ( QLineEdit   , ItemWidget     ) ;
  ////////////////////////////////////////////////////////////
  if                        ( IsNull ( l )                 ) {
    removeOldItem           ( true , 0                     ) ;
    Alert                   ( Error                        ) ;
    return                                                   ;
  }                                                          ;
  ////////////////////////////////////////////////////////////
  QString n = l -> text     (                              ) ;
  LockGui                   (                              ) ;
  SqlConnection SC          ( plan -> sql                  ) ;
  ////////////////////////////////////////////////////////////
  if                        ( SC . open ( FunctionString ) ) {
    if                      ( u <= 0                       ) {
      u = SC . Unique                                        (
            PlanTable(MajorUuid)                             ,
            "uuid"                                           ,
            Unify::RealityHead                             ) ;
      SC . assureUuid                                        (
            PlanTable(MajorUuid)                             ,
            u                                                ,
            Types::Reality                                 ) ;
      SC . insertUuid                                        (
            PlanTable(Reality)                               ,
            u                                                ,
            "uuid"                                         ) ;
      JoinGroup ( SC , u                                   ) ;
    }                                                        ;
    //////////////////////////////////////////////////////////
    SC . assureName                                          (
      PlanTable(Names)                                       ,
      u                                                      ,
      vLanguageId                                            ,
      n                                                    ) ;
    //////////////////////////////////////////////////////////
    ItemEditing -> setData  ( 0 , Qt::UserRole , u         ) ;
    ItemEditing -> setText  ( 0 , n                        ) ;
    //////////////////////////////////////////////////////////
    SC . close              (                              ) ;
  }                                                          ;
  SC   . remove             (                              ) ;
  ////////////////////////////////////////////////////////////
  UnlockGui                 (                              ) ;
  removeOldItem             ( true , 0                     ) ;
}

void N::RealityLists::Paste(QString t)
{
  nDropOut ( t . length ( ) <= 0 ) ;
  VarArgs args                     ;
  args << t                        ;
  start    ( 10002 , args        ) ;
}

void N::RealityLists::Paste(void)
{
  Paste ( nClipboardText ) ;
}

void N::RealityLists::Export(void)
{
  if ( topLevelItemCount ( ) <= 0 ) return                        ;
  QString T = toText ( true )                                     ;
  if ( T . length        ( ) <= 0 ) return                        ;
  /////////////////////////////////////////////////////////////////
  QString filename = plan -> Temporary ( "" )                     ;
  if ( plan -> Variables . contains ( "RealityPath" ) )           {
    filename = plan -> Variables [ "RealityPath" ] . toString ( ) ;
  }                                                               ;
  filename = QFileDialog::getSaveFileName                         (
               this                                               ,
               LocalMsgs [ RealityListMsgExport ]                 ,
               filename                                           ,
               LocalMsgs [ RealityListMsgFilter ]               ) ;
  if ( filename . length ( ) <= 0 ) return                        ;
  /////////////////////////////////////////////////////////////////
  QFileInfo FXI ( filename )                                      ;
  QString   path  = FXI . absoluteDir ( ) . absolutePath ( )      ;
  plan -> Variables [ "RealityPath" ] = path                      ;
  /////////////////////////////////////////////////////////////////
  QByteArray B = T . toUtf8 ( )                                   ;
  File::toFile ( filename , B      )                              ;
  alert        ( "Done" , filename )                              ;
}

void N::RealityLists::doTranslations(void)
{
  UUIDs U = itemUuids ( 0 )                 ;
  if ( U . count ( ) <= 0 ) return          ;
  emit Translations ( windowTitle ( ) , U ) ;
}

bool N::RealityLists::Menu(QPoint pos)
{
  if (   isLoading  (                  ) ) return true ;
  if ( ! isFunction ( InterfaceHasMenu ) ) return true ;
  if (   isFunction ( AlternateMenuID  ) )             {
    emit RealityMenu ( this , pos )                    ;
    return true                                        ;
  }                                                    ;
  return LocalMenu   ( this , pos )                    ;
}

bool N::RealityLists::RunSorting(MenuManager & mm,QAction * aa)
{
  switch ( mm [ aa ] )                          {
    case 90301                                  :
      setSortingEnabled ( aa -> isChecked ( ) ) ;
    break                                       ;
    case 90302                                  :
      sorting = Qt::AscendingOrder              ;
      startup           (                     ) ;
    break                                       ;
    case 90303                                  :
      sorting = Qt::DescendingOrder             ;
      startup           (                     ) ;
    break                                       ;
    default                                     :
    return false                                ;
  }                                             ;
  return true                                   ;
}

void N::RealityLists::SubjectMenu(MenuManager & mm,QMenu * ma)
{
  if ( IsNull ( ma ) ) return                                         ;
  mm . add ( ma , CiosMenuId ( 12 , 37 , 1 ) , tr ( "Models"      ) ) ;
  mm . add ( ma , CiosMenuId ( 12 , 37 , 2 ) , tr ( "People"      ) ) ;
  mm . add ( ma , CiosMenuId ( 12 , 37 , 3 ) , tr ( "Pictures"    ) ) ;
  mm . add ( ma , CiosMenuId ( 12 , 37 , 4 ) , tr ( "Albums"      ) ) ;
  mm . add ( ma , CiosMenuId ( 12 , 37 , 5 ) , tr ( "Video clips" ) ) ;
}

bool N::RealityLists::RunSubject (
       MenuManager     & mm      ,
       QAction         * aa      ,
       QTreeWidgetItem * it      )
{
  if ( IsNull ( it ) ) return false          ;
  SUID u = nTreeUuid ( it , 0 )              ;
  if ( u <= 0 ) return false                 ;
  ////////////////////////////////////////////
  QString n = it -> text ( 0 )               ;
  switch ( mm [ aa ] )                       {
    case CiosMenuId ( 12 , 37 , 1 )          :
      emit Lookup ( n , Types::Model   , u ) ;
    return true                              ;
    case CiosMenuId ( 12 , 37 , 2 )          :
      emit Lookup ( n , Types::People  , u ) ;
    return true                              ;
    case CiosMenuId ( 12 , 37 , 3 )          :
      emit Lookup ( n , Types::Picture , u ) ;
    return true                              ;
    case CiosMenuId ( 12 , 37 , 4 )          :
      emit Lookup ( n , Types::Album   , u ) ;
    return true                              ;
    case CiosMenuId ( 12 , 37 , 5 )          :
      emit Lookup ( n , Types::Video   , u ) ;
    return true                              ;
  }                                          ;
  ////////////////////////////////////////////
  return false                               ;
}

bool N::RealityLists::LocalMenu(QWidget * widget,QPoint pos)
{
  nKickOut    ( ( widget != this ) , false )                ;
  nScopedMenu ( mm                 , this  )                ;
  ///////////////////////////////////////////////////////////
  QTreeWidgetItem * it       = itemAt(pos)                  ;
  QAction         * aa       = NULL                         ;
  QMenu           * mi       = NULL                         ;
  QMenu           * ma       = NULL                         ;
  QMenu           * ms       = NULL                         ;
  QMenu           * mx       = NULL                         ;
  QMenu           * mp       = NULL                         ;
  int               StartId  = 0                            ;
  int               PageSize = 0                            ;
  SUID              u        = 0                            ;
  ///////////////////////////////////////////////////////////
  if ( NotNull ( it ) ) u = nTreeUuid ( it , 0 )            ;
  ///////////////////////////////////////////////////////////
  if ( LimitValues [ 2 ] > 0 )                              {
    mp = PageMenu ( mm                                      ,
                    LocalMsgs [ RealityListMsgPage ]        ,
                    StartId                                 ,
                    PageSize                              ) ;
  }                                                         ;
  ///////////////////////////////////////////////////////////
  mm . add ( 101                                            ,
             LocalIcons [ "Update" ]                        ,
             tr ( "Refresh" )                             ) ;
  mm . add ( 102                                            ,
             LocalIcons [ "Plus"   ]                        ,
             tr ( "Insert" )                              ) ;
  if ( NotNull ( it ) && ( u > 0 ) )                        {
    mm   . add ( 103 , tr ( "Rename"     ) )                ;
    mm   . add ( 104 , tr ( "Edit names" ) )                ;
    if ( isFirst ( ) || isSecond ( ) )                      {
      mm . add ( 105 , tr ( "Delete"     ) )                ;
    }                                                       ;
    mi = mm . addMenu ( tr("Subjects"   ) )                 ;
    SubjectMenu       ( mm , mi           )                 ;
  }                                                         ;
  ///////////////////////////////////////////////////////////
  mm . addSeparator (                   )                   ;
  ma = mm . addMenu ( tr("Adjustments") )                   ;
  ms = mm . addMenu ( tr("Sorting"    ) )                   ;
  mx = mm . addMenu ( tr("Selections" ) )                   ;
  ///////////////////////////////////////////////////////////
  AdjustMenu        ( mm , ma           )                   ;
  SortingMenu       ( mm , ms           )                   ;
  SelectionsMenu    ( mm , mx           )                   ;
  ///////////////////////////////////////////////////////////
  mm . addSeparator ( ma                )                   ;
  if ( topLevelItemCount ( ) > 0 )                          {
    if ( isFirst ( ) || isSecond ( ) )                      {
      mm . add      ( ma                                    ,
                      107                                   ,
                      tr("Assign positions")              ) ;
    }                                                       ;
    mm . add        ( ma                                    ,
                      106                                   ,
                      tr("Multilingual translations")     ) ;
  }                                                         ;
  mm . add          ( ma                                    ,
                      501                                   ,
                      tr("Pending space")                   ,
                      true                                  ,
                      ! isColumnHidden ( 1 )              ) ;
  ///////////////////////////////////////////////////////////
  if ( isFunction ( DockingMenuID ) )                       {
    DockingMenu ( mm )                                      ;
  }                                                         ;
  ///////////////////////////////////////////////////////////
  mm . setFont   ( plan )                                   ;
  aa = mm . exec (      )                                   ;
  if (IsNull(aa))                                           {
    if ( NotNull ( mp ) )                                   {
      PageChanged ( StartId , PageSize )                    ;
    }                                                       ;
    return true                                             ;
  }                                                         ;
  ///////////////////////////////////////////////////////////
  if ( RunDocking     ( mm , aa )   ) return true           ;
  if ( RunAdjustment  ( mm , aa )   ) return true           ;
  if ( RunSorting     ( mm , aa )   ) return true           ;
  if ( RunSelections  ( mm , aa )   ) return true           ;
  ///////////////////////////////////////////////////////////
  if ( isFunction ( DockingMenuID ) )                       {
    if ( RunDocking ( mm , aa ) ) return true               ;
  }                                                         ;
  ///////////////////////////////////////////////////////////
  switch ( mm [ aa ] )                                      {
    case 101                                                :
      startup        (       )                              ;
    break                                                   ;
    case 102                                                :
      Insert         (       )                              ;
    break                                                   ;
    case 103                                                :
      Rename         (       )                              ;
    break                                                   ;
    case 104                                                :
      emit FullNames ( u     )                              ;
    break                                                   ;
    case 105                                                :
      Delete         (       )                              ;
    break                                                   ;
    case 106                                                :
      doTranslations (       )                              ;
    break                                                   ;
    case 107                                                :
      start          ( 10007 )                              ;
    break                                                   ;
    case 501                                                :
      setColumnHidden ( 1 , ! isColumnHidden ( 1 ) )        ;
    break                                                   ;
  }                                                         ;
  ///////////////////////////////////////////////////////////
  if ( NotNull(mp) )                                        {
    RunPageMenu ( mm , aa , StartId , PageSize )            ;
    PageChanged ( StartId , PageSize           )            ;
  }                                                         ;
  ///////////////////////////////////////////////////////////
  return true                                               ;
}
