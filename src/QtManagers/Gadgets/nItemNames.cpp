#include <qtmanagers.h>

N::ItemNames:: ItemNames  ( QWidget * parent , Plan * p )
             : TreeWidget (           parent ,        p )
             , Object     ( 0 , Types::None             )
             , dropAction ( false                       )
{
  WidgetClass   ;
  Configure ( ) ;
}

N::ItemNames::~ItemNames (void)
{
}

QSize N::ItemNames::sizeHint (void) const
{
  return SizeSuggestion ( QSize ( 320 , 240 ) ) ;
}

QMimeData * N::ItemNames::dragMime (void)
{
  QMimeData * mime = standardMime("name")                    ;
  nKickOut ( IsNull(mime) , NULL )                           ;
  QImage image = windowIcon().pixmap(QSize(32,32)).toImage() ;
  mime -> setImageData(image)                                ;
  return mime                                                ;
}

bool N::ItemNames::hasItem(void)
{
  UUIDs Uuids = selectedUuids(0) ;
  return ( Uuids.count() > 0 )   ;
}

bool N::ItemNames::startDrag(QMouseEvent * event)
{
  QTreeWidgetItem * atItem = itemAt(event->pos())           ;
  nKickOut ( IsNull(atItem) , false )                       ;
  nKickOut (!IsMask(event->buttons(),Qt::LeftButton),false) ;
  dragPoint = event->pos()                                  ;
  nKickOut (!atItem->isSelected(),false)                    ;
  nKickOut (!PassDragDrop,true)                             ;
  return true                                               ;
}

bool N::ItemNames::fetchDrag(QMouseEvent * event)
{
  nKickOut ( !IsMask(event->buttons(),Qt::LeftButton) , false) ;
  QPoint pos = event->pos()                                    ;
  pos -= dragPoint                                             ;
  return ( pos.manhattanLength() > qApp->startDragDistance() ) ;
}

void N::ItemNames::dragDone(Qt::DropAction dropIt,QMimeData * mime)
{
}

bool N::ItemNames::finishDrag(QMouseEvent * event)
{
  return true ;
}

bool N::ItemNames::acceptDrop(QWidget * source,const QMimeData * mime)
{
  nKickOut ( nEqual(source,this) , false ) ;
  return dropHandler(mime)                 ;
}

bool N::ItemNames::dropNew(QWidget * source,const QMimeData * mime,QPoint pos)
{
  Alert ( Action ) ;
  return true      ;
}

void N::ItemNames::Configure (void)
{
  QIcon icx = QIcon       ( ":/images/tableproperties.png"   ) ;
  NewTreeWidgetItem       ( head                             ) ;
  head -> setText         ( 0 , tr("Name"    )               ) ;
  head -> setText         ( 1 , tr("Language")               ) ;
  head -> setText         ( 2 , tr("Usage"   )               ) ;
  setWindowTitle          ( tr("Name lists")                 ) ;
  setWindowIcon           ( icx                              ) ;
  setAccessibleName       ( "N::ItemNames"                   ) ;
  setRootIsDecorated      ( false                            ) ;
  setAlternatingRowColors ( true                             ) ;
  setColumnCount          ( 3                                ) ;
  setSelectionMode        ( ExtendedSelection                ) ;
  setAllAlignments        ( head , Qt::AlignCenter           ) ;
  setFont                 ( head , Fonts::ListView           ) ;
  setHeaderItem           ( head                             ) ;
  setDragDropMode         ( DragDrop                         ) ;
  plan -> setFont         ( this                             ) ;
  MountClicked            ( 1                                ) ;
  MountClicked            ( 2                                ) ;
  //////////////////////////////////////////////////////////////
  setDropFlag             ( DropName , true                  ) ;
  //////////////////////////////////////////////////////////////
  Usages [ Dynamic :: Default       ] = tr ( "Default"       ) ;
  Usages [ Dynamic :: Typo          ] = tr ( "Typo"          ) ;
  Usages [ Dynamic :: Pen           ] = tr ( "Pen name"      ) ;
  Usages [ Dynamic :: Stage         ] = tr ( "Stage name"    ) ;
  Usages [ Dynamic :: Abbreviation  ] = tr ( "Abbreviation"  ) ;
  Usages [ Dynamic :: Identifier    ] = tr ( "Identifier"    ) ;
  Usages [ Dynamic :: Entry         ] = tr ( "Entry"         ) ;
  Usages [ Dynamic :: Pronunciation ] = tr ( "Pronunciation" ) ;
  Usages [ Dynamic :: Other         ] = tr ( "Other"         ) ;
}

bool N::ItemNames::FocusIn(void)
{
  DisableAllActions (                              ) ;
  AssignAction      ( Label      , windowTitle ( ) ) ;
  LinkAction        ( Refresh    , startup     ( ) ) ;
  LinkAction        ( Insert     , New         ( ) ) ;
  LinkAction        ( Delete     , Delete      ( ) ) ;
  LinkAction        ( Paste      , Paste       ( ) ) ;
  LinkAction        ( Eradicate  , Eradicate   ( ) ) ;
  LinkAction        ( GoUp       , ItemUp      ( ) ) ;
  LinkAction        ( GoDown     , ItemDown    ( ) ) ;
  LinkAction        ( Font       , setFont     ( ) ) ;
  return true                                        ;
}

void N::ItemNames::run(int T,ThreadData * d)
{
  nDropOut   ( ! IsContinue ( d ) ) ;
  ItemRunner ( T , d              ) ;
}

bool N::ItemNames::ItemRunner(int T,ThreadData * d)
{
  switch           ( T ) {
    case 10001           :
      startLoading (   ) ;
      Reload       (   ) ;
      stopLoading  (   ) ;
    return true          ;
  }                      ;
  return false           ;
}

bool N::ItemNames::startup(void)
{
  start ( 10001 ) ;
  return true     ;
}

void N::ItemNames::Reload(void)
{
  WaitClear       (                  )                 ;
  EnterSQL        ( SC , plan -> sql )                 ;
    UUIDs   Names                                      ;
    SUID    nuid                                       ;
    QString Q                                          ;
    Names = SC.Uuids                                   (
              PlanTable(NameMaps)                      ,
              "name"                                   ,
              QString("%1 %2"                          )
              .arg(SC.WhereUuid(ObjectUuid())          )
              .arg(SC.OrderByAsc("prefer"))          ) ;
    foreach (nuid,Names)                               {
      int tt = 0                                       ;
      Q = SC.sql.SelectFrom                            (
            "`usage`"                                  ,
            PlanTable(NameMaps)                        ,
            QString("where uuid = %1 and name = %2").arg(ObjectUuid()).arg(nuid) ) ;
      if (SC.Fetch(Q)) tt = SC.Int(0)                  ;
      Q = SC.sql.SelectFrom("language,name"            ,
            plan->Tables[Tables::Names]                ,
            QString("where uuid = %1").arg(nuid)   )   ;
      if (SC.Fetch(Q))                                 {
        NewTreeWidgetItem (IT)                         ;
        int     language = SC . Int    ( 0 )             ;
        QString name     = SC . String ( 1 )                   ;
        IT -> setData   ( 0 , Qt::UserRole , nuid            ) ;
        IT -> setData   ( 1 , Qt::UserRole , language        ) ;
        IT -> setData   ( 2 , Qt::UserRole , tt              ) ;
        IT -> setText   ( 0 , name                           ) ;
        IT -> setText   ( 1 , plan -> languages [ language ] ) ;
        IT -> setText   ( 2 , Usages [ tt ]                  ) ;
        addTopLevelItem ( IT                                 ) ;
      }                                                        ;
    }                                                          ;
    Alert               ( Done                               ) ;
  ErrorSQL              ( SC , plan -> sql                   ) ;
  Alert                 ( Error                              ) ;
  LeaveSQL              ( SC , plan -> sql                   ) ;
  emit AutoFit          (                                    ) ;
}

void N::ItemNames::singleClicked(QTreeWidgetItem * item,int column)
{
  if (item==ItemEditing && column==ItemColumn) return ;
  removeOldItem (       )                             ;
  Alert         ( Click )                             ;
}

void N::ItemNames::doubleClicked(QTreeWidgetItem * item,int column)
{
  removeOldItem()              ;
  switch (column)              {
    case 0                     :
      setNameItem     ( item ) ;
    break                      ;
    case 1                     :
      setLanguageItem ( item ) ;
    break                      ;
    case 2                     :
      setUsageItem    ( item ) ;
    break                      ;
  }                            ;
}

void N::ItemNames::setNameItem(QTreeWidgetItem * item)
{
  QLineEdit * name = new QLineEdit(this)  ;
  ItemEditing = item                      ;
  ItemColumn  = 0                         ;
  ItemWidget  = (QWidget *)name           ;
  name -> setText (item->text(0))         ;
  setItemWidget(item,0,name)              ;
  connect(name,SIGNAL(editingFinished())  ,
          this,SLOT  (editingFinished())) ;
  name->setFocus(Qt::TabFocusReason)      ;
}

void N::ItemNames::setLanguageItem(QTreeWidgetItem * item)
{
  int         id   = item->data(1,Qt::UserRole).toInt() ;
  QComboBox * lang = new QComboBox(this)                ;
  if (id<=0) id = plan->LanguageId                      ;
    lang  << plan->languages                            ;
  (*lang) <= id                                         ;
  ItemEditing = item                                    ;
  ItemColumn  = 1                                       ;
  ItemWidget  = (QWidget *)lang                         ;
  setItemWidget(item,1,lang)                            ;
  connect(lang,SIGNAL(activated      (int))             ,
          this,SLOT  (languageChanged(int))           ) ;
  lang -> setEditable ( true )                          ;
  lang -> showPopup   (      )                          ;
}

void N::ItemNames::setUsageItem(QTreeWidgetItem * item)
{
  int         id    = item->data(2,Qt::UserRole).toInt() ;
  QComboBox * usage = new QComboBox(this)                ;
  N :: AddItems ( *usage , Usages )                      ;
  (*usage) <= id                                         ;
  ItemEditing = item                                     ;
  ItemColumn  = 2                                        ;
  ItemWidget  = (QWidget *)usage                         ;
  setItemWidget(item,2,usage)                            ;
  nConnect ( usage , SIGNAL(activated   (int))           ,
             this  , SLOT  (usageChanged(int))         ) ;
  usage->showPopup()                                     ;
}

void N::ItemNames::editingFinished(void)
{
  nIfSafe(ItemWidget)                                  {
    QLineEdit * name = Casting( QLineEdit,ItemWidget ) ;
    nIfSafe(name) ItemEditing->setText(0,name->text()) ;
  }                                                    ;
  itemFinalize  ()                                     ;
  removeOldItem ()                                     ;
}

void N::ItemNames::languageChanged(int index)
{
  nIfSafe(ItemWidget)                                            {
    QComboBox * combo = Casting(QComboBox,ItemWidget)            ;
    nIfSafe(combo)                                               {
      int language = combo->itemData(index,Qt::UserRole).toInt() ;
      ItemEditing->setData(1,Qt::UserRole,language)              ;
      ItemEditing->setText(1,combo->currentText())               ;
    }                                                            ;
  }                                                              ;
  itemFinalize  (      )                                         ;
  removeOldItem (      )                                         ;
}

void N::ItemNames::usageChanged(int index)
{
  nIfSafe(ItemWidget)                                         {
    QComboBox * combo = Casting(QComboBox,ItemWidget)         ;
    nIfSafe(combo)                                            {
      int usage = combo->itemData(index,Qt::UserRole).toInt() ;
      ItemEditing->setData(2,Qt::UserRole,usage)              ;
      ItemEditing->setText(2,combo->currentText())            ;
    }                                                         ;
  }                                                           ;
  itemFinalize  (      )                                      ;
  removeOldItem (      )                                      ;
}

void N::ItemNames::itemFinalize(void)
{
  SUID    u        = nTreeUuid(ItemEditing,0)                  ;
  QString name     = ItemEditing->text(0)                      ;
  int     language = ItemEditing->data(1,Qt::UserRole).toInt() ;
  int     usage    = ItemEditing->data(2,Qt::UserRole).toInt() ;
  QString Q                                                    ;
  if (name.length()<=0) return                                 ;
  if (language     <=0) return                                 ;
  SqlConnection SC(plan->sql)                                  ;
  if ( SC . open ( FunctionString ) )                          {
    if (u<=0)                                                  {
      u = SC.Unique(PlanTable(MajorUuid),"uuid")               ;
      SC.assureUuid(PlanTable(MajorUuid),u,Types::Name  )      ;
      SC.assureName(PlanTable(Names)    ,u,language,name)      ;
      Q = SC.sql.InsertInto(PlanTable(NameMaps)                ,
            3,"uuid","name","prefer"                         ) ;
      SC . Prepare ( Q                       )                 ;
      SC . Bind    ( "uuid"   , ObjectUuid() )                 ;
      SC . Bind    ( "name"   , u            )                 ;
      SC . Bind    ( "prefer" , 0            )                 ;
      SC . Exec    (                         )                 ;
      ItemEditing -> setData ( 0 , Qt::UserRole , u )          ;
    } else                                                     {
      QByteArray B = name . toUtf8 ( )                         ;
      Q = SC . sql . Update                                    (
            PlanTable(Names)                                   ,
            SC.sql.Where(1,"uuid")                             ,
            3                                                  ,
            "language"                                         ,
            "length"                                           ,
            "name"                                           ) ;
      SC . Prepare ( Q                         )               ;
      SC . Bind    ( "uuid"     , u            )               ;
      SC . Bind    ( "language" , language     )               ;
      SC . Bind    ( "length"   , B . size ( ) )               ;
      SC . Bind    ( "name"     , B            )               ;
      SC . Exec    (                           )               ;
    }                                                          ;
    int index = indexOfTopLevelItem(ItemEditing)               ;
    Q = SC.sql.Update(PlanTable(NameMaps)                      ,
          SC . sql . Where ( 2 , "uuid" , "name" )             ,
          2                                                    ,
          "prefer"                                             ,
          "usage"                                            ) ;
    SC . Prepare ( Q                       )                   ;
    SC . Bind    ( "uuid"   , ObjectUuid() )                   ;
    SC . Bind    ( "name"   , u            )                   ;
    SC . Bind    ( "prefer" , index + 1    )                   ;
    SC . Bind    ( "usage"  , usage        )                   ;
    SC . Exec    (                         )                   ;
    SC . close   (                         )                   ;
  }                                                            ;
  SC.remove()                                                  ;
  emit Update ( this , ObjectUuid() )                          ;
  Alert       ( Done                )                          ;
}

void N::ItemNames::removeOldItem(void)
{
  if (IsNull(ItemEditing)) return                  ;
  SUID    u    = nTreeUuid(ItemEditing,ItemColumn) ;
  QString name = ItemEditing->text(0)              ;
  removeItemWidget (ItemEditing,ItemColumn)        ;
  if (u<=0 && name.length()<=0)                    {
    int index = indexOfTopLevelItem(ItemEditing)   ;
    takeTopLevelItem(index)                        ;
  }                                                ;
  ItemEditing = NULL                               ;
  ItemWidget  = NULL                               ;
  ItemColumn  = -1                                 ;
  if ( ! plan -> Booleans [ "Desktop" ] )          {
    allowGesture = true                            ;
  }                                                ;
}

void N::ItemNames::New(void)
{
  NewTreeWidgetItem ( IT                    ) ;
  IT -> setData     ( 0  , Qt::UserRole , 0 ) ;
  IT -> setData     ( 1  , Qt::UserRole , 0 ) ;
  IT -> setData     ( 2  , Qt::UserRole , 0 ) ;
  addTopLevelItem   ( IT                    ) ;
  scrollToItem      ( IT                    ) ;
  doubleClicked     ( IT , 0                ) ;
  Alert             ( Action                ) ;
}

void N::ItemNames::Delete(void)
{
  QTreeWidgetItem * item = currentItem ()          ;
  if (IsNull(item)) return                         ;
  int  index = indexOfTopLevelItem(item)           ;
  SUID u = nTreeUuid(item,0)                       ;
  SqlConnection SC(plan->sql)                      ;
  if (SC.open("nItemNames","Delete"))              {
    UUIDs Uuids                                    ;
    QString Q                                      ;
    Q = SC.sql.DeleteFrom                          (
          PlanTable(NameMaps)                      ,
          QString("where uuid = %1 and name = %2"  )
            .arg(ObjectUuid()).arg(u)            ) ;
    SC.Query(Q)                                    ;
    Uuids = SC.Uuids                               (
              PlanTable(NameMaps)                  ,
              "name"                               ,
              QString("%1 %2"                      )
              .arg(SC.WhereUuid(ObjectUuid())      )
              .arg(SC.OrderByAsc("position"))    ) ;
    for (int i=0;i<Uuids.count();i++)              {
      Q = SC.sql.Update                            (
            PlanTable(NameMaps)                    ,
            "where uuid = :UUID and name = :NAME"  ,
            1                                      ,
            "prefer"                             ) ;
      SC . Prepare ( Q                           ) ;
      SC . Bind    ( "uuid"   , ObjectUuid()     ) ;
      SC . Bind    ( "name"   , Uuids[i]         ) ;
      SC . Bind    ( "prefer" , i + 1            ) ;
      SC . Exec    (                             ) ;
    }                                              ;
    SC.close()                                     ;
  }                                                ;
  SC.remove()                                      ;
  takeTopLevelItem ( index )                       ;
  Alert            ( Done  )                       ;
}

void N::ItemNames::Paste(void)
{
  QString T = nClipboardText      ;
  if (T.length()<=0) return       ;
  QStringList S = T.split('\n')   ;
  S = File::PurifyLines(S)        ;
  if (S.count()<=0) return        ;
  foreach (T,S) if (T.length()>0) {
    NewTreeWidgetItem ( IT      ) ;
    IT -> setText     ( 0 , T   ) ;
    addTopLevelItem   ( IT      ) ;
  }                               ;
}

void N::ItemNames::Eradicate(void)
{
  UUIDs U = selectedUuids ( 0 )         ;
  if (U.count()<=0) return              ;
  GroupItems    GI ( plan        )      ;
  SqlConnection SC ( plan -> sql )      ;
  if (SC.open("ItemNames","Eradicate")) {
    SUID u                              ;
    foreach (u,U)                       {
      GI.EradicateNames(SC,u)           ;
      plan -> processEvents ( )         ;
    }                                   ;
    SC . close  ( )                     ;
  }                                     ;
  SC   . remove ( )                     ;
  Reload        ( )                     ;
}

void N::ItemNames::ItemUp(void)
{
  QTreeWidgetItem * item = currentItem ()         ;
  if (IsNull(item)) return                        ;
  int  index = indexOfTopLevelItem(item)          ;
  if (index<=0) return                            ;
  takeTopLevelItem(index)                         ;
  insertTopLevelItem(index-1,item)                ;
  setCurrentItem(item)                            ;
  SqlConnection SC(plan->sql)                     ;
  if (SC.open("nItemNames","ItemUp"))             {
    QString Q                                     ;
    UUIDs Uuids                                   ;
    for (int i=0;i<topLevelItemCount();i++)       {
      SUID tuid = nTreeUuid(topLevelItem(i),0)    ;
      Uuids << tuid                               ;
    }                                             ;
    for (int i=0;i<Uuids.count();i++)             {
      Q = SC.sql.Update                           (
            PlanTable(NameMaps)                   ,
            "where uuid = :UUID and name = :NAME" ,
            1,"prefer"                          ) ;
      SC.Prepare(Q)                               ;
      SC.Bind("uuid",ObjectUuid())                ;
      SC.Bind("name",Uuids[i])                    ;
      SC.Bind("prefer",i+1)                       ;
      SC.Exec()                                   ;
    }                                             ;
    SC.close()                                    ;
  }                                               ;
  SC.remove()                                     ;
  Alert ( Click )                                 ;
}

void N::ItemNames::ItemDown(void)
{
  QTreeWidgetItem * item = currentItem ()         ;
  if (IsNull(item)) return                        ;
  int  index = indexOfTopLevelItem(item) +1       ;
  if (index>=topLevelItemCount()) return          ;
  takeTopLevelItem(index-1)                       ;
  insertTopLevelItem(index,item)                  ;
  setCurrentItem(item)                            ;
  SqlConnection SC(plan->sql)                     ;
  if (SC.open("nItemNames","ItemDown"))           {
    QString Q                                     ;
    UUIDs Uuids                                   ;
    for (int i=0;i<topLevelItemCount();i++)       {
      SUID tuid = nTreeUuid(topLevelItem(i),0)    ;
      Uuids << tuid                               ;
    }                                             ;
    for (int i=0;i<Uuids.count();i++)             {
      Q = SC.sql.Update                           (
            PlanTable(NameMaps)                   ,
            "where uuid = :UUID and name = :NAME" ,
            1,"prefer"                          ) ;
      SC.Prepare(Q)                               ;
      SC.Bind("uuid",ObjectUuid())                ;
      SC.Bind("name",Uuids[i])                    ;
      SC.Bind("prefer",i+1)                       ;
      SC.Exec()                                   ;
    }                                             ;
    SC.close()                                    ;
  }                                               ;
  SC.remove()                                     ;
  Alert ( Click )                                 ;
}

bool N::ItemNames::dropNames(QWidget * source,QPointF pos,const UUIDs & Uuids)
{ Q_UNUSED ( source )                          ;
  Q_UNUSED ( pos    )                          ;
  nKickOut       ( ObjectUuid() <= 0 , false ) ;
  EnterSQL       ( SC , plan->sql    )         ;
    QString Q                                  ;
    int     prefer = topLevelItemCount() + 1   ;
    SUID    u                                  ;
    foreach (u,Uuids)                          {
      Q = SC.sql.InsertInto                    (
            PlanTable(NameMaps)                ,
            3                                  ,
            "uuid"                             ,
            "name"                             ,
            "prefer"                         ) ;
      SC . Prepare ( Q                       ) ;
      SC . Bind    ( "uuid"   , ObjectUuid() ) ;
      SC . Bind    ( "name"   , u            ) ;
      SC . Bind    ( "prefer" , prefer       ) ;
      if (SC.Exec()) prefer++                  ;
    }                                          ;
  LeaveSQL       ( SC , plan->sql    )         ;
  Reload         (                   )         ;
  return true                                  ;
}
