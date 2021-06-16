#include <qtmanagers.h>

N::TimeAlignment:: TimeAlignment ( QWidget * parent , Plan * p )
                 : TableWidget   (           parent ,        p )
{
  WidgetClass   ;
  Configure ( ) ;
}

N::TimeAlignment::~TimeAlignment(void)
{
}

void N::TimeAlignment::Configure(void)
{
  StartT = 0                                   ;
  EndT   = 0                                   ;
  dT     = 3600                                ;
  Lists << Calendars::Geogrian                 ;
  //////////////////////////////////////////////
  setWindowTitle  ( tr("Timeline alignment") ) ;
  plan -> setFont ( this                     ) ;
}

bool N::TimeAlignment::Menu(QPoint pos)
{
  MenuManager        mm ( this )              ;
  QAction          * aa                       ;
  QTableWidgetItem * it = itemAt(pos)         ;
  mm . add     ( 101 , tr("Set time range") ) ;
  if (columnCount()>0)                        {
    mm . add   ( 102 , tr("Add timeline"  ) ) ;
  }                                           ;
  if (NotNull(it))                            {
    mm . add   ( 103 , tr("Remove timeline")) ;
  }                                           ;
  mm . setFont ( plan                       ) ;
  aa = mm.exec (                            ) ;
  nKickOut     ( IsNull(aa) , true          ) ;
  TimeScope * ts                              ;
  switch       ( mm[aa]                     ) {
    case 101                                  :
      ts  = new TimeScope ( this , plan )     ;
      ts -> setStart      ( StartT      )     ;
      ts -> setEnd        ( EndT        )     ;
      ts -> setGap        ( dT          )     ;
      if (ts->exec()==QDialog::Accepted)      {
        StartT = ts -> Start ( )              ;
        EndT   = ts -> End   ( )              ;
        dT     = ts -> dT    ( )              ;
        Reflush              ( )              ;
      }                                       ;
      ts -> deleteLater ( )                   ;
    break                                     ;
    case 102                                  :
      addTime ( )                             ;
    break                                     ;
    case 103                                  :
      if (NotNull(it))                        {
        int col = column(it)                  ;
        if (col>=0 && col<Lists.count())      {
          Lists . takeAt ( col )              ;
          Reflush        (     )              ;
        }                                     ;
      }                                       ;
    break                                     ;
  }                                           ;
  return true                                 ;
}

void N::TimeAlignment::addTime(void)
{
  TimeFactory TF(plan)            ;
  TF . load ( )                   ;
  UUIDs       U = TF.Names.keys() ;
  QStringList L                   ;
  SUID        uuid                ;
  foreach (uuid,U)                {
    L << TF.Names[uuid]           ;
  }                               ;
  if (L.count()<=0) return        ;
  QString R                       ;
  bool    okay = false            ;
  R = QInputDialog::getItem       (
        this                      ,
        tr("Add timeline")        ,
        tr("Calendar")            ,
        L                         ,
        0                         ,
        true                      ,
        &okay                   ) ;
  if (!okay) return               ;
  int index = L.indexOf(R)        ;
  if (index<0) return             ;
  Lists << index                  ;
  Reflush ( )                     ;
}

void N::TimeAlignment::Reflush(void)
{
  TimeFactory TF(plan)                           ;
  int         rows = 1 + ( (EndT-StartT) / dT )  ;
  TUID        T    =        StartT               ;
  QStringList L                                  ;
  QTableWidgetItem *  it                         ;
  QList<StarDate   *> SDs                        ;
  TF . load ( )                                  ;
  setColumnCount            ( Lists.count()    ) ;
  setRowCount               ( rows             ) ;
  while ( T <= EndT )                            {
    L <<  QString::number(T)                     ;
    T += dT                                      ;
  }                                              ;
  setVerticalHeaderLabels   ( L                ) ;
  plan -> processEvents     (                  ) ;
  L . clear                 (                  ) ;
  for (int i=0;i<Lists.count();i++)              {
    int        type = Lists[i]                   ;
    StarDate * sd   = TF.create(type)            ;
    SDs << sd                                    ;
    L   << TF.Names[type]                        ;
  }                                              ;
  setHorizontalHeaderLabels ( L                ) ;
  plan -> processEvents     (                  ) ;
  for (int i=0;i<SDs.count();i++)                {
    StarDate * sd = SDs[i]                       ;
    T = StartT                                   ;
    for (int j=0;j<rows;j++,T+=dT)               {
      int         type = sd->type()              ;
      QString     fmt  = TF.Formats[type]        ;
      sd-> assign ( T )                          ;
      sd-> locate (   )                          ;
      it = new QTableWidgetItem()                ;
      it-> setText ( sd->toString(fmt) )         ;
      it-> setTextAlignment ( Qt::AlignRight     |
                              Qt::AlignVCenter ) ;
      setItem      ( j , i , it        )         ;
      plan -> processEvents (                  ) ;
    }                                            ;
  }                                              ;
  for (int i=0;i<SDs.count();i++) delete SDs[i]  ;
  SDs . clear ( )                                ;
  resizeColumnsToContents ( )                    ;
}
