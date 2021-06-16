#include <qtmanagers.h>

N::ItemEditor:: ItemEditor (QWidget * parent,Plan * p)
              : TreeDock   (          parent,       p)
{
  WidgetClass   ;
  Configure ( ) ;
}

N::ItemEditor::~ItemEditor(void)
{
}

UUIDs N::ItemEditor::ItemUuids(void)
{
  UUIDs Uuids                                 ;
  for (int i=0;i<topLevelItemCount();i++)     {
    QTreeWidgetItem * it = topLevelItem ( i ) ;
    Uuids << nTreeUuid ( it , 0 )             ;
  }                                           ;
  return Uuids                                ;
}

bool N::ItemEditor::FocusIn(void)
{
  DisableAllActions (                              ) ;
  AssignAction      ( Label      , windowTitle ( ) ) ;
  LinkAction        ( Refresh    , startup     ( ) ) ;
  LinkAction        ( Insert     , New         ( ) ) ;
  LinkAction        ( Rename     , Rename      ( ) ) ;
  LinkAction        ( Paste      , Paste       ( ) ) ;
  LinkAction        ( Export     , Export      ( ) ) ;
  LinkAction        ( Copy       , Copy        ( ) ) ;
  LinkAction        ( SelectNone , SelectNone  ( ) ) ;
  LinkAction        ( SelectAll  , SelectAll   ( ) ) ;
  LinkAction        ( Font       , setFont     ( ) ) ;
  return true                                        ;
}

bool N::ItemEditor::acceptDrop(QWidget * source,const QMimeData * mime)
{
  nKickOut ( nEqual(source,this) , false ) ;
  return dropHandler(mime)                 ;
}

bool N::ItemEditor::dropNew(QWidget * source,const QMimeData * mime,QPoint pos)
{
  Alert ( Action ) ;
  return true      ;
}

bool N::ItemEditor::dropText(QWidget * source,QPointF pos,const QString & text)
{ Q_UNUSED ( source ) ;
  Q_UNUSED ( pos    ) ;
  Paste    ( text   ) ;
  return true         ;
}

void N::ItemEditor::Configure(void)
{
  PassDragDrop = false                          ;
  setDropFlag ( DropText , true )               ;
  ///////////////////////////////////////////////
  setFocusPolicy          ( Qt::WheelFocus    ) ;
  setDragDropMode         ( DragDrop          ) ;
  setAlternatingRowColors ( true              ) ;
  setRootIsDecorated      ( false             ) ;
  setSelectionMode        ( ExtendedSelection ) ;
  setColumnCount          ( 1                 ) ;
  ///////////////////////////////////////////////
  MountClicked            ( 2                 ) ;
  plan->setFont           ( this              ) ;
  if (!plan->Booleans["Desktop"])               {
    allowGesture = true                         ;
    grabGesture ( Qt::TapAndHoldGesture )       ;
    grabGesture ( Qt::PanGesture        )       ;
  }                                             ;
}

UUIDs N::ItemEditor::LoadUuids(SqlConnection & connection)
{
  return connection.Uuids(ItemTable,"uuid","order by id asc") ;
}

void N::ItemEditor::LoadNames(SqlConnection & connection,UUIDs & Uuids)
{
  int  index = 0                                 ;
  SUID uuid  = 0                                 ;
  QTreeWidgetItem * it                           ;
  foreach (uuid,Uuids)                           {
    QString name = connection.getName            (
                     plan->Tables[Tables::Names] ,
                     "uuid",vLanguageId,uuid   ) ;
    it = addItem ( name , uuid , 0 )             ;
    setCheckable ( it              )             ;
    index++                                      ;
    if (nMod(index,100))                         {
      plan->processEvents()                      ;
    }                                            ;
  }                                              ;
}

SUID N::ItemEditor::AppendUuid(SqlConnection & connection)
{
  SUID uuid                                    ;
  uuid = connection.Unique                     (
           plan->Tables[Tables::MajorUuid]     ,
           "uuid"                            ) ;
  connection.assureUuid                        (
    plan->Tables[Tables::MajorUuid]            ,
    uuid,ItemType                            ) ;
  connection.insertUuid(ItemTable,uuid,"uuid") ;
  return uuid                                  ;
}

bool N::ItemEditor::AppendName(SqlConnection & connection,SUID uuid,QString name)
{
  QString Q                                                ;
  bool    exists = false                                   ;
  Q = connection.sql.SelectFrom                            (
        "id"                                               ,
        plan->Tables[Tables::Names]                        ,
        QString("where uuid = %1 and language = %2")
        .arg(uuid).arg(vLanguageId) )                      ;
  if (connection.Fetch(Q)) exists = true                   ;
  if (!exists)                                             {
    Q = connection.sql.ReplaceInto                         (
          plan->Tables[Tables::Names]                      ,
          3,"uuid","language","name"                     ) ;
  } else                                                   {
    Q = connection.sql.Update                              (
          plan->Tables[Tables::Names]                      ,
          "where uuid = :UUID and language = :LANGUAGE"    ,
          1,"name"                                       ) ;
  }                                                        ;
  return connection . insertName (Q,uuid,vLanguageId,name) ;
}

QTreeWidgetItem * N::ItemEditor::AppendItem (void)
{
  NewTreeWidgetItem (IT)        ;
  IT->setData(0,Qt::UserRole,0) ;
  addTopLevelItem   (IT)        ;
  setCheckable      (IT)        ;
  return IT                     ;
}

void N::ItemEditor::setCheckable(QTreeWidgetItem * item)
{ Q_UNUSED ( item ) ;
}

void N::ItemEditor::run(int T,ThreadData * data)
{
  if ( ! IsContinue ( data ) ) return ;
  switch           ( T )              {
    case 10001                        :
      startLoading (   )              ;
      List         (   )              ;
      stopLoading  (   )              ;
    break                             ;
  }                                   ;
}

bool N::ItemEditor::startup(void)
{
  start ( 10001 ) ;
  return true     ;
}

bool N::ItemEditor::List(void)
{
  LockGui                   (                  ) ;
  emit clearItems           (                  ) ;
  emit assignEnabling       ( false            ) ;
  SqlConnection SC          ( plan -> sql      ) ;
  if ( SC . open            ( FunctionString ) ) {
    UUIDs Uuids = LoadUuids ( SC               ) ;
    LoadNames               ( SC , Uuids       ) ;
    SC . close              (                  ) ;
  }                                              ;
  SC   . remove             (                  ) ;
  emit assignEnabling       ( true             ) ;
  UnlockGui                 (                  ) ;
  Alert                     ( Done             ) ;
  return true                                    ;
}

void N::ItemEditor::doubleClicked(QTreeWidgetItem * item,int column)
{
  QLineEdit * name = new QLineEdit(this)  ;
  SUID u      = nTreeUuid(item,column)    ;
  ItemEditing = item                      ;
  ItemColumn  = 0                         ;
  ItemWidget  = (QWidget *)name           ;
  name -> setText (item->text(0))         ;
  setItemWidget(item,0,name)              ;
  connect(name,SIGNAL(returnPressed  ())  ,
          this,SLOT  (editingFinished())) ;
  name->setFocus(Qt::TabFocusReason)      ;
}

void N::ItemEditor::New(void)
{
  removeOldItem     (  )                  ;
  QTreeWidgetItem * IT = AppendItem ()    ;
  scrollToItem(IT)                        ;
  QLineEdit * name = new QLineEdit(this)  ;
  ItemEditing = IT                        ;
  ItemColumn  = 0                         ;
  ItemWidget  = (QWidget *)name           ;
  IT->setData(0,Qt::UserRole  ,0        ) ;
  setItemWidget(IT,0,name)                ;
  connect(name,SIGNAL(returnPressed  ())  ,
          this,SLOT  (editingFinished())) ;
  name->setFocus(Qt::TabFocusReason)      ;
}

void N::ItemEditor::Rename(void)
{
  QTreeWidgetItem * it     ;
  it = currentItem ( )     ;
  if (IsNull(it)) return   ;
  doubleClicked ( it , 0 ) ;
}

void N::ItemEditor::removeOldItem(void)
{
  if (IsNull(ItemEditing)) return                ;
  SUID uuid = nTreeUuid(ItemEditing,ItemColumn)  ;
  removeItemWidget (ItemEditing,ItemColumn)      ;
  if (uuid<=0)                                   {
    int index = indexOfTopLevelItem(ItemEditing) ;
    takeTopLevelItem(index)                      ;
  }                                              ;
  ItemEditing = NULL                             ;
  ItemWidget  = NULL                             ;
  ItemColumn  = -1                               ;
  if (!plan->Booleans["Desktop"])                {
    allowGesture = true                          ;
  }                                              ;
}

void N::ItemEditor::editingFinished(void)
{
  SUID uuid = nTreeUuid(ItemEditing,ItemColumn)            ;
  QLineEdit * name = qobject_cast<QLineEdit *>(ItemWidget) ;
  if (NotNull(name))                                       {
    QString task = name->text()                            ;
    if (task.length()>0)                                   {
      VirtualGui::Mutex.lock  ()                           ;
      Sql sql = plan->sql                                  ;
      SqlConnection SC(sql)                                ;
      if (SC.open("nItemEditor","editingFinished"))        {
        if (uuid<=0) uuid = AppendUuid(SC)                 ;
        if (uuid>0 && AppendName(SC,uuid,task))            {
          ItemEditing->setData(0,Qt::UserRole,uuid)        ;
          ItemEditing->setText(0,task)                     ;
          emit Update ( this , uuid )                      ;
        }                                                  ;
        SC.close()                                         ;
      }                                                    ;
      SC.remove()                                          ;
      VirtualGui::Mutex.unlock()                           ;
    }                                                      ;
  }                                                        ;
  removeOldItem ( )                                        ;
}

void N::ItemEditor::Paste(QString text)
{
  if (text.length()<=0) return               ;
  QStringList sl = text.split("\n")          ;
  VirtualGui::Mutex.lock  ()                 ;
  Sql sql = plan->sql                        ;
  SqlConnection SC(sql)                      ;
  if (SC.open("nItemEditor","Paste"))        {
    for (int i=0;i<sl.count();i++)           {
      QString s = sl[i]                      ;
      s = s.replace("\r","")                 ;
      s = s.replace("\n","")                 ;
      if (s.length()>0)                      {
        SUID uuid = AppendUuid(SC)           ;
        if (uuid>0 && AppendName(SC,uuid,s)) {
          addItem (s,uuid,0)                 ;
        }                                    ;
      }                                      ;
    }                                        ;
    SC.close()                               ;
  }                                          ;
  SC.remove()                                ;
  VirtualGui::Mutex.unlock()                 ;
}

void N::ItemEditor::Paste(void)
{
  QString text = qApp->clipboard()->text() ;
  Paste(text) ;
}

void N::ItemEditor::Export(void)
{
  QString filename = QFileDialog::getSaveFileName(this) ;
  if (filename.length()<=0) return ;
  QFile F(filename) ;
  if (!F.open(QIODevice::WriteOnly)) return;
  for (int i=0;i<topLevelItemCount();i++) {
    QString n = topLevelItem(i)->text(0) ;
    n += "\r\n" ;
    F.write(n.toUtf8()) ;
  };
  F.close() ;
}

void N::ItemEditor::Copy(void)
{
  QStringList s;
  for (int i=0;i<topLevelItemCount();i++)
    if (topLevelItem(i)->isSelected()) s << topLevelItem(i)->text(0) ;
  QString v = s.join("\r\n") ;
  qApp->clipboard()->setText(v) ;
}
