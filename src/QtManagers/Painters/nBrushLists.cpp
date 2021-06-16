#include <qtmanagers.h>

N::BrushLists:: BrushLists (QWidget * parent,Plan * p)
              : ListDock   (          parent,       p)
{
  WidgetClass   ;
  Configure ( ) ;
}

N::BrushLists::~BrushLists(void)
{
}

QIcon N::BrushLists::getIcon(Brush & brush)
{
  QSize    IR( 64 , 64               ) ;
  QRect    XR(  0 ,  0 , 64 , 64     ) ;
  QPixmap  IC( IR                    ) ;
  QPen     Pen (QColor(255,255,255)  ) ;
  QPainter p                           ;
  IC. fill     ( QColor(255,255,255) ) ;
  p . begin    ( &IC    )              ;
  p . setPen   (  Pen   )              ;
  p . setBrush (  brush )              ;
  p . drawRect (  XR    )              ;
  p . end      (        )              ;
  return QIcon ( IC     )              ;
}

void N::BrushLists::assureBrush(QListWidgetItem * item,QByteArray * d)
{
  if ( NULL == d ) return            ;
  N::Brush b                         ;
  b . setConfigure ( *d            ) ;
  item -> setIcon  ( getIcon ( b ) ) ;
}

void N::BrushLists::Configure(void)
{
  setAccessibleName            ( "N::BrushLists"             )               ;
  setObjectName                ( "N::BrushLists"             )               ;
  setWindowTitle               ( tr("Brush configure lists") )               ;
  setViewMode                  ( IconMode                    )               ;
  setIconSize                  ( QSize(64,64)                )               ;
  setGridSize                  ( QSize(80,96)                )               ;
  setMovement                  ( Snap                        )               ;
  setResizeMode                ( Adjust                      )               ;
  setSelectionMode             ( SingleSelection             )               ;
  setWordWrap                  ( true                        )               ;
  setMouseTracking             ( true                        )               ;
  setWrapping                  ( true                        )               ;
  setTextElideMode             ( Qt::ElideNone               )               ;
  setHorizontalScrollBarPolicy ( Qt::ScrollBarAsNeeded       )               ;
  setVerticalScrollBarPolicy   ( Qt::ScrollBarAsNeeded       )               ;
  setMinimumSize               ( QSize(128,200)              )               ;
  setDragDropMode              ( DragOnly                    )               ;
  plan -> setFont              ( this                        )               ;
  ////////////////////////////////////////////////////////////////////////////
  nConnect ( this , SIGNAL ( assignBrush (QListWidgetItem*,QByteArray*) )    ,
             this , SLOT   ( assureBrush (QListWidgetItem*,QByteArray*) )  ) ;
}

bool N::BrushLists::FocusIn(void)
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

QMimeData * N::BrushLists::dragMime(void)
{
  QListWidgetItem * IT = currentItem()            ;
  if (IsNull(IT)) return NULL                     ;
  SUID  uuid  = nListUuid(IT)                     ;
  QMimeData * mime = new QMimeData()              ;
  setMime ( mime , "brush/uuid"  , uuid )         ;
  if (NotNull(IT))                                {
    QIcon icon  = IT->icon()                      ;
    if (!icon.isNull())                           {
      QSize  s     = iconSize()                   ;
      QImage image = icon . pixmap(s) . toImage() ;
      if (!image.isNull())                        {
        mime -> setImageData(image)               ;
      }                                           ;
    }                                             ;
  }                                               ;
  return mime                                     ;
}

bool N::BrushLists::hasItem(void)
{
  QListWidgetItem * item = currentItem() ;
  return NotNull ( item )                ;
}

bool N::BrushLists::startDrag(QMouseEvent * event)
{
  QListWidgetItem * atItem = itemAt(event->pos())            ;
  if (IsNull(atItem)) return false                           ;
  if (!IsMask(event->buttons(),Qt::LeftButton)) return false ;
  dragPoint = event->pos()                                   ;
  if (!atItem->isSelected()) return false                    ;
  if (!PassDragDrop) return true                             ;
  return false                                               ;
}

bool N::BrushLists::fetchDrag(QMouseEvent * event)
{
  if (!IsMask(event->buttons(),Qt::LeftButton)) return false   ;
  QPoint pos = event->pos()                                    ;
  pos -= dragPoint                                             ;
  return ( pos.manhattanLength() > qApp->startDragDistance() ) ;
}

void N::BrushLists::dragDone(Qt::DropAction dropIt,QMimeData * mime)
{
}

bool N::BrushLists::finishDrag(QMouseEvent * event)
{
  return true   ;
}

bool N::BrushLists::ItemRunner(int T,ThreadData * d)
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

void N::BrushLists::run(int T,ThreadData * d)
{
  nDropOut   ( ! IsContinue ( d ) ) ;
  ItemRunner ( T , d              ) ;
}

void N::BrushLists::Load(ThreadData * d)
{
  nDropOut   ( ! IsContinue ( d ) )                  ;
  WaitClear  (                    )                  ;
  Bustle     (                    )                  ;
  ConnectSQL ( SC , plan -> sql   )                  ;
    QString Q                                        ;
    QString name                                     ;
    SUID    uuid                                     ;
    UUIDs   Uuids = SC . Uuids                       (
      PlanTable(Painters)                            ,
      "uuid"                                         ,
      QString("where type = %1 order by id asc"      )
      .arg(Graphics::Brush)                        ) ;
    foreach (uuid,Uuids)                             {
      QByteArray      * p    = NULL                  ;
      QListWidgetItem * item = NULL                  ;
      name = SC . getName                            (
        PlanTable(Names)                             ,
        "uuid"                                       ,
        vLanguageId                                  ,
        uuid                                       ) ;
      Q = SC . sql . SelectFrom                      (
            "paraments"                              ,
            PlanTable(Painters)                      ,
            SC . WhereUuid ( uuid )                ) ;
      if ( SC . Fetch ( Q ) )                        {
        p = new QByteArray ( )                       ;
        (*p) = SC . ByteArray ( 0 )                  ;
      }                                              ;
      item = new QListWidgetItem ( )                 ;
      blockSignals         ( true                  ) ;
      item -> setText      ( name                  ) ;
      item -> setData      ( Qt::UserRole , uuid   ) ;
      QListWidget::addItem ( item                  ) ;
      blockSignals         ( false                 ) ;
      if                   ( NULL != p             ) {
        emit assignBrush   ( item , p              ) ;
      }                                              ;
    }                                                ;
  DisconnectSQL ( SC                               ) ;
  Vacancy       (                                  ) ;
  alert         ( "Done" , tr("Brushes")           ) ;
}

bool N::BrushLists::startup(void)
{
  start ( 10001 ) ;
  return true     ;
}

void N::BrushLists::List(void)
{
  startup ( ) ;
}

void N::BrushLists::Insert(void)
{
  emit Append ( this ) ;
}

void N::BrushLists::Delete(void)
{
  QListWidgetItem * IT        ;
  IT = currentItem()          ;
  if ( IsNull(IT) ) return    ;
  SUID uuid = nListUuid(IT)   ;
  if ( uuid <= 0  ) return    ;
  takeItem ( row ( IT ) )     ;
  plan -> processEvents ()    ;
  GraphicsManager GM(plan)    ;
  EnterSQL ( SC , plan->sql ) ;
    GM.DeletePainter(SC,uuid) ;
  LeaveSQL ( SC , plan->sql ) ;
  Alert    ( Done           ) ;
}

void N::BrushLists::Copy(void)
{
  QMimeData * mime = dragMime    (      ) ;
  if (IsNull(mime)) return                ;
  qApp->clipboard()->setMimeData ( mime ) ;
}

bool N::BrushLists::Menu(QPoint pos)
{
  QListWidgetItem * item   = itemAt ( pos )                        ;
  MenuManager       mm (this)                                      ;
  QAction         * aa                                             ;
  mm . add                         ( 102 , tr("Refresh") )         ;
  if ( NotNull ( item ) ) mm . add ( 201 , tr("Export")  )         ;
  DockingMenu    ( mm                                            ) ;
  mm . setFont   ( plan                                          ) ;
  aa = mm . exec (                                               ) ;
  if ( IsNull     (      aa ) ) return true                        ;
  if ( RunDocking ( mm , aa ) ) return true                        ;
  switch ( mm [ aa ] )                                             {
    case 102                                                       :
      List ( )                                                     ;
    break                                                          ;
    case 201                                                       :
      if                          ( NotNull ( item )             ) {
        SUID uuid = nListUuid     ( item                         ) ;
        if                        ( uuid > 0                     ) {
          Brush brush                                              ;
          GraphicsManager GM      ( plan                         ) ;
          EnterSQL                ( SC   , plan -> sql           ) ;
            brush = GM . GetBrush ( SC   , uuid                  ) ;
          LeaveSQL                ( SC   , plan -> sql           ) ;
          emit Export             ( this , item->text() , &brush ) ;
        }                                                          ;
      }                                                            ;
    break                                                          ;
    default                                                        :
    break                                                          ;
  }                                                                ;
  return true                                                      ;
}
