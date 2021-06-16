#include <qtmanagers.h>

QList<N::SqlPlugin *> N::Plugins::LoadSqlPlugins(QDir & dir)
{
  QStringList        DLLs                                  ;
  QFileInfoList      fil                                   ;
  QList<SqlPlugin *> plugins                               ;
  #ifdef Q_OS_WIN
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
      SqlPlugin * rp = Casting(SqlPlugin,plugin)           ;
      nSafeExec(rp) plugins << rp                          ;
    }                                                      ;
  }                                                        ;
  return plugins                                           ;
}

bool N::Plugins::LoadSqls                           (
       QDir                               & dir     ,
       QList<SqlPlugin                 *> & SQLs    ,
       QMap <SqlPlugin *,QPluginLoader *> & Loaders )
{
  QStringList   DLLs                                                       ;
  QFileInfoList fil                                                        ;
  #ifdef Q_OS_WIN
  DLLs << "*.dll"                                                          ;
  #else
  DLLs << "*.so"                                                           ;
  #endif
  fil = dir.entryInfoList(DLLs)                                            ;
  if (fil.count()<=0) return false                                         ;
  //////////////////////////////////////////////////////////////////////////
  for (int ii=0;ii<fil.count();ii++)                                       {
    QPluginLoader * loader = new QPluginLoader(fil[ii].absoluteFilePath()) ;
    nSafeExec(loader)                                                      {
      QObject * plugin = loader->instance()                                ;
      nSafeExec(plugin)                                                    {
        SqlPlugin * rp = Casting(SqlPlugin,plugin)                         ;
        nSafeExec(rp)                                                      {
          SQLs  << rp                                                      ;
          Loaders[rp] = loader                                             ;
        } else                                                             {
          loader->unload()                                                 ;
          delete loader                                                    ;
        }                                                                  ;
      } else                                                               {
        delete loader                                                      ;
      }                                                                    ;
    }                                                                      ;
  }                                                                        ;
  return ( SQLs.count() > 0 )                                              ;
}

#ifdef XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

bool N::Plugins::RunSqlPlugins(Plan * plan,QDir & dir)
{
  bool        success = true;
  QStringList DLLs;
  QFileInfoList fil;
  DLLs << "*.dll";
  fil = dir.entryInfoList(DLLs);
  if (fil.count()<=0) return false;
  for (int ii=0;ii<fil.count();ii++) {
    QPluginLoader pluginLoader(fil[ii].absoluteFilePath());
    QObject * plugin = pluginLoader.instance();
    if (NULL!=plugin) {
      nSqlPlugin * rp = qobject_cast<nSqlPlugin *>(plugin);
      if (rp!=NULL) {
        if (rp->initialize(plan)) {
          if (!rp->exists()) {
            if (!rp->create()) success = false;
          };
        } else success = false;
      };
      pluginLoader.unload();
    };
  };
  return success;
}

#endif
