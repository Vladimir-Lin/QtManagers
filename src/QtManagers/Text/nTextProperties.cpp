#include <qtmanagers.h>
#include "ui_nTextProperties.h"

N::TextProperties:: TextProperties ( QWidget * parent , Plan * p )
                  : Dialog         (           parent ,        p )
                  , ui             ( new Ui::nTextProperties     )
{
  Configure ( ) ;
}

N::TextProperties::~TextProperties(void)
{
  delete ui ;
}

void N::TextProperties::Configure(void)
{
  ui -> setupUi ( this )                                ;
  EnterSQL(SC,plan->sql)                                ;
    QString Q                                           ;
    Q = SC.sql.SelectFrom                               (
          "id,name"                                     ,
          PlanTable(Encodings)                          ,
          "order by id asc"  )                          ;
    SqlLoopNow(SC,Q)                                    ;
      int     l = SC . Int       ( 0     )              ;
      QString n = SC . String    ( 1     )              ;
      ui -> Encodings -> addItem ( n , l )              ;
      Encodings[l] = n                                  ;
      rEncode  [n] = l                                  ;
    SqlLoopErr(SC,Q)                                    ;
    SqlLoopEnd(SC,Q)                                    ;
  LeaveSQL(SC,plan->sql)                                ;
  for (int i=0;i<plan->languages.languages.count();i++) {
    int     l  = plan->languages.languages[i]           ;
    SUID    u  = plan->languages.Uuids    [l]           ;
    QString n  = plan->languages.Names    [u]           ;
    ui->Languages->addItem(n,l)                         ;
  }                                                     ;
  (*(ui->Languages)) <= vLanguageId                     ;
}

int N::TextProperties::Encoding(void)
{
  return (int)N::GetUuid(ui->Encodings) ;
}

int N::TextProperties::Language(void)
{
  return (int)N::GetUuid(ui->Languages) ;
}

void N::TextProperties::setLanguage(int language)
{
  (*(ui->Languages)) <= language ;
}

void N::TextProperties::setEncoding(int encoding)
{
  (*(ui->Encodings)) <= encoding ;
}

void N::TextProperties::setHidden(int item)
{
  switch (item)                        {
    case 0                             :
      ui->Languages->setVisible(false) ;
    break                              ;
    case 1                             :
      ui->Encodings->setVisible(false) ;
    break                              ;
  }                                    ;
}
