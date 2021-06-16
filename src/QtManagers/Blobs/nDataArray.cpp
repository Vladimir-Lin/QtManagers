#include <qtmanagers.h>

N::DataArray:: DataArray(void)
{
  OldSize        = -99 ;
  AddressNumbers =   4 ;
  AddressOffset  =   0 ;
}

N::DataArray::~DataArray(void)
{
}

int N::DataArray::addressOffset(void)
{
  return AddressOffset ;
}

void N::DataArray::setAddressOffset(int offset)
{
  AddressOffset = offset;
}

int N::DataArray::addressWidth(void)
{
  return AddressNumbers ;
}

void N::DataArray::setAddressWidth(int width)
{
  if ((width >= 0) && (width<=6)) {
    AddressNumbers = width        ;
  }                               ;
}

QByteArray & N::DataArray::data(void)
{
  return Data;
}

void N::DataArray::setData(QByteArray data)
{
  Data        = data                              ;
  ChangedData = QByteArray(data.length(),char(0)) ;
}

void N::DataArray::setData(File & File)
{
  File >> Data                                      ;
  ChangedData . clear ( )                           ;
  if (Data.size()>0)                                {
    ChangedData = QByteArray(Data.length(),char(0)) ;
  }                                                 ;
}

bool N::DataArray::dataChanged(int i)
{
  return bool(ChangedData[i]);
}

QByteArray N::DataArray::dataChanged(int i,int len)
{
  return ChangedData.mid(i,len);
}

void N::DataArray::setDataChanged(int i,bool state)
{
  ChangedData[i] = char(state);
}

void N::DataArray::setDataChanged(int i,const QByteArray & state)
{
  int len                                  ;
  int length = state.length()              ;
  if ((i + length) > ChangedData.length()) {
    len = ChangedData.length() - i         ;
  } else len = length                      ;
  ChangedData.replace(i,len,state)         ;
}

int N::DataArray::realAddressNumbers()
{
  if (OldSize != Data.size())                       {
    QString test = QString("%1"                     )
                   .arg(Data.size() + AddressOffset ,
                        AddressNumbers              ,
                        16                          ,
                        QChar('0')                ) ;
    RealAddressNumbers = test.size()                ;
  }                                                 ;
  return RealAddressNumbers                         ;
}

int N::DataArray::size(void)
{
  return Data.size() ;
}

QByteArray & N::DataArray::insert(int i,char ch)
{
  Data        . insert( i , ch      ) ;
  ChangedData . insert( i , char(1) ) ;
  return Data                         ;
}

QByteArray & N::DataArray::insert(int i,const QByteArray & ba)
{
  Data        . insert ( i , ba                              ) ;
  ChangedData . insert ( i , QByteArray(ba.length(),char(1)) ) ;
  return Data                                                  ;
}

QByteArray & N::DataArray::remove(int i,int len)
{
  Data        . remove ( i , len ) ;
  ChangedData . remove ( i , len ) ;
  return Data                      ;
}

QByteArray & N::DataArray::replace(int index, char ch)
{
  Data        [ index ] = ch      ;
  ChangedData [ index ] = char(1) ;
  return Data                     ;
}

QByteArray & N::DataArray::replace(int index,const QByteArray & ba)
{
  int    len = ba . length ( )        ;
  return replace ( index , len , ba ) ;
}

QByteArray & N::DataArray::replace(int index,int length,const QByteArray & ba)
{
  int len                                                     ;
  if ((index + length) > Data.length())                       {
    len = Data.length() - index                               ;
  } else len = length                                         ;
  Data        . replace ( index,len,ba.mid(0, len)          ) ;
  ChangedData . replace ( index,len,QByteArray(len,char(1)) ) ;
  return Data                                                 ;
}

QChar N::DataArray::asciiChar(int index)
{
  char ch = Data[index]                    ;
  if ((ch < 0x20) || (ch > 0x7e)) ch = '.' ;
  return QChar(ch)                         ;
}

QString N::DataArray::toReadableString(int start,int end)
{
  int adrWidth = realAddressNumbers()                      ;
  if (AddressNumbers > adrWidth) adrWidth = AddressNumbers ;
  if (end            < 0       ) end      = Data.size()    ;
  //////////////////////////////////////////////////////////
  QString result                                           ;
  for (int i=start; i < end; i += 16)                      {
    QString hexStr                                         ;
    QString ascStr                                         ;
    QString adrStr = QString("%1"                          )
                     .arg                                  (
                      AddressOffset + i                    ,
                      adrWidth                             ,
                      16                                   ,
                      QChar('0')                         ) ;
    for (int j=0;j<16;j++)                                 {
      if ((i + j) < Data.size())                           {
        hexStr.append(" "                                  )
              .append(Data.mid (i+j,1).toHex()           ) ;
        ascStr.append(asciiChar(i+j)                     ) ;
      }                                                    ;
    }                                                      ;
    result += adrStr + " "                                 +
              QString("%1").arg(hexStr, -48) + "  "        +
              QString("%1").arg(ascStr, -17) + "\n"        ;
  }                                                        ;
  //////////////////////////////////////////////////////////
  return result                                            ;
}
