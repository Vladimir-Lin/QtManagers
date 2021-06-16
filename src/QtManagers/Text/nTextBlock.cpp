#include <qtmanagers.h>

N::TextBlock:: TextBlock(void)
{
  clear ( ) ;
}

N::TextBlock:: TextBlock(QString text)
{
  clear ( )    ;
  body = text  ;
}

N::TextBlock:: TextBlock(const TextBlock & b)
{
  ME = b ;
}

N::TextBlock::~TextBlock(void)
{
  deleteSections ( ) ;
}

N::TextBlock & N::TextBlock::operator = (const TextBlock & b)
{
  nMemberCopy ( b , index    ) ;
  nMemberCopy ( b , body     ) ;
  nMemberCopy ( b , dirty    ) ;
  nMemberCopy ( b , selected ) ;
  nMemberCopy ( b , start    ) ;
  nMemberCopy ( b , finish   ) ;
  nMemberCopy ( b , space    ) ;
  nMemberCopy ( b , position ) ;
  nMemberCopy ( b , area     ) ;
  nMemberCopy ( b , sections ) ;
  nMemberCopy ( b , linkages ) ;
  return ME                    ;
}

N::TextSection * N::TextBlock::operator [] (int i)
{
  nKickOut ( i<0                 , NULL ) ;
  nKickOut ( i>=sections.count() , NULL ) ;
  return sections [ i ]                   ;
}

N::TextSection * N::TextBlock::NewSection(QString text,int i)
{
  return new TextSection ( this , text , i ) ;
}

N::TextBlock * N::TextBlock::clone(void)
{
  TextBlock * tb = new TextBlock()       ;
  tb->index       = index                ;
  tb->body        = body                 ;
  tb->dirty       = dirty                ;
  tb->selected    = selected             ;
  tb->arrangement = arrangement          ;
  tb->start       = start                ;
  tb->finish      = finish               ;
  tb->space       = space                ;
  tb->position    = position             ;
  tb->area        = area                 ;
  tb->linkages    = linkages             ;
  for (int i=0;i<sections.count();i++)   {
    tb->sections << sections[i]->clone() ;
  }                                      ;
  return tb                              ;
}

void N::TextBlock::Recover(const TextBlock & block)
{
  index       = block.index                     ;
  body        = block.body                      ;
  dirty       = block.dirty                     ;
  selected    = block.selected                  ;
  arrangement = block.arrangement               ;
  start       = block.start                     ;
  finish      = block.finish                    ;
  space       = block.space                     ;
  position    = block.position                  ;
  area        = block.area                      ;
  linkages    = block.linkages                  ;
  if (sections.count()==block.sections.count()) {
    for (int i=0;i<sections.count();i++)        {
      sections[i]->Recover(*block.sections[i])  ;
    }                                           ;
  } else                                        {
    for (int i=0;i<sections.count();i++)        {
      delete sections[i]                        ;
    }                                           ;
    sections . clear ( )                        ;
    for (int i=0;i<block.sections.count();i++)  {
      sections << block.sections[i]->clone()    ;
    }                                           ;
  }                                             ;
  for (int i=0;i<sections.count();i++)          {
    sections[i]->block = this                   ;
  }                                             ;
}

int N::TextBlock::join(TextSection * section)
{
  sections << section         ;
  return sections . count ( ) ;
}

int N::TextBlock::remove(int from,int end)
{
  for (int i=from;i<end;i++)   {
    if (i<sections.count())    {
      delete sections[i]       ;
    }                          ;
  }                            ;
  for (int i=from;i<end;i++)   {
    if (from<sections.count()) {
      sections.takeAt(from)    ;
    }                          ;
  }                            ;
  return sections . count ( )  ;
}

int N::TextBlock::count(void)
{
  return sections . count ( ) ;
}

bool N::TextBlock::completeSelected(void)
{
  for (int i=0;i<sections.count();i++)                 {
    if (!sections[i]->completeSelected()) return false ;
  }                                                    ;
  return true                                          ;
}

void N::TextBlock::clear(void)
{
  body        = ""        ;
  index       = -1        ;
  dirty       = false     ;
  selected    = false     ;
  arrangement = false     ;
  start       = -1        ;
  finish      = -1        ;
  linkages    . clear ( ) ;
  deleteSections      ( ) ;
}

void N::TextBlock::deleteSections (void)
{
  for (int i=0;i<sections.count();i++) {
    delete sections[i]                 ;
  }                                    ;
  sections . clear ( )                 ;
}

void N::TextBlock::ClearCR(void)
{
  body = body . replace ( "\r" , "" ) ;
  body = body . replace ( "\n" , "" ) ;
}

void N::TextBlock::setSpace(QString replacement)
{
  space = replacement ;
}

bool N::TextBlock::intersects(QPainterPath a)
{
  QRectF TA = Rectangle  (    ) ;
  return a  . intersects ( TA ) ;
}

QString N::TextBlock::Measure(int i,Font & font,QRectF a)
{
  QString LR = body.mid(i) ;
  QString LS               ;
  QFontMetricsF FMF(font)  ;
  LS = FMF.elidedText      (
         LR                ,
         Qt::ElideRight    ,
         a.width()       ) ;
  int len = LS.length()    ;
  LS = body.mid(i,len)     ;
  return  LS               ;
}

void N::TextBlock::Collect(void)
{
  body = ""                            ;
  for (int i=0;i<sections.count();i++) {
    body += sections[i]->section       ;
  }                                    ;
}

QRectF N::TextBlock::boundingRect(QPointF pos)
{
  QPainterPath p                       ;
  for (int i=0;i<sections.count();i++) {
    p.addRect(sections[i]->region)     ;
  }                                    ;
  QRectF R = p . boundingRect ( )      ;
  position = pos                       ;
  area . setTopLeft ( QPointF(0,0) )   ;
  area . setSize    ( R.size (   ) )   ;
  return R                             ;
}

QRectF N::TextBlock::boundingRect(void)
{
  QPainterPath p                       ;
  for (int i=0;i<sections.count();i++) {
    p.addRect(sections[i]->region)     ;
  }                                    ;
  QRectF R = p . boundingRect ( )      ;
  position = R . topLeft      ( )      ;
  area . setTopLeft ( QPointF(0,0) )   ;
  area . setSize    ( R.size (   ) )   ;
  return R                             ;
}

QRectF N::TextBlock::Rectangle(void)
{
  QSizeF  SF = area . size    ( ) ;
  QPointF TL = area . topLeft ( ) ;
  TL        += position           ;
  return QRectF ( TL , SF )       ;
}

N::TextSection * N::TextBlock::contains(QPointF pos)
{
  QRectF a = Rectangle ( )             ;
  nKickOut( !a.contains(pos),NULL )    ;
  for (int i=0;i<sections.count();i++) {
    if (sections[i]->contains(pos))    {
      return sections[i]               ;
    }                                  ;
  }                                    ;
  return NULL                          ;
}

int N::TextBlock::addLinkage(int node,TextBlock * b)
{
  linkages [ node ] = b       ;
  return linkages . count ( ) ;
}

N::TextBlock * N::TextBlock::linkage(int node)
{
  nKickOut ( !linkages.contains(node) , NULL ) ;
  return linkages [ node ]                     ;
}

void N::TextBlock::Insert(TextCursor & cursor,QString text)
{
  nDropOut(!cursor.isActivated())        ;
  cursor.Section->Insert(cursor.At,text) ;
  Collect ( )                            ;
  dirty = true                           ;
}

void N::TextBlock::Cut(TextCursor & cursor,int length)
{
  nDropOut(!cursor.isActivated())       ;
  cursor.Section->Cut(cursor.At,length) ;
  Collect ( )                           ;
  dirty = true                          ;
}

void N::TextBlock::Delete(TextCursor & cursor)
{
  nDropOut(!cursor.isActivated())   ;
  cursor.Section->Delete(cursor.At) ;
  Collect ( )                       ;
  dirty = true                      ;
}

int N::TextBlock::ParagraphAt(TextCursor & cursor)
{
  TextSection * is        = NULL           ;
  TextSection * ts        = cursor.Section ;
  bool          Done      = false          ;
  int           Paragraph = 0              ;
  Collect ( )                              ;
  for (int i=0;!Done && i<count();i++)     {
    is = sections[i]                       ;
    if (is==ts)                            {
      Paragraph += cursor.At               ;
      Done = true                          ;
    } else                                 {
      Paragraph += is->length()            ;
    }                                      ;
  }                                        ;
  return Paragraph                         ;
}
