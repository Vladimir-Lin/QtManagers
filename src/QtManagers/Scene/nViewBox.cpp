#include <qtmanagers.h>

#ifdef QT_OPENGL_LIB

N::ViewBox:: ViewBox  ( QObject * parent , Plan * p )
           : PickView (           parent ,        p )
{
  WidgetClass   ;
  Configure ( ) ;
}

N::ViewBox:: ViewBox  ( QObject * parent , Plan * p , SceneDestructor * des )
           : PickView (           parent ,        p ,                   des )
{
  WidgetClass   ;
  Configure ( ) ;
}

N::ViewBox::~ViewBox (void)
{
}

void N::ViewBox::Configure(void)
{
  QObject::setProperty ( "Type" , "ViewBox" ) ;
  setVariable          ( "Type" , "ViewBox" ) ;
}

#endif
