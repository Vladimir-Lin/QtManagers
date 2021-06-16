#include <qtmanagers.h>

QList<N::DocumentPlugin *> N::Plugins::LoadDocuments(QDir & dir)
{
  QStringList             DLLs                             ;
  QFileInfoList           fil                              ;
  QList<DocumentPlugin *> plugins                          ;
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
      DocumentPlugin * rp = Casting(DocumentPlugin,plugin) ;
      nSafeExec(rp) plugins << rp                          ;
    }                                                      ;
  }                                                        ;
  return plugins                                           ;
}

bool N::Plugins::LoadDocuments                             (
       QDir                                    & dir       ,
       QList<DocumentPlugin                 *> & Documents ,
       QMap <DocumentPlugin *,QPluginLoader *> & Loaders   )
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
        DocumentPlugin * rp = Casting(DocumentPlugin,plugin)               ;
        nSafeExec(rp)                                                      {
          Documents  << rp                                                 ;
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
  return ( Documents.count() > 0 )                                         ;
}
