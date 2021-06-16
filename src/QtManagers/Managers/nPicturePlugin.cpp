#include <qtmanagers.h>

QList<N::PicturePlugin *> N::Plugins::LoadPictures(QDir & dir)
{
  QStringList               DLLs                                       ;
  QFileInfoList             fil                                        ;
  QList<N::PicturePlugin *> plugins                                    ;
  #ifdef Q_OS_WIN
  DLLs << "*.dll"                                                      ;
  #else
  DLLs << "*.so"                                                       ;
  #endif
  fil = dir.entryInfoList(DLLs)                                        ;
  if (fil.count()<=0) return plugins                                   ;
  for (int ii=0;ii<fil.count();ii++)                                   {
    QPluginLoader pluginLoader(fil[ii].absoluteFilePath())             ;
    QObject * plugin = pluginLoader.instance()                         ;
    if (NULL!=plugin)                                                  {
      N::PicturePlugin * rp = qobject_cast<N::PicturePlugin *>(plugin) ;
      if (NULL!=rp) plugins << rp                                      ;
    }                                                                  ;
  }                                                                    ;
  return plugins                                                       ;
}

bool N::Plugins::LoadPictures                               (
       QDir & dir                                           ,
       QList<N::PicturePlugin                 *> & Pictures ,
       QMap <N::PicturePlugin *,QPluginLoader *> & Loaders  )
{
  QStringList DLLs                                               ;
  QFileInfoList fil                                              ;
  #ifdef Q_OS_WIN
  DLLs << "*.dll"                                                ;
  #else
  DLLs << "*.so"                                                 ;
  #endif
  fil = dir.entryInfoList(DLLs)                                  ;
  if (fil.count()<=0) return false                               ;
  ////////////////////////////////////////////////////////////////
  for (int ii=0;ii<fil.count();ii++)                             {
    QPluginLoader * loader = new QPluginLoader                   (
                               fil[ii].absoluteFilePath())       ;
    if (NotNull(loader))                                         {
      QObject * plugin = loader->instance()                      ;
      if (NotNull(plugin))                                       {
        N::PicturePlugin * rp = Casting(N::PicturePlugin,plugin) ;
        if (NotNull(rp))                                         {
          Pictures << rp                                         ;
          Loaders[rp] = loader                                   ;
        } else                                                   {
          loader->unload()                                       ;
          delete loader                                          ;
        }                                                        ;
      } else                                                     {
        delete loader                                            ;
      }                                                          ;
    }                                                            ;
  }                                                              ;
  return ( Pictures.count() > 0 )                                ;
}
