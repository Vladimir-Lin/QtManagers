#include <qtmanagers.h>

N::DataEntries:: DataEntries ( QWidget * parent , Plan * p )
               : ItemEditor  (           parent ,        p )
{
  WidgetClass   ;
  Configure ( ) ;
}

N::DataEntries::~DataEntries(void)
{
}

void N::DataEntries::Configure(void)
{
  ItemTable = PlanTable(Entries)           ;
  ItemType  = Types::Entry                 ;
  NewTreeWidgetItem ( head               ) ;
  head->setText     ( 0 , tr("Entry")    ) ;
  setWindowTitle    ( tr("Data entries") ) ;
  assignHeaderItems ( head               ) ;
  plan -> setFont   ( this               ) ;
}

bool N::DataEntries::Menu(QPoint pos)
{
  QTreeWidgetItem * it = itemAt(pos)                      ;
  if (IsNull(it)) return false                            ;
  nScopedMenu ( mm , this )                               ;
  QAction * aa                                            ;
  mm . add     ( 901 , tr("Entries"    ) )                ;
  mm . add     ( 101 , tr("Refresh"    ) )                ;
  mm . addSeparator ( )                                   ;
  mm . add     ( 102 , tr("New"        ) )                ;
  mm . add     ( 103 , tr("Rename"     ) )                ;
  mm . addSeparator ( )                                   ;
  mm . add     ( 201 , tr("Export"     ) )                ;
  mm . add     ( 202 , tr("Copy"       ) )                ;
  mm . addSeparator ( )                                   ;
  mm . add     ( 301 , tr("Select none") )                ;
  mm . add     ( 302 , tr("Select all" ) )                ;
  mm . addSeparator ( )                                   ;
  mm . add     ( 401 , tr("Assign font") )                ;
  mm . setFont ( plan                    )                ;
  aa = mm.exec (                         )                ;
  if (IsNull(aa)) return true                             ;
  switch ( mm[aa] )                                       {
    case 101                                              :
      startup     ( )                                     ;
    break                                                 ;
    case 102                                              :
      New         ( )                                     ;
    break                                                 ;
    case 103                                              :
      Rename      ( )                                     ;
    break                                                 ;
    case 201                                              :
      Export      ( )                                     ;
    break                                                 ;
    case 202                                              :
      Copy        ( )                                     ;
    break                                                 ;
    case 301                                              :
      SelectNone  ( )                                     ;
    break                                                 ;
    case 302                                              :
      SelectAll   ( )                                     ;
    break                                                 ;
    case 401                                              :
      setFont     ( )                                     ;
    break                                                 ;
    case 901                                              :
      emit Entries ( it->text(0) , nTreeUuid ( it , 0 ) ) ;
    break                                                 ;
  }                                                       ;
  return true                                             ;
}
