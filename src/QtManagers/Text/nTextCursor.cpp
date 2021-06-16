#include <qtmanagers.h>

N::TextCursor:: TextCursor(void)
{
  clear ( ) ;
}

N::TextCursor:: TextCursor(const TextCursor & cursor)
{
  set ( cursor ) ;
}

N::TextCursor::~TextCursor(void)
{
}

void N::TextCursor::clear(void)
{
  Block       = -1                        ;
  Page        = -1                        ;
  Paragraph   = -1                        ;
  At          = -1                        ;
  InputMethod = false                     ;
  Selected    = false                     ;
  Section     = NULL                      ;
  Full        = NULL                      ;
  Original    = NULL                      ;
  Position    = QPointF ( 0 , 0         ) ;
  Area        = QRectF  ( 0 , 0 , 0 , 0 ) ;
}

N::TextCursor & N::TextCursor::operator = (const TextCursor & cursor)
{
  return set ( cursor ) ;
}

N::TextCursor & N::TextCursor::set(const TextCursor & cursor)
{
  nMemberCopy ( cursor , Block       ) ;
  nMemberCopy ( cursor , Page        ) ;
  nMemberCopy ( cursor , Paragraph   ) ;
  nMemberCopy ( cursor , At          ) ;
  nMemberCopy ( cursor , Position    ) ;
  nMemberCopy ( cursor , Area        ) ;
  nMemberCopy ( cursor , Section     ) ;
  nMemberCopy ( cursor , Full        ) ;
  nMemberCopy ( cursor , Original    ) ;
  nMemberCopy ( cursor , InputMethod ) ;
  nMemberCopy ( cursor , Selected    ) ;
  nMemberCopy ( cursor , Selections  ) ;
  nMemberCopy ( cursor , Clicks      ) ;
  nMemberCopy ( cursor , Points      ) ;
  return ME                            ;
}

void N::TextCursor::empty(void)
{
  At      = -1   ;
  Section = NULL ;
}

bool N::TextCursor::isActivated(void)
{
  nKickOut ( At<0            , false ) ;
  nKickOut ( IsNull(Section) , false ) ;
  return true                          ;
}

QPointF N::TextCursor::sectionAt(void)
{
  return Section->sectionAt(At) ;
}

void N::TextCursor::clearSelection(void)
{
  for (int i=0;i<Selections.count();i++) {
    TextSection * ts = Selections[i]     ;
    ts->selected = false                 ;
  }                                      ;
  Selected   = false                     ;
  Clicks     . clear ( )                 ;
  Selections . clear ( )                 ;
  Points     . clear ( )                 ;
}

void N::TextCursor::setPosition(int id,QPointF pos)
{
  Points[id] = pos ;
}

QPointF N::TextCursor::Left(void)
{
  At --              ;
  return sectionAt() ;
}

QPointF N::TextCursor::Right(void)
{
  At ++              ;
  return sectionAt() ;
}

void N::TextCursor::Anchor(void)
{
  nDropOut(!isActivated()) ;
  Section->start = At      ;
  Clicks [ 0 ]   = At      ;
}

void N::TextCursor::Last(TextSection * section)
{
  Section = section              ;
  At      = -1                   ;
  nDropOut ( IsNull(Section) )   ;
  At = Section->section.length() ;
}

void N::TextCursor::Home(TextSection * section)
{
  Section = section         ;
  At      = -1              ;
  nDropOut(IsNull(Section)) ;
  At = 0                    ;
}

int N::TextCursor::sectionLength(void)
{
  return Section->section.length() ;
}

void N::TextCursor::Booking(void)
{
  Block     = 0 ;
  Page      = 0 ;
  Paragraph = 0 ;
}

void N::TextCursor::MarkStart(void)
{
  nDropOut(IsNull(Section)) ;
  if (Section->start<0)     {
    Section -> start  = At  ;
    Section -> finish = At  ;
    Clicks[11] = At         ;
  }                         ;
  Selected = true           ;
}

void N::TextCursor::MarkSelected(void)
{
  nDropOut(IsNull(Section))          ;
  Clicks[12] = At                    ;
  Section -> selected = true         ;
  Section -> finish   = At           ;
  if (!Selections.contains(Section)) {
    Selections << Section            ;
  }                                  ;
  Selected = true                    ;
}

void N::TextCursor::MarkFirst(void)
{
  nDropOut ( IsNull(Section) ) ;
}

void N::TextCursor::MarkSecond(void)
{
  nDropOut ( IsNull(Section) ) ;
}

bool N::TextCursor::Select(void)
{
  nKickOut ( IsNull(Section) , false ) ;
  return true                          ;
}

void N::TextCursor::EnterIM(void)
{
  if (InputMethod    )          {
    At = Clicks[99]             ;
    Full -> Recover (*Original) ;
    return                      ;
  }                             ;
  nDropOut(IsNull(Section))     ;
  InputMethod = true            ;
  Full        = Section->block  ;
  Original    = Full->clone()   ;
  Clicks[99]  = At              ;
}

void N::TextCursor::LeaveIM(void)
{
  At = Clicks[99]             ;
  nDropOut(!InputMethod    )  ;
  nDropOut(IsNull(Full)    )  ;
  nDropOut(IsNull(Original))  ;
  At = Clicks[99]             ;
  InputMethod = false         ;
  Full -> Recover (*Original) ;
  nEnsureNull ( Original )    ;
}
