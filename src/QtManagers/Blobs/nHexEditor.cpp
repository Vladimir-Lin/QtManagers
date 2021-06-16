#include <qtmanagers.h>

N::HexEditor:: HexEditor  ( QWidget * parent , Plan * p )
             : ScrollArea (           parent ,        p )
{
  Configure ( ) ;
}

N::HexEditor::~HexEditor(void)
{
}

void N::HexEditor::Configure(void)
{
  qHexEdit_p = new HexWidget ( this , plan )               ;
  setWidget                  ( qHexEdit_p  )               ;
  setWidgetResizable         ( true        )               ;
  //////////////////////////////////////////////////////////
  connect(qHexEdit_p,SIGNAL(currentAddressChanged(int ))   ,
          this      ,SIGNAL(currentAddressChanged(int )) ) ;
  connect(qHexEdit_p,SIGNAL(currentSizeChanged   (int ))   ,
          this      ,SIGNAL(currentSizeChanged   (int )) ) ;
  connect(qHexEdit_p,SIGNAL(dataChanged          (    ))   ,
          this      ,SIGNAL(dataChanged          (    )) ) ;
  connect(qHexEdit_p,SIGNAL(overwriteModeChanged (bool))   ,
          this      ,SIGNAL(overwriteModeChanged (bool)) ) ;
  //////////////////////////////////////////////////////////
  setFocusPolicy ( Qt::NoFocus )                           ;
}

int N::HexEditor::indexOf(const QByteArray & ba,int from) const
{
  return qHexEdit_p->indexOf(ba,from) ;
}

void N::HexEditor::insert(int i,const QByteArray & ba)
{
  qHexEdit_p->insert(i,ba) ;
}

void N::HexEditor::insert(int i,char ch)
{
  qHexEdit_p->insert(i,ch) ;
}

int N::HexEditor::lastIndexOf(const QByteArray & ba,int from) const
{
  return qHexEdit_p->lastIndexOf(ba,from) ;
}

void N::HexEditor::remove(int pos,int len)
{
  qHexEdit_p->remove(pos,len) ;
}

void N::HexEditor::replace(int pos,int len,const QByteArray & after)
{
  qHexEdit_p->replace(pos,len,after) ;
}

QString N::HexEditor::toReadableString(void)
{
  return qHexEdit_p->toReadableString() ;
}

QString N::HexEditor::selectionToReadableString(void)
{
  return qHexEdit_p->selectionToReadableString() ;
}

void N::HexEditor::setAddressArea(bool addressArea)
{
  qHexEdit_p->setAddressArea(addressArea) ;
}

void N::HexEditor::redo(void)
{
  qHexEdit_p->redo() ;
}

void N::HexEditor::undo(void)
{
  qHexEdit_p->undo();
}

void N::HexEditor::setAddressWidth(int addressWidth)
{
  qHexEdit_p->setAddressWidth(addressWidth) ;
}

void N::HexEditor::setAsciiArea(bool asciiArea)
{
  qHexEdit_p->setAsciiArea(asciiArea) ;
}

void N::HexEditor::setHighlighting(bool mode)
{
  qHexEdit_p->setHighlighting(mode) ;
}

void N::HexEditor::setAddressOffset(int offset)
{
  qHexEdit_p->setAddressOffset(offset) ;
}

int N::HexEditor::addressOffset(void)
{
  return qHexEdit_p->addressOffset() ;
}

void N::HexEditor::setCursorPosition(int cursorPos)
{
  qHexEdit_p->setCursorPos( cursorPos * 2 ) ;
}

int N::HexEditor::cursorPosition(void)
{
  return qHexEdit_p->cursorPos() / 2;
}

void N::HexEditor::setName(QString name)
{
  qHexEdit_p -> Name = name ;
  setWindowTitle ( name )   ;
}

void N::HexEditor::setUuid(SUID uuid,int sourceType)
{
  qHexEdit_p -> uuid = uuid       ;
  qHexEdit_p -> type = sourceType ;
}

void N::HexEditor::setData(const QByteArray & data)
{
  qHexEdit_p->setData(data) ;
}

QByteArray N::HexEditor::data(void)
{
  return qHexEdit_p->data() ;
}

void N::HexEditor::setAddressAreaColor(const QColor & color)
{
  qHexEdit_p->setAddressAreaColor(color) ;
}

QColor N::HexEditor::addressAreaColor(void)
{
  return qHexEdit_p->addressAreaColor();
}

void N::HexEditor::setHighlightingColor(const QColor & color)
{
  qHexEdit_p->setHighlightingColor(color) ;
}

QColor N::HexEditor::highlightingColor(void)
{
  return qHexEdit_p->highlightingColor() ;
}

void N::HexEditor::setSelectionColor(const QColor & color)
{
  qHexEdit_p->setSelectionColor(color) ;
}

QColor N::HexEditor::selectionColor(void)
{
  return qHexEdit_p->selectionColor() ;
}

void N::HexEditor::setOverwriteMode(bool overwriteMode)
{
  qHexEdit_p->setOverwriteMode(overwriteMode) ;
}

bool N::HexEditor::overwriteMode(void)
{
  return qHexEdit_p->overwriteMode() ;
}

void N::HexEditor::setReadOnly(bool readOnly)
{
  qHexEdit_p->setReadOnly(readOnly) ;
}

bool N::HexEditor::isReadOnly(void)
{
  return qHexEdit_p->isReadOnly() ;
}

void N::HexEditor::setFont(const QFont &font)
{
  qHexEdit_p->setFont(font) ;
}

const QFont & N::HexEditor::font(void) const
{
  return qHexEdit_p->font() ;
}

bool N::HexEditor::load(void)
{
  SUID uuid = qHexEdit_p -> uuid  ;
  nKickOut ( uuid <= 0 , false )  ;
  QByteArray Body                 ;
  QString    name                 ;
  EnterSQL ( SC , plan->sql )     ;
    QString Q                     ;
    Q = SC.sql.SelectFrom         (
          "value"                 ,
          PlanTable(Arrays)       ,
          SC.WhereUuid(uuid)    ) ;
    if (SC.Fetch(Q))              {
      Body = SC . ByteArray ( 0 ) ;
    }                             ;
    name = SC.getName             (
      PlanTable(Names)            ,
      "uuid"                      ,
      vLanguageId                 ,
      uuid                      ) ;
  LeaveSQL ( SC , plan->sql )     ;
  if ( Body.size() <= 0 )         {
    Alert ( Error )               ;
    return false                  ;
  }                               ;
  setWindowTitle      ( name )    ;
  qHexEdit_p->Name =    name      ;
  qHexEdit_p->setData ( Body )    ;
  return true                     ;
}
