#include <qtmanagers.h>

N::FontLists:: FontLists (QWidget * parent,Plan * p)
             : TreeDock  (          parent,       p)
{
  WidgetClass   ;
  Configure ( ) ;
}

N::FontLists::~FontLists(void)
{
}

QMimeData * N::FontLists::dragMime (void)
{
  QTreeWidgetItem * IT = currentItem()   ;
  if (IsNull(IT)) return NULL            ;
  SUID  uuid  = nTreeUuid(IT,0)          ;
  QMimeData * mime = new QMimeData()     ;
  setMime ( mime , "font/uuid"  , uuid ) ;
  return mime                            ;
}

bool N::FontLists::hasItem(void)
{
  QTreeWidgetItem * item = currentItem () ;
  return NotNull ( item )                 ;
}

bool N::FontLists::startDrag(QMouseEvent * event)
{
  QTreeWidgetItem * atItem = itemAt(event->pos())            ;
  if (IsNull(atItem)) return false                           ;
  if (!IsMask(event->buttons(),Qt::LeftButton)) return false ;
  dragPoint = event->pos()                                   ;
  if (!atItem->isSelected()) return false                    ;
  if (!PassDragDrop) return true                             ;
  return true                                                ;
}

bool N::FontLists::fetchDrag(QMouseEvent * event)
{
  if (!IsMask(event->buttons(),Qt::LeftButton)) return false   ;
  QPoint pos = event->pos()                                    ;
  pos -= dragPoint                                             ;
  return ( pos.manhattanLength() > qApp->startDragDistance() ) ;
}

void N::FontLists::dragDone(Qt::DropAction dropIt,QMimeData * mime)
{
}

bool N::FontLists::finishDrag(QMouseEvent * event)
{
  return true   ;
}

void N::FontLists::Configure(void)
{
  setAccessibleName            ( "N::FontLists"             ) ;
  setObjectName                ( "N::FontLists"             ) ;
  NewTreeWidgetItem            ( head                       ) ;
  head -> setText              ( 0 , tr("Font configure")   ) ;
  setWindowTitle               ( tr("Font configure lists") ) ;
  setDragDropMode              ( DragOnly                   ) ;
  setRootIsDecorated           ( false                      ) ;
  setAlternatingRowColors      ( true                       ) ;
  setSelectionMode             ( SingleSelection            ) ;
  setColumnCount               ( 1                          ) ;
  setHorizontalScrollBarPolicy ( Qt::ScrollBarAsNeeded      ) ;
  setVerticalScrollBarPolicy   ( Qt::ScrollBarAsNeeded      ) ;
  assignHeaderItems            ( head                       ) ;
  plan -> setFont              ( this                       ) ;
}

bool N::FontLists::FocusIn(void)
{
  nKickOut          ( IsNull ( plan ) , false           ) ;
  DisableAllActions (                                   ) ;
  AssignAction      ( Label           , windowTitle ( ) ) ;
  LinkAction        ( Insert          , Insert      ( ) ) ;
  LinkAction        ( Delete          , Delete      ( ) ) ;
  LinkAction        ( SelectNone      , SelectNone  ( ) ) ;
  LinkAction        ( SelectAll       , SelectAll   ( ) ) ;
  return true                                             ;
}

bool N::FontLists::ItemRunner(int T,ThreadData * d)
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

void N::FontLists::run(int T,ThreadData * d)
{
  nDropOut         ( ! IsContinue ( d ) ) ;
  switch           ( T                  ) {
    case 10001                            :
      startLoading (                    ) ;
      Load         ( d                  ) ;
      stopLoading  (                    ) ;
    break                                 ;
  }                                       ;
}

void N::FontLists::Load(ThreadData * d)
{
  nDropOut          ( ! IsContinue ( d )     ) ;
  WaitClear         (                        ) ;
  Bustle            (                        ) ;
  SqlConnection  SC ( plan -> sql            ) ;
  if ( SC . open    ( FunctionString       ) ) {
    QString Q                                  ;
    QString name                               ;
    SUID    uuid                               ;
    UUIDs   Uuids = SC.Uuids                   (
      PlanTable(Painters)                      ,
      "uuid"                                   ,
      QString("where type = %1 order by id asc")
      . arg ( Graphics::Font )               ) ;
    foreach (uuid,Uuids)                       {
      Font F                                   ;
      NewTreeWidgetItem ( IT )                 ;
      name = SC . getName                      (
        PlanTable(Names)                       ,
        "uuid"                                 ,
        vLanguageId                            ,
        uuid                                 ) ;
      Q = SC . sql . SelectFrom                (
            "paraments"                        ,
            PlanTable(Painters)                ,
            SC . WhereUuid ( uuid )          ) ;
      if ( SC . Fetch ( Q ) )                  {
        QByteArray conf = SC . ByteArray ( 0 ) ;
        F . setConfigure ( conf              ) ;
        F . setScreen    ( plan -> screen    ) ;
      }                                        ;
      IT -> setData   ( 0,Qt::UserRole,uuid  ) ;
      IT -> setText   ( 0,name               ) ;
      IT -> setFont   ( 0,F                  ) ;
      addTopLevelItem ( IT                   ) ;
    }                                          ;
    SC . close  (                            ) ;
  }                                            ;
  SC   . remove (                            ) ;
  Vacancy       (                            ) ;
  alert         ( "Done" , tr("Fonts")       ) ;
}

bool N::FontLists::startup(void)
{
  start ( 10001 ) ;
  return true     ;
}

void N::FontLists::List(void)
{
  startup ( ) ;
}

void N::FontLists::Insert(void)
{
  emit Append ( this ) ;
}

void N::FontLists::Delete(void)
{
  QTreeWidgetItem * IT = currentItem() ;
  if ( IsNull(IT) ) return             ;
  SUID uuid = nTreeUuid(IT,0)          ;
  if ( uuid <= 0  ) return             ;
  int index = indexOfTopLevelItem (IT) ;
  takeTopLevelItem ( index )           ;
  plan -> processEvents ()             ;
  AbstractGui :: Mutex . lock ( )      ;
  SqlConnection SC ( plan->sql )       ;
  GraphicsManager GM(plan)             ;
  if (SC.open("nFontLists"))           {
    GM.DeletePainter(SC,uuid)          ;
    SC.close()                         ;
  } else                               {
  }                                    ;
  SC.remove()                          ;
  AbstractGui :: Mutex . unlock ( )    ;
  Alert ( Done )                       ;
}

void N::FontLists::Update(QString name,Font * font)
{
  QByteArray fconf = font->Configure()                    ;
  SUID       uuid  = font->uuid                           ;
  AbstractGui :: Mutex . lock ( )                         ;
  SqlConnection SC ( plan->sql )                          ;
  if (SC.open("nFontLists","Update"))                     {
    QString Q                                             ;
    if (uuid<=0)                                          {
      NewTreeWidgetItem ( IT )                            ;
      uuid = SC.Unique(PlanTable(MajorUuid),"uuid" )      ;
      SC.assureUuid(PlanTable(MajorUuid),uuid             ,
        Types::Painting                  )      ;
      Q = SC.sql.InsertInto                               (
            PlanTable(Painters)                           ,
            3,"uuid","type","paraments"            )      ;
      SC.Prepare ( Q                               )      ;
      SC.Bind    ("uuid",uuid                      )      ;
      SC.Bind    ("type",Graphics::Font  )      ;
      SC.Bind    ("paraments",fconf                )      ;
      SC.Exec    (                                 )      ;
      SC.assureName                                       (
        PlanTable(Names)                                  ,
        uuid                                              ,
        vLanguageId                                       ,
        name                                       )      ;
      Font F                                              ;
      F.setConfigure( fconf                        )      ;
      F.setScreen   ( plan->screen                 )      ;
      IT -> setData ( 0 , Qt::UserRole , uuid      )      ;
      IT -> setText ( 0 , name                     )      ;
      IT -> setFont ( 0 , F                        )      ;
      addTopLevelItem ( IT )                              ;
    } else                                                {
      QTreeWidgetItem * IT = NULL                         ;
      for (int i=0;IT==NULL && i<topLevelItemCount();i++) {
        QTreeWidgetItem * XT = topLevelItem (i)           ;
        SUID xuid = nTreeUuid ( XT , 0 )                  ;
        if (uuid==xuid) IT = XT                           ;
      }                                                   ;
      Q = SC.sql.Update                                   (
            PlanTable(Painters)                           ,
            QString("where uuid = :UUID"           )      ,
            2,"type","paraments"                   )      ;
      SC.Prepare ( Q                               )      ;
      SC.Bind    ("uuid",uuid                      )      ;
      SC.Bind    ("type",Graphics::Font  )      ;
      SC.Bind    ("paraments",fconf                )      ;
      SC.Exec    (                                 )      ;
      SC.assureName                                       (
        PlanTable(Names)                                  ,
        uuid                                              ,
        vLanguageId                                       ,
        name                                       )      ;
      if (NotNull(IT))                                    {
        Font F                                            ;
        F.setConfigure( fconf                      )      ;
        F.setScreen   ( plan->screen               )      ;
        IT -> setData ( 0 , Qt::UserRole , uuid    )      ;
        IT -> setText ( 0 , name                   )      ;
        IT -> setFont ( 0 , F                      )      ;
      }                                                   ;
    }                                                     ;
    SC.close()                                            ;
  } else                                                  {
  }                                                       ;
  SC.remove()                                             ;
  AbstractGui :: Mutex . unlock ( )                       ;
}


bool N::FontLists::Menu(QPoint pos)
{
  QTreeWidgetItem * IT     = currentItem()                     ;
  MenuManager       mm (this)                                  ;
  QAction         * aa                                         ;
  if ( NotNull ( IT ) ) mm . add ( 101 , tr ( "Edit"   )     ) ;
  if ( NotNull ( IT ) ) mm . add ( 201 , tr ( "Export" )     ) ;
  mm . add        ( 102 , tr("Refresh") )                      ;
  DockingMenu     ( mm                  )                      ;
  mm . setFont    ( plan                )                      ;
  aa = mm . exec  (                     )                      ;
  if ( IsNull     (      aa ) ) return true                    ;
  if ( RunDocking ( mm , aa ) ) return true                    ;
  switch ( mm [ aa ] )                                         {
    case 101                                                   :
      if ( NotNull ( IT ) )                                    {
        SUID uuid = nTreeUuid ( IT , 0 )                       ;
        if ( uuid > 0 )                                        {
          Font F                                               ;
          SqlConnection SC ( plan -> sql )                     ;
          if ( SC . open ( FunctionString ) )                  {
            QString Q                                          ;
            Q = SC . sql . SelectFrom                          (
                  "paraments"                                  ,
                  PlanTable(Painters)                          ,
                  SC . WhereUuid ( uuid )                    ) ;
            if ( SC . Fetch ( Q ) )                            {
              QByteArray conf = SC . Value(0) . toByteArray () ;
              F . setConfigure ( conf )                        ;
            }                                                  ;
            SC . close  (                                    ) ;
          }                                                    ;
          SC   . remove (                                    ) ;
          emit EditFont ( this , IT -> text ( 0 ) , &F       ) ;
        }                                                      ;
      }                                                        ;
    break                                                      ;
    case 102                                                   :
      List ( )                                                 ;
    break                                                      ;
    case 201                                                   :
      if                        ( NotNull ( IT )             ) {
        SUID uuid = nTreeUuid   ( IT   , 0                   ) ;
        if                      ( uuid > 0                   ) {
          Font font                                            ;
          GraphicsManager GM    ( plan                       ) ;
          EnterSQL              ( SC   , plan -> sql         ) ;
            font = GM . GetFont ( SC   , uuid                ) ;
          LeaveSQL              ( SC   , plan -> sql         ) ;
          emit Export           ( this , IT->text(0) , &font ) ;
        }                                                      ;
      }                                                        ;
    break                                                      ;
    default                                                    :
    break                                                      ;
  }                                                            ;
  return true                                                  ;
}
