#include <qtmanagers.h>

N::FileBrowser:: FileBrowser (QWidget * parent,Plan * p)
               : Widget      (          parent,       p)
               , model       (NULL                     )
               , path        (NULL                     )
               , file        (NULL                     )
               , head        (NULL                     )
               , tool        (NULL                     )
               , gaps        (8                        )
{
  WidgetClass   ;
  Configure ( ) ;
}

N::FileBrowser::~FileBrowser (void)
{
}

void N::FileBrowser::showEvent(QShowEvent * event)
{
  QWidget::showEvent(event) ;
  nDropOut ( !isVisible() ) ;
  Relocation ()             ;
}

void N::FileBrowser::resizeEvent(QResizeEvent * event)
{
  QWidget :: resizeEvent ( event ) ;
  Relocation             (       ) ;
}

void N::FileBrowser::closeEvent(QCloseEvent * event)
{
  QString p = path->lineEdit()->text() ;
  if (p.length()>0)                    {
    emit Lastest(        p )           ;
    emit Lastest( this , p )           ;
  }                                    ;
  QWidget::closeEvent(event)           ;
}

int N::FileBrowser::setGaps(int gap)
{
  gaps = gap  ;
  return gaps ;
}

int N::FileBrowser::Gaps(void)
{
  return gaps ;
}

void N::FileBrowser::Configure(void)
{
  path  = new ComboBox         ( this , plan                  )  ;
  file  = new TreeView         ( this , plan                  )  ;
  head  = new HeaderView       ( Qt::Horizontal , file , plan )  ;
  tool  = new QToolButton      ( this                         )  ;
  model = new QFileSystemModel ( this                         )  ;
  #if !defined(Q_OS_IOS) && !defined(Q_OS_ANDROID)
  file -> setDragDropMode (QAbstractItemView::DragDrop         ) ;
  #endif
  file -> setSelectionMode(QAbstractItemView::ExtendedSelection) ;
  file -> setHeader       (head                                ) ;
  file -> setModel        (model                               ) ;
  tool -> setIcon         (QIcon(":/images/menu.png")          ) ;
  tool -> setAutoRaise    (true                                ) ;
  plan -> setFont         (this                                ) ;
  plan -> setFont         (head,N::Fonts::ListView             ) ;
  head -> setSectionsMovable    ( false )                        ;
  QList<QFileInfo> Devices = QDir::drives()                      ;
  QString cp = QDir::currentPath()                               ;
  for (int i=0;i<Devices.count();i++)                            {
    path -> addItem                                              (
      QIcon(":/images/harddrive.png")                            ,
      Devices[i].absoluteFilePath  ()            )               ;
    if (cp==Devices[i].absoluteFilePath()) cp = ""               ;
  }                                                              ;
  if (cp.length()>0) path->addItem(cp)                           ;
  path -> setEditable (true)                                     ;
  path -> lineEdit()->setText(QDir::currentPath())               ;
  #if defined(Q_OS_IOS) || defined(Q_OS_ANDROID)
  nConnect ( file , SIGNAL (clicked      (QModelIndex))          ,
             this , SLOT   (fileClicked  (QModelIndex)) )        ;
  #else
  nConnect ( file , SIGNAL (doubleClicked(QModelIndex))          ,
             this , SLOT   (fileClicked  (QModelIndex)) )        ;
  #endif
  nConnect ( path , SIGNAL (activated    (QString    ))          ,
             this , SLOT   (setRootPath  (QString    )) )        ;
  nConnect ( tool , SIGNAL (clicked    ()             )          ,
             this , SLOT   (menuClicked()             )  )       ;
  setRootPath (QDir::currentPath())                              ;
  last = QFileInfo(QDir::currentPath())                          ;
}

void N::FileBrowser::Relocation(void)
{
  int ps = plan->fonts[N::Fonts::ComboBox].pixelSize()      ;
  int ph = ps + gaps                                        ;
  tool -> setIconSize ( QSize ( ps , ps )                 ) ;
  tool -> move        (                 0 ,             0 ) ;
  tool -> resize      (           ph + ps ,            ph ) ;
  path -> move        (           ph + ps ,             0 ) ;
  path -> resize      ( width() - ph - ps ,            ph ) ;
  file -> move        (                 0 ,            ph ) ;
  file -> resize      ( width()           , height() - ph ) ;
}

void N::FileBrowser::setRootPath(QString root)
{
  setWindowTitle (root)                      ;
  model -> setRootPath  (root              ) ;
  file  -> setRootIndex (model->index(root)) ;
  int index = path->findText  (root )        ;
  if (index<0)                               {
    path->blockSignals        (true )        ;
    path->addItem             (root )        ;
    path->lineEdit()->setText (root )        ;
    path->blockSignals        (false)        ;
  } else                                     {
    path->blockSignals        (true )        ;
    path->setCurrentIndex     (index)        ;
    path->blockSignals        (false)        ;
  }                                          ;
  last = QFileInfo(root)                     ;
  plan -> setFont (head,N::Fonts::ListView)  ;
}

void N::FileBrowser::setDragDrop(QAbstractItemView::DragDropMode mode)
{
  file->setDragDropMode(mode) ;
}

void N::FileBrowser::fileClicked(const QModelIndex & index)
{
  QFileInfo info = model->fileInfo(index) ;
  last = info                             ;
  emit Clicked (        info )            ;
  emit Clicked ( this , info )            ;
}

void N::FileBrowser::menuClicked(void)
{
  QRect  tg = tool->geometry() ;
  QPoint mp = tg.bottomLeft () ;
  mp = mapToGlobal ( mp )      ;
  Menu ( mp )                  ;
}

void N::FileBrowser::headerMenu(MenuManager & mm,QMenu * hm)
{
  mm . add(hm,301,tr("Header movable"),true,head->sectionsMovable()) ;
  mm . addSeparator ( hm )                                           ;
  for (int i=0;i<head->count();i++)                                  {
    QString h                                                        ;
    h = file->model()->headerData(i,Qt::Horizontal).toString()       ;
    mm . add ( hm , 901 + i , h , true , !head->isSectionHidden(i) ) ;
  }                                                                  ;
}

bool N::FileBrowser::runHeaderMenu(MenuManager & mm,QAction * aa)
{
  switch (mm[aa])                                  {
    case 301                                       :
      head->setSectionsMovable(aa->isChecked())    ;
    return true                                    ;
    default                                        :
      if (mm[aa]>900)                              {
        int l = mm[aa] - 901                       ;
        head->setSectionHidden(l,!aa->isChecked()) ;
        return true                                ;
      }                                            ;
    break                                          ;
  }                                                ;
  return false                                     ;
}

bool N::FileBrowser::Menu(QPoint pos)
{
  nScopedMenu ( mm , this )                                 ;
  QMenu   * hm                                              ;
  QAction * aa                                              ;
  if (last.isDir())                                         {
    mm.add(201,tr("Go to %1").arg(last.absoluteFilePath())) ;
  }                                                         ;
  hm = mm . addMenu(tr("Header"))                           ;
  mm.add(801,tr("Sorting"),true,file->isSortingEnabled())   ;
  headerMenu(mm,hm)                                         ;
  mm . setFont ( plan )                                     ;
  aa = mm.exec ( pos  )                                     ;
  nKickOut ( IsNull(aa) , true )                            ;
  if (runHeaderMenu(mm,aa)) return true                     ;
  switch (mm[aa])                                           {
    case 201                                                :
      setRootPath(last.absoluteFilePath())                  ;
    break                                                   ;
    case 801                                                :
      file->setSortingEnabled(aa->isChecked())              ;
    break                                                   ;
  }                                                         ;
  return true                                               ;
}
