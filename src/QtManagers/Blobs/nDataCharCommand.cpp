#include <qtmanagers.h>

N::DataCharCommand:: DataCharCommand        (
                     DataArray    * XDATA   ,
                     Cmd            CMD     ,
                     int            CHARPOS ,
                     char           NEWCHAR ,
                     QUndoCommand * parent  )
                   : QUndoCommand ( parent  )
{
  xData   = XDATA   ;
  charPos = CHARPOS ;
  newChar = NEWCHAR ;
  cmd     = CMD     ;
}

N::DataCharCommand::~DataCharCommand(void)
{
}

bool N::DataCharCommand::mergeWith(const QUndoCommand * command)
{
  const DataCharCommand * nextCommand = static_cast<const DataCharCommand *>(command) ;
  bool  result = false                     ;
  if (cmd != remove)                       {
    if (nextCommand->cmd == replace)       {
      if (nextCommand->charPos == charPos) {
        newChar = nextCommand->newChar     ;
        result = true                      ;
      }                                    ;
    }                                      ;
  }                                        ;
  return result                            ;
}

void N::DataCharCommand::undo(void)
{
  switch (cmd)                                     {
    case insert                                    :
      xData->remove           (charPos,1         ) ;
    break                                          ;
    case replace                                   :
      xData->replace          (charPos,oldChar   ) ;
      xData->setDataChanged   (charPos,wasChanged) ;
    break                                          ;
    case remove                                    :
      xData -> insert         (charPos,oldChar   ) ;
      xData -> setDataChanged (charPos,wasChanged) ;
    break                                          ;
  }                                                ;
}

void N::DataCharCommand::redo(void)
{
  switch (cmd)                                   {
    case insert                                  :
      xData     -> insert  ( charPos , newChar ) ;
    break                                        ;
    case replace                                 :
      oldChar    = xData->data()     [ charPos ] ;
      wasChanged = xData->dataChanged( charPos ) ;
      xData     -> replace     (charPos,newChar) ;
    break                                        ;
    case remove                                  :
      oldChar    = xData->data()     [charPos  ] ;
      wasChanged = xData->dataChanged(charPos  ) ;
      xData     -> remove            (charPos,1) ;
    break                                        ;
  }                                              ;
}
