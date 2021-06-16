#include <qtmanagers.h>
#include "ui_nDataToCpp.h"

N::DataToCpp:: DataToCpp ( QWidget * parent , Plan * p )
             : Dialog    (           parent ,        p )
             , ui        ( new Ui::nDataToCpp          )
{
  WidgetClass   ;
  Configure ( ) ;
}

N::DataToCpp::~DataToCpp(void)
{
  delete ui ;
}

void N::DataToCpp::Configure(void)
{
  ui -> setupUi ( this )                                         ;
  QPushButton * ok = ui->buttonBox->button(QDialogButtonBox::Ok) ;
  if (NotNull(ok)) ok -> setEnabled ( false )                    ;
}

void N::DataToCpp::DataSize(int)
{
  EditorChanged("") ;
}

void N::DataToCpp::DataSource(void)
{
  QString filename = QFileDialog::getOpenFileName (
                       this                       ,
                       tr("Data source")          ,
                       plan->Temporary("")        ,
                       tr("*.*")                ) ;
  nDropOut     ( filename.length()<=0 )           ;
  QFileInfo FI ( filename             )           ;
  if (!FI.exists())                               {
    Alert (Error)                                 ;
    return                                        ;
  }                                               ;
  ui -> Source -> setText ( filename  )           ;
}

void N::DataToCpp::ExportCpp(void)
{
  QString filename = QFileDialog::getSaveFileName (
                       this                       ,
                       tr("Export to C++")        ,
                       plan->Temporary("")        ,
                       tr("*.cpp")              ) ;
  nDropOut ( filename.length()<=0 )               ;
  ui -> Cpp -> setText ( filename )               ;
}

void N::DataToCpp::EditorChanged(QString)
{
  bool          eb = true                          ;
  bool          ds = ui->DataSize ->isChecked ( )  ;
  QString       dn = ui->DataName ->text      ( )  ;
  QString       sn = ui->SizeName ->text      ( )  ;
  QString       sf = ui->Source   ->text      ( )  ;
  QString       ec = ui->Cpp      ->text      ( )  ;
  QPushButton * ok                                 ;
  ok = ui->buttonBox->button(QDialogButtonBox::Ok) ;
  if   (dn.length()<=0) eb = false                 ;
  if   (sf.length()<=0) eb = false                 ;
  if   (ec.length()<=0) eb = false                 ;
  if (ds)                                          {
    if (sn.length()<=0) eb = false                 ;
  }                                                ;
  ok -> setEnabled (eb)                            ;
}

bool N::DataToCpp::Export(void)
{
  bool    ds = ui -> DataSize -> isChecked ( )  ;
  QString dn = ui -> DataName -> text      ( )  ;
  QString sn = ui -> SizeName -> text      ( )  ;
  QString sf = ui -> Source   -> text      ( )  ;
  QString ec = ui -> Cpp      -> text      ( )  ;
  ds         = Cpp::ExportToCpp(sf,ec,dn,ds,sn) ;
  if (ds)                                       {
    Alert ( Done  )                             ;
  } else                                        {
    Alert ( Error )                             ;
  }                                             ;
  return  ds                                    ;
}
