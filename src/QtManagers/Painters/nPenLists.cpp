#include <qtmanagers.h>

N::PenLists:: PenLists (QWidget * parent,Plan * p)
            : TreeDock (          parent,       p)
{
  WidgetClass   ;
  Configure ( ) ;
}

N::PenLists::~PenLists(void)
{
}

void N::PenLists::setIcon(Pen & pen,QTreeWidgetItem * item)
{
  QRect    VR = visualItemRect (item ) ;
  QSize    IR(VR.height(),VR.height()) ;
  QPixmap  IC(IR                     ) ;
  QPainter p                           ;
  IC. fill   ( QColor (255,255,255)  ) ;
  p . begin  ( &IC  )                  ;
  p . setPen (  pen )                  ;
  for (int y=0;y<IR.height();y++)      {
    p . drawLine( 0,y,IR.width()-1,y ) ;
  }                                    ;
  p . end    (      )                  ;
  item -> setIcon ( 0 , QIcon ( IC ) ) ;
}

void N::PenLists::assurePen(QTreeWidgetItem * item,QByteArray * d)
{
  if ( NULL == d ) return ;
  N::Pen p                ;
  p . setConfigure ( *d ) ;
  delete d                ;
  setIcon ( p , item )    ;
}

QMimeData * N::PenLists::dragMime (void)
{
  QTreeWidgetItem * IT = currentItem()            ;
  if (IsNull(IT)) return NULL                     ;
  SUID  uuid  = nTreeUuid(IT,0)                   ;
  QMimeData * mime = new QMimeData()              ;
  setMime ( mime , "pen/uuid"  , uuid )           ;
  if (NotNull(IT))                                {
    QIcon icon  = IT->icon(0)                     ;
    if (!icon.isNull())                           {
      QRect VR = visualItemRect ( IT )            ;
      QSize s (VR.height(),VR.height())           ;
      QImage image = icon . pixmap(s) . toImage() ;
      if (!image.isNull())                        {
        mime -> setImageData(image)               ;
      }                                           ;
    }                                             ;
  }                                               ;
  return mime                                     ;
}

bool N::PenLists::hasItem(void)
{
  QTreeWidgetItem * item = currentItem () ;
  return NotNull ( item )                 ;
}

bool N::PenLists::startDrag(QMouseEvent * event)
{
  QTreeWidgetItem * atItem = itemAt(event->pos())            ;
  if (IsNull(atItem)) return false                           ;
  if (!IsMask(event->buttons(),Qt::LeftButton)) return false ;
  dragPoint = event->pos()                                   ;
  if (!atItem->isSelected()) return false                    ;
  if (!PassDragDrop) return true                             ;
  return true                                                ;
}

bool N::PenLists::fetchDrag(QMouseEvent * event)
{
  if (!IsMask(event->buttons(),Qt::LeftButton)) return false   ;
  QPoint pos = event->pos()                                    ;
  pos -= dragPoint                                             ;
  return ( pos.manhattanLength() > qApp->startDragDistance() ) ;
}

void N::PenLists::dragDone(Qt::DropAction dropIt,QMimeData * mime)
{
}

bool N::PenLists::finishDrag(QMouseEvent * event)
{
  return true   ;
}

void N::PenLists::Configure(void)
{
  setAccessibleName            ( "N::PenLists"             )            ;
  setObjectName                ( "N::PenLists"             )            ;
  NewTreeWidgetItem            ( head                      )            ;
  head -> setText              ( 0 , tr("Pen configure")   )            ;
  setWindowTitle               ( tr("Pen configure lists") )            ;
  setDragDropMode              ( DragOnly                  )            ;
  setRootIsDecorated           ( false                     )            ;
  setAlternatingRowColors      ( true                      )            ;
  setSelectionMode             ( SingleSelection           )            ;
  setColumnCount               ( 1                         )            ;
  setHorizontalScrollBarPolicy ( Qt::ScrollBarAsNeeded     )            ;
  setVerticalScrollBarPolicy   ( Qt::ScrollBarAsNeeded     )            ;
  assignHeaderItems            ( head                      )            ;
  plan -> setFont              ( this                      )            ;
  PassDragDrop = false                                                  ;
  ///////////////////////////////////////////////////////////////////////
  nConnect ( this , SIGNAL (assignPen (QTreeWidgetItem*,QByteArray*))   ,
             this , SLOT   (assurePen (QTreeWidgetItem*,QByteArray*)) ) ;
}

bool N::PenLists::FocusIn(void)
{
  nKickOut          ( IsNull ( plan ) , false           ) ;
  DisableAllActions (                                   ) ;
  AssignAction      ( Label           , windowTitle ( ) ) ;
  LinkAction        ( Insert          , Insert      ( ) ) ;
  LinkAction        ( Delete          , Delete      ( ) ) ;
  LinkAction        ( Copy            , Copy        ( ) ) ;
  LinkAction        ( SelectNone      , SelectNone  ( ) ) ;
  LinkAction        ( SelectAll       , SelectAll   ( ) ) ;
  return true                                             ;
}

bool N::PenLists::ItemRunner(int T,ThreadData * d)
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

void N::PenLists::run(int T,ThreadData * d)
{
  nDropOut   ( ! IsContinue ( d ) ) ;
  ItemRunner ( T , d              ) ;
}

void N::PenLists::Load(ThreadData * d)
{
  nDropOut   ( ! IsContinue ( d ) )               ;
  WaitClear  (                    )               ;
  Bustle     (                    )               ;
  ConnectSQL ( SC , plan -> sql   )               ;
    QString Q                                     ;
    QString name                                  ;
    SUID    uuid                                  ;
    UUIDs   Uuids                                 ;
    Uuids = SC . Uuids                            (
      PlanTable(Painters)                         ,
      "uuid"                                      ,
      QString("where type = %1 order by id asc"   )
      .arg(Graphics::Pen)                       ) ;
    foreach ( uuid , Uuids )                      {
      QByteArray * p = NULL                       ;
      NewTreeWidgetItem ( IT )                    ;
      name = SC.getName                           (
        PlanTable(Names)                          ,
        "uuid"                                    ,
        vLanguageId                               ,
        uuid                                    ) ;
      Q = SC . sql . SelectFrom                   (
            "paraments"                           ,
            PlanTable(Painters)                   ,
            SC . WhereUuid ( uuid )             ) ;
      if ( SC . Fetch ( Q ) )                     {
        p = new QByteArray ( )                    ;
        (*p) = SC . ByteArray ( 0 )               ;
      }                                           ;
      IT -> setData   ( 0 , Qt::UserRole , uuid ) ;
      IT -> setText   ( 0 , name                ) ;
      addTopLevelItem ( IT                      ) ;
      if ( NULL != p ) emit assignPen ( IT , p  ) ;
    }                                             ;
  DisconnectSQL ( SC                            ) ;
  Vacancy       (                               ) ;
  alert         ( "Done" , tr("Pens")           ) ;
}

bool N::PenLists::startup(void)
{
  start ( 10001 ) ;
  return true     ;
}

void N::PenLists::List(void)
{
  startup ( ) ;
}

void N::PenLists::Insert(void)
{
  emit Append ( this ) ;
}

void N::PenLists::Delete(void)
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
  if (SC.open("nPenLists"))            {
    GM.DeletePainter(SC,uuid)          ;
    SC.close()                         ;
  } else                               {
  }                                    ;
  SC.remove()                          ;
  AbstractGui :: Mutex . unlock ( )    ;
  Alert ( Done )                       ;
}

void N::PenLists::Update(QString name,Pen * pen)
{
  QByteArray fconf = pen->Configure()                     ;
  SUID       uuid  = pen->uuid                            ;
  AbstractGui :: Mutex . lock ( )                         ;
  SqlConnection SC ( plan->sql )                          ;
  if (SC.open("nPenLists","Update"))                      {
    QString Q                                             ;
    if (uuid<=0)                                          {
      NewTreeWidgetItem ( IT )                            ;
      uuid = SC.Unique                                    (
        plan->Tables[Tables::MajorUuid]                   ,
        "uuid"                                     )      ;
      SC.assureUuid                                       (
        plan->Tables[Tables::MajorUuid]                   ,
        uuid,Types::Painting                       )      ;
      Q = SC.sql.InsertInto                               (
            plan->Tables[Tables::Painters]                ,
            3,"uuid","type","paraments"            )      ;
      SC.Prepare ( Q                               )      ;
      SC.Bind    ("uuid",uuid                      )      ;
      SC.Bind    ("type",Graphics::Pen             )      ;
      SC.Bind    ("paraments",fconf                )      ;
      SC.Exec    (                                 )      ;
      Q = SC.NameSyntax                                   (
            plan->Tables[Tables::Names]                   ,
            "uuid,language,name"                   )      ;
      SC.insertName(Q,uuid,vLanguageId,name        )      ;
      Pen P                                               ;
      P.setConfigure( fconf                        )      ;
      IT -> setData ( 0 , Qt::UserRole , uuid      )      ;
      IT -> setText ( 0 , name                     )      ;
      addTopLevelItem ( IT )                              ;
    } else                                                {
      QTreeWidgetItem * IT = NULL                         ;
      for (int i=0;IT==NULL && i<topLevelItemCount();i++) {
        QTreeWidgetItem * XT = topLevelItem (i)           ;
        SUID xuid = nTreeUuid ( XT , 0 )                  ;
        if (uuid==xuid) IT = XT                           ;
      }                                                   ;
      Q = SC.sql.Update                                   (
            plan->Tables[Tables::Painters]      ,
            QString("where uuid = :UUID"           )      ,
            2,"type","paraments"                   )      ;
      SC.Prepare ( Q                               )      ;
      SC.Bind    ("uuid",uuid                      )      ;
      SC.Bind    ("type",Graphics::Pen   )      ;
      SC.Bind    ("paraments",fconf                )      ;
      SC.Exec    (                                 )      ;
      Q = SC.sql.Update                                   (
            plan->Tables[Tables::Names]         ,
            QString("where uuid = :UUID"           )      ,
            2,"language","name"                    )      ;
      SC.insertName(Q,uuid,vLanguageId,name        )      ;
      if (NotNull(IT))                                    {
        Pen P                                             ;
        P.setConfigure( fconf                      )      ;
        IT -> setData ( 0 , Qt::UserRole , uuid    )      ;
        IT -> setText ( 0 , name                   )      ;
      }                                                   ;
    }                                                     ;
    SC.close()                                            ;
  } else                                                  {
  }                                                       ;
  SC.remove()                                             ;
  AbstractGui::Mutex.unlock()                             ;
}

void N::PenLists::Copy(void)
{
  QMimeData * mime = dragMime    (      ) ;
  if (IsNull(mime)) return                ;
  qApp->clipboard()->setMimeData ( mime ) ;
}

bool N::PenLists::Menu(QPoint pos)
{
  QTreeWidgetItem * IT     = currentItem()                           ;
  MenuManager       mm (this)                                        ;
  QAction         * aa                                               ;
  if ( NotNull ( IT ) ) mm . add ( 101 , tr("Edit"  ) )              ;
  if ( NotNull ( IT ) ) mm . add ( 201 , tr("Export") )              ;
  mm . add ( 102 , tr("Refresh") )                                   ;
  DockingMenu     ( mm      )                                        ;
  mm . setFont    ( plan    )                                        ;
  aa = mm . exec  (         )                                        ;
  if ( IsNull     (      aa ) ) return true                          ;
  if ( RunDocking ( mm , aa ) ) return true                          ;
  switch ( mm [ aa ] )                                               {
    case     101                                                     :
      if (NotNull(IT))                                               {
        SUID uuid = nTreeUuid ( IT , 0 )                             ;
        if (uuid>0)                                                  {
          Pen P                                                      ;
          SqlConnection SC(plan->sql)                                ;
          if ( SC . open ( FunctionString ) )                        {
            QString Q                                                ;
            Q = SC . sql . SelectFrom                                (
                  "paraments"                                        ,
                  PlanTable(Painters)                                ,
                  SC . WhereUuid ( uuid )                          ) ;
            if ( SC . Fetch ( Q ) )                                  {
              QByteArray conf = SC . Value ( 0 ) . toByteArray ( )   ;
              P . setConfigure ( conf )                              ;
            }                                                        ;
            SC.close()                                               ;
          }                                                          ;
          SC.remove()                                                ;
          emit EditPen ( this , IT -> text ( 0 ) , &P )              ;
        }                                                            ;
      }                                                              ;
    break                                                            ;
    case 102                                                         :
      List ( )                                                       ;
    break                                                            ;
    case 201                                                         :
      if                      ( NotNull ( IT )                 )     {
        SUID uuid = nTreeUuid ( IT , 0                         )     ;
        if                    ( uuid > 0                       )     {
          Pen pen                                                    ;
          GraphicsManager GM  ( plan                           )     ;
          EnterSQL            ( SC   , plan -> sql             )     ;
            pen = GM . GetPen ( SC   , uuid                    )     ;
          LeaveSQL            ( SC   , plan -> sql             )     ;
          emit Export         ( this , IT -> text ( 0 ) , &pen )     ;
        }                                                            ;
      }                                                              ;
    break                                                            ;
    default                                                          :
    break                                                            ;
  }                                                                  ;
  return true                                                        ;
}
