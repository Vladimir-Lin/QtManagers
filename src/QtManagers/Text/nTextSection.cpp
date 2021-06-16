#include <qtmanagers.h>

N::TextSection:: TextSection(void)
{
  clear ( ) ;
}

N::TextSection:: TextSection(QString text,int i)
{
  clear ( )      ;
  section = text ;
  index   = i    ;
}

N::TextSection:: TextSection(TextBlock * b,QString text,int i)
{
  clear ( )      ;
  block   = b    ;
  section = text ;
  index   = i    ;
}

N::TextSection:: TextSection(const TextSection & sect)
{
  ME = sect ;
}

N::TextSection::~TextSection(void)
{
}

void N::TextSection::clear(void)
{
  index       = -1                               ;
  arrangement = false                            ;
  selected    = false                            ;
  start       = -1                               ;
  finish      = -1                               ;
  section     = ""                               ;
  block       = NULL                             ;
  font        = NULL                             ;
  pen         = NULL                             ;
  brush       = NULL                             ;
  paperPen    = NULL                             ;
  paperBrush  = NULL                             ;
  selectPen   = NULL                             ;
  selectBrush = NULL                             ;
  relation    = NULL                             ;
  attributes  = 0                                ;
  dirty       = false                            ;
  alignment   = Qt::AlignLeft | Qt::AlignVCenter ;
  linkages    . clear ( )                        ;
}

N::TextSection & N::TextSection::operator = (const TextSection & sect)
{
  nMemberCopy ( sect , block       ) ;
  nMemberCopy ( sect , index       ) ;
  nMemberCopy ( sect , section     ) ;
  nMemberCopy ( sect , selected    ) ;
  nMemberCopy ( sect , start       ) ;
  nMemberCopy ( sect , finish      ) ;
  nMemberCopy ( sect , font        ) ;
  nMemberCopy ( sect , pen         ) ;
  nMemberCopy ( sect , brush       ) ;
  nMemberCopy ( sect , paperPen    ) ;
  nMemberCopy ( sect , paperBrush  ) ;
  nMemberCopy ( sect , selectPen   ) ;
  nMemberCopy ( sect , selectBrush ) ;
  nMemberCopy ( sect , position    ) ;
  nMemberCopy ( sect , area        ) ;
  nMemberCopy ( sect , region      ) ;
  nMemberCopy ( sect , alignment   ) ;
  nMemberCopy ( sect , attributes  ) ;
  nMemberCopy ( sect , dirty       ) ;
  nMemberCopy ( sect , linkages    ) ;
  nMemberCopy ( sect , relation    ) ;
  return ME                          ;
}

N::TextSection * N::TextSection::clone(void)
{
  return new TextSection(ME) ;
}

void N::TextSection::Recover(const TextSection & sect)
{
  ME = sect ;
}

void N::TextSection::clearRendition (void)
{
  position = QPointF ( 0 , 0         ) ;
  area     = QRectF  ( 0 , 0 , 0 , 0 ) ;
  region   = QRectF  ( 0 , 0 , 0 , 0 ) ;
}

void N::TextSection::Render(QPainter * p)
{
  if (!selected) RenderText ( p ) ; else           {
    if (section.length()<=0) RenderEmpty     ( p ) ;
                       else  RenderSelection ( p ) ;
  }                                                ;
}

void N::TextSection::TextPainter(QPainter * p)
{
  nSafeExec ( font       ) p -> setFont  ( *font       ) ;
  nSafeExec ( pen        ) p -> setPen   ( *pen        ) ;
  nSafeExec ( brush      ) p -> setBrush ( *brush      ) ;
}

void N::TextSection::SymbolPainter(QPainter * p)
{ Q_UNUSED ( p ) ;
}

void N::TextSection::PaperPainter(QPainter * p)
{
  nSafeExec ( paperPen   ) p -> setPen   ( *paperPen   ) ;
  nSafeExec ( paperBrush ) p -> setBrush ( *paperBrush ) ;
}

void N::TextSection::SelectedPainter(QPainter * p)
{
  nSafeExec ( font       ) p -> setFont  ( *font       ) ;
  nSafeExec ( selectPen  ) p -> setPen   ( *selectPen  ) ;
  nSafeExec ( brush      ) p -> setBrush ( *brush      ) ;
}

void N::TextSection::SelectionPainter(QPainter * p)
{
  QColor c = selectBrush->color() ;
  p -> setPen    ( QPen(c)      ) ;
  p -> setBrush  ( *selectBrush ) ;
}

void N::TextSection::RenderText(QPainter * p)
{
  QRectF RT = Rectangle ( )                     ;
  PaperPainter    ( p                         ) ;
  if (NotNull(paperPen) || NotNull(paperBrush)) {
    p -> drawRect ( RT )                        ;
  }                                             ;
  TextPainter     ( p                         ) ;
  p -> drawText   ( RT , alignment , section  ) ;
}

void N::TextSection::RenderSymbol(QPainter * p)
{ Q_UNUSED ( p ) ;
}

void N::TextSection::RenderEmpty(QPainter * p)
{
  QRectF RT = Rectangle (                       ) ;
  SelectionPainter      ( p                     ) ;
  RT . setWidth         ( font->letterSpacing() ) ;
  p -> drawRect         ( RT                    ) ;
}

void N::TextSection::RenderSelection(QPainter * p)
{
  QRectF            RT  = Rectangle         ( )           ;
  int               len = section . length  ( )           ;
  QPointF           lp  = RT      . topLeft ( )           ;
  qreal             w                                     ;
  QMap<int,QString> Segments                              ;
  QMap<int,QRectF > Rects                                 ;
  if (start>0)                                            {
    Segments [0] = section.left(start)                    ;
    Rects    [0] = Measure(Segments[0])                   ;
    w = Rects[0] . width()                                ;
    w           += font->letterSpacing ( )                ;
    Rects    [0] = RT                                     ;
    Rects    [0] . setWidth ( w )                         ;
    lp           = Rects[0] . topRight ( )                ;
  }                                                       ;
  if (start<len)                                          {
    Segments [1] = section.mid(start,finish-start)        ;
    Rects    [1] = Measure(Segments[1])                   ;
    w = Rects[1] . width()                                ;
    w           += font->letterSpacing ( )                ;
    Rects    [1] = RT                                     ;
    Rects    [1] . setTopLeft ( lp )                      ;
    Rects    [1] . setWidth   ( w  )                      ;
    lp           = Rects[1] . topRight ( )                ;
  }                                                       ;
  if (finish<len)                                         {
    Segments [2] = section.mid(finish,len-finish)         ;
    Rects    [2] = Measure(Segments[2])                   ;
    w = Rects[2] . width()                                ;
    w           += font->letterSpacing ( )                ;
    Rects    [2] = RT                                     ;
    Rects    [2] . setTopLeft ( lp )                      ;
    Rects    [2] . setWidth   ( w  )                      ;
  }                                                       ;
  if (Segments.contains(0))                               {
    PaperPainter   ( p                                  ) ;
    if (NotNull(paperPen) || NotNull(paperBrush))         {
      p -> drawRect  ( Rects[0] )                         ;
    }                                                     ;
    TextPainter    ( p                                  ) ;
    p -> drawText  ( Rects[0] , alignment , Segments[0] ) ;
  }                                                       ;
  if (Segments.contains(1))                               {
    if (NotNull(selectBrush))                             {
      SelectionPainter ( p                              ) ;
      p -> drawRect  ( Rects[1]     )                     ;
    }                                                     ;
    SelectedPainter    ( p                              ) ;
    p -> drawText  ( Rects[1] , alignment , Segments[1] ) ;
  }                                                       ;
  if (Segments.contains(2))                               {
    PaperPainter   ( p                                  ) ;
    if (NotNull(paperPen) || NotNull(paperBrush))         {
      p -> drawRect  ( Rects[2] )                         ;
    }                                                     ;
    TextPainter    ( p                                  ) ;
    p -> drawText  ( Rects[2] , alignment , Segments[2] ) ;
  }                                                       ;
}

int N::TextSection::length(void)
{
  return section . length ( ) ;
}

QRectF N::TextSection::Rectangle(void)
{
  QSizeF  SF = area . size    ( ) ;
  QPointF TL = area . topLeft ( ) ;
  TL        += position           ;
  return QRectF ( TL , SF )       ;
}

QRectF N::TextSection::Measure(void)
{
  QRectF Z(0,0,0,0)                       ;
  nKickOut(IsNull(font),Z)                ;
  QFontMetricsF FMF(*font)                ;
  if (section.length()<=0)                {
    Z = FMF.boundingRect(" "    )         ;
  } else                                  {
    Z = FMF.boundingRect(section)         ;
  }                                       ;
  return QRectF(0,0,Z.width(),Z.height()) ;
}

QRectF N::TextSection::Measure(QString text)
{
  QRectF Z(0,0,0,0)                       ;
  nKickOut(IsNull(font),Z)                ;
  QFontMetricsF FMF(*font)                ;
  if (text.length()<=0)                   {
    Z = FMF.boundingRect(" " )            ;
  } else                                  {
    Z = FMF.boundingRect(text)            ;
  }                                       ;
  return QRectF(0,0,Z.width(),Z.height()) ;
}

bool N::TextSection::intersects(QPainterPath a)
{
  QRectF TA = Rectangle  (    ) ;
  return a  . intersects ( TA ) ;
}

bool N::TextSection::contains(QPainterPath a)
{
  QRectF TA = Rectangle (    ) ;
  return a  . contains  ( TA ) ;
}

bool N::TextSection::contains(QPointF pos)
{
  return region . contains  ( pos ) ;
}

bool N::TextSection::completeSelected(void)
{
  nKickOut ( NotEqual ( start  , 0                ) , false ) ;
  nKickOut ( NotEqual ( finish , section.length() ) , false ) ;
  return true                                                 ;
}

QPointF N::TextSection::Locate(QPointF pos)
{
  nKickOut(IsNull(font)       ,position)          ;
  nKickOut(section.length()<=0,position)          ;
  QPointF ps = pos                                ;
  qreal   x  = 0                                  ;
  qreal   p                                       ;
  QRectF  Z                                       ;
  QPointF At                                      ;
  bool    found = false                           ;
  ps -= position                                  ;
  QFontMetricsF FMF(*font)                        ;
  for (int i=1;!found && i<=section.length();i++) {
    QString s = section.left(i)                   ;
    Z = FMF.boundingRect(s)                       ;
    p = Z.right()                                 ;
    found = ((ps.x()>=x) && (ps.x()<=p))          ;
    if (!found) x = p                             ;
  }                                               ;
  At  . setX ( position.x() + x + 0.001 )         ;
  At  . setY ( position.y()     + 0.001 )         ;
  return At                                       ;
}

QPointF N::TextSection::sectionAt(int i)
{
  QPointF p(0,0)                           ;
  nKickOut(IsNull(font)       ,p+position) ;
  nKickOut(section.length()<=0,p+position) ;
  if (i>section.length())                  {
    i = section.length()                   ;
  }                                        ;
  QFontMetricsF FMF(*font)                 ;
  QString s = section.left(i)              ;
  QRectF  Z = FMF.boundingRect(s)          ;
  p  . setX(Z.right()+0.0001)              ;
  p += position                            ;
  return p                                 ;
}

int N::TextSection::cursorAt(QPointF pos)
{
  nKickOut(IsNull(font)       ,-1)                ;
  nKickOut(section.length()<=0,-1)                ;
  QPointF ps = pos                                ;
  qreal   x  = 0                                  ;
  qreal   p                                       ;
  QRectF  Z                                       ;
  bool    found = false                           ;
  ps -= position                                  ;
  QFontMetricsF FMF(*font)                        ;
  for (int i=1;!found && i<=section.length();i++) {
    QString s = section.left(i)                   ;
    Z = FMF.boundingRect(s)                       ;
    p = Z.right()                                 ;
    found = ((ps.x()>=x) && (ps.x()<=p))          ;
    if (!found) x = p ; else return i - 1         ;
  }                                               ;
  return -1                                       ;
}

void N::TextSection::Insert(int i,QString text)
{
  section.insert(i,text) ;
  dirty = true           ;
}

void N::TextSection::Cut(int i,int length)
{
  section.remove(i,length) ;
  dirty = true             ;
}

void N::TextSection::Delete(int i)
{
  section.remove(i,1) ;
  dirty = true        ;
}

int N::TextSection::addLinkage(int node,TextSection * sect)
{
  linkages [ node ] = sect   ;
  return linkages . count( ) ;
}

N::TextSection * N::TextSection::linkage(int node)
{
  nKickOut(!linkages.contains(node),NULL) ;
  return linkages [ node ]                ;
}

QRectF N::TextSection::TextRegion(int pos,int len)
{
  QString L  = section.left  ( pos            ) ;
  QString M  = section.mid   ( pos , len      ) ;
  QRectF  R  = Rectangle     (                ) ;
  QRectF  RL = Measure       ( L              ) ;
  QRectF  RM = Measure       ( M              ) ;
  QPointF TL = R  . topLeft  (                ) ;
  QPointF PL                 ( 0 , RL.width() ) ;
  TL += PL                                      ;
  R   . setTopLeft           ( TL             ) ;
  R   . setWidth             ( RM . width ( ) ) ;
  return R                                      ;
}
