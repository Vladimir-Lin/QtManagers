#include <qtmanagers.h>

QList<N::HunterPlugin *> N::Plugins::LoadHunters(QDir & dir)
{
  QStringList           DLLs                               ;
  QFileInfoList         fil                                ;
  QList<HunterPlugin *> plugins                            ;
  #ifdef WIN32
  DLLs << "*.dll"                                          ;
  #else
  DLLs << "*.so"                                           ;
  #endif
  fil = dir.entryInfoList(DLLs)                            ;
  if (fil.count()<=0) return plugins                       ;
  for (int ii=0;ii<fil.count();ii++)                       {
    QPluginLoader pluginLoader(fil[ii].absoluteFilePath()) ;
    QObject * plugin = pluginLoader.instance()             ;
    nSafeExec(plugin)                                      {
      HunterPlugin * rp = Casting(HunterPlugin,plugin)     ;
      nSafeExec(rp) plugins << rp                          ;
    }                                                      ;
  }                                                        ;
  return plugins                                           ;
}

bool N::Plugins::LoadHunters                           (
       QDir & dir                                      ,
       QList<HunterPlugin                 *> & Hunters ,
       QMap <HunterPlugin *,QPluginLoader *> & Loaders )
{
  QStringList   DLLs                                       ;
  QFileInfoList fil                                        ;
  #ifdef WIN32
  DLLs << "*.dll"                                          ;
  #else
  DLLs << "*.so"                                           ;
  #endif
  fil = dir.entryInfoList(DLLs)                            ;
  if (fil.count()<=0) return false                         ;
  //////////////////////////////////////////////////////////
  for (int ii=0;ii<fil.count();ii++)                       {
    QPluginLoader * loader                                 ;
    loader = new QPluginLoader(fil[ii].absoluteFilePath()) ;
    nSafeExec(loader)                                      {
      QObject * plugin = loader->instance()                ;
      nSafeExec(plugin)                                    {
        HunterPlugin * rp = Casting(HunterPlugin,plugin)   ;
        nSafeExec(rp)                                      {
          Hunters << rp                                    ;
          Loaders[rp] = loader                             ;
        } else                                             {
          loader->unload()                                 ;
          delete loader                                    ;
        }                                                  ;
      } else                                               {
        delete loader                                      ;
      }                                                    ;
    }                                                      ;
  }                                                        ;
  return ( Hunters.count() > 0 )                           ;
}
