#include <qtmanagers.h>
#include "ui_nBlobToCpp.h"

N::BlobToCpp:: BlobToCpp  ( QWidget * parent , Plan * p )
             : Dialog     (           parent ,        p )
             , ui         ( new Ui::nBlobToCpp          )
             , Data       ( NULL                        )
             , Type       ( 0                           )
{
  WidgetClass   ;
  Configure ( ) ;
}

N::BlobToCpp::~BlobToCpp(void)
{
  delete ui ;
}

void N::BlobToCpp::Configure(void)
{
  ui -> setupUi ( this ) ;
}

bool N::BlobToCpp::setData(QByteArray * data,int type)
{
  Data = data                                     ;
  Type = type                                     ;
  switch (Type)                                   {
    case Cpp::Undecided                           :
    case Cpp::Char                                :
    case Cpp::Byte                                :
    case Cpp::Blob                                :
    case Cpp::Ann                                 :
      ui->Stacked->setCurrentWidget(ui->Blob    ) ;
    break                                         ;
    case Cpp::Short                               :
    case Cpp::UShort                              :
    case Cpp::Integer                             :
    case Cpp::UInt                                :
    case Cpp::LongLong                            :
    case Cpp::ULongLong                           :
      ui->Stacked->setCurrentWidget(ui->Fixed   ) ;
    break                                         ;
    case Cpp::Float                               :
    case Cpp::Double                              :
    case Cpp::LDouble                             :
      ui->Stacked->setCurrentWidget(ui->Floating) ;
    break                                         ;
    case Cpp::StringBlob                          :
      ui->Stacked->setCurrentWidget(ui->String  ) ;
    break                                         ;
  }                                               ;
  return true                                     ;
}

void N::BlobToCpp::ExportCpp(void)
{
  QString filename = QFileDialog::getSaveFileName (
                       this                       ,
                       tr("Export to C++")        ,
                       plan->Temporary("")        ,
                       tr("*.cpp")              ) ;
  nDropOut ( filename.length()<=0 )               ;
  ui -> CppFile -> setText ( filename )           ;
}

bool N::BlobToCpp::Export(void)
{
  bool       ds = ui -> DataSize    -> isChecked ( )                     ;
  bool       pc = ui -> isPrecision -> isChecked ( )                     ;
  QString    dn = ui -> DataName    -> text      ( )                     ;
  QString    sn = ui -> SizeName    -> text      ( )                     ;
  QString    ec = ui -> CppFile     -> text      ( )                     ;
  QString    qs = ui -> StrName     -> text      ( )                     ;
  int        dg = ui -> Precision   -> value     ( )                     ;
  int        sx                                                          ;
  QByteArray OC                                                          ;
  QFile      F(ec)                                                       ;
  QString    S                                                           ;
  switch (Type)                                                          {
    case Cpp::Undecided                                                  :
    case Cpp::Char                                                       :
    case Cpp::Byte                                                       :
    case Cpp::Blob                                                       :
    case Cpp::Ann                                                        :
      if (ec.length()<=0) return false                                   ;
      if (dn.length()<=0) return false                                   ;
      if (ds && sn.length()<=0) return false                             ;
      OC = Cpp::ExportToCpp(*Data,dn,ds,sn)                              ;
      if (OC.size()>0)                                                   {
        if (F.open(QIODevice::WriteOnly))                                {
          F.write(OC)                                                    ;
          F.close()                                                      ;
          Alert ( Done  )                                                ;
          return true                                                    ;
        } else                                                           {
          Alert ( Error )                                                ;
        }                                                                ;
      } else                                                             {
        Alert ( Error )                                                  ;
      }                                                                  ;
    return false                                                         ;
    case Cpp::StringBlob                                                 :
      if (ec.length()<=0) return false                                   ;
      if (dn.length()<=0) return false                                   ;
      if (qs.length()<=0) return false                                   ;
      if (ds && sn.length()<=0) return false                             ;
      S = QString("QString %1 = QString::fromUtf8(QByteArray((const char *)%2,%3)) ;\n")
              .arg(qs).arg(dn).arg(Data->size())                         ;
      OC = Cpp::ExportToCpp(*Data,dn,ds,sn)                              ;
      OC . append ("\n")                                                 ;
      OC . append (S.toUtf8())                                           ;
      if (OC.size()>0)                                                   {
        if (F.open(QIODevice::WriteOnly))                                {
          F.write(OC)                                                    ;
          F.close()                                                      ;
          Alert ( Done  )                                                ;
          return true                                                    ;
        } else                                                           {
          Alert ( Error )                                                ;
        }                                                                ;
      } else                                                             {
        Alert ( Error )                                                  ;
      }                                                                  ;
    return false                                                         ;
    case Cpp::Short                                                      :
    case Cpp::UShort                                                     :
    case Cpp::Integer                                                    :
    case Cpp::UInt                                                       :
    case Cpp::LongLong                                                   :
    case Cpp::ULongLong                                                  :
      sx = Cpp::SizeOf((Cpp::ValueTypes)Type)                            ;
      if ((Data->size()%sx)>0) return false                              ;
      if (ec.length()<=0) return false                                   ;
      if (dn.length()<=0) return false                                   ;
      if (ds && sn.length()<=0) return false                             ;
      OC = Cpp::FixedToCpp(*Data,Type,dn,ds,sn)                          ;
      if (OC.size()>0)                                                   {
        if (F.open(QIODevice::WriteOnly))                                {
          F.write(OC)                                                    ;
          F.close()                                                      ;
          Alert ( Done  )                                                ;
          return true                                                    ;
        } else                                                           {
          Alert ( Error )                                                ;
        }                                                                ;
      } else                                                             {
        Alert ( Error )                                                  ;
      }                                                                  ;
    break                                                                ;
    case Cpp::Float                                                      :
    case Cpp::Double                                                     :
    case Cpp::LDouble                                                    :
      sx = Cpp::SizeOf((Cpp::ValueTypes)Type)                            ;
      if ((Data->size()%sx)>0) return false                              ;
      if (ec.length()<=0) return false                                   ;
      if (dn.length()<=0) return false                                   ;
      if (ds && sn.length()<=0) return false                             ;
      OC = Cpp::FloatingToCpp(*Data,Type,dn,pc,dg,ds,sn)                 ;
      if (OC.size()>0)                                                   {
        if (F.open(QIODevice::WriteOnly))                                {
          F.write(OC)                                                    ;
          F.close()                                                      ;
          Alert ( Done  )                                                ;
          return true                                                    ;
        } else                                                           {
          Alert ( Error )                                                ;
        }                                                                ;
      } else                                                             {
        Alert ( Error )                                                  ;
      }                                                                  ;
    break                                                                ;
  }                                                                      ;
  return true                                                            ;
}
