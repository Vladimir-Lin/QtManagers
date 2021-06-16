#include <qtmanagers.h>

QStringList N::EventTypeNames(void)
{
  QStringList s                       ;
  s << QObject::tr( "None"          ) ;
  s << QObject::tr( "Notify"        ) ;
  s << QObject::tr( "Period"        ) ;
  s << QObject::tr( "Record"        ) ;
  s << QObject::tr( "Meeting"       ) ;
  s << QObject::tr( "Automation"    ) ;
  s << QObject::tr( "Operation"     ) ;
  s << QObject::tr( "Sync"          ) ;
  s << QObject::tr( "Download"      ) ;
  s << QObject::tr( "Upload"        ) ;
  s << QObject::tr( "Audio"         ) ;
  s << QObject::tr( "Video"         ) ;
  s << QObject::tr( "TV"            ) ;
  s << QObject::tr( "Programming"   ) ;
  s << QObject::tr( "Backup"        ) ;
  s << QObject::tr( "File Update"   ) ;
  s << QObject::tr( "Cooking"       ) ;
  s << QObject::tr( "Sleep"         ) ;
  s << QObject::tr( "Housework"     ) ;
  s << QObject::tr( "Shopping"      ) ;
  s << QObject::tr( "Communication" ) ;
  s << QObject::tr( "Finish"        ) ;
  s << QObject::tr( "Obsolete"      ) ;
  return s                            ;
}
