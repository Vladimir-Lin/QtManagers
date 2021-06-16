#include <qtmanagers.h>

N::GradientLists:: GradientLists (QWidget * parent,Plan * p)
                 : ListDock      (          parent,       p)
{
  WidgetClass   ;
  Configure ( ) ;
}

N::GradientLists::~GradientLists(void)
{
}

QIcon N::GradientLists::getIcon(Gradient & gradient)
{
  QLinearGradient * linear = gradient . linear ( ) ;
  //////////////////////////////////////////////////
  if ( NotNull ( linear ) )                        {
    linear -> setStart     ( QPointF ( 0 ,  0 ) )  ;
    linear -> setFinalStop ( QPointF ( 0 , 64 ) )  ;
  }                                                ;
  //////////////////////////////////////////////////
  QSize    IR    ( 64 , 64             )           ;
  QRect    XR    (  0 ,  0 , 64 , 64   )           ;
  QPixmap  IC    ( IR                  )           ;
  QPen     Pen   ( QColor(255,255,255) )           ;
  QBrush   brush ( *gradient.gradient  )           ;
  QPainter p                                       ;
  IC . fill      ( QColor(255,255,255) )           ;
  p  . begin     ( &IC                 )           ;
  p  . setPen    ( Pen                 )           ;
  p  . setBrush  ( brush               )           ;
  p  . drawRect  ( XR                  )           ;
  p  . end       (                     )           ;
  return QIcon   ( IC                  )           ;
}

void N::GradientLists::assureGradient(QListWidgetItem * item,QByteArray * d)
{
  if ( NULL == d ) return            ;
  N::Gradient g                      ;
  g . setConfigure ( *d            ) ;
  item -> setIcon  ( getIcon ( g ) ) ;
  delete d                           ;
}

void N::GradientLists::Configure(void)
{
  setAccessibleName            ( "N::GradientLists"     )                    ;
  setObjectName                ( "N::GradientLists"     )                    ;
  setWindowTitle               ( tr("Gradient lists")   )                    ;
  setViewMode                  ( IconMode               )                    ;
  setIconSize                  ( QSize(64,64)           )                    ;
  setGridSize                  ( QSize(80,96)           )                    ;
  setMovement                  ( Snap                   )                    ;
  setResizeMode                ( Adjust                 )                    ;
  setSelectionMode             ( SingleSelection        )                    ;
  setWordWrap                  ( true                   )                    ;
  setMouseTracking             ( true                   )                    ;
  setWrapping                  ( true                   )                    ;
  setTextElideMode             ( Qt::ElideNone          )                    ;
  setHorizontalScrollBarPolicy ( Qt::ScrollBarAlwaysOff )                    ;
  setVerticalScrollBarPolicy   ( Qt::ScrollBarAlwaysOff )                    ;
  setMinimumSize               ( QSize(128,200)         )                    ;
  setDragDropMode              ( DragOnly               )                    ;
  plan -> setFont              ( this                   )                    ;
  ////////////////////////////////////////////////////////////////////////////
  nConnect ( this , SIGNAL ( assignGradient(QListWidgetItem*,QByteArray*))   ,
             this , SLOT   ( assureGradient(QListWidgetItem*,QByteArray*)) ) ;
}

bool N::GradientLists::FocusIn(void)
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

QMimeData * N::GradientLists::dragMime(void)
{
  QListWidgetItem * IT = currentItem()            ;
  if (IsNull(IT)) return NULL                     ;
  SUID  uuid  = nListUuid(IT)                     ;
  QMimeData * mime = new QMimeData()              ;
  setMime ( mime , "gradient/uuid"  , uuid )      ;
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

bool N::GradientLists::hasItem(void)
{
  QListWidgetItem * item = currentItem() ;
  return NotNull ( item )                ;
}

bool N::GradientLists::startDrag(QMouseEvent * event)
{
  QListWidgetItem * atItem = itemAt(event->pos())            ;
  if (IsNull(atItem)) return false                           ;
  if (!IsMask(event->buttons(),Qt::LeftButton)) return false ;
  dragPoint = event->pos()                                   ;
  if (!atItem->isSelected()) return false                    ;
  if (!PassDragDrop) return true                             ;
  return false                                               ;
}

bool N::GradientLists::fetchDrag(QMouseEvent * event)
{
  if (!IsMask(event->buttons(),Qt::LeftButton)) return false   ;
  QPoint pos = event->pos()                                    ;
  pos -= dragPoint                                             ;
  return ( pos.manhattanLength() > qApp->startDragDistance() ) ;
}

void N::GradientLists::dragDone(Qt::DropAction dropIt,QMimeData * mime)
{
}

bool N::GradientLists::finishDrag(QMouseEvent * event)
{
  return true   ;
}

bool N::GradientLists::ItemRunner(int T,ThreadData * d)
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

void N::GradientLists::run(int T,ThreadData * d)
{
  nDropOut   ( ! IsContinue ( d ) ) ;
  ItemRunner ( T , d              ) ;
}

void N::GradientLists::Load(ThreadData * d)
{
  nDropOut   ( ! IsContinue ( d ) )                  ;
  WaitClear  (                    )                  ;
  Bustle     (                    )                  ;
  ConnectSQL ( SC , plan -> sql   )                  ;
    QString Q                                        ;
    QString name                                     ;
    SUID    uuid                                     ;
    UUIDs   Uuids = SC.Uuids                         (
      PlanTable(Painters)                            ,
      "uuid"                                         ,
      QString("where type = %1 order by id asc"      )
      .arg(Graphics::Gradient)                     ) ;
    foreach ( uuid , Uuids )                         {
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
      blockSignals          ( true                 ) ;
      item -> setText       ( name                 ) ;
      item -> setData       ( Qt::UserRole , uuid  ) ;
      QListWidget::addItem  ( item                 ) ;
      blockSignals          ( false                ) ;
      if                    ( NULL != p            ) {
        emit assignGradient ( item , p             ) ;
      }                                              ;
    }                                                ;
  DisconnectSQL ( SC                               ) ;
  Vacancy       (                                  ) ;
  alert         ( "Done" , tr("Gradients")         ) ;
}

bool N::GradientLists::startup(void)
{
  start ( 10001 ) ;
  return true     ;
}

void N::GradientLists::List(void)
{
  startup ( ) ;
}

void N::GradientLists::Insert(void)
{
  emit Append ( this ) ;
}

void N::GradientLists::Delete(void)
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

void N::GradientLists::Copy(void)
{
  QMimeData * mime = dragMime    (      ) ;
  if (IsNull(mime)) return                ;
  qApp->clipboard()->setMimeData ( mime ) ;
}

bool N::GradientLists::Menu(QPoint pos)
{
  QListWidgetItem * item = itemAt ( pos )                            ;
  MenuManager       mm (this)                                        ;
  QAction         * aa                                               ;
  mm.add(102,tr("Refresh"))                                          ;
  if ( NotNull ( item ) ) mm . add ( 201 , tr("Export") )            ;
  DockingMenu     ( mm      )                                        ;
  mm . setFont    ( plan    )                                        ;
  aa = mm . exec  (         )                                        ;
  if ( IsNull     (      aa ) ) return true                          ;
  if ( RunDocking ( mm , aa ) ) return true                          ;
  switch ( mm [ aa ] )                                               {
    case 102                                                         :
      List ( )                                                       ;
    break                                                            ;
    case 201                                                         :
      if ( NotNull ( item ) )                                        {
        SUID uuid = nListUuid ( item )                               ;
        if ( uuid > 0 )                                              {
          Gradient gradient                                          ;
          GraphicsManager GM ( plan                                ) ;
          EnterSQL           ( SC   , plan -> sql                  ) ;
            gradient = GM . GetGradient ( SC , uuid                ) ;
          LeaveSQL           ( SC   , plan -> sql                  ) ;
          emit Export        ( this , item -> text ( ) , &gradient ) ;
        }                                                            ;
      }                                                              ;
    break                                                            ;
    default                                                          :
    break                                                            ;
  }                                                                  ;
  return true                                                        ;
}
