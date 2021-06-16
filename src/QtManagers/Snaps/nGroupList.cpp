#include <qtmanagers.h>

N::GroupList:: GroupList  ( QWidget * parent , Plan * p )
             : TreeDock   (           parent ,        p )
             , Object     ( 0 , Types  :: None          )
             , Relation   ( 0 , Groups :: Subordination )
             , GroupItems (                           p )
             , Sorting    ( Qt::DescendingOrder         )
             , dropAction ( false                       )
{
  WidgetClass   ;
  Configure ( ) ;
}

N::GroupList::~GroupList (void)
{
}

bool N::GroupList::acceptDrop(QWidget * source,const QMimeData * mime)
{
  nKickOut ( nEqual(source,this) , false ) ;
  return dropHandler(mime)                 ;
}

bool N::GroupList::dropNew(QWidget * source,const QMimeData * mime,QPoint pos)
{
  Alert ( Action ) ;
  return true      ;
}

void N::GroupList::Configure (void)
{
  setFunction             ( 50001 , true      ) ;
  setHeaderHidden         ( true              ) ;
  setFocusPolicy          ( Qt::WheelFocus    ) ;
  setDragDropMode         ( DropOnly          ) ;
  setAlternatingRowColors ( true              ) ;
  setRootIsDecorated      ( false             ) ;
  setSelectionMode        ( ExtendedSelection ) ;
  setColumnCount          ( 1                 ) ;
  MountClicked            ( 1                 ) ;
  MountClicked            ( 2                 ) ;
  ///////////////////////////////////////////////
  setDropFlag ( DropPicture       , true )      ;
  setDropFlag ( DropPeople        , true )      ;
  setDropFlag ( DropAudio         , true )      ;
  setDropFlag ( DropVideo         , true )      ;
  setDropFlag ( DropAlbum         , true )      ;
  setDropFlag ( DropDivision      , true )      ;
  setDropFlag ( DropURIs          , true )      ;
  setDropFlag ( DropBookmark      , true )      ;
  setDropFlag ( DropShape         , true )      ;
  setDropFlag ( DropMember        , true )      ;
  setDropFlag ( DropSet           , true )      ;
  setDropFlag ( DropActions       , true )      ;
  setDropFlag ( DropDecision      , true )      ;
  setDropFlag ( DropCondition     , true )      ;
  setDropFlag ( DropExecution     , true )      ;
  setDropFlag ( DropSqlTable      , true )      ;
  setDropFlag ( DropDatabase      , true )      ;
  setDropFlag ( DropTask          , true )      ;
  setDropFlag ( DropNation        , true )      ;
  setDropFlag ( DropContour       , true )      ;
  setDropFlag ( DropManifold      , true )      ;
  setDropFlag ( DropSource        , true )      ;
  setDropFlag ( DropDocument      , true )      ;
  setDropFlag ( DropEyes          , true )      ;
  setDropFlag ( DropHairs         , true )      ;
  setDropFlag ( DropKeyword       , true )      ;
  setDropFlag ( DropTerminology   , true )      ;
  setDropFlag ( DropKnowledge     , true )      ;
  setDropFlag ( DropField         , true )      ;
  setDropFlag ( DropKnowledgeBase , true )      ;
  setDropFlag ( DropSqlColumn     , true )      ;
  setDropFlag ( DropUuid          , true )      ;
  setDropFlag ( DropCommodity     , true )      ;
  setDropFlag ( DropOrganization  , true )      ;
  setDropFlag ( DropBlob          , true )      ;
  setDropFlag ( DropVariable      , true )      ;
  setDropFlag ( DropTorrent       , true )      ;
  setDropFlag ( DropCamera        , true )      ;
  setDropFlag ( DropFace          , true )      ;
  setDropFlag ( DropColorGroup    , true )      ;
  setDropFlag ( DropSetsAlgebra   , true )      ;
  ///////////////////////////////////////////////
  PassDragDrop = false                          ;
  plan -> setFont ( this )                      ;
}

bool N::GroupList::FocusIn(void)
{
  LinkAction ( Refresh    , startup    ( ) ) ;
  LinkAction ( Insert     , New        ( ) ) ;
  LinkAction ( Paste      , Paste      ( ) ) ;
  LinkAction ( Copy       , Copy       ( ) ) ;
  LinkAction ( SelectNone , SelectNone ( ) ) ;
  LinkAction ( SelectAll  , SelectAll  ( ) ) ;
  LinkAction ( Font       , setFont    ( ) ) ;
  return true                                ;
}

void N::GroupList::run(int Type,ThreadData * data)
{ Q_UNUSED ( data ) ;
  switch   ( Type ) {
    case 10001      :
      List (      ) ;
    break           ;
  }                 ;
}

void N::GroupList::List(void)
{
  SqlConnection SC ( plan->sql )               ;
  if (SC.open("GroupList","List"))             {
    UUIDs   U                                  ;
    SUID    u                                  ;
    U = GroupItems :: Groups                   (
          SC                                   ,
          (Types::ObjectTypes)ObjectType()     ,
          SC.OrderBy("id",Sorting)           ) ;
    foreach (u,U)                              {
      NewTreeWidgetItem ( it )                 ;
      QString n                                ;
      n = SC . getName                         (
            PlanTable(Names)                   ,
            "uuid"                             ,
            vLanguageId                        ,
            u                                ) ;
      it -> setData   ( 0 , Qt::UserRole , u ) ;
      it -> setText   ( 0 , n                ) ;
      addTopLevelItem ( it                   ) ;
    }                                          ;
    SC . close        (                      ) ;
  }                                            ;
  SC    . remove      (                      ) ;
  reportItems         (                      ) ;
  plan -> StopBusy    (                      ) ;
  emit    AutoFit     (                      ) ;
  Alert               ( Done                 ) ;
}

bool N::GroupList::startup(void)
{
  clear             (       ) ;
  plan -> StartBusy (       ) ;
  start             ( 10001 ) ;
  return true                 ;
}

void N::GroupList::New(void)
{
  NewTreeWidgetItem      ( IT     ) ;
  IT->setData(0,Qt::UserRole,0)     ;
  setAlignments          ( IT     ) ;
  switch (Sorting)                  {
    case Qt::AscendingOrder         :
      addTopLevelItem    ( IT     ) ;
    break                           ;
    case Qt::DescendingOrder        :
      insertTopLevelItem ( 0 , IT ) ;
    break                           ;
  }                                 ;
  scrollToItem           ( IT     ) ;
  doubleClicked          ( IT , 0 ) ;
}

void N::GroupList::singleClicked(QTreeWidgetItem * item,int column)
{ Q_UNUSED ( item   ) ;
  Q_UNUSED ( column ) ;
  Alert    ( Action ) ;
}

void N::GroupList::doubleClicked(QTreeWidgetItem * item,int column)
{
  nDropOut ( column > 1 )              ;
  QLineEdit * line                     ;
  removeOldItem()                      ;
  line = setLineEdit                   (
           item                        ,
           column                      ,
           SIGNAL(returnPressed  ())   ,
           SLOT  (editingFinished()) ) ;
  line->setFocus(Qt::TabFocusReason)   ;
}

void N::GroupList::editingFinished(void)
{
  nDropOut ( IsNull(ItemEditing) )                      ;
  QLineEdit * line = Casting  (QLineEdit  ,ItemWidget)  ;
  if (IsNull(line)) return                              ;
  ItemEditing -> setText ( ItemColumn , line->text() )  ;
  ///////////////////////////////////////////////////////
  SUID    u = nTreeUuid(ItemEditing,0)                  ;
  QString n = line->text()                              ;
  EnterSQL ( SC , plan->sql )                           ;
    if (u<=0) u = NewDivision                           (
                    SC                                  ,
                    n                                   ,
                    (Types::ObjectTypes)ObjectType())   ;
    SC . assureName (PlanTable(Names),u,vLanguageId,n)  ;
    ItemEditing -> setData ( 0 , Qt::UserRole , u )     ;
    ItemEditing -> setText ( 0 , n                )     ;
  LeaveSQL      ( SC , plan->sql )                      ;
  removeOldItem ( true , 0       )                      ;
  Alert         ( Done           )                      ;
}

void N::GroupList::Paste(QString text)
{
  nDropOut ( text.length() <= 0 )                       ;
  QStringList sl = text.split("\n")                     ;
  EnterSQL(SC,plan->sql)                                ;
    for (int i=0;i<sl.count();i++)                      {
      QString s = sl[i]                                 ;
      s = s.replace("\r","")                            ;
      s = s.replace("\n","")                            ;
      if (s.length()>0)                                 {
        SUID u                                          ;
        u = NewDivision                                 (
              SC                                        ,
              s                                         ,
              (Types::ObjectTypes)ObjectType()        ) ;
        SC.assureName(PlanTable(Names),u,vLanguageId,s) ;
      }                                                 ;
    }                                                   ;
  LeaveSQL(SC,plan->sql)                                ;
}

void N::GroupList::Paste(void)
{
  Paste ( nClipboardText ) ;
}

void N::GroupList::Copy(void)
{
  QStringList s                           ;
  for (int i=0;i<topLevelItemCount();i++) {
    if (topLevelItem(i)->isSelected())    {
      s << topLevelItem(i)->text(0)       ;
    }                                     ;
  }                                       ;
  nDropOut      ( s.count() <= 0 )        ;
  nSetClipboard ( s.join("\r\n") )        ;
}

bool N::GroupList::dropPictures(QWidget * source,QPointF pos,const UUIDs & Uuids)
{
  return dropObjects(source,pos,Uuids,Types::Picture) ;
}

bool N::GroupList::dropPeople(QWidget * source,QPointF pos,const UUIDs & Uuids)
{
  return dropObjects(source,pos,Uuids,Types::People) ;
}

bool N::GroupList::dropAudios(QWidget * source,QPointF pos,const UUIDs & Uuids)
{
  return dropObjects(source,pos,Uuids,Types::Audio) ;
}

bool N::GroupList::dropVideos(QWidget * source,QPointF pos,const UUIDs & Uuids)
{
  return dropObjects(source,pos,Uuids,Types::Video) ;
}

bool N::GroupList::dropAlbums(QWidget * source,QPointF pos,const UUIDs & Uuids)
{
  return dropObjects(source,pos,Uuids,Types::Album) ;
}

bool N::GroupList::dropDivisions(QWidget * source,QPointF pos,const UUIDs & Uuids)
{
  return dropObjects(source,pos,Uuids,Types::Division) ;
}

bool N::GroupList::dropURIs(QWidget * source,QPointF pos,const UUIDs & Uuids)
{
  return dropObjects(source,pos,Uuids,Types::URL) ;
}

bool N::GroupList::dropBookmarks(QWidget * source,QPointF pos,const UUIDs & Uuids)
{
  return true ;
}

bool N::GroupList::dropShapes(QWidget * source,QPointF pos,const UUIDs & Uuids)
{
  return dropObjects(source,pos,Uuids,Types::Shapes) ;
}

bool N::GroupList::dropMembers(QWidget * source,QPointF pos,const UUIDs & Uuids)
{
  return true ;
}

bool N::GroupList::dropSets(QWidget * source,QPointF pos,const UUIDs & Uuids)
{
  return dropObjects(source,pos,Uuids,Types::SetMember) ;
}

bool N::GroupList::dropActions(QWidget * source,QPointF pos,const UUIDs & Uuids)
{
  return dropObjects(source,pos,Uuids,Types::Action) ;
}

bool N::GroupList::dropDecision(QWidget * source,QPointF pos,const UUIDs & Uuids)
{
  return dropObjects(source,pos,Uuids,Types::DecisionTable) ;
}

bool N::GroupList::dropCondition(QWidget * source,QPointF pos,const UUIDs & Uuids)
{
  return dropObjects(source,pos,Uuids,Types::Condition) ;
}

bool N::GroupList::dropExecution(QWidget * source,QPointF pos,const UUIDs & Uuids)
{
  return dropObjects(source,pos,Uuids,Types::Execution) ;
}

bool N::GroupList::dropSqlTable(QWidget * source,QPointF pos,const UUIDs & Uuids)
{
  return dropObjects(source,pos,Uuids,Types::SqlTable) ;
}

bool N::GroupList::dropDatabase(QWidget * source,QPointF pos,const UUIDs & Uuids)
{
  return dropObjects(source,pos,Uuids,Types::SqlConnection) ;
}

bool N::GroupList::dropTask(QWidget * source,QPointF pos,const UUIDs & Uuids)
{
  return dropObjects(source,pos,Uuids,Types::Task) ;
}

bool N::GroupList::dropNation(QWidget * source,QPointF pos,const UUIDs & Uuids)
{
  return dropObjects(source,pos,Uuids,Types::Nation) ;
}

bool N::GroupList::dropContour(QWidget * source,QPointF pos,const UUIDs & Uuids)
{
  return dropObjects(source,pos,Uuids,Types::Contour) ;
}

bool N::GroupList::dropManifold(QWidget * source,QPointF pos,const UUIDs & Uuids)
{
  return dropObjects(source,pos,Uuids,Types::Manifolds) ;
}

bool N::GroupList::dropSources(QWidget * source,QPointF pos,const UUIDs & Uuids)
{
  return dropObjects(source,pos,Uuids,Types::Source) ;
}

bool N::GroupList::dropDocuments(QWidget * source,QPointF pos,const UUIDs & Uuids)
{
  return dropObjects(source,pos,Uuids,Types::Document) ;
}

bool N::GroupList::dropEyes(QWidget * source,QPointF pos,const UUIDs & Uuids)
{
  return dropObjects(source,pos,Uuids,Types::Eyes) ;
}

bool N::GroupList::dropHairs(QWidget * source,QPointF pos,const UUIDs & Uuids)
{
  return dropObjects(source,pos,Uuids,Types::Hairs) ;
}

bool N::GroupList::dropKeywords(QWidget * source,QPointF pos,const UUIDs & Uuids)
{
  return dropObjects(source,pos,Uuids,Types::Keyword) ;
}

bool N::GroupList::dropTerminologies(QWidget * source,QPointF pos,const UUIDs & Uuids)
{
  return dropObjects(source,pos,Uuids,Types::Terminology) ;
}

bool N::GroupList::dropKnowledge(QWidget * source,QPointF pos,const UUIDs & Uuids)
{
  return dropObjects(source,pos,Uuids,Types::Knowledge) ;
}

bool N::GroupList::dropFields(QWidget * source,QPointF pos,const UUIDs & Uuids)
{
  return dropObjects(source,pos,Uuids,Types::Field) ;
}

bool N::GroupList::dropKnowledgeBase(QWidget * source,QPointF pos,const UUIDs & Uuids)
{
  return dropObjects(source,pos,Uuids,Types::KnowledgeBase) ;
}

bool N::GroupList::dropSqlColumn(QWidget * source,QPointF pos,const UUIDs & Uuids)
{
  return dropObjects(source,pos,Uuids,Types::SqlItem) ;
}

bool N::GroupList::dropUuids(QWidget * source,QPointF pos,const UUIDs & Uuids)
{
  return true ;
}

bool N::GroupList::dropCommodities(QWidget * source,QPointF pos,const UUIDs & Uuids)
{
  return dropObjects(source,pos,Uuids,Types::Commodity) ;
}

bool N::GroupList::dropOrganizations(QWidget * source,QPointF pos,const UUIDs & Uuids)
{
  return dropObjects(source,pos,Uuids,Types::Organization) ;
}

bool N::GroupList::dropBlobs(QWidget * source,QPointF pos,const UUIDs & Uuids)
{
  return true ;
}

bool N::GroupList::dropVariables(QWidget * source,QPointF pos,const UUIDs & Uuids)
{
  return dropObjects(source,pos,Uuids,Types::Variable) ;
}

bool N::GroupList::dropTorrents(QWidget * source,QPointF pos,const UUIDs & Uuids)
{
  return true ;
}

bool N::GroupList::dropCameras(QWidget * source,QPointF pos,const UUIDs & Uuids)
{
  return dropObjects(source,pos,Uuids,Types::Camera) ;
}

bool N::GroupList::dropFaces(QWidget * source,QPointF pos,const UUIDs & Uuids)
{
  return dropObjects(source,pos,Uuids,Types::Face) ;
}

bool N::GroupList::dropColorGroups(QWidget * source,QPointF pos,const UUIDs & Uuids)
{
  return dropObjects(source,pos,Uuids,Types::ColorGroup) ;
}

bool N::GroupList::dropSetsAlgebras(QWidget * source,QPointF pos,const UUIDs & Uuids)
{
  return dropObjects(source,pos,Uuids,Types::SetsAlgebra) ;
}

bool N::GroupList :: dropObjects (
       QWidget     * source      ,
       QPointF       psf         ,
       const UUIDs & Uuids       ,
       int           otype       )
{
  nKickOut ( source == this , true )   ;
  //////////////////////////////////////
  QPoint pos = psf.toPoint()           ;
  QTreeWidgetItem * item = itemAt(pos) ;
  if (IsNull(item))                    {
    Alert ( Error )                    ;
    return false                       ;
  }                                    ;
  dropAction = true                    ;
  EnterSQL ( SC , plan->sql )          ;
    UUIDs PUIDs = Uuids                ;
    SUID  u     = nTreeUuid(item,0)    ;
    Join                               (
      SC                               ,
      u                                ,
      Types::Division                  ,
      otype                            ,
      linkType                         ,
      0                                ,
      PUIDs                          ) ;
  LeaveSQL ( SC , plan->sql )          ;
  Alert    ( Done           )          ;
  dropAction = false                   ;
  return true                          ;
}

bool N::GroupList::Menu(QPoint pos)
{
  if ( ! isFunction ( 50001 ) ) return true    ;
  nScopedMenu ( mm , this )                    ;
  QTreeWidgetItem * item = itemAt  ( pos     ) ;
  QMenu         * ms                           ;
  QMenu         * ma                           ;
  QMenu         * mc                           ;
  QAction       * aa                           ;
  mm . add          ( 101 , tr("Refresh"  )  ) ;
  mm . addSeparator (                        ) ;
  mm . add          ( 201 , tr("New group")  ) ;
  if (NotNull(item))                           {
    QString mx                                 ;
    mx = tr("Open %1").arg(item->text(0))      ;
    mm . add        ( 202 , mx               ) ;
    mx = tr("Subgroups %1").arg(item->text(0)) ;
    mm . add        ( 203 , mx               ) ;
  }                                            ;
  mm . addSeparator (                        ) ;
  mc = mm .addMenu  ( tr("Selections")       ) ;
  ma = mm .addMenu  ( tr("Adjustment")       ) ;
  ms = mm .addMenu  ( tr("Sorting"   )       ) ;
  SelectionsMenu    ( mm , mc                ) ;
  AdjustMenu        ( mm , ma                ) ;
  SortingMenu       ( mm , ms                ) ;
  mm . addSeparator (                        ) ;
  DockingMenu       ( mm                     ) ;
  mm . setFont(plan)                           ;
  aa = mm.exec()                               ;
  if (IsNull(aa)) return true                  ;
  //////////////////////////////////////////////
  if ( RunAdjustment ( mm , aa ) ) return true ;
  if ( RunSorting    ( mm , aa ) )             {
    switch (mm[aa])                            {
      case 10302                               :
        Sorting = Qt::AscendingOrder           ;
      break                                    ;
      case 10303                               :
        Sorting = Qt::DescendingOrder          ;
      break                                    ;
    }                                          ;
    return true                                ;
  }                                            ;
  if ( RunSelections ( mm , aa ) ) return true ;
  if ( RunDocking    ( mm , aa ) ) return true ;
  //////////////////////////////////////////////
  switch (mm[aa])                              {
    case 101                                   :
      startup    ( )                           ;
    break                                      ;
    case 201                                   :
      New        ( )                           ;
    break                                      ;
    case 202                                   :
      emit Open                                (
             item -> text ( 0        )         ,
             nTreeUuid    ( item , 0 )         ,
             ObjectType   (          )       ) ;
    break                                      ;
    case 203                                   :
      emit Subgroups                           (
             item -> text ( 0        )         ,
             nTreeUuid    ( item , 0 )         ,
             ObjectType   (          )       ) ;
    break                                      ;
  }                                            ;
  return true                                  ;
}
