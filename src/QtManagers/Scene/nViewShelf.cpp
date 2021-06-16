#include <qtmanagers.h>

#ifdef QT_OPENGL_LIB

N::ViewShelf:: ViewShelf ( QObject * parent , Plan * p )
             : ViewBox   (           parent ,        p )
{
  WidgetClass   ;
  Configure ( ) ;
}

N::ViewShelf:: ViewShelf ( QObject * parent , Plan * p , SceneDestructor * des )
             : ViewBox   (           parent ,        p ,                   des )
{
  WidgetClass   ;
  Configure ( ) ;
}

N::ViewShelf::~ViewShelf (void)
{
}

void N::ViewShelf::Configure(void)
{
  QObject::setProperty ( "Type" , "ViewShelf" ) ;
  setVariable          ( "Type" , "ViewShelf" ) ;
}

#endif
