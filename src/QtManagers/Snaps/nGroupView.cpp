#include <qtmanagers.h>

N::GroupView:: GroupView       ( QWidget * parent , Plan * p )
             : IconDock        (           parent ,        p )
             , Object          ( 0, Types::None              )
             , GroupItems      (                           p )
             , CommandSequence ( new QTimer ( this )         )
             , display         ( ThumbIcon                   )
             , textBar         ( true                        )
             , dropAction      ( false                       )
             , Commanding      ( false                       )
{
  WidgetClass   ;
  Configure ( ) ;
}

N::GroupView::~GroupView (void)
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

void N::GroupView::setGui(QWidget * widget)
{
  Gui = widget                         ;
  Commando        -> setParent ( Gui ) ;
  CommandSequence -> setParent ( Gui ) ;
}

QListWidgetItem * N::GroupView::uuidAt(SUID u)
{
  for (int i=0;i<count();i++)      {
    QListWidgetItem * it = item(i) ;
    SUID suid = nListUuid (it)     ;
    if (suid==u) return it         ;
  }                                ;
  return NULL                      ;
}

bool N::GroupView::hasItem(void)
{
  return ( Items.count()>0 ) ;
}

bool N::GroupView::startDrag(QMouseEvent * event)
{
  Items . clear ( )                                          ;
  QListWidgetItem * atItem = itemAt(event->pos())            ;
  if (IsNull(atItem)) return false                           ;
  if (!IsMask(event->buttons(),Qt::LeftButton)) return false ;
  dragPoint = event->pos()                                   ;
  for (int i=0;i<count();i++)                                {
    if (item(i)->isSelected()) Items << item(i)              ;
  }                                                          ;
  return false                                               ;
}

bool N::GroupView::fetchDrag(QMouseEvent * event)
{
  if (!IsMask(event->buttons(),Qt::LeftButton)) return false   ;
  QPoint pos = event->pos()                                    ;
  pos -= dragPoint                                             ;
  return ( pos.manhattanLength() > qApp->startDragDistance() ) ;
}

QMimeData * N::GroupView::dragMime(void)
{
  PickedUuids . clear ( )                         ;
  if (Items.count()<=0) return NULL               ;
  QMimeData       * mime    = new QMimeData ( )   ;
  if (Items.count()==1)                           {
    SUID        t    = nListUuid(Items[0])        ;
    QByteArray  data = CreateByteArray(t)         ;
    mime -> setData ("division/uuid",data)        ;
  } else                                          {
    UUIDs       Uuids = Selected        (       ) ;
    QByteArray  data  = CreateByteArray ( Uuids ) ;
    mime -> setData ("division/uuids",data)       ;
    PickedUuids = Uuids                           ;
  }                                               ;
  QString tooltip                                 ;
  tooltip = tr("%1 items selected"                )
            .arg(Items.count()                  ) ;
  QToolTip :: showText ( QCursor::pos(),tooltip ) ;
  return mime                                     ;
}

void N::GroupView::dragDone(Qt::DropAction dropIt,QMimeData * mime)
{ Q_UNUSED ( dropIt ) ;
  Q_UNUSED ( mime   ) ;
}

bool N::GroupView::finishDrag(QMouseEvent * event)
{ Q_UNUSED ( event )   ;
  if (Items.count()>0) {
    Items . clear ( )  ;
    return false       ;
  }                    ;
  Items . clear   ( )  ;
  return true          ;
}

QString N::GroupView::MimeType(const QMimeData * mime)
{
  return VirtualGui::MimeType             (
           mime                           ,
           "album/uuid;album/uuids;"
           "video/uuid;video/uuids;"
           "picture/uuid;picture/uuids;"
           "people/uuid;people/uuids;"
           "division/uuid;division/uuids"
         )                                ;
}

UUIDs N::GroupView::MimeUuids(const QMimeData * mime,QString mimetype)
{
  UUIDs      Uuids                                           ;
  QByteArray data = mime->data(mimetype)                     ;
  if (data.size()<=0) return Uuids                           ;
  if (mimetype=="album/uuid"    ) Uuids << GetUuid  ( data ) ; else
  if (mimetype=="album/uuids"   ) Uuids  = GetUuids ( data ) ; else
  if (mimetype=="video/uuid"    ) Uuids << GetUuid  ( data ) ; else
  if (mimetype=="video/uuids"   ) Uuids  = GetUuids ( data ) ; else
  if (mimetype=="picture/uuid"  ) Uuids << GetUuid  ( data ) ; else
  if (mimetype=="picture/uuids" ) Uuids  = GetUuids ( data ) ; else
  if (mimetype=="people/uuid"   ) Uuids << GetUuid  ( data ) ; else
  if (mimetype=="people/uuids"  ) Uuids  = GetUuids ( data ) ; else
  if (mimetype=="division/uuid" ) Uuids << GetUuid  ( data ) ; else
  if (mimetype=="division/uuids") Uuids  = GetUuids ( data ) ;
  return Uuids                                               ;
}

bool N::GroupView::acceptDrop(QWidget * source,const QMimeData * mime)
{ Q_UNUSED ( source )         ;
  return dropHandler ( mime ) ;
}

bool N::GroupView::dropNew(QWidget * source,const QMimeData * mime,QPoint pos)
{
  nKickOut ( dropAction , false )                        ;
  QString    mtype                                       ;
  UUIDs      Uuids                                       ;
  Alert ( Click )                                        ;
  if (source==this)                                      {
    mtype = MimeType  (mime      )                       ;
    Uuids = MimeUuids (mime,mtype)                       ;
    plan->showMessage                                    (
      tr("Start moving %1 groups"                        )
                .arg(Uuids.count()))                     ;
    plan->processEvents()                                ;
  } else                                                 {
    if (mime->hasUrls())                                 {
    } else                                               {
      mtype = MimeType  (mime      )                     ;
      Uuids = MimeUuids (mime,mtype)                     ;
      if (mtype=="album/uuid"   || mtype=="album/uuids"  )
      plan->showMessage                                  (
        tr("Join %1 album from %2"                       )
        .arg(Uuids.count()                               )
        .arg(source->windowTitle())                    ) ;
      if (mtype=="picture/uuid" || mtype=="picture/uuids")
      plan->showMessage                                  (
        tr("Join %1 pictures from %2"                    )
        .arg(Uuids.count()                               )
        .arg(source->windowTitle())                    ) ;
      if (mtype=="people/uuid"  || mtype=="people/uuids" )
      plan->showMessage                                  (
        tr("Join %1 people from %2"                      )
        .arg(Uuids.count()                               )
        .arg(source->windowTitle())                    ) ;
      if (mtype=="video/uuid"   || mtype=="video/uuids"  )
      plan->showMessage                                  (
        tr("Join %1 video from %2"                       )
        .arg(Uuids.count()                               )
        .arg(source->windowTitle())                    ) ;
      plan->processEvents()                              ;
    }                                                    ;
  }                                                      ;
  return true                                            ;
}

bool N::GroupView::dropMoving(QWidget * source,const QMimeData * mime,QPoint pos)
{ Q_UNUSED ( mime                 )                         ;
  nKickOut ( dropAction   , false )                         ;
  nKickOut ( source!=this , true  )                         ;
  QListWidgetItem * atItem = itemAt(pos)                    ;
  if (IsNull (atItem)) return false                         ;
  if (NotNull(atItem) && atItem->isSelected()) return false ;
  return true                                               ;
}

bool N::GroupView::dropAppend(QWidget * source,const QMimeData * mime,QPoint pos)
{
  nKickOut ( dropAction , false )          ;
  return dropItems ( source , mime , pos ) ;
}

bool N::GroupView::removeDrop(void)
{
  return true ;
}

void N::GroupView::Configure(void)
{
  StackSize   = 0                                                                ;
  Reservation = false                                                            ;
  ////////////////////////////////////////////////////////////////////////////////
  setDragDropMode              ( DragDrop              )                         ;
  setViewMode                  ( IconMode              )                         ;
  setIconSize                  ( QSize(128,128)        )                         ;
  setGridSize                  ( QSize(144,180)        )                         ;
  setMovement                  ( Snap                  )                         ;
  setResizeMode                ( Adjust                )                         ;
  setSelectionMode             ( ExtendedSelection     )                         ;
  setWordWrap                  ( true                  )                         ;
  setMouseTracking             ( true                  )                         ;
  setWrapping                  ( true                  )                         ;
  setTextElideMode             ( Qt::ElideNone         )                         ;
  setHorizontalScrollBarPolicy ( Qt::ScrollBarAsNeeded )                         ;
  setVerticalScrollBarPolicy   ( Qt::ScrollBarAsNeeded )                         ;
  setMinimumSize               ( QSize(168,192)        )                         ;
  setDropFlag                  ( DropText     , true   )                         ;
  setDropFlag                  ( DropUrls     , true   )                         ;
  setDropFlag                  ( DropImage    , true   )                         ;
  setDropFlag                  ( DropHtml     , true   )                         ;
  setDropFlag                  ( DropColor    , true   )                         ;
  setDropFlag                  ( DropTag      , true   )                         ;
  setDropFlag                  ( DropPicture  , true   )                         ;
  setDropFlag                  ( DropPeople   , true   )                         ;
  setDropFlag                  ( DropVideo    , true   )                         ;
  setDropFlag                  ( DropAlbum    , true   )                         ;
  setDropFlag                  ( DropGender   , true   )                         ;
  setDropFlag                  ( DropDivision , true   )                         ;
  plan->setFont                ( this                  )                         ;
  MountCommands                (                       )                         ;
  ////////////////////////////////////////////////////////////////////////////////
  LimitValues [    49 ] =   1                                                    ;
  LimitValues [    60 ] =  10                                                    ;
  LimitValues [    61 ] =  10                                                    ;
  LimitValues [ 37714 ] = 300                                                    ;
  ////////////////////////////////////////////////////////////////////////////////
  CommandSequence -> setParent ( this )                                          ;
  nConnect ( this            , SIGNAL ( SequenceStart  ( ) )                     ,
             this            , SLOT   ( StartSequence  ( ) )                   ) ;
  nConnect ( this            , SIGNAL ( SequenceStop   ( ) )                     ,
             this            , SLOT   ( StopSequence   ( ) )                   ) ;
  nConnect ( this            , SIGNAL ( TriggerCommand ( ) )                     ,
             this            , SLOT   ( StartCommand   ( ) )                   ) ;
  nConnect ( CommandSequence , SIGNAL ( timeout        ( ) )                     ,
             this            , SLOT   ( CommandHandler ( ) )                   ) ;
  nConnect ( this            , SIGNAL (assignIcon (QListWidgetItem*,QImage*))    ,
             this            , SLOT   (setItemIcon(QListWidgetItem*,QImage*))  ) ;
}

bool N::GroupView::Shutdown(void)
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

bool N::GroupView::Relocation(void)
{
  return false ;
}

int N::GroupView::EachType(void)
{
  return type ;
}

UUIDs N::GroupView::GroupUuids(SqlConnection & connection)
{
  return GroupItems::Groups                    (
           connection                          ,
           (Types::ObjectTypes)type            ,
           connection.OrderByAsc("position") ) ;
}

QIcon N::GroupView::GetIcon(SqlConnection & connection,SUID u)
{
  PictureManager PM ( plan )               ;
  QIcon           ic                       ;
  QImage        * Thumb = NULL             ;
  switch (display)                         {
    case IconOnly                          :
      ic = icon                            ;
    break                                  ;
    case ThumbIcon                         :
      if (u>0)                             {
        Thumb = PM.Thumb( connection , u ) ;
        if (NotNull(Thumb))                {
          QSize is = iconSize()            ;
          icon = PM . Icon ( Thumb , is )  ;
          delete Thumb                     ;
        } else ic = icon                   ;
      } else ic = icon                     ;
    break                                  ;
  }                                        ;
  return ic                                ;
}

bool N::GroupView::startup(void)
{
  nKickOut ( ! isEnabled ( ) , false ) ;
  clear    (                         ) ;
  start    ( 10001                   ) ;
  return true                          ;
}

bool N::GroupView::Reload(void)
{
  emit assignToolTip  ( tr("Reading groups...") )                  ;
  emit assignEnabling ( false                   )                  ;
  Bustle              (                         )                  ;
  emit OnBusy         (                         )                  ;
  SqlConnection SC    ( plan -> sql             )                  ;
  GroupItems    GI    ( plan                    )                  ;
  GI . AutoMap    = true                                           ;
  GI . GroupTable = GI . LookTable                                 (
                      EachType()                                   ,
                      Types::Picture                               ,
                      Groups::Icon                               ) ;
  if ( SC . open ( FunctionString ) )                              {
    UUIDs Uuids = GroupUuids ( SC )                                ;
    SUID  u                                                        ;
    foreach ( u , Uuids )                                          {
      QListWidgetItem * lwi                                        ;
      QString name = SC . getName                                  (
                       PlanTable(Names)                            ,
                       "uuid"                                      ,
                       vLanguageId                                 ,
                       u                                         ) ;
      SUID puid = GI . FindSecond                                  (
                    SC                                             ,
                    u                                              ,
                    EachType()                                     ,
                    Types::Picture                                 ,
                    Groups::Icon                                   ,
                    "order by `position` asc limit 0,1"          ) ;
      lwi = addItem ( u , icon , name , "" )                       ;
      if ( NotNull ( lwi ) )                                       {
        lwi -> setData ( Qt::UserRole + 1 , (int) type )           ;
      }                                                            ;
      if ( ( display == ThumbIcon ) && ( puid > 0 ) )              {
        PictureManager PM ( plan )                                 ;
        QImage       * Thumb = NULL                                ;
        Thumb = PM . Thumb ( SC , puid )                           ;
        if ( NotNull ( Thumb ) )                                   {
          emit assignIcon ( lwi , Thumb )                          ;
        }                                                          ;
      }                                                            ;
    }                                                              ;
    SC . close        (       )                                    ;
  } else                                                           {
    plan -> showMessage ( tr ( "Database can not be connected" ) ) ;
    SC . close        (       )                                    ;
  }                                                                ;
  SC . remove         (       )                                    ;
  emit GoRelax        (       )                                    ;
  Vacancy             (       )                                    ;
  reportItems         (       )                                    ;
  emit assignEnabling ( true  )                                    ;
  return true                                                      ;
}

bool N::GroupView::Members(void)
{
  emit OnBusy (                         )                  ;
  SqlConnection SC ( plan -> sql )                         ;
  if ( SC . open ( FunctionString ) )                      {
    int Row = 0                                            ;
    startLoading ( )                                       ;
    while ( isLoading ( ) && ( Row < count ( ) ) )         {
      QListWidgetItem * it = item ( Row )                  ;
      if (NotNull(it))                                     {
        SUID u     = nListUuid ( it     )                  ;
        int  total = Counting  ( SC , u )                  ;
        it -> setToolTip ( ToolTipTotoal ( total ) )       ;
      }                                                    ;
      Row ++                                               ;
    }                                                      ;
    stopLoading  ( )                                       ;
    SC . close   (       )                                 ;
  } else                                                   {
    plan->showMessage(tr("Database can not be connected")) ;
    SC . close   (       )                                 ;
  }                                                        ;
  SC . remove    (       )                                 ;
  emit GoRelax   (       )                                 ;
  return true                                              ;
}

bool N::GroupView::FocusIn(void)
{
  nKickOut          ( IsNull(plan) , true                ) ;
  DisableAllActions (                                    ) ;
  AssignAction      ( Label        , windowTitle     ( ) ) ;
  LinkAction        ( Insert       , New             ( ) ) ;
  LinkAction        ( Rename       , Rename          ( ) ) ;
  LinkAction        ( Refresh      , startup         ( ) ) ;
  LinkAction        ( Copy         , CopyToClipboard ( ) ) ;
  LinkAction        ( SelectNone   , SelectNone      ( ) ) ;
  LinkAction        ( SelectAll    , SelectAll       ( ) ) ;
  LinkAction        ( Delete       , Delete          ( ) ) ;
  LinkAction        ( Font         , setFont         ( ) ) ;
  return true                                              ;
}

bool N::GroupView::Menu(QPoint pos)
{ Q_UNUSED ( pos ) ;
  return true      ;
}

void N::GroupView::New(void)
{
  NewListWidgetItem ( LWI              )   ;
  LWI->setIcon      ( icon             )   ;
  LWI->setData      ( Qt::UserRole , 0 )   ;
  insertItem        ( 0 , LWI          )   ;
  scrollToItem      ( LWI              )   ;
  setCurrentItem    ( LWI              )   ;
  ItemEditing   = LWI                      ;
  QRect       R = visualItemRect(LWI)      ;
  QLineEdit * l = new QLineEdit(this)      ;
  QFont       f = font()                   ;
  QRect       L                            ;
  L . setTop    ( R.bottom    ()     )     ;
  L . setLeft   ( R.left      ()     )     ;
  L . setWidth  ( R.width     ()     )     ;
  L . setHeight ( f.pixelSize () + 2 )     ;
  setItemWidget ( ItemEditing , l )        ;
  l -> setGeometry ( L )                   ;
  l -> setFont     ( f )                   ;
  ItemWidget = l                           ;
  connect(l   ,SIGNAL(editingFinished())   ,
          this,SLOT  (editingFinished()) ) ;
  l->setFocus(Qt::TabFocusReason)          ;
}

void N::GroupView::editingFinished(void)
{
  QLineEdit * l = Casting(QLineEdit,ItemWidget)         ;
  if (IsNull(l)) return                                 ;
  ///////////////////////////////////////////////////////
  QString name = l->text()                              ;
  removeItemWidget(ItemEditing)                         ;
  if (name.length()<=0)                                 {
    takeItem(row(ItemEditing))                          ;
    ItemEditing = NULL                                  ;
    ItemWidget  = NULL                                  ;
    return                                              ;
  }                                                     ;
  ItemEditing->setText(name)                            ;
  ///////////////////////////////////////////////////////
  Bustle ( )                                            ;
  SqlConnection SC(plan->sql)                           ;
  if (SC.open("GroupView","editingFinished"))           {
    SUID u = AppendGroup ( SC , name )                  ;
    SC . close           (           )                  ;
    ItemEditing->setData ( Qt::UserRole   ,      u    ) ;
    ItemEditing->setData ( Qt::UserRole+1 , (int)type ) ;
  } else                                                {
    takeItem(row(ItemEditing))                          ;
  }                                                     ;
  SC . remove ( )                                       ;
  Vacancy     ( )                                       ;
  ///////////////////////////////////////////////////////
  ItemEditing = NULL                                    ;
  ItemWidget  = NULL                                    ;
  Alert ( Done )                                        ;
}

void N::GroupView::Refresh(QListWidgetItem * item)
{
  Bustle            (         )                  ;
  SqlConnection  SC (plan->sql)                  ;
  PictureManager PM (plan)                       ;
  GroupItems    GI ( plan        )               ;
  GI . AutoMap    = true                         ;
  GI . GroupTable = GI . LookTable               (
                      EachType()                 ,
                      Types::Picture             ,
                      Groups::Icon             ) ;
  if (SC.open("nGroupView","Refresh"))           {
    SUID     u       = nListUuid(item)           ;
    QString  name    = ItemName      ( SC,u )    ;
    SUID     puid    = GI . FindSecond           (
             SC                                  ,
             u                                   ,
             EachType()                          ,
             Types::Picture                      ,
             Groups::Icon                        ,
             "order by position asc limit 0,1" ) ;
    int      total   = Counting      ( SC,u    ) ;
    QImage * image   = PM.Thumb      ( SC,puid ) ;
    QString  tooltip = ToolTipTotoal ( total   ) ;
    if (NotNull(image))                          {
      QSize is = iconSize  (        )            ;
      QIcon ic = PM . Icon (image,is)            ;
      delete image                               ;
      item->setIcon(ic)                          ;
    } else                                       {
      item->setIcon(icon)                        ;
    }                                            ;
    item -> setText    ( name    )               ;
    item -> setToolTip ( tooltip )               ;
    SC.close()                                   ;
  } else                                         {
  }                                              ;
  SC . remove ( )                                ;
  Vacancy     ( )                                ;
}

void N::GroupView::Rename(void)
{
  QListWidgetItem * item = currentItem ( ) ;
  nDropOut ( IsNull(item) )                ;
  Rename   ( item         )                ;
}

void N::GroupView::Rename(QListWidgetItem * item)
{
  nDropOut ( IsNull(item) )                  ;
  setCurrentItem(item)                       ;
  ItemEditing = item                         ;
  QRect       R = visualItemRect(item)       ;
  QLineEdit * l = new QLineEdit(this)        ;
  QFont       f = font()                     ;
  QRect       L                              ;
  L . setLeft   ( R . left      ()     )     ;
  L . setTop    ( R . bottom    ()     )     ;
  L . setWidth  ( R . width     ()     )     ;
  L . setHeight ( f . pixelSize () + 2 )     ;
  setItemWidget ( ItemEditing , l )          ;
  l -> setGeometry ( L )                     ;
  l -> setFont     ( f )                     ;
  ItemWidget = l                             ;
  l -> setText  ( item -> text ( )   )       ;
  connect(l    , SIGNAL(editingFinished())   ,
          this , SLOT  (renameFinished ()) ) ;
  l -> setFocus ( Qt::TabFocusReason )       ;
}

void N::GroupView::renameFinished(void)
{
  QLineEdit * l = Casting(QLineEdit,ItemWidget) ;
  nDropOut ( IsNull(l) )                        ;
  ///////////////////////////////////////////////
  QString name = l->text()                      ;
  SUID    u    = nListUuid(ItemEditing)         ;
  removeItemWidget(ItemEditing)                 ;
  if (name.length()<=0)                         {
    ItemEditing = NULL                          ;
    ItemWidget  = NULL                          ;
    return                                      ;
  }                                             ;
  ItemEditing->setText(name)                    ;
  nDropOut ( u <= 0 )                           ;
  ///////////////////////////////////////////////
  Bustle           (           )                ;
  SqlConnection SC ( plan->sql )                ;
  if ( SC . open ( FunctionString ) )           {
    SC . assureName                             (
      PlanTable(Names)                          ,
      u                                         ,
      vLanguageId                               ,
      name                                    ) ;
    SC . close( )                               ;
  }                                             ;
  SC . remove ( )                               ;
  Vacancy     ( )                               ;
  ///////////////////////////////////////////////
  ItemEditing = NULL                            ;
  ItemWidget  = NULL                            ;
  Alert ( Done )                                ;
}

void N::GroupView::Remove(SUID u)
{ Q_UNUSED ( u ) ;
}

void N::GroupView::Remove(UUIDs & Uuids)
{ Q_UNUSED ( Uuids ) ;
}

void N::GroupView::Delete(void)
{
  UUIDs Uuids = Selected (    ) ;
  nDropOut ( Uuids.count()<=0 ) ;
  Remove   ( Uuids            ) ;
}

SUID N::GroupView::AppendGroup(SqlConnection & connection,QString name)
{
  return GroupItems::NewDivision(connection,name,(Types::ObjectTypes)type) ;
}

QString N::GroupView::ToolTipTotoal(int total)
{
  return tr("%1 items in this group").arg(total) ;
}

int N::GroupView::Counting(SqlConnection & connection,SUID u)
{
  GroupItems GI ( plan )                        ;
  GI . AutoMap    = true                        ;
  GI . GroupTable = GI . LookTable              (
                      type                      ,
                      Types  :: Picture         ,
                      Groups :: Subordination ) ;
  return GI . Count                             (
           connection                           ,
           u                                    ,
           type                                 ,
           Types::Picture                       ,
           Groups::Subordination              ) ;
}

bool N::GroupView::setGroupIcon(SUID u,const QImage & image)
{
  PictureManager PM ( plan )         ;
  SUID puid    = 0                   ;
  bool success = false               ;
  QImage I = image                   ;
  Bustle   (                       ) ;
  success = PM . Import ( I , puid ) ;
  Vacancy  (                       ) ;
  nKickOut ( puid <= 0 , true      ) ;
  return setGroupIcon   ( u , puid ) ;
}

bool N::GroupView::setGroupIcon(SUID u,SUID picture)
{
  Bustle  ( )                              ;
  SqlConnection SC ( plan->sql )           ;
  GroupItems    GI ( plan      )           ;
  if (SC.open("GroupView","setGroupIcon")) {
    GI . AutoMap    = true                 ;
    GI . GroupTable = GI . LookTable       (
                        EachType()         ,
                        Types  :: Picture  ,
                        Groups :: Icon   ) ;
    GI . Delegate                          (
      SC                                   ,
      u                                    ,
      picture                              ,
      EachType()                           ,
      Types  :: Picture                    ,
      Groups :: Icon                     ) ;
    SC.close  (                          ) ;
  }                                        ;
  SC . remove (                          ) ;
  Vacancy     (                          ) ;
  return true                              ;
}

bool N::GroupView::dropText(QWidget * source,QPointF psf,const QString & text)
{
  nKickOut ( nEqual(source,this) , false ) ;
  QPoint pos = psf.toPoint()           ;
  QListWidgetItem * item = itemAt(pos) ;
  if (IsNull(item)) return  false      ;
  SUID u = nListUuid(item)             ;
  if (u<=0) return false               ;
  return JoinText(u,text)              ;
}

bool N::GroupView::dropUrls(QWidget * source,QPointF psf,const QList<QUrl> & urls)
{
  nKickOut ( nEqual(source,this) , false ) ;
  QPoint pos = psf.toPoint()           ;
  QListWidgetItem * item = itemAt(pos) ;
  if (IsNull(item)) return  false      ;
  SUID u = nListUuid(item)             ;
  if (u<=0) return false               ;
  return JoinUrls(u,urls)              ;
}

bool N::GroupView::dropImage(QWidget * source,QPointF psf,const QImage & image)
{
  nKickOut ( nEqual(source,this) , false ) ;
  QPoint pos = psf.toPoint()           ;
  QListWidgetItem * item = itemAt(pos) ;
  if (IsNull(item)) return  false      ;
  SUID u = nListUuid(item)             ;
  if (u<=0) return false               ;
  return JoinImage(u,image)            ;
}

bool N::GroupView::dropHtml(QWidget * source,QPointF psf,const QString & html)
{
  nKickOut ( nEqual(source,this) , false ) ;
  QPoint pos = psf.toPoint()           ;
  QListWidgetItem * item = itemAt(pos) ;
  if (IsNull(item)) return  false      ;
  SUID u = nListUuid(item)             ;
  if (u<=0) return false               ;
  return JoinHtml(u,html)              ;
}

bool N::GroupView::dropColor(QWidget * source,QPointF psf,const QColor & color)
{
  nKickOut ( nEqual(source,this) , false ) ;
  QPoint pos = psf.toPoint()           ;
  QListWidgetItem * item = itemAt(pos) ;
  if (IsNull(item)) return  false      ;
  SUID u = nListUuid(item)             ;
  if (u<=0) return false               ;
  return JoinColor(u,color)            ;
}

bool N::GroupView::dropTags(QWidget * source,QPointF psf,const UUIDs & Uuids)
{
  nKickOut ( nEqual(source,this) , false ) ;
  QPoint pos = psf.toPoint()           ;
  QListWidgetItem * item = itemAt(pos) ;
  if (IsNull(item)) return  false      ;
  SUID u = nListUuid(item)             ;
  if (u<=0) return false               ;
  return JoinTags(u,Uuids)             ;
}

bool N::GroupView::dropPictures(QWidget * source,QPointF psf,const UUIDs & Uuids)
{
  nKickOut ( nEqual(source,this) , false ) ;
  QPoint pos = psf.toPoint()           ;
  QListWidgetItem * item = itemAt(pos) ;
  if (IsNull(item)) return  false      ;
  SUID u = nListUuid(item)             ;
  if (u<=0) return false               ;
  return JoinPictures(u,Uuids)         ;
}

bool N::GroupView::dropPeople(QWidget * source,QPointF psf,const UUIDs & Uuids)
{
  nKickOut ( nEqual(source,this) , false ) ;
  QPoint pos = psf.toPoint()           ;
  QListWidgetItem * item = itemAt(pos) ;
  nKickOut ( IsNull(item) , false )    ;
  SUID u = nListUuid(item)             ;
  nKickOut ( u <= 0 , false )          ;
  return JoinPeople(u,Uuids)           ;
}

bool N::GroupView::dropVideos(QWidget * source,QPointF psf,const UUIDs & Uuids)
{
  nKickOut ( nEqual(source,this) , false ) ;
  QPoint pos = psf.toPoint()           ;
  QListWidgetItem * item = itemAt(pos) ;
  nKickOut ( IsNull(item) , false )    ;
  SUID u = nListUuid(item)             ;
  nKickOut ( u <= 0 , false )          ;
  return JoinVideos(u,Uuids)           ;
}

bool N::GroupView::dropAlbums(QWidget * source,QPointF psf,const UUIDs & Uuids)
{
  nKickOut ( nEqual(source,this) , false ) ;
  QPoint pos = psf.toPoint()           ;
  QListWidgetItem * item = itemAt(pos) ;
  nKickOut ( IsNull(item) , false )    ;
  SUID u = nListUuid(item)             ;
  nKickOut ( u <= 0 , false )          ;
  return JoinAlbums(u,Uuids)           ;
}

bool N::GroupView::dropGender(QWidget * source,QPointF psf,const SUID gender)
{
  nKickOut ( nEqual(source,this) , false ) ;
  QPoint pos = psf.toPoint()           ;
  QListWidgetItem * item = itemAt(pos) ;
  nKickOut ( IsNull(item) , false )    ;
  SUID u = nListUuid(item)             ;
  nKickOut ( u <= 0 , false )          ;
  return JoinGender(u,gender)          ;
}

bool N::GroupView::dropDivisions(QWidget * source,QPointF psf,const UUIDs & Uuids)
{
  QPoint pos = psf.toPoint()           ;
  QListWidgetItem * item = itemAt(pos) ;
  SUID u = 0                           ;
  if (source==this)                    {
    nKickOut ( IsNull(item) , false )  ;
    u = nListUuid(item)                ;
    nKickOut ( u <= 0    , false )     ;
    return MoveGroups(u,Uuids)         ;
  }                                    ;
  return JoinGroups(u,Uuids)           ;
}

bool N::GroupView::JoinText(SUID u,const QString & text)
{ Q_UNUSED ( u    ) ;
  Q_UNUSED ( text ) ;
  return true       ;
}

bool N::GroupView::JoinUrls(SUID u,const QList<QUrl> & urls)
{ Q_UNUSED ( u    ) ;
  Q_UNUSED ( urls ) ;
  return true       ;
}

bool N::GroupView::JoinImage(SUID u,const QImage & image)
{ Q_UNUSED ( u     ) ;
  Q_UNUSED ( image ) ;
  return true        ;
}

bool N::GroupView::JoinHtml(SUID u,const QString & html)
{ Q_UNUSED ( u    ) ;
  Q_UNUSED ( html ) ;
  return true       ;
}

bool N::GroupView::JoinColor(SUID u,const QColor & color)
{ Q_UNUSED ( u     ) ;
  Q_UNUSED ( color ) ;
  return true        ;
}

bool N::GroupView::JoinTags(SUID u,const UUIDs & Uuids)
{ Q_UNUSED ( u     ) ;
  Q_UNUSED ( Uuids ) ;
  return true        ;
}

bool N::GroupView::JoinPictures(SUID u,const UUIDs & Uuids)
{ Q_UNUSED ( u     ) ;
  Q_UNUSED ( Uuids ) ;
  return true        ;
}

bool N::GroupView::JoinGallery(SUID u,const UUIDs & Uuids)
{
  QListWidgetItem * item  = uuidAt(u)                                            ;
  nKickOut ( IsNull(item) , false )                                              ;
  ////////////////////////////////////////////////////////////////////////////////
  dropAction = true                                                              ;
  ////////////////////////////////////////////////////////////////////////////////
  QString title = item->text()                                                   ;
  QString jmsg                                                                   ;
  jmsg  = tr("%1 pictures join gallery %2").arg(Uuids.count()).arg(title)        ;
  plan -> showMessage   ( jmsg )                                                 ;
  plan -> processEvents (      )                                                 ;
  ////////////////////////////////////////////////////////////////////////////////
  QString fsg                                                                    ;
  UUIDs   U   = Uuids                                                            ;
  VarArgs V                                                                      ;
  Group   G                                                                      ;
  fsg = tr("%1 pictures join gallery %2 finished").arg(Uuids.count()).arg(title) ;
  ////////////////////////////////////////////////////////////////////////////////
  G  . first      = u                                                            ;
  G  . second     = 0                                                            ;
  G  . t1         = EachType( )                                                  ;
  G  . t2         = Types  :: Picture                                            ;
  G  . relation   = Groups :: Subordination                                      ;
  G  . position   = 0                                                            ;
  G  . membership = 1.0                                                          ;
  V << 10011                                                                     ;
  V << fsg                                                                       ;
  G  . addParaments ( V     )                                                    ;
  V << U . count    (       )                                                    ;
  toVariants        ( U , V )                                                    ;
  addSequence       ( V     )                                                    ;
  ////////////////////////////////////////////////////////////////////////////////
  dropAction = false                                                             ;
  emit TriggerCommand ( )                                                        ;
  ////////////////////////////////////////////////////////////////////////////////
  return true                                                                    ;
}

bool N::GroupView::JoinGallery(SUID u,const QImage & image)
{
  PictureManager PM ( plan )         ;
  SUID puid    = 0                   ;
  bool success = false               ;
  QImage I = image                   ;
  Bustle  ( )                        ;
  success = PM . Import ( I , puid ) ;
  Vacancy ( )                        ;
  nKickOut ( puid <= 0 , true )      ;
  UUIDs Uuids                        ;
  Uuids << puid                      ;
  return JoinGallery ( u , Uuids )   ;
}

bool N::GroupView::JoinPeople(SUID u,const UUIDs & Uuids)
{
  QListWidgetItem * item  = uuidAt(u)                                     ;
  nKickOut ( IsNull(item) , false )                                       ;
  /////////////////////////////////////////////////////////////////////////
  dropAction = true                                                       ;
  /////////////////////////////////////////////////////////////////////////
  QString title = item->text()                                            ;
  QString msg                                                             ;
  msg = tr("%1 people join group %2").arg(Uuids.count()).arg(title)       ;
  plan -> showMessage   ( msg )                                           ;
  plan -> processEvents (     )                                           ;
  /////////////////////////////////////////////////////////////////////////
  QString fsg                                                             ;
  UUIDs   U   = Uuids                                                     ;
  VarArgs V                                                               ;
  Group   G                                                               ;
  int     T1  = EachType( )                                               ;
  if ( Types::People == T1 ) T1 = Types::Division                         ;
  fsg = tr("%1 people join groups %2 finished").arg(U.count()).arg(title) ;
  /////////////////////////////////////////////////////////////////////////
  G  . first      = u                                                     ;
  G  . second     = 0                                                     ;
  G  . t1         = T1                                                    ;
  G  . t2         = Types  :: People                                      ;
  G  . relation   = Groups :: Subordination                               ;
  G  . position   = 0                                                     ;
  G  . membership = 1.0                                                   ;
  V << 10011                                                              ;
  V << fsg                                                                ;
  G  . addParaments ( V     )                                             ;
  V << U . count    (       )                                             ;
  toVariants        ( U , V )                                             ;
  addSequence       ( V     )                                             ;
  /////////////////////////////////////////////////////////////////////////
  dropAction = false                                                      ;
  emit TriggerCommand ( )                                                 ;
  return true                                                             ;
}

bool N::GroupView::JoinVideos(SUID u,const UUIDs & Uuids)
{
  QListWidgetItem * item  = uuidAt(u)                                         ;
  nKickOut ( IsNull(item) , false )                                           ;
  /////////////////////////////////////////////////////////////////////////////
  dropAction = true                                                           ;
  /////////////////////////////////////////////////////////////////////////////
  QString title = item->text()                                                ;
  QString jmsg                                                                ;
  jmsg = tr("%1 videos join group %2").arg(Uuids.count()).arg(title)          ;
  plan -> showMessage   ( jmsg )                                              ;
  plan -> processEvents (      )                                              ;
  /////////////////////////////////////////////////////////////////////////////
  QString fsg                                                                 ;
  UUIDs   U   = Uuids                                                         ;
  VarArgs V                                                                   ;
  Group   G                                                                   ;
  fsg = tr("%1 videos join groups %2 finished").arg(Uuids.count()).arg(title) ;
  /////////////////////////////////////////////////////////////////////////////
  G  . first      = u                                                         ;
  G  . second     = 0                                                         ;
  G  . t1         = EachType( )                                               ;
  G  . t2         = Types  :: Video                                           ;
  G  . relation   = Groups :: Subordination                                   ;
  G  . position   = 0                                                         ;
  G  . membership = 1.0                                                       ;
  V << 10011                                                                  ;
  V << fsg                                                                    ;
  G  . addParaments ( V     )                                                 ;
  V << U . count    (       )                                                 ;
  toVariants        ( U , V )                                                 ;
  addSequence       ( V     )                                                 ;
  /////////////////////////////////////////////////////////////////////////////
  dropAction = false                                                          ;
  emit TriggerCommand ( )                                                     ;
  /////////////////////////////////////////////////////////////////////////////
  return true                                                                 ;
}

bool N::GroupView::JoinAlbums(SUID u,const UUIDs & Uuids)
{
  QListWidgetItem * item  = uuidAt(u)                                         ;
  nKickOut ( IsNull(item) , false )                                           ;
  /////////////////////////////////////////////////////////////////////////////
  dropAction = true                                                           ;
  /////////////////////////////////////////////////////////////////////////////
  QString title = item->text()                                                ;
  QString jmsg                                                                ;
  jmsg = tr("%1 albums join group %2").arg(Uuids.count()).arg(title)          ;
  plan -> showMessage   ( jmsg )                                              ;
  plan -> processEvents (      )                                              ;
  /////////////////////////////////////////////////////////////////////////////
  QString fsg                                                                 ;
  UUIDs   U   = Uuids                                                         ;
  VarArgs V                                                                   ;
  Group   G                                                                   ;
  fsg = tr("%1 albums join groups %2 finished").arg(Uuids.count()).arg(title) ;
  /////////////////////////////////////////////////////////////////////////////
  G  . first      = u                                                         ;
  G  . second     = 0                                                         ;
  G  . t1         = Types  :: Division                                        ;
  G  . t2         = Types  :: Album                                           ;
  G  . relation   = Groups :: Subordination                                   ;
  G  . position   = 0                                                         ;
  G  . membership = 1.0                                                       ;
  V << 10011                                                                  ;
  V << fsg                                                                    ;
  G  . addParaments ( V     )                                                 ;
  V << U . count    (       )                                                 ;
  toVariants        ( U , V )                                                 ;
  addSequence       ( V     )                                                 ;
  /////////////////////////////////////////////////////////////////////////////
  dropAction = false                                                          ;
  emit TriggerCommand ( )                                                     ;
  /////////////////////////////////////////////////////////////////////////////
  return true                                                                 ;
}

bool N::GroupView::JoinGender(SUID u,const SUID gender)
{ Q_UNUSED ( u      ) ;
  Q_UNUSED ( gender ) ;
  return true         ;
}

bool N::GroupView::JoinGroups(SUID u,const UUIDs & Uuids)
{ Q_UNUSED ( u     ) ;
  Q_UNUSED ( Uuids ) ;
  return true        ;
}

void N::GroupView::GroupPositions(RMAPs & Position,UUIDs & Uuids)
{
  int Id = 1            ;
  SUID u                ;
  foreach (u,Uuids)     {
    Position [ u ] = Id ;
    Id ++               ;
  }                     ;
}

bool N::GroupView::MoveGroups(SUID atUuid,const UUIDs & Uuids)
{
  dropAction = true                                     ;
  SUID            u                                     ;
  UUIDs           ITEMs = ItemUuids ( )                 ;
  ListWidgetItems mItems                                ;
  foreach (u,Uuids)                                     {
    QListWidgetItem * it = uuidAt ( u )                 ;
    mItems << it                                        ;
    takeItem ( row ( it ) )                             ;
    ITEMs . takeAt ( ITEMs . indexOf (u) )              ;
  }                                                     ;
  QListWidgetItem * rit   = uuidAt ( atUuid )           ;
  int               rid   = row    ( rit    )           ;
  for (int i=0;i<mItems.count();i++,rid++)              {
    insertItem ( rid , mItems [ i ] )                   ;
  }                                                     ;
  rid = ITEMs . indexOf ( atUuid )                      ;
  if (rid<0) rid = 0                                    ;
  foreach (u,Uuids)                                     {
    ITEMs . insert ( rid , u )                          ;
    rid++                                               ;
  }                                                     ;
  RMAPs Position                                        ;
  GroupPositions(Position,ITEMs)                        ;
  ///////////////////////////////////////////////////////
  Bustle ( )                                            ;
  SqlConnection SC(plan->sql)                           ;
  if (SC.open("GroupView","MoveAlbums"))                {
    QString Q                                           ;
    foreach (u,ITEMs)                                   {
      int Id = Position [ u ]                           ;
      Q = SC.sql.Update                                 (
            PlanTable(Divisions)                        ,
            QString("where uuid = %1 ").arg(u)          ,
            1,"position"                            )   ;
      SC . Prepare ( Q               )                  ;
      SC . Bind    ( "position" , Id )                  ;
      SC . Exec    (                 )                  ;
    }                                                   ;
    SC.close()                                          ;
  }                                                     ;
  SC.remove()                                           ;
  ///////////////////////////////////////////////////////
  Vacancy  (      )                                     ;
  Alert    ( Done )                                     ;
  dropAction = false                                    ;
  return true                                           ;
}

void N::GroupView::JoinGroupThread(Group group,UUIDs & Uuids)
{
  SqlConnection SC ( plan -> sql )            ;
  GroupItems    GI ( plan        )            ;
  if (SC.open("GroupView","JoinGroupThread")) {
    startLoading ( )                          ;
    GI . AutoMap    = true                    ;
    GI . GroupTable = GI . LookTable          (
                   group.t1                   ,
                   group.t2                   ,
                   group.relation           ) ;
    GI . Join                                 (
      SC                                      ,
      group.first                             ,
      group.t1                                ,
      group.t2                                ,
      group.relation                          ,
      group.position                          ,
      Uuids                                   ,
      true                                  ) ;
    stopLoading  ( )                          ;
    SC . close   ( )                          ;
  }                                           ;
  SC   . remove  ( )                          ;
}

void N::GroupView::JoinOwnerThread (Group group,UUIDs & Uuids)
{
  SqlConnection SC ( plan -> sql )            ;
  GroupItems    GI ( plan        )            ;
  if (SC.open("GroupView","JoinOwnerThread")) {
    startLoading ( )                          ;
    GI . AutoMap    = true                    ;
    GI . GroupTable = GI . LookTable          (
                   group.t1                   ,
                   group.t2                   ,
                   group.relation           ) ;
    GI . JoinOwner                            (
      SC                                      ,
      group.second                            ,
      group.t1                                ,
      group.t2                                ,
      group.relation                          ,
      Uuids                                   ,
      true                                  ) ;
    stopLoading  ( )                          ;
    SC . close   ( )                          ;
  }                                           ;
  SC   . remove  ( )                          ;
}

#define LoadingID   1000001

void N::GroupView::IsCompleted(void)
{
  if ( LimitValues [ LoadingID ] > 1 ) return ;
  update (      )                             ;
  Alert  ( Done )                             ;
}

bool N::GroupView::ReportDetails(SqlConnection & Connection,QListWidgetItem * item,int flags)
{
  return false ;
}

void N::GroupView::DetailsThread(int flags)
{
  SqlConnection  SC ( plan -> sql )         ;
  if (SC.open("GroupView","DetailsThread")) {
    int i = 0                               ;
    startLoading ( )                        ;
    while (isLoading() && i<count())        {
      QListWidgetItem * it                  ;
      SUID              u                   ;
      it = item      ( i  )                 ;
      u  = nListUuid ( it )                 ;
      i++                                   ;
      if (u>0)                              {
        ReportDetails ( SC , it , flags )   ;
      }                                     ;
    }                                       ;
    stopLoading  ( )                        ;
    SC . close   ( )                        ;
  }                                         ;
  SC   . remove  ( )                        ;
}

void N::GroupView::setItemIcon(QListWidgetItem * it,QImage * image)
{
  PictureManager PM(plan)             ;
  QSize is = iconSize()               ;
  QIcon ic = PM . Icon ( image , is ) ;
  delete image                        ;
  it->setIcon(ic)                     ;
}

void N::GroupView::StartSequence(void)
{
  nDropOut ( CommandSequence -> isActive ( ) ) ;
  CommandSequence -> start ( 300 )             ;
}

void N::GroupView::StopSequence(void)
{
  CommandSequence -> stop ( ) ;
}

void N::GroupView::StartCommand(void)
{
  emit SequenceStart ( ) ;
}

void N::GroupView::CommandHandler(void)
{
  nDropOut ( Commanding )     ;
  Commanding = true           ;
  while (Sequences.count()>0) {
    RunCommand(Sequences[0])  ;
    Sequences.takeAt(0)       ;
  }                           ;
  Commanding = false          ;
  emit SequenceStop ( )       ;
}

void N::GroupView::run(int Type,ThreadData * data)
{
  if ( ! IsContinue ( data ) ) return            ;
  VarArgs  V      = data -> Arguments            ;
  QString  TT                                    ;
  N::Group group                                 ;
  UUIDs    U                                     ;
  ////////////////////////////////////////////////
  switch ( Type )                                {
    case 10001                                   :
      if                    ( isEnabled ( )    ) {
        startLoading        (                  ) ;
        Reload              (                  ) ;
        stopLoading         (                  ) ;
        addSequence         ( 10002            ) ;
        emit TriggerCommand (                  ) ;
      }                                          ;
    break                                        ;
    case 10002                                   :
      startLoading          (                  ) ;
      Members               (                  ) ;
      stopLoading           (                  ) ;
    break                                        ;
    case 10011                                   :
      emit OnBusy           (                  ) ;
      TT = V[0] . toString  (                  ) ;
      V . takeAt            ( 0                ) ;
      ArgsToGroup           ( 0 , V , group    ) ;
      ArgsToUuids           ( 7 , V , U        ) ;
      if ( group.first > 0 )                     {
        startLoading        (                  ) ;
        JoinGroupThread     ( group , U        ) ;
        stopLoading         (                  ) ;
        IsCompleted         (                  ) ;
      }                                          ;
      emit GoRelax          (                  ) ;
      ////////////////////////////////////////////
      V  . clear ( )                             ;
      V << 31001                                 ;
      V << TT                                    ;
      addSequence           ( V                ) ;
      emit TriggerCommand   (                  ) ;
    break                                        ;
    case 10012                                   :
      emit OnBusy           (                  ) ;
      TT = V[0] . toString  (                  ) ;
      V . takeAt            ( 0                ) ;
      ArgsToGroup           ( 0 , V , group    ) ;
      ArgsToUuids           ( 7 , V , U        ) ;
      if ( group.first > 0 )                     {
        startLoading        (                  ) ;
        JoinGroupThread     ( group , U        ) ;
        stopLoading         (                  ) ;
        IsCompleted         (                  ) ;
      }                                          ;
      emit GoRelax          (                  ) ;
      ////////////////////////////////////////////
      V  . clear ( )                             ;
      V << 31001                                 ;
      V << TT                                    ;
      addSequence           ( V                ) ;
      addSequence           ( 10001            ) ;
      emit TriggerCommand   (                  ) ;
    break                                        ;
    case 10013                                   :
      emit OnBusy           (                  ) ;
      TT = V[0] . toString  (                  ) ;
      V . takeAt            ( 0                ) ;
      ArgsToGroup           ( 0 , V , group    ) ;
      ArgsToUuids           ( 7 , V , U        ) ;
      if ( group.first > 0 )                     {
        startLoading        (                  ) ;
        JoinOwnerThread     ( group , U        ) ;
        stopLoading         (                  ) ;
        IsCompleted         (                  ) ;
      }                                          ;
      emit GoRelax          (                  ) ;
      ////////////////////////////////////////////
      V  . clear ( )                             ;
      V << 31001                                 ;
      V << TT                                    ;
      addSequence           ( V                ) ;
      emit TriggerCommand   (                  ) ;
    break                                        ;
    case 10014                                   :
      emit OnBusy           (                  ) ;
      TT = V[0] . toString  (                  ) ;
      V . takeAt            ( 0                ) ;
      ArgsToGroup           ( 0 , V , group    ) ;
      ArgsToUuids           ( 7 , V , U        ) ;
      if ( group.first > 0 )                     {
        startLoading        (                  ) ;
        JoinOwnerThread     ( group , U        ) ;
        stopLoading         (                  ) ;
        IsCompleted         (                  ) ;
      }                                          ;
      emit GoRelax          (                  ) ;
      ////////////////////////////////////////////
      V  . clear ( )                             ;
      V << 31001                                 ;
      V << TT                                    ;
      addSequence           ( V                ) ;
      addSequence           ( 10001            ) ;
      emit TriggerCommand   (                  ) ;
    break                                        ;
  }                                              ;
}

bool N::GroupView::RunCommand(VarArgs & arguments)
{
  if (arguments.count()<1) return false  ;
  VarArgs V = arguments                  ;
  int     c = V [ 0 ] . toInt ( )        ;
  switch         ( c     )               {
    case 10001                           :
    case 10002                           :
      start      ( c     )               ;
    break                                ;
    case 10011                           :
    case 10012                           :
    case 10013                           :
    case 10014                           :
      V . takeAt ( 0     )               ;
      start      ( c , V )               ;
    break                                ;
    case 31001                           :
      V . takeAt ( 0     )               ;
      plan->showMessage(V[0].toString()) ;
    break                                ;
    default                              :
    return false                         ;
  }                                      ;
  return true                            ;
}
