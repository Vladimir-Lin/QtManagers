#include <qtmanagers.h>

N::SnapView:: SnapView        ( QWidget * parent , Plan * p )
            : ListDock        (           parent ,        p )
            , GroupItems      (                           p )
            , CommandSequence ( new QTimer ( this )         )
            , Commanding      ( false                       )
{
  WidgetClass   ;
  Configure ( ) ;
}

N::SnapView::~SnapView(void)
{
  if ( NULL != CommandSequence ) {
     CommandSequence -> stop ( ) ;
  }                              ;
  if ( NULL != Commando        ) {
     Commando        -> stop ( ) ;
  }                              ;
  while ( Commanding )           {
    Time::skip ( 10 )            ;
  }                              ;
}

void N::SnapView::setGui(QWidget * widget)
{
  Gui = widget                         ;
  Commando        -> setParent ( Gui ) ;
  CommandSequence -> setParent ( Gui ) ;
}

bool N::SnapView::hasItem(void)
{
  return ( Items.count()>0 ) ;
}

bool N::SnapView::startDrag(QMouseEvent * event)
{
  Items.clear()                                                 ;
  QListWidgetItem * atItem = itemAt(event->pos())               ;
  nKickOut ( IsNull(atItem)                           , false ) ;
  nKickOut ( !IsMask(event->buttons(),Qt::LeftButton) , false ) ;
  dragPoint = event->pos    ( )                                 ;
  Items     = selectedItems ( )                                 ;
  nKickOut ( !PassDragDrop                            , true  ) ;
  return false                                                  ;
}

bool N::SnapView::fetchDrag(QMouseEvent * event)
{
  nKickOut ( !IsMask(event->buttons(),Qt::LeftButton) , false ) ;
  QPoint pos = event->pos()                                     ;
  pos -= dragPoint                                              ;
  return ( pos.manhattanLength() > qApp->startDragDistance() )  ;
}

void N::SnapView::dragDone(Qt::DropAction dropIt,QMimeData * mime)
{
  ListWidget::dragDone(dropIt,mime)  ;
}

bool N::SnapView::finishDrag(QMouseEvent * event)
{ Q_UNUSED ( event )   ;
  if (Items.count()>0) {
    Items.clear()      ;
    return false       ;
  }                    ;
  Items.clear()        ;
  return true          ;
}

bool N::SnapView::acceptDrop(QWidget * source,const QMimeData * mime)
{
  QString    mtype                      ;
  if (source==this)                     {
    mtype = MimeType(mime)              ;
    if (mtype.length()<=0) return false ;
  } else                                {
    if (mime->hasImage()) return true   ;
    if (mime->hasUrls()) return true    ;
    mtype = MimeType(mime)              ;
    if (mtype.length()<=0) return false ;
  }                                     ;
  return true                           ;
}

bool N::SnapView::removeDrop(void)
{
  return true ;
}

void N::SnapView::Configure(void)
{
  setViewMode                  ( IconMode               )            ;
  setIconSize                  ( QSize(128,128)         )            ;
  setGridSize                  ( QSize(144,144)         )            ;
  setMovement                  ( Snap                   )            ;
  setResizeMode                ( Adjust                 )            ;
  setSelectionMode             ( ExtendedSelection      )            ;
  setWordWrap                  ( true                   )            ;
  setMouseTracking             ( true                   )            ;
  setWrapping                  ( true                   )            ;
  setTextElideMode             ( Qt::ElideNone          )            ;
  setHorizontalScrollBarPolicy ( Qt::ScrollBarAlwaysOff )            ;
  setVerticalScrollBarPolicy   ( Qt::ScrollBarAlwaysOff )            ;
  setMinimumSize               ( QSize(144,192)         )            ;
  setDragDropMode              ( DragDrop               )            ;
  plan->setFont                ( this                   )            ;
  ////////////////////////////////////////////////////////////////////
  LimitValues [    49 ] =   1                                        ;
  LimitValues [    60 ] =  10                                        ;
  LimitValues [    61 ] =  10                                        ;
  LimitValues [ 37714 ] = 300                                        ;
  ////////////////////////////////////////////////////////////////////
  CommandSequence -> setParent ( this )                              ;
  nConnect ( this            , SIGNAL ( SequenceStart  ( ) )         ,
             this            , SLOT   ( StartSequence  ( ) )       ) ;
  nConnect ( this            , SIGNAL ( SequenceStop   ( ) )         ,
             this            , SLOT   ( StopSequence   ( ) )       ) ;
  nConnect ( this            , SIGNAL ( TriggerCommand ( ) )         ,
             this            , SLOT   ( StartCommand   ( ) )       ) ;
  nConnect ( CommandSequence , SIGNAL ( timeout        ( ) )         ,
             this            , SLOT   ( CommandHandler ( ) )       ) ;
  nConnect ( this , SIGNAL (setIcon    (QImage*,QListWidgetItem*))   ,
             this , SLOT   (setItemIcon(QImage*,QListWidgetItem*)) ) ;
}

QListWidgetItem * N::SnapView::uuidAt(SUID uuid)
{
  for (int i=0;i<count();i++)      {
    QListWidgetItem * it = item(i) ;
    SUID suid = nListUuid (it)     ;
    if (suid==uuid) return it      ;
  }                                ;
  return NULL                      ;
}

bool N::SnapView::Shutdown(void)
{
  if ( ! isStopped() )      {
    stopForcely ( )         ;
    while ( ! isStopped() ) {
      Time::msleep(100)     ;
      qApp->processEvents() ;
    }                       ;
  }                         ;
  emit Leave ( this )       ;
  return true               ;
}

bool N::SnapView::Relocation(void)
{
  if (count()>Page())                                      {
    setHorizontalScrollBarPolicy (Qt::ScrollBarAsNeeded  ) ;
    setVerticalScrollBarPolicy   (Qt::ScrollBarAsNeeded  ) ;
  } else                                                   {
    setHorizontalScrollBarPolicy (Qt::ScrollBarAlwaysOff ) ;
    setVerticalScrollBarPolicy   (Qt::ScrollBarAlwaysOff ) ;
  }                                                        ;
  return false                                             ;
}

void N::SnapView::AnotherALERT(int command)
{
  if (LimitValues[49]<=0) return ;
  plan -> Play ( (SUID)command ) ;
}

void N::SnapView::NameThread(int from,int to)
{
  SqlConnection SC ( plan -> sql )      ;
  if (SC.open("SnapView","NameThread")) {
    setUuidsName ( SC , from , to )     ;
    SC . close   (                )     ;
  }                                     ;
  SC   . remove  (                )     ;
}

void N::SnapView::NameThread(VarArgs & V)
{
  if (V.count()<2) return                     ;
  NameThread ( V[0].toInt() , V[1].toInt() ) ;
}

void N::SnapView::NameThread(void)
{
  SqlConnection SC ( plan -> sql )      ;
  if (SC.open("SnapView","NameThread")) {
    setUuidsName ( SC )                 ;
    SC . close   (    )                 ;
  }                                     ;
  SC   . remove  (    )                 ;
}

void N::SnapView::NameThreads(int CommandId)
{
  NamesThreads ( CommandId ) ;
}

void N::SnapView::NamesThread(int from,int to)
{
  SqlConnection SC ( plan -> sql )       ;
  if (SC.open("SnapView","NamesThread")) {
    setUuidsNames ( SC , from , to )     ;
    SC . close    (                )     ;
  }                                      ;
  SC   . remove   (                )     ;
}

void N::SnapView::NamesThread(VarArgs & V)
{
  if (V.count()<2) return                     ;
  NamesThread ( V[0].toInt() , V[1].toInt() ) ;
}

void N::SnapView::NamesThread(void)
{
  SqlConnection SC ( plan -> sql )       ;
  if (SC.open("SnapView","NamesThread")) {
    setUuidsNames ( SC )                 ;
    SC . close    (    )                 ;
  }                                      ;
  SC   . remove   (    )                 ;
}

void N::SnapView::NamesThreads(int CommandId)
{
  VarArgs V                                      ;
  int     total    = count()                     ;
  int     page     = LimitValues [ 60 ]          ;
  int     threads  = LimitValues [ 61 ]          ;
  int     requires = ( total + page - 1 ) / page ;
  if (requires<threads) threads = requires       ;
  if (threads <= 0    ) threads = 1              ;
  ////////////////////////////////////////////////
  if (threads == 1    )                          {
    V << CommandId                               ;
    V << 0                                       ;
    V << total                                   ;
    addSequence          ( V )                   ;
    emit TriggerCommand  (   )                   ;
    return                                       ;
  }                                              ;
  ////////////////////////////////////////////////
  page = ( total + threads - 1 ) / threads       ;
  for (int i=0;i<threads;i++)                    {
    int start = i * page                         ;
    int end   = start + page                     ;
    if (end>count()) end = count()               ;
    V . clear ( )                                ;
    V << CommandId                               ;
    V << start                                   ;
    V << end                                     ;
    addSequence          ( V )                   ;
  }                                              ;
  emit TriggerCommand ( )                        ;
}

void N::SnapView::setItemIcon  (
       QImage          * image ,
       QListWidgetItem * item  )
{
  QSize          IS = iconSize (  ) ;
  PictureManager PM ( plan        ) ;
  QIcon icon = PM . Icon (image,IS) ;
  item -> setIcon ( icon )          ;
  delete image                      ;
}

void N::SnapView::ThumbsThread(int start,int end)
{
  if (start>=count()) return              ;
  if (start<=0      ) start = 0           ;
  if (end  >=count()) end   = count ( )   ;
  PictureManager PM ( plan        )       ;
  SqlConnection  SC ( plan -> sql )       ;
  if (SC.open("SnapView","ThumbsThread")) {
    startLoading ( )                      ;
    while (isLoading() && start<end)      {
      QListWidgetItem * it                ;
      SUID              u = 0             ;
      it = item        ( start )          ;
      if (NotNull(it))                    {
        u  = nListUuid ( it    )          ;
      }                                   ;
      start++                             ;
      if (u>0)                            {
        QImage * image                    ;
        image = PM . Thumb ( SC , u )     ;
        if ( NotNull(image) )             {
          emit setIcon( image , it )      ;
        }                                 ;
      }                                   ;
    }                                     ;
    stopLoading  ( )                      ;
    SC . close   ( )                      ;
  }                                       ;
  SC   . remove  ( )                      ;
}

void N::SnapView::ThumbsThread(VarArgs & V)
{
  if (V.count()<2) return                      ;
  ThumbsThread ( V[0].toInt() , V[1].toInt() ) ;
}

void N::SnapView::ThumbsThread(void)
{
  ThumbsThread ( 0 , count() ) ;
}

void N::SnapView::GoThumbsThreads (int CommandId)
{
  VarArgs V                                      ;
  int     total    = count()                     ;
  int     page     = LimitValues [ 60 ]          ;
  int     threads  = LimitValues [ 61 ]          ;
  int     requires = ( total + page - 1 ) / page ;
  if (requires<threads) threads = requires       ;
  if (threads <= 0    ) threads = 1              ;
  ////////////////////////////////////////////////
  if (threads == 1    )                          {
    V << CommandId                               ;
    V << 0                                       ;
    V << total                                   ;
    addSequence ( V )                            ;
    emit TriggerCommand ( )                      ;
    return                                       ;
  }                                              ;
  ////////////////////////////////////////////////
  page = ( total + threads - 1 ) / threads       ;
  for (int i=0;i<threads;i++)                    {
    int start = i * page                         ;
    int end   = start + page                     ;
    if (end>count()) end = count()               ;
    V . clear ( )                                ;
    V << CommandId                               ;
    V << start                                   ;
    V << end                                     ;
    addSequence ( V )                            ;
  }                                              ;
  emit TriggerCommand ( )                        ;
}

void N::SnapView::IconsThread(Group group,int start,int end)
{
  if (start>=count()) return             ;
  if (start<=0      ) start = 0          ;
  if (end  >=count()) end   = count ( )  ;
  PictureManager PM ( plan        )      ;
  GroupItems     GI ( plan        )      ;
  SqlConnection  SC ( plan -> sql )      ;
  GI . AutoMap    = true                 ;
  GI . GroupTable = GI.LookTable         (
                      group.t1           ,
                      group.t2           ,
                      group.relation   ) ;
  if (SC.open("SnapView","IconsThread")) {
    QString L                            ;
    QString O                            ;
    L  = SC . sql.Limit  ( 0 , 1       ) ;
    O  = SC . OrderByAsc ( "position"  ) ;
    startLoading ( )                     ;
    while (isLoading() && start<end)     {
      QListWidgetItem * it               ;
      SUID              u = 0            ;
      it = item        ( start )         ;
      if (NotNull(it))                   {
        u  = nListUuid ( it    )         ;
      }                                  ;
      start++                            ;
      if (u>0)                           {
        SUID     puid                    ;
        QImage * image                   ;
        puid = GI . FindSecond           (
                 SC                      ,
                 u                       ,
                 group.t1                ,
                 group.t2                ,
                 group.relation          ,
                 O                       ,
                 L                     ) ;
        image = PM . Thumb ( SC , puid ) ;
        if ( NotNull(image) )            {
          emit setIcon ( image , it )    ;
        }                                ;
      }                                  ;
    }                                    ;
    stopLoading  ( )                     ;
    SC . close   ( )                     ;
  }                                      ;
  SC   . remove  ( )                     ;
}

void N::SnapView::IconsThread(VarArgs & V)
{
  if (V.count()<3) return             ;
  Group group ( V )                   ;
  int   start = V[0].toInt()          ;
  int   end   = V[1].toInt()          ;
  IconsThread ( group , start , end ) ;
}

void N::SnapView::IconsThread(Group group)
{
  IconsThread ( group , 0 , count() ) ;
}

void N::SnapView::GoIconsThreads(int CommandId,Group group)
{
  VarArgs V                                      ;
  int     total    = count()                     ;
  int     page     = LimitValues [ 60 ]          ;
  int     threads  = LimitValues [ 61 ]          ;
  int     requires = ( total + page - 1 ) / page ;
  if (requires<threads) threads = requires       ;
  if (threads <= 0    ) threads = 1              ;
  ////////////////////////////////////////////////
  if (threads == 1    )                          {
    V << CommandId                               ;
    V << 0                                       ;
    V << total                                   ;
    group . addParaments ( V )                   ;
    addSequence          ( V )                   ;
    emit TriggerCommand  (   )                   ;
    return                                       ;
  }                                              ;
  ////////////////////////////////////////////////
  page = ( total + threads - 1 ) / threads       ;
  for (int i=0;i<threads;i++)                    {
    int start = i * page                         ;
    int end   = start + page                     ;
    if (end>count()) end = count()               ;
    V . clear ( )                                ;
    V << CommandId                               ;
    V << start                                   ;
    V << end                                     ;
    group . addParaments ( V )                   ;
    addSequence          ( V )                   ;
  }                                              ;
  emit TriggerCommand ( )                        ;
}

void N::SnapView::JoinGroupThread(Group group,UUIDs & Uuids)
{
  SqlConnection  SC ( plan -> sql )          ;
  if (SC.open("SnapView","JoinGroupThread")) {
    startLoading ( )                         ;
    GroupItems :: Join                       (
      SC                                     ,
      group.first                            ,
      group.t1                               ,
      group.t2                               ,
      group.relation                         ,
      group.position                         ,
      Uuids                                  ,
      false                                ) ;
    stopLoading  ( )                         ;
    SC . close   ( )                         ;
  }                                          ;
  SC   . remove  ( )                         ;
}

void N::SnapView::JoinOwnerThread (Group group,UUIDs & Uuids)
{
  SqlConnection  SC ( plan -> sql )          ;
  if (SC.open("SnapView","JoinOwnerThread")) {
    startLoading ( )                         ;
    GroupItems :: JoinOwner                  (
      SC                                     ,
      group.second                           ,
      group.t1                               ,
      group.t2                               ,
      group.relation                         ,
      Uuids                                  ,
      false                                ) ;
    stopLoading  ( )                         ;
    SC . close   ( )                         ;
  }                                          ;
  SC   . remove  ( )                         ;
}

#define LoadingID   1000001

void N::SnapView::IsCompleted(void)
{
  if ( LimitValues [ LoadingID ] > 1 ) return ;
  update (      )                             ;
  Alert  ( Done )                             ;
}

bool N::SnapView::ReportDetails(SqlConnection & Connection,QListWidgetItem * item,int flags)
{
  return false ;
}

void N::SnapView::DetailsThread(int flags)
{
  SqlConnection  SC ( plan -> sql )        ;
  if (SC.open("SnapView","DetailsThread")) {
    int i = 0                              ;
    startLoading ( )                       ;
    while (isLoading() && i<count())       {
      QListWidgetItem * it                 ;
      SUID              u                  ;
      it = item      ( i  )                ;
      u  = nListUuid ( it )                ;
      i++                                  ;
      if (u>0)                             {
        ReportDetails ( SC , it , flags )  ;
      }                                    ;
    }                                      ;
    stopLoading  ( )                       ;
    SC . close   ( )                       ;
  }                                        ;
  SC   . remove  ( )                       ;
}

void N::SnapView::PreferLanguage(SUID u,int t,int l)
{
  SqlConnection  SC ( plan -> sql )                 ;
  if (SC.open("SnapView","PreferLanguage"))         {
    startLoading                 (                ) ;
    GroupItems :: assignLanguage ( SC , u , t , l ) ;
    stopLoading                  (                ) ;
    SC . close                   (                ) ;
  }                                                 ;
  Alert                          ( Done           ) ;
  vLanguageId = l                                   ;
}

void N::SnapView::PreferLanguage(VarArgs & V)
{
  if ( V.count() < 3 ) return        ;
  SUID u = V [ 0 ] . toULongLong ( ) ;
  int  t = V [ 1 ] . toInt       ( ) ;
  int  l = V [ 2 ] . toInt       ( ) ;
  PreferLanguage ( u , t , l )       ;
}

bool N::SnapView::startup(void)
{
  return false ;
}

void N::SnapView::StartSequence(void)
{
  nDropOut ( Commanding )                            ;
  CommandSequence -> start ( LimitValues [ 37714 ] ) ;
}

void N::SnapView::StopSequence(void)
{
  CommandSequence -> stop ( ) ;
}

void N::SnapView::StartCommand(void)
{
  nDropOut ( Commanding ) ;
  emit SequenceStart ( )  ;
}

void N::SnapView::CommandHandler(void)
{
  nDropOut ( Commanding )         ;
  Commanding = true               ;
  while ( Sequences.count() > 0 ) {
    RunCommand ( Sequences[0] )   ;
    Sequences . takeAt ( 0 )      ;
  }                               ;
  Commanding = false              ;
  emit SequenceStop ( )           ;
}

bool N::SnapView::RunCommand(VarArgs & arguments)
{
  if (arguments.count()<1) return false ;
  VarArgs V = arguments                 ;
  UUIDs   U                             ;
  int     c = V [ 0 ] . toInt ( )       ;
  switch         ( c     )              {
    case 10001                          :
      startup    (       )              ;
    break                               ;
    case 10011                          :
    case 10012                          :
    case 11001                          :
    case 30002                          :
    case 30003                          :
      V . takeAt ( 0     )              ;
      start      ( c , V )              ;
    break                               ;
    case 12001                          :
      ArgsToUuids ( 1 , V , U )         ;
      plan -> processEvents ( )         ;
      setSelections ( U )               ;
    break                               ;
    default                             :
    return false                        ;
  }                                     ;
  return true                           ;
}

void N::SnapView::pendingStartup(void)
{
  addSequence         ( 10001 ) ;
  emit TriggerCommand (       ) ;
}
