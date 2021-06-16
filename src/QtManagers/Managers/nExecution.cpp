#include <qtmanagers.h>

N::Execution:: Execution (void)
{
  clear ( ) ;
}

N::Execution::~Execution (void)
{
}

void N::Execution::clear(void)
{
  Name       = ""   ;
  Register   = ""   ;
  uuid       = 0    ;
  IconUuid   = 0    ;
  Identifier = 0    ;
  Platform   = 0    ;
  Parallel   = true ;
  Executable = ""   ;
  Environ    = ""   ;
  Paraments  = ""   ;
}

bool N::Execution::Run(void)
{
  QStringList arguments = Paraments.split(' ');
  if (Parallel) {
    if (Paraments.length()>0) {
      if (Environ.length()<=0)
        QProcess::startDetached(Executable,arguments);
      else
        QProcess::startDetached(Executable,arguments,Environ);
      return true ;
    } else {
      if (Environ.length()>0) {
        arguments.clear();
        QProcess::startDetached(Executable,arguments,Environ);
      } else {
        QProcess::startDetached(Executable);
      };
      return true ;
    };
  } else {
    if (Environ.length()<=0 && Paraments.length()<=0)
      QProcess::execute(Executable);
    else
    if (Environ.length()<=0)
      QProcess::execute(Executable,arguments);
    else {
      QProcess P;
      P.setWorkingDirectory(Environ);
      P.start(Executable,arguments);
    };
    return true ;
  };
  return false;
}

UUIDs N::Execution::All(Plan * plan,SqlConnection & Connection)
{
  return Connection.Uuids(plan->Tables[Tables::Executions],"uuid"," order by id asc") ;
}

UUIDs N::Execution::All(Plan * plan)
{
  UUIDs all                                ;
  SqlConnection Connection(plan->sql)      ;
  if (Connection.open("nExecution","All")) {
    all = All(plan,Connection)             ;
    Connection.close()                     ;
  } else                                   {
  }                                        ;
  Connection.remove()                      ;
  return all                               ;
}

bool N::Execution::Save(Plan * plan,SqlConnection & Connection)
{
  QString Q                                                             ;
  SUID    u = 0                                                         ;
  Q = plan->sql.SelectFrom                                              (
        "uuid"                                                          ,
        plan->Tables[Tables::Executions]                                ,
        "where register = :REGISTER;"                                 ) ;
  Connection.Prepare(Q)                                                 ;
  Connection.Bind("register",Register.toUtf8())                         ;
  if (Connection.Exec() && Connection.Next()) uuid = Connection.Uuid(0) ;
  if (u<=0)                                                             {
    u = Connection.Unique(plan->Tables[Tables::MajorUuid  ],"uuid");
    Connection.insertUuid(plan->Tables[Tables::MajorUuid  ],u,Types::Execution);
    Connection.insertUuid(plan->Tables[Tables::Automations],u,"uuid");
    Q = plan->sql.InsertInto(plan->Tables[Tables::Names],3,"uuid","language","name") ;
    Connection.insertName(Q,u,plan->LanguageId,Name);
    Q = plan->sql.InsertInto(
          plan->Tables[Tables::Executions],
          9,
          "uuid",
          "icon",
          "identifier",
          "platform",
          "parallel",
          "register",
          "executable",
          "environment",
          "paraments"
        ) ;
    Connection.Prepare(Q);
    Connection.Bind("uuid",u);
    Connection.Bind("icon",IconUuid);
    Connection.Bind("identifier",Identifier);
    Connection.Bind("platform",Platform);
    Connection.Bind("parallel",Parallel ? 1 : 0);
    Connection.Bind("register",Register.toUtf8());
    Connection.Bind("executable",Executable.toUtf8());
    Connection.Bind("environment",Environ.toUtf8());
    Connection.Bind("paraments",Paraments.toUtf8());
    Connection.Exec();
    uuid = u ;
  } else {
    Q = plan->sql.Update(
          plan->Tables[Tables::Names],
          "where language = :LANGUAGE and uuid = :UUID;",
          1,
          "name"
        ) ;
    Connection.insertName(Q,uuid,plan->LanguageId,Name);
    Q = plan->sql.Update(
          plan->Tables[Tables::Executions],
          " where uuid = :UUID;",
          8,
          "icon",
          "identifier",
          "platform",
          "parallel",
          "register",
          "executable",
          "environment",
          "paraments"
        ) ;
    Connection.Prepare(Q);
    Connection.Bind("uuid",uuid);
    Connection.Bind("icon",IconUuid);
    Connection.Bind("identifier",Identifier);
    Connection.Bind("platform",Platform);
    Connection.Bind("parallel",Parallel ? 1 : 0);
    Connection.Bind("register",Register.toUtf8());
    Connection.Bind("executable",Executable.toUtf8());
    Connection.Bind("environment",Environ.toUtf8());
    Connection.Bind("paraments",Paraments.toUtf8());
    Connection.Exec();
  };
  return true;
}

bool N::Execution::Save(Plan * plan)
{
  bool success = false                      ;
  SqlConnection Connection(plan->sql)       ;
  if (Connection.open("nExecution","Save")) {
    success = Save(plan,Connection)         ;
    Connection.close()                      ;
  }                                         ;
  Connection.remove()                       ;
  return success                            ;
}

bool N::Execution::Load(Plan * plan,SqlConnection & Connection,int Identifier)
{
  SUID u = 0                                                        ;
  QString Q                                                         ;
  Q = plan->sql.SelectFrom                                          (
        "uuid"                                                      ,
        plan->Tables[Tables::Executions]                            ,
        "where identifier = " + QString::number(Identifier) + ";" ) ;
  if (Connection.Query(Q) && Connection.Next())                     {
    u = Connection.Value(0).toInt()                                 ;
  }                                                                 ;
  if (u<=0) return false                                            ;
  return Load(plan,Connection,u)                                    ;
}

bool N::Execution::Load(Plan * plan,SqlConnection & Connection,SUID u)
{
  QString Q;
  PictureManager PM(plan);
  Q = plan->sql.SelectFrom(
        "icon,identifier,platform,parallel,register,executable,environment,paraments",
        plan->Tables[Tables::Executions]              ,
        " where uuid = " + QString::number(u) + ";" ) ;
  if (Connection.Fetch(Q))                            {
    uuid       = u                                    ;
    IconUuid   = Connection.Uuid  (0)                 ;
    Identifier = Connection.Value (1).toInt()         ;
    Platform   = Connection.Uuid  (2)                 ;
    Parallel   = (Connection.Value(3).toInt()==1)     ;
    Register   = Connection.String(4)                 ;
    Executable = Connection.String(5)                 ;
    Environ    = Connection.String(6)                 ;
    Paraments  = Connection.String(7)                 ;
    if (IconUuid>0)                                   {
      QImage * image = NULL                           ;
      image = PM.Thumb(Connection,IconUuid)           ;
      Icon  = QIcon(QPixmap::fromImage(*image))       ;
    }                                                 ;
  }                                                   ;
  if (uuid>0)
    Name = Connection.getName(plan->Tables[Tables::Names],"uuid",plan->LanguageId,uuid);
  return (uuid>0);
}

bool N::Execution::Load(Plan * plan,SqlConnection & Connection,QString name)
{
  SUID u = 0                                                         ;
  QString Q                                                          ;
  Q = plan->sql.SelectFrom                                           (
        "uuid"                                                       ,
        plan->Tables[Tables::Executions]                             ,
        "where register = :REGISTER ;"     )                         ;
  Connection.Prepare(Q)                                              ;
  Connection.Bind("register",name.toUtf8() )                         ;
  if (Connection.Exec() && Connection.Next()) u = Connection.Uuid(0) ;
  if (u<=0) return false                                             ;
  return Load(plan,Connection,u)                                     ;
}

bool N::Execution::Load(Plan * plan,int Identifier)
{
  bool success = false                         ;
  SqlConnection Connection(plan->sql)          ;
  if (Connection.open("nExecution","Load"))    {
    success = Load(plan,Connection,Identifier) ;
    Connection.close()                         ;
  }                                            ;
  Connection.remove()                          ;
  return success                               ;
}

bool N::Execution::Load(Plan * plan,SUID u)
{
  bool success = false                      ;
  SqlConnection Connection(plan->sql)       ;
  if (Connection.open("nExecution","Load")) {
    success = Load(plan,Connection,u)       ;
    Connection.close()                      ;
  }                                         ;
  Connection.remove()                       ;
  return success                            ;
}

bool N::Execution::Load(Plan * plan,QString name)
{
  bool success = false                      ;
  SqlConnection Connection(plan->sql)       ;
  if (Connection.open("nExecution","Load")) {
    success = Load(plan,Connection,name)    ;
    Connection.close()                      ;
  }                                         ;
  Connection.remove()                       ;
  return success                            ;
}
