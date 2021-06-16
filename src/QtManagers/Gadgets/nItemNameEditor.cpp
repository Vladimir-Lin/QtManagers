#include <qtmanagers.h>

N::ItemNameEditor:: ItemNameEditor ( QWidget * parent , Plan * p )
                  : TableWidget    (           parent ,        p )
                  , LanguageBox    ( NULL                        )
                  , SearchText     ( NULL                        )
                  , EditIndex      ( -1                          )
{
  WidgetClass   ;
  Configure ( ) ;
}

N::ItemNameEditor::~ItemNameEditor (void)
{
}

void N::ItemNameEditor::Configure(void)
{
  plan -> setFont ( this )                    ;
  nConnect ( this , SIGNAL ( setTags  ( ) )   ,
             this , SLOT   ( TagFonts ( ) ) ) ;
}

void N::ItemNameEditor::contextMenuEvent(QContextMenuEvent * event)
{
  if ( Menu ( event -> pos () ) ) event -> accept (       ) ;
  else QTableWidget :: contextMenuEvent           ( event ) ;
}

bool N::ItemNameEditor::FocusIn(void)
{
  nKickOut          ( IsNull(plan) , false             ) ;
  DisableAllActions (                                  ) ;
  AssignAction      ( Label      , windowTitle     ( ) ) ;
  LinkAction        ( Refresh    , reload          ( ) ) ;
  LinkAction        ( Search     , Search          ( ) ) ;
  LinkAction        ( Export     , Export          ( ) ) ;
  LinkAction        ( Copy       , CopyToClipboard ( ) ) ;
  LinkAction        ( SelectAll  , SelectAll       ( ) ) ;
  LinkAction        ( SelectNone , SelectNone      ( ) ) ;
  return true                                            ;
}

void N::ItemNameEditor::ToTraditional(QTableWidgetItem * s,QTableWidgetItem * t)
{
  ScriptStrings ss             ( this  ) ;
  QString       sr = s -> text (       ) ;
  ss      . setupTranslation   (       ) ;
  sr = ss . ToTraditional      ( sr    ) ;
  blockSignals                 ( true  ) ;
  t -> setText                 ( sr    ) ;
  blockSignals                 ( false ) ;
}

void N::ItemNameEditor::ToSimplified(QTableWidgetItem * t,QTableWidgetItem * s)
{
  ScriptStrings ss             ( this  ) ;
  QString       sr = t -> text (       ) ;
  ss      . setupTranslation   (       ) ;
  sr = ss . ToSimplified       ( sr    ) ;
  blockSignals                 ( true  ) ;
  s -> setText                 ( sr    ) ;
  blockSignals                 ( false ) ;
}

void N::ItemNameEditor::TagFonts(void)
{
  QFont f = plan -> fonts [ Fonts::ListView ] . toQFont ( ) ;
  int   a = Qt::AlignRight | Qt::AlignVCenter               ;
  ///////////////////////////////////////////////////////////
  if ( columnCount ( ) > 0 )                                {
    for (int i = 0 ; i < columnCount ( ) ; i++ )            {
      QTableWidgetItem * it = horizontalHeaderItem ( i )    ;
      nIfSafe(it) it -> setFont ( f )                       ;
    }                                                       ;
  }                                                         ;
  ///////////////////////////////////////////////////////////
  if ( rowCount ( ) > 0 )                                   {
    for (int i = 0 ; i < rowCount ( ) ; i++ )               {
      QTableWidgetItem * it = verticalHeaderItem ( i )      ;
      nIfSafe(it)                                           {
        it -> setFont          ( f )                        ;
        it -> setTextAlignment ( a )                        ;
      }                                                     ;
    }                                                       ;
  }                                                         ;
}

void N::ItemNameEditor::run(int T,ThreadData * data)
{
  VarArgs V = data -> Arguments               ;
  switch ( T )                                {
    case 10001                                :
      startLoading (                        ) ;
      LoadUuids    (                        ) ;
      stopLoading  (                        ) ;
    break                                     ;
    case 10002                                :
      startLoading (                        ) ;
      LoadNames    (                        ) ;
      stopLoading  (                        ) ;
    break                                     ;
    case 10003                                :
      startLoading (                        ) ;
      Refresh      (                        ) ;
      stopLoading  (                        ) ;
    break                                     ;
    case 10011                                :
      startLoading (                        ) ;
      Search       ( V [ 0 ] . toString ( ) ) ;
      stopLoading  (                        ) ;
    break                                     ;
  }                                           ;
}

void N::ItemNameEditor::startup(UUIDs & uuids,CUIDs & supports)
{
  nDropOut                  ( IsNull ( plan )          ) ;
  ItemUuids    = uuids                                   ;
  ItemSupports = supports                                ;
  ////////////////////////////////////////////////////////
  QStringList langs                                      ;
  langs = plan -> languages . LanguageNames ( supports ) ;
  LockGui                   (                          ) ;
  setColumnCount            ( supports . count ( )     ) ;
  setRowCount               ( uuids    . count ( )     ) ;
  setHorizontalHeaderLabels ( langs                    ) ;
  plan -> setFont           ( this                     ) ;
  emit assignEnabling       ( false                    ) ;
  plan -> processEvents     (                          ) ;
  start                     ( 10001                    ) ;
}

void N::ItemNameEditor::startup(QStringList & names,UUIDs & uuids,CUIDs & supports)
{
  nDropOut                  ( IsNull ( plan )          ) ;
  ItemNames    = names                                   ;
  ItemUuids    = uuids                                   ;
  ItemSupports = supports                                ;
  ////////////////////////////////////////////////////////
  QStringList langs                                      ;
  langs = plan -> languages . LanguageNames ( supports ) ;
  LockGui                   (                          ) ;
  setColumnCount            ( supports . count ( )     ) ;
  setRowCount               ( uuids    . count ( )     ) ;
  setHorizontalHeaderLabels ( langs                    ) ;
  setVerticalHeaderLabels   ( names                    ) ;
  emit assignEnabling       ( false                    ) ;
  plan -> processEvents     (                          ) ;
  start                     ( 10002                    ) ;
}

void N::ItemNameEditor::LoadUuids(void)
{
  load                ( ItemUuids , ItemSupports      ) ;
  emit ColumnsFit     (                               ) ;
  Uuids    = ItemUuids                                  ;
  Supports = ItemSupports                               ;
  emit setTags        (                               ) ;
  QHeaderView * lhv = horizontalHeader ( )              ;
  nConnect ( lhv  , SIGNAL(sectionDoubleClicked(int))   ,
             this , SLOT  (languageSection     (int)) ) ;
  nConnect ( this , SIGNAL(cellChanged(int,int))        ,
             this , SLOT  (nameChanged(int,int))      ) ;
  emit assignEnabling ( true                          ) ;
  UnlockGui           (                               ) ;
  Alert               ( Done                          ) ;
}

void N::ItemNameEditor::LoadNames(void)
{
  TableWidgetItems itemx                                    ;
  load            ( ItemUuids , ItemSupports )              ;
  emit ColumnsFit (                          )              ;
  Uuids    = ItemUuids                                      ;
  Supports = ItemSupports                                   ;
  emit setTags ( )                                          ;
  itemx = horizontalHeaderItems (                         ) ;
  setFont                       ( itemx , Fonts::ListView ) ;
  itemx = verticalHeaderItems   (                         ) ;
  setFont                       ( itemx , Fonts::ListView ) ;
  QHeaderView * lhv = horizontalHeader ( )                  ;
  QHeaderView * nhv = verticalHeader   ( )                  ;
  nConnect ( lhv  , SIGNAL ( sectionDoubleClicked(int))     ,
             this , SLOT   ( languageSection     (int))   ) ;
  nConnect ( nhv  , SIGNAL ( sectionDoubleClicked(int))     ,
             this , SLOT   ( itemSection         (int))   ) ;
  nConnect ( this , SIGNAL ( cellChanged(int,int))          ,
             this , SLOT   ( nameChanged(int,int))        ) ;
  emit assignEnabling           ( true                    ) ;
  UnlockGui                     (                         ) ;
  Alert                         ( Done                    ) ;
}

bool N::ItemNameEditor::load(UUIDs & uuids,CUIDs & supports)
{
  if ( IsNull ( plan ) ) return false                                        ;
  blockSignals ( true )                                                      ;
  ////////////////////////////////////////////////////////////////////////////
  Sql           sql = plan->sql                                              ;
  SqlConnection NC ( sql )                                                   ;
  if ( NC . open ( FunctionString ) )                                        {
    qint64 total = ( uuids . count ( ) * supports . count ( ) )              ;
    qint64 index = 0                                                         ;
    bool   go    = true                                                      ;
    int    id    = -1                                                        ;
    //////////////////////////////////////////////////////////////////////////
    id = plan -> Progress ( tr("Translations") , tr("%v/%m") )               ;
    plan -> Start    ( id , &index , &go   )                                 ;
    plan -> setRange ( id , 0      , total )                                 ;
    //////////////////////////////////////////////////////////////////////////
    for (int y=0;go && y<uuids.count();y++)                                  {
      for (int x=0;go && x<supports.count();x++)                             {
        QTableWidgetItem * TWI  = item ( y , x )                             ;
        QString            name = NC . getName                               (
                                    PlanTable(Names)                         ,
                                    "uuid"                                   ,
                                    supports [ x ]                           ,
                                    uuids    [ y ]                         ) ;
        if (IsNull(TWI)) TWI = new QTableWidgetItem ( )                      ;
        TWI -> setText ( name        )                                       ;
        setItem ( y , x , TWI )                                              ;
        index++                                                              ;
      }                                                                      ;
    }                                                                        ;
    //////////////////////////////////////////////////////////////////////////
    plan -> Finish ( id    )                                                 ;
    NC . close     (       )                                                 ;
  }                                                                          ;
  NC   . remove    (       )                                                 ;
  ////////////////////////////////////////////////////////////////////////////
  Supports = supports                                                        ;
  blockSignals     ( false )                                                 ;
  ////////////////////////////////////////////////////////////////////////////
  return true                                                                ;
}

bool N::ItemNameEditor::load(UUIDs & uuids,int index)
{
  if ( IsNull ( plan ) ) return false                                        ;
  blockSignals ( true )                                                      ;
  ////////////////////////////////////////////////////////////////////////////
  Sql           sql = plan -> sql                                            ;
  SqlConnection NC ( sql )                                                   ;
  int language = Supports [ index ]                                          ;
  if ( NC . open ( FunctionString ) )                                        {
    qint64 total = uuids . count ( )                                         ;
    qint64 index = 0                                                         ;
    bool   go    = true                                                      ;
    int    id    = -1                                                        ;
    //////////////////////////////////////////////////////////////////////////
    id = plan -> Progress ( tr("Translations") , tr("%v/%m") )               ;
    plan -> Start    ( id , &index , &go   )                                 ;
    plan -> setRange ( id , 0      , total )                                 ;
    //////////////////////////////////////////////////////////////////////////
    for (int y = 0 ; go && y < uuids . count ( ) ; y++ )                     {
      QTableWidgetItem * TWI  = item ( y , index )                           ;
      QString            name = NC . getName                                 (
                                  PlanTable(Names)                           ,
                                  "uuid"                                     ,
                                  language                                   ,
                                  uuids    [ y ]                           ) ;
      if (IsNull(TWI)) TWI = new QTableWidgetItem ( )                        ;
      TWI -> setText ( name            )                                     ;
      setItem        ( y , index , TWI )                                     ;
      index++                                                                ;
    }                                                                        ;
    //////////////////////////////////////////////////////////////////////////
    plan -> Finish ( id    )                                                 ;
    NC . close     (       )                                                 ;
  }                                                                          ;
  NC   . remove    (       )                                                 ;
  ////////////////////////////////////////////////////////////////////////////
  blockSignals     ( false )                                                 ;
  return true                                                                ;
}

void N::ItemNameEditor::languageSection(int index)
{
  if ( IsNull ( plan ) ) return                                              ;
  QPoint pos = QCursor::pos ( )                                              ;
  if ( Supports . count ( ) < index ) return                                 ;
  ////////////////////////////////////////////////////////////////////////////
  QMenu            * m   = new QMenu ( this )                                ;
  QTableWidgetItem * TWI = NULL                                              ;
  QAction          * a                                                       ;
  QMap<QAction*,int> Actions                                                 ;
  ////////////////////////////////////////////////////////////////////////////
  if ( index < 0 )                                                           {
    a = m -> addAction ( tr("Insert language") )                             ;
    Actions [ a ] = 21                                                       ;
  } else                                                                     {
    TWI = horizontalHeaderItem ( index )                                     ;
    a = m  -> addAction ( TWI -> text ( ) )                                  ;
    Actions [ a ] = 1                                                        ;
    a      -> setEnabled   ( false )                                         ;
    m      -> addSeparator (       )                                         ;
    a  = m -> addAction ( tr("Remove [%1]") . arg ( TWI -> text ( ) ) )      ;
    Actions [ a ] = 11                                                       ;
    a  = m -> addAction(tr("Insert language before [%1]").arg(TWI->text()))  ;
    Actions [ a ] = 12                                                       ;
  }                                                                          ;
  a = m -> exec ( pos )                                                      ;
  ////////////////////////////////////////////////////////////////////////////
  if (NotNull(a))                                                            {
    int           Id = Actions [ a ]                                         ;
    int           x,w,h                                                      ;
    QHeaderView * hv = horizontalHeader ( )                                  ;
    //////////////////////////////////////////////////////////////////////////
    switch ( Id )                                                            {
      case 11                                                                :
        Supports . takeAt ( index )                                          ;
        removeColumn      ( index )                                          ;
      break                                                                  ;
      case 12                                                                :
        EditIndex = index                                                    ;
        Supports . insert         ( index , 0 )                              ;
        insertColumn              ( index     )                              ;
        plan   -> processEvents   (           )                              ;
        x = hv -> sectionPosition ( EditIndex )                              ;
        w = hv -> sectionSize     ( EditIndex )                              ;
        h = hv -> height          (           )                              ;
        LanguageBox  = plan -> ComboBox ( hv )                               ;
        LanguageBox << plan -> languages                                     ;
        LanguageBox -> setGeometry ( QRect ( x , 0 , w , h ) )               ;
        nConnect ( LanguageBox , SIGNAL (activated       (int))              ,
                   this        , SLOT   (languageSelected(int))            ) ;
        LanguageBox -> show      ( )                                         ;
        LanguageBox -> showPopup ( )                                         ;
      break                                                                  ;
      case 21                                                                :
        EditIndex = Supports . count ( )                                     ;
        Supports << 0                                                        ;
        insertColumn ( EditIndex )                                           ;
        plan   -> processEvents   (           )                              ;
        x = hv -> sectionPosition ( EditIndex )                              ;
        w = hv -> sectionSize     ( EditIndex )                              ;
        h = hv -> height          (           )                              ;
        LanguageBox  = plan -> ComboBox ( hv )                               ;
        LanguageBox << plan -> languages                                     ;
        LanguageBox -> setGeometry ( QRect ( x , 0 , w , h ) )               ;
        nConnect ( LanguageBox , SIGNAL(activated       (int))               ,
                   this        , SLOT  (languageSelected(int))             ) ;
        LanguageBox -> show      ( )                                         ;
        LanguageBox -> showPopup ( )                                         ;
      break                                                                  ;
    }                                                                        ;
  }                                                                          ;
  ////////////////////////////////////////////////////////////////////////////
  m -> deleteLater ( )                                                       ;
}

void N::ItemNameEditor::itemSection(int index)
{
  if ( IsNull ( plan ) ) return                                              ;
  if ( index < 0       ) return                                              ;
  ////////////////////////////////////////////////////////////////////////////
  QPoint pos = QCursor::pos ( )                                              ;
  if ( Uuids . count ( ) < index ) return                                    ;
  ////////////////////////////////////////////////////////////////////////////
  QMenu            * m   = new QMenu          ( this  )                      ;
  QTableWidgetItem * TWI = verticalHeaderItem ( index )                      ;
  QAction          * a                                                       ;
  QMap<QAction*,int> Actions                                                 ;
  ////////////////////////////////////////////////////////////////////////////
  a = m -> addAction    ( TWI -> text ( )                             )      ;
  Actions [ a ] = 1                                                          ;
  a     -> setEnabled   ( false                                       )      ;
  a = m -> addAction    ( QString::number(Uuids[index])               )      ;
  Actions [ a ] = 2                                                          ;
  a     -> setEnabled   ( false                                       )      ;
      m -> addSeparator (                                             )      ;
  a = m -> addAction    ( tr("Remove [%1]") . arg ( TWI -> text ( ) ) )      ;
  Actions [ a ] = 11                                                         ;
  a = m -> exec         ( pos                                         )      ;
  ////////////////////////////////////////////////////////////////////////////
  if ( NotNull ( a ) )                                                       {
    int Id = Actions [ a ]                                                   ;
    switch (Id)                                                              {
      case 11                                                                :
        Uuids . takeAt ( index )                                             ;
        removeRow      ( index )                                             ;
      break                                                                  ;
    }                                                                        ;
  }                                                                          ;
  ////////////////////////////////////////////////////////////////////////////
  m -> deleteLater ( )                                                       ;
}

void N::ItemNameEditor::nameChanged(int row,int column)
{
  if (row    >= Uuids    .count()) return        ;
  if (column >= Supports .count()) return        ;
  ////////////////////////////////////////////////
  QTableWidgetItem * TWI = item ( row , column ) ;
  if (IsNull(TWI))                 return        ;
  int     language = Supports [ column ]         ;
  SUID    uuid     = Uuids    [ row    ]         ;
  QString name     = TWI -> text ( )             ;
  ////////////////////////////////////////////////
  LockGui       ( )                              ;
  Sql           sql = plan -> sql                ;
  SqlConnection NC ( sql )                       ;
  if ( NC . open ( FunctionString ) )            {
    QString Q                                    ;
    NC . assureName                              (
      PlanTable(Names)                           ,
      uuid                                       ,
      language                                   ,
      name                                     ) ;
    NC . close  ( )                              ;
  }                                              ;
  NC   . remove ( )                              ;
  ////////////////////////////////////////////////
  UnlockGui     ( )                              ;
}

void N::ItemNameEditor::languageSelected(int index)
{
  int language = LanguageBox->itemData(index,Qt::UserRole).toInt()      ;
  LanguageBox -> deleteLater()                                          ;
  LanguageBox  = NULL                                                   ;
  if (Supports.contains(language))                                      {
    Supports.takeAt(index)                                              ;
    removeColumn(index)                                                 ;
    return                                                              ;
  }                                                                     ;
  Supports[EditIndex] = language                                        ;
  setHorizontalHeaderLabels ( plan->languages.LanguageNames(Supports) ) ;
  plan -> processEvents     (                                         ) ;
  reload                    (                                         ) ;
}

void N::ItemNameEditor::reload(void)
{
  start ( 10003 ) ;
}

void N::ItemNameEditor::Refresh(void)
{
  LockGui             (                   ) ;
  emit assignEnabling ( false             ) ;
  load                ( Uuids , EditIndex ) ;
  emit ColumnsFit     (                   ) ;
  emit assignEnabling ( true              ) ;
  UnlockGui           (                   ) ;
  EditIndex = -1                            ;
}

bool N::ItemNameEditor::Relocation(void)
{
  return false ;
}

void N::ItemNameEditor::Export(void)
{
  QStringList S                                    ;
  //////////////////////////////////////////////////
  for (int i=0;i<rowCount();i++)                   {
    QStringList L                                  ;
    L . clear ( )                                  ;
    for (int j=0;j<columnCount();j++)              {
      QTableWidgetItem * it                        ;
      it = item(i,j)                               ;
      if (NotNull(it))                             {
        L << it->text()                            ;
      } else L << ""                               ;
    }                                              ;
    S << L.join("\t")                              ;
  }                                                ;
  //////////////////////////////////////////////////
  if (S.count()<=0) return                         ;
  //////////////////////////////////////////////////
  QString m = S.join("\n")                         ;
  if (m.length()<=0) return                        ;
  //////////////////////////////////////////////////
  QString filename                                 ;
  filename = QFileDialog::getSaveFileName          (
               this                                ,
               tr("Export translations into file") ,
               plan->Temporary("")                 ,
               tr("Plain text (*.txt)")          ) ;
  if (filename.length()<=0) return                 ;
  QFile F(filename)                                ;
  if (F.open(QIODevice::WriteOnly))                {
    QByteArray B = m.toUtf8()                      ;
    F.write ( B )                                  ;
    F.close (   )                                  ;
  } else                                           {
    Alert ( Done )                                 ;
  }                                                ;
}

void N::ItemNameEditor::Search(void)
{
  if (NotNull(SearchText)) return                      ;
  //////////////////////////////////////////////////////
  SearchText  = new QComboBox(plan->status)            ;
  SearchText -> setInsertPolicy(QComboBox::NoInsert)   ;
  SearchText -> setEditable( true )                    ;
  //////////////////////////////////////////////////////
  QFont f    = plan->fonts[N::Fonts::Status]           ;
  QSize s    = plan->status->size()                    ;
  QSize m(s.width()/6,s.height()-4)                    ;
  if (f.pixelSize()>0)                                 {
    f   .setPixelSize (f.pixelSize()-2)                ;
    SearchText ->setFont (f           )                ;
  }                                                    ;
  SearchText -> setMinimumSize ( m )                   ;
  SearchText -> setMaximumSize ( m )                   ;
  //////////////////////////////////////////////////////
  QStringList words                                    ;
  TableWidgetItems items = allItems()                  ;
  for (int i=0;i<items.count();i++)                    {
    QTableWidgetItem * it = items[i]                   ;
    QString t = it->text()                             ;
    if (t.length()>0)                                  {
      SearchText->addItem(t)                           ;
      words << t                                       ;
    }                                                  ;
  }                                                    ;
  //////////////////////////////////////////////////////
  QCompleter * comp = new QCompleter(words,SearchText) ;
  QLineEdit  * le   = SearchText->lineEdit()           ;
  le->setFont     (f   )                               ;
  le->setCompleter(comp)                               ;
  le->setText     (""  )                               ;
  le->setPlaceholderText(tr("Search"))                 ;
  nConnect ( le   , SIGNAL(returnPressed(   ))         ,
             this , SLOT  (SearchReady  (   ))       ) ;
  //////////////////////////////////////////////////////
  plan -> status -> addPermanentWidget ( SearchText )  ;
  SearchText     -> show               (            )  ;
  le -> setFocus ( Qt::TabFocusReason )                ;
  Alert          ( Action             )                ;
}

void N::ItemNameEditor::Find(QString text)
{
  VarArgs args           ;
  args << text           ;
  start ( 10011 , args ) ;
}

void N::ItemNameEditor::Search(QString text)
{
  int r  = 0                        ;
  int c  = 0                        ;
  int rc = rowCount    ( )          ;
  int cc = columnCount ( )          ;
  if ( rc <= 0 ) return             ;
  ///////////////////////////////////
  QTableWidgetItem * atItem         ;
  atItem = currentItem ( )          ;
  nIfSafe(atItem)                   {
    r = row    ( atItem )           ;
    c = column ( atItem )           ;
  }                                 ;
  ///////////////////////////////////
  while ( c < cc )                  {
    QTableWidgetItem * it           ;
    it = item ( r , c )             ;
    nIfSafe(it)                     {
      if ( it -> text ( ) == text ) {
        setCurrentCell ( r , c )    ;
        scrollToItem   ( it    )    ;
        Alert          ( Done  )    ;
        return                      ;
      }                             ;
    }                               ;
    r++                             ;
    if ( r >= rc )                  {
      r = 0                         ;
      c++                           ;
    }                               ;
  }                                 ;
  ///////////////////////////////////
  Alert ( Error )                   ;
}

void N::ItemNameEditor::SearchReady(void)
{
  if ( IsNull ( SearchText ) ) return                   ;
  QString text = SearchText -> lineEdit ( ) -> text ( ) ;
  SearchText  -> deleteLater ( )                        ;
  SearchText   = NULL                                   ;
  Find ( text )                                         ;
}

bool N::ItemNameEditor::Menu(QPoint pos)
{
  QTableWidgetItem * Item = itemAt(pos)               ;
  nKickOut ( IsNull(Item) , false )                   ;
  int Column = column ( Item )                        ;
  int Row    = row    ( Item )                        ;
  nKickOut ( Column >= Supports.count() , false )     ;
  /////////////////////////////////////////////////////
  int language = Supports [ Column ]                  ;
  nScopedMenu ( mm , this )                           ;
  QAction     * aa = NULL                             ;
  mm . add ( 101 , tr("Search") )                     ;
  mm . add ( 102 , tr("Export") )                     ;
  /////////////////////////////////////////////////////
  switch ( language )                                 {
    case 4871                                         :
      if ( Supports . contains ( 7523 ) )             {
        mm . addSeparator ( )                         ;
        mm . add(201,tr("Simplified to Traditional")) ;
      }                                               ;
    break                                             ;
    case 7523                                         :
      if ( Supports . contains ( 4871 ) )             {
        mm . addSeparator ( )                         ;
        mm . add(202,tr("Traditional to Simplified")) ;
      }                                               ;
    break                                             ;
  }                                                   ;
  mm . addSeparator (                               ) ;
  mm . add          ( 301 , tr("Copy to clipboard") ) ;
  mm . add          ( 302 , tr("Select all"       ) ) ;
  mm . add          ( 303 , tr("Select none"      ) ) ;
  /////////////////////////////////////////////////////
  mm . setFont      ( plan                          ) ;
  aa = mm . exec    (                               ) ;
  if ( IsNull ( aa ) ) return true                    ;
  /////////////////////////////////////////////////////
  QTableWidgetItem * sItem                            ;
  int                index                            ;
  switch (mm[aa])                                     {
    nFastCast ( 101 , Search          ( ) )           ;
    nFastCast ( 102 , Export          ( ) )           ;
    nFastCast ( 301 , CopyToClipboard ( ) )           ;
    nFastCast ( 302 , SelectAll       ( ) )           ;
    nFastCast ( 303 , SelectNone      ( ) )           ;
    case 201                                          :
      index = Supports . indexOf ( 7523 )             ;
      if                ( index >= 0        )         {
        sItem = item    ( Row   , index     )         ;
        if              ( NotNull ( sItem ) )         {
          ToTraditional ( sItem , Item      )         ;
          nameChanged   ( Row   , Column    )         ;
        }                                             ;
      }                                               ;
    break                                             ;
    case 202                                          :
      index = Supports . indexOf ( 4871 )             ;
      if                ( index >= 0        )         {
        sItem = item    ( Row  , index      )         ;
        if              ( NotNull ( sItem ) )         {
          ToSimplified  ( sItem , Item      )         ;
          nameChanged   ( Row   , Column    )         ;
        }                                             ;
      }                                               ;
    break                                             ;
  }                                                   ;
  /////////////////////////////////////////////////////
  return true                                         ;
}
