#include <qtmanagers.h>

N::DataArrayCommand:: DataArrayCommand      (
                      DataArray    * XDATA  ,
                      Cmd            CMD    ,
                      int            BAPOS  ,
                      QByteArray     NEWBA  ,
                      int            LEN    ,
                      QUndoCommand * parent )
                    : QUndoCommand ( parent )
{
  cmd   = CMD   ;
  xData = XDATA ;
  baPos = BAPOS ;
  newBa = NEWBA ;
  len   = LEN   ;
}

N::DataArrayCommand::~DataArrayCommand(void)
{
}

void N::DataArrayCommand::undo(void)
{
  switch (cmd)                                {
    case insert                               :
      xData->remove(baPos,newBa.length())     ;
    break                                     ;
    case replace                              :
      xData->replace       (baPos,oldBa     ) ;
      xData->setDataChanged(baPos,wasChanged) ;
    break                                     ;
    case remove                               :
      xData->insert        (baPos,oldBa     ) ;
      xData->setDataChanged(baPos,wasChanged) ;
    break                                     ;
  }                                           ;
}

void N::DataArrayCommand::redo(void)
{
  switch (cmd)                                     {
    case insert                                    :
      xData->insert(baPos,newBa)                   ;
    break                                          ;
    case replace                                   :
      oldBa      = xData->data().mid (baPos,len  ) ;
      wasChanged = xData->dataChanged(baPos,len  ) ;
      xData     -> replace           (baPos,newBa) ;
    break                                          ;
    case remove                                    :
      oldBa      = xData->data().mid (baPos,len  ) ;
      wasChanged = xData->dataChanged(baPos,len  ) ;
      xData     -> remove            (baPos,len  ) ;
    break                                          ;
  }                                                ;
}
