#include <qtmanagers.h>

N::TextDocument:: TextDocument(void)
{
  clear ( ) ;
}

N::TextDocument:: TextDocument(Plan * p,SUID u)
{
  clear         (       ) ;
  loadPlainText ( p , u ) ;
}

N::TextDocument:: TextDocument(const TextDocument & d)
{
  ME = d ;
}

N::TextDocument::~TextDocument(void)
{
  deleteBlocks ( ) ;
}

N::TextDocument & N::TextDocument::operator = (const TextDocument & d)
{
  nMemberCopy ( d , body         ) ;
  nMemberCopy ( d , lineFeed     ) ;
  nMemberCopy ( d , lineSplit    ) ;
  nMemberCopy ( d , encoding     ) ;
  nMemberCopy ( d , codec        ) ;
  nMemberCopy ( d , layout       ) ;
  nMemberCopy ( d , selected     ) ;
  nMemberCopy ( d , start        ) ;
  nMemberCopy ( d , finish       ) ;
  nMemberCopy ( d , dirty        ) ;
  nMemberCopy ( d , blocks       ) ;
  nMemberCopy ( d , sectionLists ) ;
  nMemberCopy ( d , opBlocks     ) ;
  nMemberCopy ( d , opSections   ) ;
  return ME                        ;
}

int N::TextDocument::size(void)
{
  return body . size ( ) ;
}

int N::TextDocument::blockCount(void)
{
  return blocks . count ( ) ;
}

bool N::TextDocument::isDirty(void)
{
  return dirty ;
}

void N::TextDocument::Clear(void)
{
  deleteBlocks ( ) ;
  body . clear ( ) ;
}

void N::TextDocument::clear(void)
{
  body      . clear                      (         ) ;
  codec     = QTextCodec :: codecForName ( "UTF-8" ) ;
  lineFeed  = "\n"                                   ;
  lineSplit = "\n"                                   ;
  encoding  = "UTF-8"                                ;
  layout    = Texts::TopLeftToTopRight               ;
  selected  = false                                  ;
  start     = -1                                     ;
  finish    = -1                                     ;
  dirty     = false                                  ;
}

void N::TextDocument::clean(void)
{
  deleteBlocks         ( ) ;
  sectionLists . clear ( ) ;
  opSections   . clear ( ) ;
  opBlocks     . clear ( ) ;
}

void N::TextDocument::deleteBlocks(void)
{
  for (int i=0;i<blockCount();i++) {
    delete blocks[i]               ;
  }                                ;
  blocks . clear ( )               ;
}

bool N::TextDocument::setEncoding(QString name)
{
  codec    = QTextCodec :: codecForName ( name . toUtf8() ) ;
  encoding = name                                           ;
  return NotNull ( codec )                                  ;
}

void N::TextDocument::setLineFeed(QString lf)
{
  lineFeed = lf ;
}

void N::TextDocument::setLineSplit(QString ls)
{
  lineSplit = ls ;
}

bool N::TextDocument::loadPlainText(Plan * plan,SUID uuid)
{
  Clear ( )                                     ;
  SqlConnection SC ( plan->sql )                ;
  if (SC.open("nTextDocument","loadPlainText")) {
    QString Q                                   ;
    Q = SC.sql.SelectFrom                       (
          "text,encoding"                       ,
          PlanTable(PlainText)                  ,
          SC.WhereUuid(uuid)                  ) ;
    if (SC.Fetch(Q))                            {
      int eid = SC.Int(0)                       ;
      body = SC.ByteArray(0)                    ;
      Q = SC.sql.SelectFrom                     (
            "name" , PlanTable(Encodings)       ,
            QString("where id = %1").arg(eid) ) ;
      if (SC.Fetch(Q))                          {
        setEncoding ( SC . String ( 0 ) )       ;
      }                                         ;
    }                                           ;
    SC . close ( )                              ;
  }                                             ;
  SC . remove  ( )                              ;
  return true                                   ;
}

bool N::TextDocument::loadFile(Plan * plan,SUID uuid)
{
  Clear ( )                                ;
  SqlConnection SC ( plan->sql )           ;
  CodeManager   CM ( plan      )           ;
  if (SC.open("nTextDocument","loadFile")) {
    CM . LoadFile ( SC , uuid , body )     ;
    SC . close    (                  )     ;
  }                                        ;
  SC   . remove   (                  )     ;
  return true                              ;
}

bool N::TextDocument::setBody(QByteArray & b)
{
  body = b               ;
  return (body.size()>0) ;
}

N::TextDocument & N::TextDocument::operator = (QByteArray & b)
{
  setBody ( b ) ;
  return   ME   ;
}

int N::TextDocument::Prepare(void)
{
  clean ( )                                    ;
  nKickOut ( IsNull(codec) , blockCount() )    ;
  QString text = codec->toUnicode ( body )     ;
  nKickOut ( text.length() <= 0 , 0 )          ;
  QStringList lines                            ;
  lines = text.split ( lineSplit )             ;
  for (int i=0;i<lines.count();i++)            {
    TextBlock * tb = new TextBlock( lines[i] ) ;
    tb -> ClearCR  (          )                ;
    tb -> index = i                            ;
    blocks << tb                               ;
  }                                            ;
  return blockCount  (           )             ;
}

int N::TextDocument::toByteArray(void)
{
  nKickOut ( IsNull(codec) , body.size() ) ;
  QStringList lines                        ;
  QString     text                         ;
  for (int i=0;i<blockCount();i++)         {
    lines << blocks[i]->body               ;
  }                                        ;
  text = lines . join ( lineFeed )         ;
  body = codec->fromUnicode(text.toUtf8()) ;
  return body . size ( )                   ;
}

N::TextBlock * N::TextDocument::operator [] (int i)
{
  nKickOut ( i <  0            , NULL ) ;
  nKickOut ( i >= blockCount() , NULL ) ;
  return blocks [ i ]                   ;
}

N::TextBlock * N::TextDocument::operator [] (TextCursor & cursor)
{
  nKickOut ( cursor.Block <  0            , NULL ) ;
  nKickOut ( cursor.Block >= blockCount() , NULL ) ;
  return blocks [ cursor.Block ]                   ;
}

int N::TextDocument::indexOf(TextBlock * b)
{
  return blocks.indexOf(b) ;
}

void N::TextDocument::Collect(void)
{
  QStringList S                      ;
  QString     T                      ;
  for (int i=0;i<blocks.count();i++) {
    blocks [ i ] -> Collect ( )      ;
    S << blocks [ i ] -> body        ;
  }                                  ;
  T = S.join(lineFeed)               ;
  body = codec->fromUnicode(T)       ;
}

N::TextSection * N::TextDocument::contains(QPointF pos)
{
  for (int i=0;i<blocks.count();i++) {
    TextSection * sect = NULL        ;
    sect = blocks[i]->contains(pos)  ;
    nKickOut(NotNull(sect),sect)     ;
  }                                  ;
  return NULL                        ;
}

bool N::TextDocument::FindSection (
       int         & block        ,
       int         & section      ,
       TextSection * text         )
{
  block   = -1                      ;
  section = -1                      ;
  for (int i=0;i<blockCount();i++)  {
    TextBlock * tb = blocks[i]      ;
    for (int j=0;j<tb->count();j++) {
      TextSection * ts = (*tb)[j]   ;
      if (ts==text)                 {
        block   = i                 ;
        section = j                 ;
        return true                 ;
      }                             ;
    }                               ;
  }                                 ;
  return false                      ;
}

void N::TextDocument::EnumSections(void)
{
  sectionLists . clear ( )                 ;
  //////////////////////////////////////////
  for (int i=0;i<blockCount();i++)         {
    TextBlock * tb = blocks[i]             ;
    sectionLists << tb->sections           ;
  }                                        ;
  //////////////////////////////////////////
  for (int i=0;i<sectionLists.count();i++) {
    TextSection * ts = sectionLists[i]     ;
    ts -> index = i                        ;
  }                                        ;
}

N::TextSection * N::TextDocument::Previous(TextSection * section)
{
  int i = sectionLists.indexOf(section) ;
  i--                                   ;
  nKickOut ( i < 0 , NULL )             ;
  return sectionLists [ i ]             ;
}

N::TextSection * N::TextDocument::Next(TextSection * section)
{
  int i = sectionLists.indexOf(section)  ;
  i++                                    ;
  nKickOut(i>=sectionLists.count(),NULL) ;
  return sectionLists [ i ]              ;
}

N::TextSection * N::TextDocument::Up(TextSection * sect)
{ Q_UNUSED ( sect ) ;
  return NULL       ;
}

N::TextSection * N::TextDocument::Down(TextSection * sect)
{ Q_UNUSED ( sect ) ;
  return NULL       ;
}

bool N::TextDocument::Insert(TextCursor & cursor,QString text)
{
  nKickOut ( !cursor.isActivated() , false ) ;
  int           length  = text.length()      ;
  TextSection * section = cursor.Section     ;
  TextBlock   * block   = section->block     ;
  if (cursor.InputMethod)                    {
    section->section.insert(cursor.At,text)  ;
    cursor.At += length                      ;
  } else                                     {
    section->section.insert(cursor.At,text)  ;
    cursor.At += length                      ;
  }                                          ;
  nKickOut ( IsNull(block) , false )         ;
  block->Collect()                           ;
  dirty = true                               ;
  return true                                ;
}

bool N::TextDocument::Paste(TextCursor & cursor,QString text)
{
  nKickOut ( !cursor.isActivated() , false ) ;
  QStringList lines                          ;
  lines = text.split ( lineSplit )           ;
  nKickOut ( lines.count()==0 , false )      ;
  if (lines.count()==1)                      {
    QString       L  = lines[0]              ;
    int           At = cursor.At             ;
    TextSection * ts = cursor.Section        ;
    TextBlock   * tb = ts->block             ;
    L = L . replace ( "\r" , "" )            ;
    L = L . replace ( "\n" , "" )            ;
    ts -> section . insert ( At , L )        ;
    tb -> Collect ( )                        ;
  } else                                     {
    QString       L   = lines[0]             ;
    QString       T   = ""                   ;
    QString       F   = ""                   ;
    int           At  = cursor . At          ;
    TextSection * ts  = cursor . Section     ;
    TextBlock   * tb  = ts    -> block       ;
    int           sat                        ;
    int           len                        ;
    int           total                      ;
    int           idx                        ;
    len = ts -> section  . length  (    )    ;
    sat = tb -> sections . indexOf ( ts )    ;
    T   = ts -> section  . mid ( At,len-At ) ;
    F   = ts -> section  . left( At        ) ;
    F  += L                                  ;
    ts   -> section = F                      ;
    total = tb->sections.count()             ;
    tb -> remove ( sat , total )             ;
    total = lines.count()                    ;
    lines[total-1] += T                      ;
    idx   = blocks.indexOf(tb)               ;
    for (int i=1;i<total;i++)                {
      QString     N  = lines[i]              ;
      TextBlock * tb = new TextBlock( N )    ;
      tb -> ClearCR  (          )            ;
      tb -> index     = idx + i              ;
      tb -> sections << tb->NewSection(N)    ;
      blocks . insert (idx+i,tb)             ;
    }                                        ;
  }                                          ;
  EnumSections ( )                           ;
  return true                                ;
}

bool N::TextDocument::Cut(TextCursor & cursor,int length)
{
  if (length<0)                                     {
    nKickOut(cursor.Selections.count()<=0,false)    ;
    if (cursor.Selections.count()==1)               { // one section
      TextSection * ts = cursor.Selections[0]       ;
      int start        = ts->start                  ;
      int finish       = ts->finish                 ;
      cursor.Section   = ts                         ;
      cursor.At        = ts->start                  ;
      ts -> Cut ( start , finish-start )            ;
      ts -> block -> Collect ( )                    ;
    } else                                          {
      QList<TextSection *> SS                       ;
      QList<TextBlock   *> BS                       ;
      QList<TextBlock   *> DS                       ;
      for (int i=0;i<cursor.Selections.count();i++) {
        TextSection * ts = cursor.Selections[i]     ;
        TextBlock   * tb = ts->block                ;
        SS << ts                                    ;
        if (!BS.contains(tb)) BS << tb              ;
      }                                             ;
      for (int i=0;i<BS.count();i++)                {
        TextBlock * tb = BS[i]                      ;
        if (tb->completeSelected()) DS << tb        ;
      }                                             ;
      for (int i=0;i<DS.count();i++)                {
        TextBlock * tb = DS[i]                      ;
        int idx = blocks.indexOf(tb)                ;
        for (int j=0;j<tb->count();j++)             {
          TextSection * ts = tb->sections[j]        ;
          int sat = SS.indexOf(ts)                  ;
          if (sat>=0) SS.takeAt(sat)                ;
        }                                           ;
        delete tb                                   ;
        blocks.takeAt(idx)                          ;
      }                                             ;
      for (int i=0;i<SS.count();i++)                {
        TextSection * ts = SS[i]                    ;
        TextBlock   * tb = ts->block                ;
        if (ts->completeSelected())                 {
          int idx = tb->sections.indexOf(ts)        ;
          if (idx>=0)                               {
            tb->sections.takeAt(idx)                ;
            delete ts                               ;
          }                                         ;
        } else                                      {
          int     sat  = ts->start                  ;
          int     len  = ts->finish - ts->start     ;
          QString st   = ts->section                ;
          st           = st.remove(sat,len)         ;
          ts->section  = st                         ;
          ts->selected = false                      ;
          ts->start    = -1                         ;
          ts->finish   = -1                         ;
        }                                           ;
      }                                             ;
    }                                               ;
  } else                                            {
    // Cut what ?
  }                                                 ;
  cursor . clearSelection ( )                       ;
  dirty = true                                      ;
  return true                                       ;
}

bool N::TextDocument::Delete(TextCursor & cursor)
{
  TextSection * section = cursor.Section    ;
  TextBlock   * block   = section->block    ;
  if (cursor.Selected)                      {
    return Cut(cursor)                      ;
  } else                                    {
    if (cursor.At==section->length())       { // Merge with next block
      int idx = indexOf(block)              ;
      if (idx>=0)                           {
        idx++                               ;
        if (idx<blockCount())               {
          TextBlock   * tb = blocks[idx]    ;
          TextSection * is                  ;
          QString T                         ;
          tb -> Collect()                   ;
          T   = tb    -> body               ;
          is  = block -> NewSection(T)      ;
          block -> join   ( is  )           ;
          blocks . takeAt ( idx )           ;
        }                                   ;
      }                                     ;
    } else                                  {
      section->section.remove(cursor.At,1)  ;
    }                                       ;
  }                                         ;
  nKickOut ( IsNull(block) , false )        ;
  block -> Collect ( )                      ;
  dirty  = true                             ;
  return true                               ;
}

bool N::TextDocument::Split(TextCursor & cursor)
{
  TextSection * section = cursor   . Section           ;
  TextBlock   * block   = section -> block             ;
  if (cursor.Selected) Cut(cursor)                     ;
  int     paragraph = block->ParagraphAt(cursor)       ;
  int     L         = block->body.length() - paragraph ;
  int     X         = block->sections.indexOf(section) ;
  int     T         = block->count()                   ;
  int     CA        = cursor.At                        ;
  QString S         = ""                               ;
  QString N         = ""                               ;
  QString E         = ""                               ;
  if (paragraph>0) S = block  ->body.left(paragraph  ) ;
  if (L>0        ) N = block  ->body.mid (paragraph,L) ;
  if (cursor.At>0) E = section->section.mid(0,CA)      ;
  section->section = E                                 ;
  X += 1                                               ;
  block->remove(X,T)                                   ;
  block->body = S                                      ;
  int idx = blocks.indexOf(block) + 1                  ;
  TextBlock * tb = new TextBlock( N )                  ;
  tb -> ClearCR  (          )                          ;
  tb -> index = idx                                    ;
  tb -> sections << tb->NewSection(N)                  ;
  blocks . insert (idx,tb)                             ;
  nKickOut ( IsNull(block) , false )                   ;
  dirty = true                                         ;
  return true                                          ;
}

QRectF N::TextDocument::boundingRect (TextCursor & cursor)
{
  return blocks[cursor.Block]->boundingRect() ;
}

bool N::TextDocument::contains(TextCursor & cursor)
{
  return (cursor.Block<blockCount());
}

int N::TextDocument::length(TextCursor & cursor)
{
  return blocks[cursor.Block]->body.length() ;
}

N::TextBlock * N::TextDocument::Block(TextCursor & cursor)
{
  return blocks[cursor.Block] ;
}

void N::TextDocument::setBlock(TextCursor & cursor,TextSection * Sect)
{
  Sect->block = Block(cursor) ;
}

QString N::TextDocument::Measure(TextCursor & cursor,Font & f,QRectF a)
{
  return blocks[cursor.Block]->Measure(cursor.Paragraph,f,a) ;
}

void N::TextDocument::clearSelection(TextCursor & cursor)
{
  for (int i=0;i<cursor.Selections.count();i++) {
    TextSection * ts                            ;
    TextBlock   * tb                            ;
    ts = cursor.Selections[i]                   ;
    if (NotNull(ts))                            {
      ts -> selected = false                    ;
      ts -> start    = -1                       ;
      ts -> finish   = -1                       ;
      tb  = ts->block                           ;
      if (NotNull(tb))                          {
        tb -> selected = false                  ;
        tb -> start    = -1                     ;
        tb -> finish   = -1                     ;
      }                                         ;
    }                                           ;
  }                                             ;
  cursor . Selections . clear()                 ;
  cursor . Selected   = false                   ;
}

QString N::TextDocument::selectedText(TextCursor & cursor)
{
  QStringList             T                       ;
  QList <TextBlock *    > BDs                     ;
  QMap  <TextBlock *,int> IDs                     ;
  for (int i=0;i<cursor.Selections.count();i++)   {
    TextSection * section = cursor.Selections[i]  ;
    TextBlock   * block   = section->block        ;
    if (!BDs.contains(block))                     {
      IDs [block] = BDs.count()                   ;
      BDs << block                                ;
      T   << ""                                   ;
    }                                             ;
  }                                               ;
  for (int i=0;i<cursor.Selections.count();i++)   {
    TextSection * section = cursor.Selections[i]  ;
    TextBlock   * block   = section->block        ;
    int           id      = IDs[block]            ;
    int           first   = section->start        ;
    int           length  = section->finish       ;
    QString       cut                             ;
    length -= first                               ;
    cut = section->section.mid(first,length)      ;
    T[id] += cut                                  ;
  }                                               ;
  nKickOut ( T.count() <= 0 , "" )                ;
  return T.join(lineFeed)                         ;
}

void N::TextDocument::Select (
       TextCursor  & cursor  ,
       TextSection * start   ,
       int           sat     ,
       TextSection * finish  ,
       int           eat     )
{
  clearSelection ( cursor )              ;
  int sid = sectionLists.indexOf(start ) ;
  int eid = sectionLists.indexOf(finish) ;
  nDropOut ( sid < 0 )                   ;
  nDropOut ( eid < 0 )                   ;
  if (sid==eid)                          {
    cursor.Selections << start           ;
    if (sat>eat)                         {
      int t                              ;
      t   = sat                          ;
      sat = eat                          ;
      eat = t                            ;
    }                                    ;
    start->selected = true               ;
    start->start    = sat                ;
    start->finish   = eat                ;
    start->block->selected = true        ;
    return                               ;
  }                                      ;
  int           len                      ;
  TextSection * s = start                ;
  TextSection * f = finish               ;
  TextSection * z                        ;
  if (sid>eid)                           {
    int t                                ;
    t   = sat                            ;
    sat = eat                            ;
    eat = t                              ;
    t   = sid                            ;
    sid = eid                            ;
    eid = t                              ;
    z   = s                              ;
    s   = f                              ;
    f   = z                              ;
  }                                      ;
  len = s -> section.length()            ;
  s  -> selected = true                  ;
  s  -> start    = sat                   ;
  s  -> finish   = len                   ;
  cursor.Selections << s                 ;
  for (int i=sid+1;i<eid;i++)            {
    z   = sectionLists[i]                ;
    len = z -> section.length()          ;
    z -> selected = true                 ;
    z -> start    = 0                    ;
    z -> finish   = len                  ;
    cursor.Selections << z               ;
  }                                      ;
  f -> selected = true                   ;
  f -> start    = 0                      ;
  f -> finish   = eat                    ;
  cursor.Selections << f                 ;
  cursor.Selected = true                 ;
}

void N::TextDocument::SelectAll(TextCursor & cursor)
{
  cursor.Selected   = true                 ;
  cursor.Selections = sectionLists         ;
  for (int i=0;i<sectionLists.count();i++) {
    TextSection * ts = sectionLists[i]     ;
    int len = ts->section.length()         ;
    ts -> selected = true                  ;
    ts -> start    = 0                     ;
    ts -> finish   = len                   ;
  }                                        ;
  for (int i=0;i<blocks.count();i++)       {
    int len                                ;
    TextBlock * tb = blocks[i]             ;
    tb            -> Collect ( )           ;
    len            = tb->body.length()     ;
    tb->selected   = true                  ;
    tb->start      = 0                     ;
    tb->finish     = len                   ;
  }                                        ;
}

void N::TextDocument::Locate(TextCursor & cursor)
{
  cursor.Full  = cursor.Section->block ;
  cursor.Block = indexOf(cursor.Full)  ;
}
