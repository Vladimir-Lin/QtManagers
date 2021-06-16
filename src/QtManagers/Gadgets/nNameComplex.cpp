#include <qtmanagers.h>

N::NameComplex:: NameComplex ( QWidget * parent , Plan * p    )
               : TreeWidget  (           parent ,        p    )
               , Ownership   ( 0                , Types::None )
               , dropAction  ( false                          )
               , titled      ( false                          )
{
  WidgetClass   ;
  Configure ( ) ;
}

N::NameComplex::~NameComplex(void)
{
}

QSize N::NameComplex::sizeHint(void) const
{
  return SizeSuggestion ( QSize ( 320 , 240 ) ) ;
}

void N::NameComplex::Configure(void)
{
  QIcon icx = QIcon       ( ":/images/tableproperties.png" ) ;
  ////////////////////////////////////////////////////////////
  setAccessibleName       ( "N::NameComplex"               ) ;
  setObjectName           ( "N::NameComplex"               ) ;
  ////////////////////////////////////////////////////////////
  NewTreeWidgetItem       ( head                           ) ;
  head -> setText         ( 0,tr("Name"    )               ) ;
  head -> setText         ( 1,tr("Language")               ) ;
  head -> setText         ( 2,tr("Section" )               ) ;
  head -> setText         ( 3,tr("Usage"   )               ) ;
  ////////////////////////////////////////////////////////////
  setWindowTitle          ( tr("Name lists")               ) ;
  setWindowIcon           ( icx                            ) ;
  setRootIsDecorated      ( false                          ) ;
  setAlternatingRowColors ( true                           ) ;
  setColumnCount          ( 4                              ) ;
  setSelectionMode        ( ExtendedSelection              ) ;
  setAllAlignments        ( head,Qt::AlignCenter           ) ;
  setFont                 ( head,Fonts::ListView           ) ;
  setHeaderItem           ( head                           ) ;
  setDragDropMode         ( DragDrop                       ) ;
  ////////////////////////////////////////////////////////////
  plan -> setFont         ( this                           ) ;
  MountClicked            ( 1                              ) ;
  MountClicked            ( 2                              ) ;
  ////////////////////////////////////////////////////////////
  setDropFlag             ( DropName , true                ) ;
  addConnector            ( "Restart"                        ,
                            this                             ,
                            SIGNAL ( Restart ( ) )           ,
                            this                             ,
                            SLOT   ( startup ( ) )         ) ;
  onlyConnector           ( "Restart"                      ) ;
  ////////////////////////////////////////////////////////////
  Usages [ Dynamic :: Default       ] = tr("Default"       ) ;
  Usages [ Dynamic :: Typo          ] = tr("Typo"          ) ;
  Usages [ Dynamic :: Pen           ] = tr("Pen name"      ) ;
  Usages [ Dynamic :: Stage         ] = tr("Stage name"    ) ;
  Usages [ Dynamic :: Abbreviation  ] = tr("Abbreviation"  ) ;
  Usages [ Dynamic :: Identifier    ] = tr("Identifier"    ) ;
  Usages [ Dynamic :: Entry         ] = tr("Entry"         ) ;
  Usages [ Dynamic :: Pronunciation ] = tr("Pronunciation" ) ;
  Usages [ Dynamic :: Other         ] = tr("Other"         ) ;
  ////////////////////////////////////////////////////////////
  Maps   [ 0                        ] = tr("Undecided"     ) ;
  Maps   [ 1                        ] = tr("Master"        ) ;
  Maps   [ 2                        ] = tr("Alias"         ) ;
}

bool N::NameComplex::FocusIn(void)
{
  nKickOut          ( IsNull(plan) , false           ) ;
  DisableAllActions (                                ) ;
  AssignAction      ( Label        , windowTitle ( ) ) ;
  LinkAction        ( Refresh      , startup     ( ) ) ;
  LinkAction        ( Insert       , New         ( ) ) ;
  LinkAction        ( Delete       , Delete      ( ) ) ;
  LinkAction        ( Paste        , Paste       ( ) ) ;
  LinkAction        ( Eradicate    , Eradicate   ( ) ) ;
  LinkAction        ( GoUp         , ItemUp      ( ) ) ;
  LinkAction        ( GoDown       , ItemDown    ( ) ) ;
  LinkAction        ( Font         , setFont     ( ) ) ;
  return true                                          ;
}

bool N::NameComplex::hasItem(void)
{
  UUIDs U = selectedUuids ( 0 ) ;
  return ( U . count ( ) > 0 )  ;
}

bool N::NameComplex::startDrag(QMouseEvent * e)
{
  QTreeWidgetItem * atItem = itemAt ( e -> pos ( ) )                  ;
  nKickOut (   IsNull ( atItem ) , false                            ) ;
  nKickOut ( ! IsMask ( e -> buttons ( ) , Qt::LeftButton ) , false ) ;
  dragPoint = e -> pos ( )                                            ;
  nKickOut ( ! atItem -> isSelected ( ) , false                     ) ;
  nKickOut ( ! PassDragDrop             , true                      ) ;
  return true                                                         ;
}

bool N::NameComplex::fetchDrag(QMouseEvent * e)
{
  nKickOut ( ! IsMask ( e -> buttons ( ) , Qt::LeftButton ) , false )  ;
  QPoint pos = e -> pos ( )                                            ;
  pos -= dragPoint                                                     ;
  return ( pos . manhattanLength ( ) > qApp -> startDragDistance ( ) ) ;
}

QMimeData * N::NameComplex::dragMime(void)
{
  QMimeData * mime = standardMime("name")                    ;
  nKickOut ( IsNull(mime) , NULL )                           ;
  QImage image = windowIcon().pixmap(QSize(32,32)).toImage() ;
  mime -> setImageData(image)                                ;
  return mime                                                ;
}

void N::NameComplex::dragDone(Qt::DropAction dropIt,QMimeData * mime)
{
}

bool N::NameComplex::finishDrag(QMouseEvent * e)
{
  return true ;
}

bool N::NameComplex::acceptDrop(QWidget * source,const QMimeData * mime)
{
  nKickOut           ( nEqual ( source , this ) , false ) ;
  return dropHandler ( mime                             ) ;
}

bool N::NameComplex::dropNew(QWidget * source,const QMimeData * mime,QPoint pos)
{
  Alert ( Action ) ;
  return true      ;
}

bool N::NameComplex::dropNames(QWidget * source,QPointF pos,const UUIDs & U)
{ Q_UNUSED   ( source                      ) ;
  Q_UNUSED   ( pos                         ) ;
  nKickOut   ( ObjectUuid ( ) <= 0 , false ) ;
  VarArgs args                               ;
  toVariants ( U     , args                ) ;
  start      ( 10002 , args                ) ;
  return true                                ;
}

void N::NameComplex::run(int T,ThreadData * d)
{
  if ( ! IsContinue ( d ) ) return ;
  switch            ( T )          {
    case 10001                     :
      startLoading  (   )          ;
      Reload        ( d )          ;
      stopLoading   (   )          ;
    break                          ;
    case 10002                     :
      startLoading  (   )          ;
      JoinMaps      ( d )          ;
      stopLoading   (   )          ;
    break                          ;
    case 10003                     :
      startLoading  (   )          ;
      Eradicate     ( d )          ;
      stopLoading   (   )          ;
    break                          ;
  }                                ;
}

void N::NameComplex::JoinMaps(ThreadData * d)
{
  UUIDs U                                        ;
  FromVariants   ( d -> Arguments , U )          ;
  EnterSQL       ( SC , plan->sql     )          ;
    QString Q                                    ;
    int     prefer = topLevelItemCount ( ) + 1   ;
    SUID    u                                    ;
    foreach ( u , U )                            {
      if ( ! MapUuids . contains ( u ) )         {
        Q = SC . sql . InsertInto                (
              PlanTable(NameMaps)                ,
              3                                  ,
              "uuid"                             ,
              "name"                             ,
              "prefer"                         ) ;
        SC . Prepare ( Q                       ) ;
        SC . Bind    ( "uuid"   , ObjectUuid() ) ;
        SC . Bind    ( "name"   , u            ) ;
        SC . Bind    ( "prefer" , prefer       ) ;
        if ( SC . Exec ( ) ) prefer ++           ;
      }                                          ;
    }                                            ;
  LeaveSQL           ( SC , plan -> sql        ) ;
  emit Restart       (                         ) ;
}

void N::NameComplex::FetchName(SqlConnection & SC,ThreadData * d)
{
  QString Q                                                     ;
  Q = SC . sql . SelectFrom                                     (
        "`id`,`language`,`name`"                                ,
        PlanTable(Names)                                        ,
        SC . WhereUuid   ( ObjectUuid ( )                     ) ,
        SC . OrderByDesc ( "ltime"                          ) ) ;
  SqlLoopNow             ( SC , Q                             ) ;
    NewTreeWidgetItem    ( IT                                 ) ;
    /////////////////////////////////////////////////////////////
    int     id       = SC . Int    ( 0 )                        ;
    int     language = SC . Int    ( 1 )                        ;
    QString name     = SC . String ( 2 )                        ;
    /////////////////////////////////////////////////////////////
    IT -> setData        ( 0 , Qt::UserRole , ObjectUuid ( )  ) ;
    IT -> setData        ( 1 , Qt::UserRole , language        ) ;
    IT -> setData        ( 2 , Qt::UserRole , 1               ) ;
    IT -> setData        ( 3 , Qt::UserRole , id              ) ;
    IT -> setText        ( 0 , name                           ) ;
    IT -> setText        ( 1 , plan -> languages [ language ] ) ;
    IT -> setText        ( 2 , Maps    [ 1 ]                  ) ;
    IT -> setText        ( 3 , ""                             ) ;
    /////////////////////////////////////////////////////////////
    addTopLevelItem      ( IT                                 ) ;
    /////////////////////////////////////////////////////////////
    if                   ( ! titled                           ) {
      emit assignWindowTitle ( name )                           ;
      titled = true                                             ;
    }                                                           ;
    /////////////////////////////////////////////////////////////
    if                   ( language == plan -> LanguageId     ) {
      emit assignWindowTitle ( name )                           ;
      titled = true                                             ;
    }                                                           ;
    /////////////////////////////////////////////////////////////
  SqlLoopErr             ( SC , Q                             ) ;
  SqlLoopEnd             ( SC , Q                             ) ;
}

void N::NameComplex::FetchNames(SqlConnection & SC,ThreadData * d)
{
  QString Q                                                     ;
  int     total                                                 ;
  SUID    nuid                                                  ;
  MapUuids . clear ( )                                          ;
  MapUuids = SC . Uuids                                         (
               PlanTable(NameMaps)                              ,
               "name"                                           ,
               QString ( "%1 %2"                                )
               . arg   ( SC . WhereUuid  ( ObjectUuid ( ) )     )
               . arg   ( SC . OrderByAsc ( "prefer"       ) ) ) ;
  total    = MapUuids . count ( )                               ;
  for (int i=0; IsContinue ( d ) && ( i < total ) ; i++ )       {
    int tt = 0                                                  ;
    nuid = MapUuids [ i ]                                       ;
    Q = SC . sql . SelectFrom                                   (
          "`usage`"                                             ,
          PlanTable(NameMaps)                                   ,
          QString ( "where uuid = %1 and name = %2"             )
          . arg   ( ObjectUuid ( )                              )
          . arg   ( nuid                                    ) ) ;
    if ( SC . Fetch ( Q ) ) tt = SC . Int ( 0 )                 ;
    Q = SC . sql . SelectFrom                                   (
          "`language`,`name`"                                   ,
          PlanTable(Names)                                      ,
          SC . WhereUuid ( nuid )                             ) ;
    if                   ( SC . Fetch ( Q )                   ) {
      NewTreeWidgetItem  ( IT                                 ) ;
      int     language = SC . Int    ( 0 )                      ;
      QString name     = SC . String ( 1 )                      ;
      IT -> setData      ( 0 , Qt::UserRole , nuid            ) ;
      IT -> setData      ( 1 , Qt::UserRole , language        ) ;
      IT -> setData      ( 2 , Qt::UserRole , 2               ) ;
      IT -> setData      ( 3 , Qt::UserRole , tt              ) ;
      IT -> setText      ( 0 , name                           ) ;
      IT -> setText      ( 1 , plan -> languages [ language ] ) ;
      IT -> setText      ( 2 , Maps   [ 2  ]                  ) ;
      IT -> setText      ( 3 , Usages [ tt ]                  ) ;
      addTopLevelItem    ( IT                                 ) ;
      if                 ( ! titled                           ) {
        emit assignWindowTitle ( name )                         ;
        titled = true                                           ;
      }                                                         ;
    }                                                           ;
  }                                                             ;
}

void N::NameComplex::Reload(ThreadData * d)
{
  titled = false                    ;
  ///////////////////////////////////
  EnterSQL     ( SC , plan -> sql ) ;
  FetchName    ( SC , d           ) ;
  FetchNames   ( SC , d           ) ;
  Alert        ( Done             ) ;
  ErrorSQL     ( SC , plan -> sql ) ;
  Alert        ( Error            ) ;
  LeaveSQL     ( SC , plan -> sql ) ;
  ///////////////////////////////////
  emit AutoFit (                  ) ;
}

bool N::NameComplex::startup(void)
{
  clear (       ) ;
  start ( 10001 ) ;
  return true     ;
}

void N::NameComplex::New(void)
{
  NewTreeWidgetItem ( IT                    ) ;
  IT -> setData     ( 0  , Qt::UserRole , 0 ) ;
  IT -> setData     ( 1  , Qt::UserRole , 0 ) ;
  IT -> setData     ( 2  , Qt::UserRole , 0 ) ;
  IT -> setData     ( 3  , Qt::UserRole , 0 ) ;
  IT -> setText     ( 2  , Maps [ 0 ]       ) ;
  addTopLevelItem   ( IT                    ) ;
  scrollToItem      ( IT                    ) ;
  doubleClicked     ( IT , 0                ) ;
  Alert             ( Action                ) ;
}

bool N::NameComplex::DeleteMaster(SqlConnection & SC,int id)
{
  if ( id <= 0 ) return false                        ;
  QString Q                                          ;
  Q = SC . sql . DeleteFrom                          (
        PlanTable(Names)                             ,
        QString ( "where `id` = %1" ) . arg ( id ) ) ;
  return SC . Query ( Q )                            ;
}

bool N::NameComplex::DeleteAlias(SqlConnection & SC,SUID u)
{
  UUIDs   U                                              ;
  QString Q                                              ;
  ////////////////////////////////////////////////////////
  Q = SC . sql . DeleteFrom                              (
        PlanTable(NameMaps)                              ,
        QString ( "where uuid = %1 and name = %2"        )
        . arg   ( ObjectUuid ( )                         )
        . arg   ( u                                  ) ) ;
  SC . Query    ( Q                                    ) ;
  ////////////////////////////////////////////////////////
  U = SC . Uuids                                         (
            PlanTable(NameMaps)                          ,
            "name"                                       ,
            QString ( "%1 %2"                            )
            . arg   ( SC . WhereUuid  ( ObjectUuid ( ) ) )
            . arg   ( SC . OrderByAsc ( "position" ) ) ) ;
  ////////////////////////////////////////////////////////
  for (int i = 0 ; i < U . count ( ) ; i++ )             {
    Q = SC . sql . Update                                (
          PlanTable(NameMaps)                            ,
          "where uuid = :UUID and name = :NAME"          ,
          1                                              ,
          "prefer"                                     ) ;
    SC . Prepare ( Q                                   ) ;
    SC . Bind    ( "uuid"   , ObjectUuid ( )           ) ;
    SC . Bind    ( "name"   , U [ i ]                  ) ;
    SC . Bind    ( "prefer" , i + 1                    ) ;
    SC . Exec    (                                     ) ;
  }                                                      ;
  ////////////////////////////////////////////////////////
  return true                                            ;
}

void N::NameComplex::Delete(void)
{
  QTreeWidgetItem * item = currentItem ( )      ;
  if ( IsNull ( item ) ) return                 ;
  int  index = indexOfTopLevelItem ( item     ) ;
  SUID u     = nTreeUuid           ( item , 0 ) ;
  SUID m     = nTreeInt            ( item , 2 ) ;
  SUID p     = nTreeInt            ( item , 3 ) ;
  ///////////////////////////////////////////////
  SqlConnection SC   ( plan -> sql )            ;
  if ( SC . open ( FunctionString ) )           {
    switch           ( m           )            {
      case 1                                    :
        DeleteMaster ( SC , p      )            ;
      break                                     ;
      case 2                                    :
        DeleteAlias  ( SC , u      )            ;
      break                                     ;
    }                                           ;
    SC . close       (             )            ;
  }                                             ;
  SC   . remove      (             )            ;
  ///////////////////////////////////////////////
  takeTopLevelItem   ( index       )            ;
  Alert              ( Done        )            ;
}

void N::NameComplex::Paste(void)
{
  QString T = nClipboardText                    ;
  if ( T . length ( ) <= 0 ) return             ;
  ///////////////////////////////////////////////
  QStringList S = T . split ( '\n' )            ;
  S = File::PurifyLines(S)                      ;
  if ( S . count ( ) <= 0 ) return              ;
  ///////////////////////////////////////////////
  foreach ( T , S ) if ( T . length ( ) > 0   ) {
    NewTreeWidgetItem  ( IT                   ) ;
    IT -> setData      ( 0 , Qt::UserRole , 0 ) ;
    IT -> setData      ( 1 , Qt::UserRole , 0 ) ;
    IT -> setData      ( 2 , Qt::UserRole , 0 ) ;
    IT -> setData      ( 3 , Qt::UserRole , 0 ) ;
    IT -> setText      ( 0 , T                ) ;
    IT -> setText      ( 2 , Maps [ 0 ]       ) ;
    addTopLevelItem    ( IT                   ) ;
  }                                             ;
}

void N::NameComplex::Eradicate(ThreadData * d)
{
  QList<QTreeWidgetItem *> items                  ;
  items = selectedItems ( )                       ;
  if ( items . count ( ) <= 0 ) return            ;
  /////////////////////////////////////////////////
  GroupItems    GI ( plan        )                ;
  SqlConnection SC ( plan -> sql )                ;
  /////////////////////////////////////////////////
  if ( SC . open ( FunctionString ) )             {
    for (int i=0;i<items.count();i++)             {
      QTreeWidgetItem * it = items     [ i      ] ;
      SUID              u  = nTreeUuid ( it , 0 ) ;
      int               m  = nTreeInt  ( it , 2 ) ;
      int               p  = nTreeInt  ( it , 3 ) ;
      switch ( m )                                {
        case 1                                    :
          DeleteMaster        ( SC , p )          ;
        break                                     ;
        case 2                                    :
          GI . EradicateNames ( SC , u )          ;
        break                                     ;
      }                                           ;
    }                                             ;
    SC . close  ( )                               ;
  }                                               ;
  SC   . remove ( )                               ;
  /////////////////////////////////////////////////
  startup       ( )                               ;
}

void N::NameComplex::Eradicate(void)
{
  start ( 10003 ) ;
}

void N::NameComplex::FlushIndex(void)
{
  SqlConnection SC ( plan -> sql )                     ;
  if ( SC . open ( FunctionString ) )                  {
    QString Q                                          ;
    UUIDs   U                                          ;
    ////////////////////////////////////////////////////
    for (int i = 0 ; i < topLevelItemCount ( ) ; i++ ) {
      QTreeWidgetItem * it = topLevelItem ( i      )   ;
      SUID              tu = nTreeUuid    ( it , 0 )   ;
      int               m  = nTreeInt     ( it , 2 )   ;
      if ( 2 == m ) U << tu                            ;
    }                                                  ;
    ////////////////////////////////////////////////////
    if ( U . count ( ) > 0 )                           {
      for (int i = 0 ; i < U . count ( ) ; i++ )       {
        Q = SC . sql . Update                          (
              PlanTable(NameMaps)                      ,
              SC . sql . Where ( 2 , "uuid" , "name" ) ,
              1                                        ,
              "prefer"                               ) ;
        SC . Prepare ( Q                             ) ;
        SC . Bind    ( "uuid"   , ObjectUuid ( )     ) ;
        SC . Bind    ( "name"   , U [ i ]            ) ;
        SC . Bind    ( "prefer" , i + 1              ) ;
        SC . Exec    (                               ) ;
      }                                                ;
    }                                                  ;
    ////////////////////////////////////////////////////
    SC . close       (                               ) ;
  }                                                    ;
  SC   . remove      (                               ) ;
}

void N::NameComplex::ItemUp(void)
{
  QTreeWidgetItem * it = currentItem ( )  ;
  if ( IsNull ( it ) ) return             ;
  /////////////////////////////////////////
  int m = nTreeInt ( it , 2 )             ;
  if ( 2 != m ) return                    ;
  /////////////////////////////////////////
  int  index = indexOfTopLevelItem ( it ) ;
  if ( index <= 0 ) return                ;
  /////////////////////////////////////////
  takeTopLevelItem   ( index          )   ;
  insertTopLevelItem ( index - 1 , it )   ;
  setCurrentItem     ( it             )   ;
  FlushIndex         (                )   ;
  /////////////////////////////////////////
  Alert              ( Click          )   ;
}

void N::NameComplex::ItemDown(void)
{
  QTreeWidgetItem * it = currentItem ( )       ;
  if ( IsNull ( it ) ) return                  ;
  //////////////////////////////////////////////
  int m = nTreeInt ( it , 2 )                  ;
  if ( 2 != m ) return                         ;
  //////////////////////////////////////////////
  int  index  = indexOfTopLevelItem ( it ) + 1 ;
  if ( index >= topLevelItemCount ( ) ) return ;
  //////////////////////////////////////////////
  takeTopLevelItem   ( index - 1  )            ;
  insertTopLevelItem ( index , it )            ;
  setCurrentItem     ( it         )            ;
  FlushIndex         (            )            ;
  //////////////////////////////////////////////
  Alert              ( Click      )            ;
}

void N::NameComplex::singleClicked(QTreeWidgetItem * item,int column)
{
  if ( IsNull ( item )                                     ) return ;
  if ( ( item == ItemEditing ) && ( column == ItemColumn ) ) return ;
  removeOldItem       (                               )             ;
  alert               ( "Action" , item -> text ( 0 ) )             ;
}

void N::NameComplex::doubleClicked(QTreeWidgetItem * item,int column)
{
  removeOldItem       (        ) ;
  switch              ( column ) {
    case 0                       :
      setNameItem     ( item   ) ;
    break                        ;
    case 1                       :
      setLanguageItem ( item   ) ;
    break                        ;
    case 2                       :
      setMapsItem     ( item   ) ;
    break                        ;
    case 3                       :
      setUsageItem    ( item   ) ;
    break                        ;
  }                              ;
}

void N::NameComplex::removeOldItem(void)
{
  if ( IsNull ( ItemEditing ) ) return                  ;
  SUID    u    = nTreeUuid ( ItemEditing , ItemColumn ) ;
  QString name = ItemEditing -> text ( 0 )              ;
  removeItemWidget ( ItemEditing , ItemColumn )         ;
  if ( ( u <= 0 ) && ( name . length ( ) <= 0 ) )       {
    int index = indexOfTopLevelItem ( ItemEditing )     ;
    takeTopLevelItem                ( index       )     ;
  }                                                     ;
  ItemEditing = NULL                                    ;
  ItemWidget  = NULL                                    ;
  ItemColumn  = -1                                      ;
  if ( ! plan -> Booleans [ "Desktop" ] )               {
    allowGesture = true                                 ;
  }                                                     ;
}

void N::NameComplex::setNameItem(QTreeWidgetItem * item)
{
  QLineEdit * name = new QLineEdit ( this )     ;
  ItemEditing = item                            ;
  ItemColumn  = 0                               ;
  ItemWidget  = (QWidget *) name                ;
  name -> setText ( item -> text ( 0 ) )        ;
  setItemWidget   ( item , 0 , name    )        ;
  nConnect ( name , SIGNAL(editingFinished())   ,
             this , SLOT  (editingFinished()) ) ;
  name -> setFocus ( Qt::TabFocusReason )       ;
}

void N::NameComplex::setLanguageItem(QTreeWidgetItem * item)
{
  int         col  = 1                             ;
  int         id   = nTreeInt ( item , col )       ;
  QComboBox * lang = new QComboBox(this)           ;
  if (id<=0) id = plan -> LanguageId               ;
    lang  << plan->languages                       ;
  (*lang) <= id                                    ;
  ItemEditing = item                               ;
  ItemColumn  = col                                ;
  ItemWidget  = (QWidget *) lang                   ;
  setItemWidget ( item , col , lang )              ;
  nConnect ( lang , SIGNAL(activated      (int))   ,
             this , SLOT  (languageChanged(int)) ) ;
  lang -> setEditable ( true )                     ;
  lang -> showPopup   (      )                     ;
}

void N::NameComplex::setMapsItem(QTreeWidgetItem * item)
{
  int         col  = 2                         ;
  int         id   = nTreeInt ( item , col )   ;
  QComboBox * maps = new QComboBox(this)       ;
  N :: AddItems ( *maps , Maps )               ;
  (*maps) <= id                                ;
  ItemEditing = item                           ;
  ItemColumn  = col                            ;
  ItemWidget  = (QWidget *) maps               ;
  setItemWidget ( item , col , maps )          ;
  nConnect ( maps , SIGNAL(activated  (int))   ,
             this , SLOT  (mapsChanged(int)) ) ;
  maps -> showPopup ( )                        ;
}

void N::NameComplex::setUsageItem(QTreeWidgetItem * item)
{
  int         col   = 3                          ;
  int         id    = nTreeInt ( item , col )    ;
  QComboBox * usage = new QComboBox(this)        ;
  N :: AddItems ( *usage , Usages )              ;
  (*usage) <= id                                 ;
  ItemEditing = item                             ;
  ItemColumn  = col                              ;
  ItemWidget  = (QWidget *) usage                ;
  setItemWidget ( item , col , usage )           ;
  nConnect ( usage , SIGNAL(activated   (int))   ,
             this  , SLOT  (usageChanged(int)) ) ;
  usage -> showPopup ( )                         ;
}

void N::NameComplex::editingFinished(void)
{
  nIfSafe ( ItemWidget )                                             {
    QLineEdit * name = Casting ( QLineEdit,ItemWidget )              ;
    nIfSafe ( name ) ItemEditing -> setText ( 0 , name -> text ( ) ) ;
  }                                                                  ;
  itemFinalize  ( )                                                  ;
  removeOldItem ( )                                                  ;
}

void N::NameComplex::languageChanged(int index)
{
  comboChanged ( index , 1 ) ;
}

void N::NameComplex::mapsChanged(int index)
{
  comboChanged ( index , 2 ) ;
}

void N::NameComplex::usageChanged(int index)
{
  comboChanged ( index , 3 ) ;
}

void N::NameComplex::comboChanged(int index,int column)
{
  nIfSafe ( ItemWidget )                                             {
    QComboBox * combo = Casting ( QComboBox , ItemWidget )           ;
    nIfSafe ( combo )                                                {
      int v = combo -> itemData ( index , Qt::UserRole ) . toInt ( ) ;
      ItemEditing -> setData ( column , Qt::UserRole , v         )   ;
      ItemEditing -> setText ( column , combo -> currentText ( ) )   ;
    }                                                                ;
  }                                                                  ;
  itemFinalize  ( )                                                  ;
  removeOldItem ( )                                                  ;
}

int N::NameComplex::UpdateMaster(SqlConnection & SC,int id,int language,QString name)
{
  QByteArray B = name . toUtf8 ( )                                           ;
  QString    Q                                                               ;
  ////////////////////////////////////////////////////////////////////////////
  if ( id <= 0 )                                                             {
    //////////////////////////////////////////////////////////////////////////
    Q = SC . sql . InsertInto                                                (
          PlanTable(Names)                                                   ,
          4                                                                  ,
          "uuid"                                                             ,
          "language"                                                         ,
          "length"                                                           ,
          "name"                                                           ) ;
    SC . Prepare ( Q                                                       ) ;
    SC . Bind    ( "uuid"     , ObjectUuid ( )                             ) ;
    SC . Bind    ( "language" , language                                   ) ;
    SC . Bind    ( "length"   , B . size   ( )                             ) ;
    SC . Bind    ( "name"     , B                                          ) ;
    SC . Exec    (                                                         ) ;
    //////////////////////////////////////////////////////////////////////////
    Q = SC . sql . SelectFrom                                                (
          "id"                                                               ,
          PlanTable(Names)                                                   ,
          SC . sql . Where                                                   (
            4                                                                ,
            "uuid"                                                           ,
            "language"                                                       ,
            "length"                                                         ,
            "name"                                                         ) ,
          SC . OrderByDesc ( "id" )                                        ) ;
    SC . Prepare ( Q                                                       ) ;
    SC . Bind    ( "uuid"     , ObjectUuid ( )                             ) ;
    SC . Bind    ( "language" , language                                   ) ;
    SC . Bind    ( "length"   , B . size   ( )                             ) ;
    SC . Bind    ( "name"     , B                                          ) ;
    if ( SC . Exec ( ) && SC . Next ( ) )                                    {
      id = SC . Int ( 0 )                                                    ;
    } else id = -1                                                           ;
    //////////////////////////////////////////////////////////////////////////
  } else                                                                     {
    Q = SC . sql . Update                                                    (
          PlanTable(Names)                                                   ,
          QString ( "where `id` = %1" ) . arg ( id )                         ,
          4                                                                  ,
          "uuid"                                                             ,
          "language"                                                         ,
          "length"                                                           ,
          "name"                                                           ) ;
    SC . Prepare ( Q                                                       ) ;
    SC . Bind    ( "uuid"     , ObjectUuid ( )                             ) ;
    SC . Bind    ( "language" , language                                   ) ;
    SC . Bind    ( "length"   , B . size   ( )                             ) ;
    SC . Bind    ( "name"     , B                                          ) ;
    SC . Exec    (                                                         ) ;
  }                                                                          ;
  ////////////////////////////////////////////////////////////////////////////
  return id                                                                  ;
}

int N::NameComplex::MapIndex(QTreeWidgetItem * item)
{
  int id = 1                              ;
  for (int i=0;i<topLevelItemCount();i++) {
    QTreeWidgetItem * it                  ;
    it = topLevelItem ( i )               ;
    if ( it == item ) return id           ;
    int map = nTreeInt ( it , 2 )         ;
    if ( 2 == map ) id ++                 ;
  }                                       ;
  return id                               ;
}

void N::NameComplex::itemFinalize(void)
{
  SUID    u        = nTreeUuid           ( ItemEditing , 0 )                 ;
  QString name     = ItemEditing -> text ( 0               )                 ;
  int     language = nTreeInt            ( ItemEditing , 1 )                 ;
  int     map      = nTreeInt            ( ItemEditing , 2 )                 ;
  int     usage    = nTreeInt            ( ItemEditing , 3 )                 ;
  ////////////////////////////////////////////////////////////////////////////
  if ( 1 == map ) ItemEditing -> setText ( 3 , ""               )            ;
  if ( 2 == map ) ItemEditing -> setText ( 3 , Usages [ usage ] )            ;
  ////////////////////////////////////////////////////////////////////////////
  if ( name . length ( ) <= 0 ) return                                       ;
  if ( language          <= 0 ) return                                       ;
  if ( map               <= 0 ) return                                       ;
  ////////////////////////////////////////////////////////////////////////////
  SqlConnection SC               ( plan -> sql                             ) ;
  QString       Q                                                            ;
  int           id                                                           ;
  int           index                                                        ;
  if                             ( SC . open ( FunctionString )            ) {
    //////////////////////////////////////////////////////////////////////////
    switch                       ( map                                     ) {
      case 1                                                                 :
        //////////////////////////////////////////////////////////////////////
        id = UpdateMaster        ( SC , usage , language , name            ) ;
        if ( id > 0 )                                                        {
          ItemEditing -> setData ( 0 , Qt::UserRole , ObjectUuid ( )       ) ;
          ItemEditing -> setData ( 3 , Qt::UserRole , id                   ) ;
          Alert                  ( Done                                    ) ;
        } else                                                               {
          Alert                  ( Error                                   ) ;
        }                                                                    ;
        //////////////////////////////////////////////////////////////////////
      break                                                                  ;
      case 2                                                                 :
        //////////////////////////////////////////////////////////////////////
        if                       ( u <= 0                                  ) {
          ////////////////////////////////////////////////////////////////////
          u = SC . Unique ( PlanTable(MajorUuid)                             ,
                            "uuid"                                           ,
                            Unify::NamesHead                               ) ;
          SC . assureUuid ( PlanTable(MajorUuid) , u , Types::Name         ) ;
          SC . assureName ( PlanTable(Names)     , u , language , name     ) ;
          Q = SC . sql . InsertInto                                          (
                PlanTable(NameMaps)                                          ,
                3                                                            ,
                "uuid"                                                       ,
                "name"                                                       ,
                "prefer"                                                   ) ;
          SC . Prepare           ( Q                         )               ;
          SC . Bind              ( "uuid"   , ObjectUuid ( ) )               ;
          SC . Bind              ( "name"   , u              )               ;
          SC . Bind              ( "prefer" , 0              )               ;
          SC . Exec              (                           )               ;
          ItemEditing -> setData ( 0 , Qt::UserRole , u      )               ;
          ////////////////////////////////////////////////////////////////////
        } else                                                               {
          ////////////////////////////////////////////////////////////////////
          QByteArray B = name . toUtf8 ( )                                   ;
          Q = SC . sql . Update                                              (
                PlanTable(Names)                                             ,
                SC . sql . Where ( 1 , "uuid" )                              ,
                3                                                            ,
                "language"                                                   ,
                "length"                                                     ,
                "name"                                                     ) ;
          SC . Prepare ( Q                         )                         ;
          SC . Bind    ( "uuid"     , u            )                         ;
          SC . Bind    ( "language" , language     )                         ;
          SC . Bind    ( "length"   , B . size ( ) )                         ;
          SC . Bind    ( "name"     , B            )                         ;
          SC . Exec    (                           )                         ;
          ////////////////////////////////////////////////////////////////////
        }                                                                    ;
        //////////////////////////////////////////////////////////////////////
        index = MapIndex ( ItemEditing )                                     ;
        Q = SC . sql . Update                                                (
              PlanTable(NameMaps)                                            ,
              SC . sql . Where ( 2 , "uuid" , "name" )                       ,
              2                                                              ,
              "prefer"                                                       ,
              "usage"                                                      ) ;
        SC . Prepare ( Q                         )                           ;
        SC . Bind    ( "uuid"   , ObjectUuid ( ) )                           ;
        SC . Bind    ( "name"   , u              )                           ;
        SC . Bind    ( "prefer" , index          )                           ;
        SC . Bind    ( "usage"  , usage          )                           ;
        SC . Exec    (                           )                           ;
        SC . close   (                           )                           ;
        //////////////////////////////////////////////////////////////////////
        Alert        ( Done                      )                           ;
        //////////////////////////////////////////////////////////////////////
      break                                                                  ;
    }                                                                        ;
  }                                                                          ;
  SC   . remove      (                           )                           ;
  emit Update        ( this , ObjectUuid ( )     )                           ;
}

bool N::NameComplex::Menu(QPoint pos)
{ Q_UNUSED ( pos ) ;
  return true      ;
}
