#include <qtmanagers.h>

const int HEXCHARS_IN_LINE = 47 ;
const int GAP_ADR_HEX      = 10 ;
const int GAP_HEX_ASCII    = 16 ;
const int BYTES_PER_LINE   = 16 ;

N::HexWidget:: HexWidget    ( QScrollArea * parent , Plan * p )
             : Widget       (               parent ,        p )
             , Object       ( 0 , Cpp::Undecided              )
             , _cursorTimer ( new QTimer ( this )             )
             , Name         ( ""                              )
{
  WidgetClass          ;
  _scrollArea = parent ;
  Configure ( )        ;
}

N::HexWidget::~HexWidget(void)
{
}

bool N::HexWidget::FocusIn(void)
{
  LinkAction ( Save , Save() ) ;
  return true                  ;
}

void N::HexWidget::Configure(void)
{
  _undoStack = new QUndoStack(this)                       ;
  _capital   = false                                      ;
  /////////////////////////////////////////////////////////
  setAddressWidth      ( 4                              ) ;
  setAddressOffset     ( 0                              ) ;
  setAddressArea       ( true                           ) ;
  setAsciiArea         ( true                           ) ;
  setHighlighting      ( true                           ) ;
  setOverwriteMode     ( true                           ) ;
  setReadOnly          ( false                          ) ;
  setFont              ( QFont("Courier New")           ) ;
  /////////////////////////////////////////////////////////
  setAddressAreaColor  ( QColor(0xd4, 0xd4, 0xd4, 0xff) ) ;
  setHighlightingColor ( QColor(0xff, 0xff, 0x99, 0xff) ) ;
  setSelectionColor    ( QColor(0x6d, 0x9e, 0xff, 0xff) ) ;
  /////////////////////////////////////////////////////////
  _size = 0                                               ;
  resetSelection(0)                                       ;
  /////////////////////////////////////////////////////////
  setFocusPolicy(Qt::StrongFocus)                         ;
  /////////////////////////////////////////////////////////
  nConnect ( _cursorTimer , SIGNAL ( timeout      ( ) )   ,
             this         , SLOT   ( updateCursor ( ) ) ) ;
  /////////////////////////////////////////////////////////
  _cursorTimer -> setInterval ( 500 )                     ;
  _cursorTimer -> start       (     )                     ;
}

void N::HexWidget::setAddressOffset(int offset)
{
  _xData . setAddressOffset ( offset ) ;
  adjust                    (        ) ;
}

int N::HexWidget::addressOffset(void)
{
  return _xData.addressOffset() ;
}

void N::HexWidget::setData(const QByteArray & data)
{
  _xData      . setData ( data ) ;
  _undoStack -> clear   (      ) ;
  adjust                (      ) ;
  setCursorPos          ( 0    ) ;
}

QByteArray N::HexWidget::data(void)
{
  return _xData.data() ;
}

void N::HexWidget::setAddressAreaColor(const QColor & color)
{
  _addressAreaColor = color ;
  update ( )                ;
}

QColor N::HexWidget::addressAreaColor(void)
{
  return _addressAreaColor ;
}

void N::HexWidget::setHighlightingColor(const QColor & color)
{
  _highlightingColor = color ;
  update ( )                 ;
}

QColor N::HexWidget::highlightingColor(void)
{
  return _highlightingColor ;
}

void N::HexWidget::setSelectionColor(const QColor & color)
{
  _selectionColor = color ;
  update ( )              ;
}

QColor N::HexWidget::selectionColor(void)
{
  return _selectionColor ;
}

void N::HexWidget::setReadOnly(bool readOnly)
{
  _readOnly = readOnly ;
}

bool N::HexWidget::isReadOnly(void)
{
  return _readOnly ;
}

N::DataArray & N::HexWidget::xData(void)
{
  return _xData ;
}

int N::HexWidget::indexOf(const QByteArray & ba,int from)
{
  if (from > (_xData.data().length() - 1))  {
      from =  _xData.data().length() - 1    ;
  }                                         ;
  ///////////////////////////////////////////
  int idx = _xData.data().indexOf(ba,from)  ;
  if (idx > -1)                             {
    int curPos = idx * 2                    ;
    setCursorPos   (curPos + ba.length()*2) ;
    resetSelection (curPos                ) ;
    setSelection   (curPos + ba.length()*2) ;
    ensureVisible  (                      ) ;
  }                                         ;
  ///////////////////////////////////////////
  return idx                                ;
}

void N::HexWidget::insert(int index,const QByteArray & ba)
{
  if (ba.length() > 0)                                {
    QUndoCommand * arrayCommand                       ;
    if (_overwriteMode)                               {
      arrayCommand = new DataArrayCommand             (
                           &_xData                    ,
                           DataArrayCommand::replace  ,
                           index                      ,
                           ba                         ,
                           ba.length()              ) ;
      _undoStack->push ( arrayCommand )               ;
      emit dataChanged (              )               ;
    } else                                            {
      arrayCommand = new DataArrayCommand             (
                           &_xData                    ,
                           DataArrayCommand::insert   ,
                           index                      ,
                           ba                         ,
                           ba.length()              ) ;
      _undoStack->push ( arrayCommand )               ;
      emit dataChanged (              )               ;
    }                                                 ;
  }                                                   ;
}

void N::HexWidget::insert(int index,char ch)
{
  QUndoCommand * charCommand                   ;
  charCommand = new DataCharCommand            (
                      &_xData                  ,
                      DataCharCommand::insert  ,
                      index                    ,
                      ch                     ) ;
  _undoStack->push ( charCommand )             ;
  emit dataChanged (             )             ;
}

int N::HexWidget::lastIndexOf(const QByteArray & ba,int from)
{
  from -= ba.length()                          ;
  if (from < 0) from = 0                       ;
  int idx = _xData.data().lastIndexOf(ba,from) ;
  if (idx > -1)                                {
    int curPos = idx * 2                       ;
    setCursorPos   ( curPos                  ) ;
    resetSelection ( curPos                  ) ;
    setSelection   ( curPos + ba.length()*2  ) ;
    ensureVisible  (                         ) ;
  }                                            ;
  return idx                                   ;
}

void N::HexWidget::remove(int index,int len)
{
  nDropOut ( len <= 0 )                               ;
  /////////////////////////////////////////////////////
  if (len == 1)                                       {
    QUndoCommand * charCommand                        ;
    if (_overwriteMode)                               {
      charCommand = new DataCharCommand               (
                          &_xData                     ,
                          DataCharCommand::replace    ,
                          index                       ,
                          char(0)                   ) ;
    } else                                            {
      charCommand = new DataCharCommand               (
                          &_xData                     ,
                          DataCharCommand::remove     ,
                          index                       ,
                          char(0)                   ) ;
    }                                                 ;
    _undoStack->push ( charCommand )                  ;
    emit dataChanged (             )                  ;
  } else                                              {
    QUndoCommand * arrayCommand                       ;
    QByteArray     ba = QByteArray(len,char(0))       ;
    if (_overwriteMode)                               {
      arrayCommand = new DataArrayCommand             (
                           &_xData                    ,
                           DataArrayCommand::replace  ,
                           index                      ,
                           ba                         ,
                           ba.length()              ) ;
    } else                                            {
      arrayCommand = new DataArrayCommand             (
                           &_xData                    ,
                           DataArrayCommand::remove   ,
                           index                      ,
                           ba                         ,
                           len                      ) ;
    }                                                 ;
    _undoStack->push ( arrayCommand )                 ;
    emit dataChanged (              )                 ;
  }                                                   ;
}

void N::HexWidget::replace(int index,char ch)
{
  QUndoCommand * charCommand                    ;
  charCommand = new DataCharCommand             (
                      &_xData                   ,
                      DataCharCommand::replace  ,
                      index                     ,
                      ch                      ) ;
  _undoStack->push ( charCommand )              ;
  resetSelection   (             )              ;
  emit dataChanged (             )              ;
}

void N::HexWidget::replace(int index, const QByteArray & ba)
{
  QUndoCommand * arrayCommand                    ;
  arrayCommand = new DataArrayCommand            (
                       &_xData                   ,
                       DataArrayCommand::replace ,
                       index                     ,
                       ba                        ,
                       ba.length()             ) ;
  _undoStack->push ( arrayCommand )              ;
  resetSelection   (              )              ;
  emit dataChanged (              )              ;
}

void N::HexWidget::replace(int pos,int len,const QByteArray & after)
{
  QUndoCommand * arrayCommand                   ;
  arrayCommand= new DataArrayCommand            (
                      &_xData                   ,
                      DataArrayCommand::replace ,
                      pos                       ,
                      after                     ,
                      len                     ) ;
  _undoStack->push ( arrayCommand )             ;
  resetSelection   (              )             ;
  emit dataChanged (              )             ;
}

void N::HexWidget::setAddressArea(bool addressArea)
{
  _addressArea = addressArea       ;
  adjust       (                 ) ;
  setCursorPos ( _cursorPosition ) ;
}

void N::HexWidget::setAddressWidth(int addressWidth)
{
  _xData.setAddressWidth ( addressWidth    ) ;
  setCursorPos           ( _cursorPosition ) ;
}

void N::HexWidget::setAsciiArea(bool asciiArea)
{
  _asciiArea = asciiArea ;
  adjust ( )             ;
}

void N::HexWidget::setFont(const QFont & font)
{
  QWidget::setFont ( font ) ;
  adjust           (      ) ;
}

void N::HexWidget::setHighlighting(bool mode)
{
  _highlighting = mode ;
  update ( )           ;
}

void N::HexWidget::setOverwriteMode(bool overwriteMode)
{
  _overwriteMode = overwriteMode ;
}

bool N::HexWidget::overwriteMode(void)
{
  return _overwriteMode ;
}

void N::HexWidget::redo(void)
{
  _undoStack->redo (                 ) ;
  emit dataChanged (                 ) ;
  setCursorPos     ( _cursorPosition ) ;
  update           (                 ) ;
}

void N::HexWidget::undo(void)
{
  _undoStack->undo (                 ) ;
  emit dataChanged (                 ) ;
  setCursorPos     ( _cursorPosition ) ;
  update           (                 ) ;
}

QString N::HexWidget::toReadableString(void)
{
  return _xData.toReadableString ( ) ;
}

QString N::HexWidget::selectionToReadableString(void)
{
  return _xData.toReadableString( getSelectionBegin() , getSelectionEnd() ) ;
}

void N::HexWidget::contextMenuEvent(QContextMenuEvent * event)
{
  if (Menu(event->pos())) event->accept(); else
  QWidget::contextMenuEvent(event);
}

void N::HexWidget::focusInEvent(QFocusEvent * event)
{
  if (!focusIn (event)) QWidget::focusInEvent (event) ;
}

void N::HexWidget::focusOutEvent(QFocusEvent * event)
{
  if (!focusOut(event)) QWidget::focusOutEvent(event) ;
}

void N::HexWidget::keyPressEvent(QKeyEvent *event)
{
  int charX = ( _cursorX - _xPosHex ) / _charWidth                            ;
  int posX  = ( charX / 3 ) * 2                             + (charX % 3)     ;
  int posBa = ( (_cursorY / _charHeight) * BYTES_PER_LINE ) +  posX  / 2      ;
  /////////////////////////////////////////////////////////////////////////////
  if (event->matches(QKeySequence::MoveToNextChar))                           {
    setCursorPos   ( _cursorPosition + 1 )                                    ;
    resetSelection ( _cursorPosition     )                                    ;
  }                                                                           ;
  /////////////////////////////////////////////////////////////////////////////
  if (event->matches(QKeySequence::MoveToPreviousChar))                       {
    setCursorPos   ( _cursorPosition - 1 )                                    ;
    resetSelection ( _cursorPosition     )                                    ;
  }                                                                           ;
  /////////////////////////////////////////////////////////////////////////////
  if (event->matches(QKeySequence::MoveToEndOfLine))                          {
    setCursorPos   ( _cursorPosition | (2 * BYTES_PER_LINE -1) )              ;
    resetSelection ( _cursorPosition                           )              ;
  }                                                                           ;
  /////////////////////////////////////////////////////////////////////////////
  if (event->matches(QKeySequence::MoveToStartOfLine))                        {
    setCursorPos   ( _cursorPosition - (_cursorPosition%(2*BYTES_PER_LINE)))  ;
    resetSelection ( _cursorPosition                                       )  ;
  }                                                                           ;
  /////////////////////////////////////////////////////////////////////////////
  if (event->matches(QKeySequence::MoveToPreviousLine))                       {
    setCursorPos   ( _cursorPosition - (2 * BYTES_PER_LINE) )                 ;
    resetSelection ( _cursorPosition                        )                 ;
  }                                                                           ;
  /////////////////////////////////////////////////////////////////////////////
  if (event->matches(QKeySequence::MoveToNextLine))                           {
    setCursorPos   ( _cursorPosition + (2 * BYTES_PER_LINE) )                 ;
    resetSelection ( _cursorPosition                        )                 ;
  }                                                                           ;
  /////////////////////////////////////////////////////////////////////////////
  if (event->matches(QKeySequence::MoveToNextPage))                           {
    setCursorPos   ( _cursorPosition                                          +
                    (((_scrollArea->viewport()->height() / _charHeight) - 1   )
                     * 2 * BYTES_PER_LINE )                                 ) ;
    resetSelection ( _cursorPosition                                        ) ;
  }                                                                           ;
  /////////////////////////////////////////////////////////////////////////////
  if (event->matches(QKeySequence::MoveToPreviousPage))                       {
    setCursorPos   ( _cursorPosition                                          -
                     (((_scrollArea->viewport()->height()/_charHeight)-1      )
                      * 2 * BYTES_PER_LINE )                                ) ;
    resetSelection ( _cursorPosition                                        ) ;
  }                                                                           ;
  /////////////////////////////////////////////////////////////////////////////
  if (event->matches(QKeySequence::MoveToEndOfDocument))                      {
    setCursorPos   ( _xData.size() * 2 )                                      ;
    resetSelection ( _cursorPosition   )                                      ;
  }                                                                           ;
  /////////////////////////////////////////////////////////////////////////////
  if (event->matches(QKeySequence::MoveToStartOfDocument))                    {
    setCursorPos   ( 0               )                                        ;
    resetSelection ( _cursorPosition )                                        ;
  }                                                                           ;
  /////////////////////////////////////////////////////////////////////////////
  if (event->matches(QKeySequence::SelectAll))                                {
    resetSelection ( 0                     )                                  ;
    setSelection   ( (2*_xData.size()) + 1 )                                  ;
  }                                                                           ;
  /////////////////////////////////////////////////////////////////////////////
  if (event->matches(QKeySequence::SelectNextChar))                           {
    int pos = _cursorPosition + 1                                             ;
    setCursorPos ( pos )                                                      ;
    setSelection ( pos )                                                      ;
  }                                                                           ;
  /////////////////////////////////////////////////////////////////////////////
  if (event->matches(QKeySequence::SelectPreviousChar))                       {
    int pos = _cursorPosition - 1                                             ;
    setSelection ( pos )                                                      ;
    setCursorPos ( pos )                                                      ;
  }                                                                           ;
  /////////////////////////////////////////////////////////////////////////////
  if (event->matches(QKeySequence::SelectEndOfLine))                          {
    int pos = _cursorPosition                                                 -
             (_cursorPosition % (2*BYTES_PER_LINE)) + (2*BYTES_PER_LINE)      ;
    setCursorPos ( pos )                                                      ;
    setSelection ( pos )                                                      ;
  }                                                                           ;
  /////////////////////////////////////////////////////////////////////////////
  if (event->matches(QKeySequence::SelectStartOfLine))                        {
    int pos = _cursorPosition - (_cursorPosition % (2 * BYTES_PER_LINE))      ;
    setCursorPos ( pos )                                                      ;
    setSelection ( pos )                                                      ;
  }                                                                           ;
  /////////////////////////////////////////////////////////////////////////////
  if (event->matches(QKeySequence::SelectPreviousLine))                       {
    int pos = _cursorPosition - (2 * BYTES_PER_LINE)                          ;
    setCursorPos ( pos )                                                      ;
    setSelection ( pos )                                                      ;
  }                                                                           ;
  /////////////////////////////////////////////////////////////////////////////
  if (event->matches(QKeySequence::SelectNextLine))                           {
    int pos = _cursorPosition + (2 * BYTES_PER_LINE)                          ;
    setCursorPos ( pos )                                                      ;
    setSelection ( pos )                                                      ;
  }                                                                           ;
  /////////////////////////////////////////////////////////////////////////////
  if (event->matches(QKeySequence::SelectNextPage))                           {
    int pos = _cursorPosition                                                 +
              (((_scrollArea->viewport()->height() / _charHeight) - 1         )
              * 2 * BYTES_PER_LINE                                          ) ;
    setCursorPos ( pos )                                                      ;
    setSelection ( pos )                                                      ;
  }                                                                           ;
  /////////////////////////////////////////////////////////////////////////////
  if (event->matches(QKeySequence::SelectPreviousPage))                       {
    int pos = _cursorPosition                                                 -
              (((_scrollArea->viewport()->height() / _charHeight) - 1         )
              * 2 * BYTES_PER_LINE                                          ) ;
    setCursorPos ( pos )                                                      ;
    setSelection ( pos )                                                      ;
  }                                                                           ;
  /////////////////////////////////////////////////////////////////////////////
  if (event->matches(QKeySequence::SelectEndOfDocument))                      {
    int pos = _xData.size() * 2                                               ;
    setCursorPos ( pos )                                                      ;
    setSelection ( pos )                                                      ;
  }                                                                           ;
  /////////////////////////////////////////////////////////////////////////////
  if (event->matches(QKeySequence::SelectStartOfDocument))                    {
    int pos = 0                                                               ;
    setCursorPos ( pos )                                                      ;
    setSelection ( pos )                                                      ;
  }                                                                           ;
  /////////////////////////////////////////////////////////////////////////////
  if (!_readOnly)                                                             {
    int key = int(event->text()[0].toLatin1())                                ;
    if ((key>='0' && key<='9') || (key>='a' && key <= 'f'))                   {
      /////////////////////////////////////////////////////////////////////////
      if (getSelectionBegin() != getSelectionEnd())                           {
        posBa = getSelectionBegin ( )                                         ;
        remove         ( posBa,getSelectionEnd()-posBa )                      ;
        setCursorPos   ( 2 * posBa                     )                      ;
        resetSelection ( 2 * posBa                     )                      ;
      }                                                                       ;
      /////////////////////////////////////////////////////////////////////////
      if (_overwriteMode == false)                                            {
        if ((charX % 3) == 0) insert(posBa,char(0))                           ;
      }                                                                       ;
      /////////////////////////////////////////////////////////////////////////
      if (_xData.size() > 0)                                                  {
        QByteArray hexValue = _xData.data().mid(posBa, 1).toHex()             ;
        if ((charX % 3) == 0) hexValue[0] = key                               ;
                         else hexValue[1] = key                               ;
        replace        ( posBa, QByteArray().fromHex(hexValue)[0] )           ;
        setCursorPos   ( _cursorPosition + 1                      )           ;
        resetSelection ( _cursorPosition                          )           ;
      }                                                                       ;
    }                                                                         ;
    ///////////////////////////////////////////////////////////////////////////
    if (event->matches(QKeySequence::Cut))                                    {
      QString result = QString()                                              ;
      for (int idx = getSelectionBegin(); idx < getSelectionEnd(); idx++)     {
        result += _xData.data().mid(idx, 1).toHex() + " "                     ;
        if ((idx % 16) == 15) result.append("\n")                             ;
      }                                                                       ;
      remove(getSelectionBegin(), getSelectionEnd() - getSelectionBegin())    ;
      /////////////////////////////////////////////////////////////////////////
      QClipboard * clipboard = QApplication::clipboard()                      ;
      clipboard -> setText ( result              )                            ;
      setCursorPos         ( getSelectionBegin() )                            ;
      resetSelection       ( getSelectionBegin() )                            ;
    }                                                                         ;
    ///////////////////////////////////////////////////////////////////////////
    if (event->matches(QKeySequence::Paste))                                  {
      QClipboard * clipboard = QApplication::clipboard()                      ;
      QByteArray   ba    = QByteArray().fromHex(clipboard->text().toLatin1()) ;
      insert         ( _cursorPosition / 2 , ba          )                    ;
      setCursorPos   ( _cursorPosition + 2 * ba.length() )                    ;
      resetSelection ( getSelectionBegin()               )                    ;
    }                                                                         ;
    ///////////////////////////////////////////////////////////////////////////
    if (event->matches(QKeySequence::Delete))                                 {
      if (getSelectionBegin() != getSelectionEnd())                           {
        posBa = getSelectionBegin()                                           ;
        remove         (     posBa , getSelectionEnd() - posBa )              ;
        setCursorPos   ( 2 * posBa                             )              ;
        resetSelection ( 2 * posBa                             )              ;
      } else                                                                  {
        if (_overwriteMode) replace ( posBa , char(0) )                       ;
                       else remove  ( posBa , 1       )                       ;
      }                                                                       ;
    }                                                                         ;
    ///////////////////////////////////////////////////////////////////////////
    if ((event->key      () == Qt::Key_Backspace)                            &&
        (event->modifiers() == Qt::NoModifier   )                           ) {
      if (getSelectionBegin() != getSelectionEnd())                           {
        posBa = getSelectionBegin()                                           ;
        remove         (     posBa , getSelectionEnd() - posBa )              ;
        setCursorPos   ( 2 * posBa                             )              ;
        resetSelection ( 2 * posBa                             )              ;
      } else                                                                  {
        if (posBa > 0)                                                        {
          if (_overwriteMode) replace ( posBa - 1 , char(0) )                 ;
                         else remove  ( posBa - 1 , 1       )                 ;
          setCursorPos ( _cursorPosition - 2 )                                ;
        }                                                                     ;
      }                                                                       ;
    }                                                                         ;
    ///////////////////////////////////////////////////////////////////////////
    if (event->matches(QKeySequence::Undo)) undo ( )                          ;
    if (event->matches(QKeySequence::Redo)) redo ( )                          ;
  }                                                                           ;
  /////////////////////////////////////////////////////////////////////////////
  if (event->matches(QKeySequence::Copy))                                     {
    QString result = QString()                                                ;
    for (int idx = getSelectionBegin(); idx < getSelectionEnd(); idx++)       {
      result += _xData.data().mid(idx, 1).toHex() + " "                       ;
      if ((idx % 16) == 15) result.append('\n')                               ;
    }                                                                         ;
    QClipboard * clipboard = QApplication::clipboard()                        ;
    clipboard->setText(result)                                                ;
  }                                                                           ;
  /////////////////////////////////////////////////////////////////////////////
  if ((event->key      () == Qt::Key_Insert)                                 &&
      (event->modifiers() == Qt::NoModifier)                                ) {
     _overwriteMode = !_overwriteMode                                         ;
     setCursorPos         ( _cursorPosition )                                 ;
     overwriteModeChanged ( _overwriteMode  )                                 ;
  }                                                                           ;
  /////////////////////////////////////////////////////////////////////////////
  ensureVisible ( )                                                           ;
  update        ( )                                                           ;
}

void N::HexWidget::mouseMoveEvent(QMouseEvent * event)
{
  _blink = false                          ;
  update                 (              ) ;
  int actPos = cursorPos ( event->pos() ) ;
  setCursorPos           ( actPos       ) ;
  setSelection           ( actPos       ) ;
}

void N::HexWidget::mousePressEvent(QMouseEvent * event)
{
  if (IsMask(event->buttons(),Qt::LeftButton)) {
    _blink = false                             ;
    update                   (              )  ;
    int cPos   = cursorPos   ( event->pos() )  ;
    resetSelection           ( cPos         )  ;
    setCursorPos             ( cPos         )  ;
  } else                                       {
    QWidget::mousePressEvent ( event        )  ;
  }                                            ;
}

void N::HexWidget::paintEvent(QPaintEvent *event)
{
  QPainter painter(this)                                                      ;
  /////////////////////////////////////////////////////////////////////////////
  painter.fillRect ( event->rect() , this->palette().color(QPalette::Base) )  ;
  /////////////////////////////////////////////////////////////////////////////
  if (_addressArea)                                                           {
    painter . fillRect                                                        (
      QRect(_xPosAdr                                                          ,
            event->rect().top()                                               ,
            _xPosHex - GAP_ADR_HEX + 2, height())                             ,
            _addressAreaColor                                               ) ;
  } else
  if (_asciiArea)                                                             {
    int linePos = _xPosAscii - (GAP_HEX_ASCII / 2)                            ;
    painter . setPen   ( Qt::gray                                      )      ;
    painter . drawLine ( linePos,event->rect().top(),linePos, height() )      ;
  }                                                                           ;
  /////////////////////////////////////////////////////////////////////////////
  painter . setPen ( this->palette().color(QPalette::WindowText) )            ;
  /////////////////////////////////////////////////////////////////////////////
  int firstLineIdx = ((event->rect().top()/_charHeight)-_charHeight)
                     * BYTES_PER_LINE                                         ;
  if (firstLineIdx < 0) firstLineIdx = 0                                      ;
  int lastLineIdx = ((event->rect().bottom() / _charHeight) + _charHeight)
                    * BYTES_PER_LINE                                          ;
  if (lastLineIdx > _xData.size()) lastLineIdx = _xData.size()                ;
  int yPosStart = ((firstLineIdx) / BYTES_PER_LINE)*_charHeight + _charHeight ;
  /////////////////////////////////////////////////////////////////////////////
  if (_addressArea)                                                           {
    for (int lineIdx = firstLineIdx, yPos = yPosStart                         ;
         lineIdx < lastLineIdx                                                ;
         lineIdx += BYTES_PER_LINE, yPos +=_charHeight                      ) {
      QString address = QString("%1"                                          )
                        .arg(lineIdx + _xData.addressOffset()                 ,
                             _xData.realAddressNumbers()                      ,
                             16                                               ,
                             QChar('0')                                     ) ;
      if (_capital) address = address.toUpper()                               ;
      painter.drawText ( _xPosAdr , yPos , address )                          ;
    }                                                                         ;
  }                                                                           ;
  /////////////////////////////////////////////////////////////////////////////
  QByteArray hexBa(_xData.data(                                               )
                   .mid(firstLineIdx, lastLineIdx - firstLineIdx + 1          )
                   .toHex()                                                 ) ;
  QBrush     highLighted    = QBrush(_highlightingColor                     ) ;
  QPen       colHighlighted = QPen  (this->palette(                           )
                                     .color(QPalette::WindowText)           ) ;
  QBrush     selected       = QBrush(_selectionColor                        ) ;
  QPen       colSelected    = QPen  (Qt::white                              ) ;
  QPen       colStandard    = QPen  (this->palette(                           )
                                     .color(QPalette::WindowText)           ) ;
  /////////////////////////////////////////////////////////////////////////////
  painter . setBackgroundMode ( Qt::TransparentMode )                         ;
  /////////////////////////////////////////////////////////////////////////////
  for (int lineIdx = firstLineIdx, yPos = yPosStart                           ;
       lineIdx < lastLineIdx                                                  ;
       lineIdx += BYTES_PER_LINE, yPos +=_charHeight                        ) {
    ///////////////////////////////////////////////////////////////////////////
    QByteArray hex                                                            ;
    int        xPos = _xPosHex                                                ;
    ///////////////////////////////////////////////////////////////////////////
    for (int colIdx = 0                                                       ;
         ((lineIdx + colIdx) < _xData.size() && (colIdx < BYTES_PER_LINE))    ;
         colIdx++                                                           ) {
      int posBa = lineIdx + colIdx                                            ;
      if ((getSelectionBegin() <= posBa) && (getSelectionEnd() > posBa))      {
        painter . setBackground     ( selected       )                        ;
        painter . setBackgroundMode ( Qt::OpaqueMode )                        ;
        painter . setPen            ( colSelected    )                        ;
      } else                                                                  {
        if (_highlighting)                                                    {
          painter.setBackground(highLighted)                                  ;
          if (_xData.dataChanged(posBa))                                      {
            painter . setPen            ( colHighlighted      )               ;
            painter . setBackgroundMode ( Qt::OpaqueMode      )               ;
          } else                                                              {
            painter . setPen            ( colStandard         )               ;
            painter . setBackgroundMode ( Qt::TransparentMode )               ;
          }                                                                   ;
        }                                                                     ;
      }                                                                       ;
      /////////////////////////////////////////////////////////////////////////
      if (colIdx == 0)                                                        {
        QString HEXS                                                          ;
        hex = hexBa.mid  ( (lineIdx - firstLineIdx     )*2,2 )                ;
        HEXS = QString::fromUtf8(hex)                                         ;
        if (_capital) HEXS = HEXS.toUpper()                                   ;
        painter.drawText ( xPos, yPos, HEXS                               )   ;
        xPos += 2 * _charWidth                                                ;
      } else                                                                  {
        QString HEXS                                                          ;
        hex = hexBa.mid  ( (lineIdx+colIdx-firstLineIdx)*2,2 ).prepend(" ")   ;
        HEXS = QString::fromUtf8(hex)                                         ;
        if (_capital) HEXS = HEXS.toUpper()                                   ;
        painter.drawText ( xPos, yPos, HEXS                               )   ;
        xPos += 3 * _charWidth                                                ;
      }                                                                       ;
    }                                                                         ;
  }                                                                           ;
  /////////////////////////////////////////////////////////////////////////////
  painter . setBackgroundMode ( Qt::TransparentMode                         ) ;
  painter . setPen            ( this->palette().color(QPalette::WindowText) ) ;
  /////////////////////////////////////////////////////////////////////////////
  if (_asciiArea)                                                             {
    for (int lineIdx = firstLineIdx, yPos = yPosStart                         ;
         lineIdx < lastLineIdx                                                ;
         lineIdx += BYTES_PER_LINE, yPos +=_charHeight                      ) {
      int xPosAscii = _xPosAscii                                              ;
      for (int colIdx = 0                                                     ;
           ((lineIdx + colIdx) < _xData.size() && (colIdx < BYTES_PER_LINE))  ;
           colIdx++)                                                          {
        painter.drawText(xPosAscii,yPos,_xData.asciiChar(lineIdx + colIdx)  ) ;
        xPosAscii += _charWidth                                               ;
      }                                                                       ;
    }                                                                         ;
  }                                                                           ;
  /////////////////////////////////////////////////////////////////////////////
  if (_blink && !_readOnly && hasFocus())                                     {
    if (_overwriteMode)                                                       {
      painter.fillRect                                                        (
        _cursorX                                                              ,
        _cursorY + _charHeight - 2                                            ,
        _charWidth                                                            ,
        2                                                                     ,
        this->palette().color(QPalette::WindowText)                         ) ;
    } else                                                                    {
      painter.fillRect                                                        (
        _cursorX                                                              ,
        _cursorY                                                              ,
        2                                                                     ,
        _charHeight                                                           ,
        this->palette().color(QPalette::WindowText)                         ) ;
    }                                                                         ;
  }                                                                           ;
  /////////////////////////////////////////////////////////////////////////////
  if (_size != _xData.size())                                                 {
    _size = _xData.size     (       )                                         ;
    emit currentSizeChanged ( _size )                                         ;
  }                                                                           ;
}

void N::HexWidget::setCursorPos(int position)
{
  _blink = false                                                        ;
  update ( )                                                            ;
  ///////////////////////////////////////////////////////////////////////
  if (_overwriteMode)                                                   {
    if (position > (_xData.size() * 2 - 1))                             {
        position =  _xData.size() * 2 - 1                               ;
    }                                                                   ;
  } else                                                                {
    if (position > (_xData.size() * 2) )                                {
        position =  _xData.size() * 2                                   ;
    }                                                                   ;
  }                                                                     ;
  ///////////////////////////////////////////////////////////////////////
  if (position < 0) position = 0                                        ;
  ///////////////////////////////////////////////////////////////////////
  _cursorPosition =  position                                           ;
  _cursorY        = (position / (2 * BYTES_PER_LINE)) * _charHeight + 4 ;
  int           x = (position % (2 * BYTES_PER_LINE))                   ;
  _cursorX        = (((x / 2) * 3) + (x % 2)) * _charWidth + _xPosHex   ;
  ///////////////////////////////////////////////////////////////////////
  _blink = true                                                         ;
  update ( )                                                            ;
  emit currentAddressChanged ( _cursorPosition / 2 )                    ;
}

int N::HexWidget::cursorPos(QPoint pos)
{
  int result = -1                                              ;
  if ((pos.x() >= _xPosHex)                                   &&
      (pos.x() < (_xPosHex + HEXCHARS_IN_LINE * _charWidth)))  {
    int x = (pos.x() - _xPosHex) / _charWidth                  ;
    if ((x % 3) == 0) x =  (x / 3) * 2                         ;
                 else x = ((x / 3) * 2) + 1                    ;
    int y = ((pos.y() - 3) / _charHeight) * 2 * BYTES_PER_LINE ;
    result = x + y                                             ;
  }                                                            ;
  return result                                                ;
}

int N::HexWidget::cursorPos(void)
{
  return _cursorPosition ;
}

void N::HexWidget::resetSelection(void)
{
  _selectionBegin = _selectionInit ;
  _selectionEnd   = _selectionInit ;
}

void N::HexWidget::resetSelection(int pos)
{
  if (pos < 0) pos = 0  ;
  pos /=  2             ;
  _selectionInit  = pos ;
  _selectionBegin = pos ;
  _selectionEnd   = pos ;
}

void N::HexWidget::setSelection(int pos)
{
  if (pos < 0) pos = 0               ;
  pos /= 2                           ;
  if (pos >= _selectionInit)         {
    _selectionEnd   = pos            ;
    _selectionBegin = _selectionInit ;
  } else                             {
    _selectionBegin = pos            ;
    _selectionEnd   = _selectionInit ;
  }                                  ;
}

int N::HexWidget::getSelectionBegin(void)
{
  return _selectionBegin ;
}

int N::HexWidget::getSelectionEnd(void)
{
  return _selectionEnd ;
}

void N::HexWidget::updateCursor(void)
{
  if (_blink) _blink = false; else _blink = true   ;
  update(_cursorX,_cursorY,_charWidth,_charHeight) ;
}

void N::HexWidget::adjust(void)
{
  _charWidth  = fontMetrics() . width  ( QLatin1Char('9') )                 ;
  _charHeight = fontMetrics() . height (                  )                 ;
  ///////////////////////////////////////////////////////////////////////////
  _xPosAdr = 0                                                              ;
  if (_addressArea)                                                         {
    _xPosHex = ( _xData.realAddressNumbers() * _charWidth ) + GAP_ADR_HEX   ;
  } else _xPosHex = 0                                                       ;
  _xPosAscii = ( _xPosHex + HEXCHARS_IN_LINE * _charWidth ) + GAP_HEX_ASCII ;
  ///////////////////////////////////////////////////////////////////////////
  setMinimumHeight ( ((_xData.size()/16 + 1) * _charHeight) + 5 )           ;
  if (_asciiArea)                                                           {
    setMinimumWidth ( _xPosAscii + (BYTES_PER_LINE   * _charWidth) )        ;
  } else                                                                    {
    setMinimumWidth ( _xPosHex   + (HEXCHARS_IN_LINE * _charWidth) )        ;
  }                                                                         ;
  ///////////////////////////////////////////////////////////////////////////
  update ( )                                                                ;
}

void N::HexWidget::ensureVisible(void)
{
  _scrollArea->ensureVisible              (
                 _cursorX                 ,
                 _cursorY + _charHeight/2 ,
                 3                        ,
                 _charHeight/2 + 2      ) ;
}

bool N::HexWidget::Menu(QPoint pos)
{
  MenuManager mm(this)                                    ;
  QAction   * aa                                          ;
  mm.add(101,tr("Save"  ))                                ;
  mm.add(102,tr("Export"))                                ;
  mm.add(103,tr("Resize"))                                ;
  mm.addSeparator()                                       ;
  aa = mm.add(901,tr("Capital letters"))                  ;
  aa-> setCheckable ( true     )                          ;
  aa-> setChecked   ( _capital )                          ;
  mm.setFont(plan)                                        ;
  aa = mm.exec()                                          ;
  if (IsNull(aa)) return false                            ;
  bool ok     = false                                     ;
  int  result = 0                                         ;
  int  maxv   = 512                                       ;
  maxv *= 1024 ; maxv *= 1024                             ;
  switch (mm[aa])                                         {
    case 101                                              :
      Save ( )                                            ;
    break                                                 ;
    case 102                                              :
    {                                                     ;
      QString filename = QFileDialog::getSaveFileName     (
                this                                      ,
                tr("Export binary to file")               ,
                plan->Temporary("")                       ,
                tr("*.*")                               ) ;
      nKickOut ( filename.length()<=0 , true )            ;
      File::toFile(filename,_xData.data())      ;
    }                                                     ;
    break                                                 ;
    case 103                                              :
      result = QInputDialog::getInt                       (
                 this                                     ,
                 tr("Blob size" )                         ,
                 tr("Data size:")                         ,
                 _xData.size()                            ,
                 0                                        ,
                 maxv                                     ,
                 1                                        ,
                 &ok                 )                    ;
      if (ok)                                             {
        int s = _xData.size()                             ;
        if (s>result)                                     {
          s -= result                                     ;
          _xData.remove(result,s)                         ;
        } else
        if (s<result)                                     {
          int d = result - s                              ;
          QByteArray da(d,0)                              ;
          _xData.insert(s,da)                             ;
        }                                                 ;
        _undoStack -> clear   (      )                    ;
        adjust                (      )                    ;
        setCursorPos          ( 0    )                    ;
      }                                                   ;
    break                                                 ;
    case 901                                              :
      _capital = aa->isChecked()                          ;
      repaint()                                           ;
    break                                                 ;
  }                                                       ;
  return true                                             ;
}

void N::HexWidget::Save(void)
{
  if (uuid>0)                                           {
    int size     = _xData.size()                        ;
    int checksum = qChecksum(_xData.data().data(),size) ;
    EnterSQL       ( SC , plan->sql )                   ;
      QString Q                                         ;
      Q = SC.sql.Update                                 (
            PlanTable(Arrays) , "where uuid = :UUID"    ,
            3,"size","checksum","value"               ) ;
      SC . Prepare ( Q                                ) ;
      SC . Bind    ( "uuid"     , uuid                ) ;
      SC . Bind    ( "size"     , size                ) ;
      SC . Bind    ( "checksum" , checksum            ) ;
      SC . Bind    ( "value"    , _xData.data()       ) ;
      SC . Exec    (                                  ) ;
    LeaveSQL       ( SC , plan->sql                   ) ;
  } else                                                {
    QString filename = QFileDialog::getSaveFileName     (
              this                                      ,
              tr("Export binary to file")               ,
              plan->Temporary("")                       ,
              tr("*.*")                               ) ;
    nDropOut ( filename.length()<=0 )                   ;
    File::toFile(filename,_xData.data())      ;
  }                                                     ;
}
