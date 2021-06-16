#include <qtmanagers.h>

//////////////////////////////////////////////////////////////////////////////

static N::SceneObject * NodeCreator (void *,QObject *)
{
  return new N::SceneObject ( ) ;
}

static N::SceneObject * ConeCreator (void *,QObject *)
{
  return new N::SceneCone ( ) ;
}

static N::SceneObject * LinearCreator (void *,QObject *)
{
  return new N::SceneLinear ( ) ;
}

//////////////////////////////////////////////////////////////////////////////

static N::Universe * WorldCreator(void *,QObject *)
{
  return new N::Universe ( ) ;
}

static N::Universe * FlatCreator(void *,QObject *)
{
  return new N::FlatUniverse ( ) ;
}

static N::Universe * SpinnerCreator(void *,QObject *)
{
  return new N::SpinnerUniverse ( ) ;
}

static N::Universe * RasterCreator(void *,QObject *)
{
  return new N::RasterUniverse ( ) ;
}

//////////////////////////////////////////////////////////////////////////////

bool N::SceneCreator::initialized = false                     ;
QMap<QString,N::ObjectCreator   *> N::SceneCreator::objects   ;
QMap<QString,N::UniverseCreator *> N::SceneCreator::universes ;

N::SceneCreator:: SceneCreator (void)
{
}

N::SceneCreator::~SceneCreator (void)
{
}

void N::SceneCreator::addDefaultCreator(void)
{
  if ( initialized ) return                 ;
  ///////////////////////////////////////////
  objects   [ "Object"   ] = NodeCreator    ;
  objects   [ "Cone"     ] = ConeCreator    ;
  objects   [ "Linear"   ] = LinearCreator  ;
  ///////////////////////////////////////////
  universes [ "Universe" ] = WorldCreator   ;
  universes [ "Flat"     ] = FlatCreator    ;
  universes [ "Spinner"  ] = SpinnerCreator ;
  universes [ "Raster"   ] = RasterCreator  ;
  ///////////////////////////////////////////
  initialized = true                        ;
}

bool N::SceneCreator::hasObject(QString name)
{
  addDefaultCreator         (      ) ;
  return objects . contains ( name ) ;
}

bool N::SceneCreator::hasUniverse(QString name)
{
  addDefaultCreator           (      ) ;
  return universes . contains ( name ) ;
}

int N::SceneCreator::add(QString name,ObjectCreator * creator)
{
  addDefaultCreator ( )      ;
  objects [ name ] = creator ;
  return objects . count ( ) ;
}

int N::SceneCreator::add(QString name,UniverseCreator * creator)
{
  addDefaultCreator ( )        ;
  universes [ name ] = creator ;
  return universes . count ( ) ;
}

N::SceneObject * N::SceneCreator::createObject (QString name,void * conf,QObject * parent)
{
  if ( ! hasObject ( name ) ) return NULL         ;
  return ( * objects [ name ] ) ( conf , parent ) ;
}

N::SceneObject * N::SceneCreator::createObject (QString name,QByteArray & conf ,QObject * parent)
{
  void * cfg = NULL                                         ;
  if ( conf . size ( ) > 0 ) cfg = (void *) conf . data ( ) ;
  return createObject ( name , cfg , parent )               ;
}

N::Universe * N::SceneCreator::createUniverse (QString name,void * conf,QObject * parent)
{
  if ( ! hasUniverse ( name ) ) return NULL         ;
  return ( * universes [ name ] ) ( conf , parent ) ;
}

N::Universe * N::SceneCreator::createUniverse (QString name,QByteArray & conf ,QObject * parent)
{
  void * cfg = NULL                                         ;
  if ( conf . size ( ) > 0 ) cfg = (void *) conf . data ( ) ;
  return createUniverse ( name , cfg , parent )             ;
}
