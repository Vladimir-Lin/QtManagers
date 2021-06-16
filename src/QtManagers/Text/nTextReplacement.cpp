#include <qtmanagers.h>
#include "ui_nTextReplacement.h"

N::TextReplacement:: TextReplacement ( QWidget * widget , Plan * p )
                   : Dialog          (           widget ,        p )
                   , ui              ( new Ui::nTextReplacement    )
{
  ui     -> setupUi ( this ) ;
  if (NotNull(plan))         {
    plan -> setFont ( this ) ;
  }                          ;
}

N::TextReplacement::~TextReplacement (void)
{
  delete ui;
}

void N::TextReplacement::setSource(QString source)
{
  ui->Source->setPlainText(source) ;
}

QString N::TextReplacement::sourceString(void)
{
  return ui->Source->toPlainText() ;
}

QString N::TextReplacement::replaceString(void)
{
  return ui->Replace->toPlainText() ;
}
