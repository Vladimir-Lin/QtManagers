#include <qtmanagers.h>

N::ThumbBuilder:: ThumbBuilder   ( QObject * parent , Plan * p )
                : QObject        (           parent            )
                , Thread         (                             )
                , PictureManager (                           p )
                , plan           (                           p )
                , ProgressId     ( 0                           )
                , ProgressValue  ( 0                           )
                , ProgressRun    ( true                        )
{
  WidgetClass ;
}

N::ThumbBuilder::~ThumbBuilder(void)
{
}

void N::ThumbBuilder::run(void)
{
  QString ConnectName = QString::number(rand())                                ;
  plan -> setRange  ( ProgressId , 0 , 100                       )             ;
  plan -> Start     ( ProgressId , &ProgressValue , &ProgressRun )             ;
  SqlConnection  SC ( plan->sql                                  )             ;
  if (SC.open("nThumbBuilder",ConnectName))                                    {
    ParamentQuery PT ( Dynamic::Functionality , 1 , "Thumb" )                  ;
    QString Q                                                                  ;
    UUIDs   Uuids                                                              ;
    UUIDs   Xuids                                                              ;
    SUID    uuid                                                               ;
    bool    done   = false                                                     ;
    int     chunk  = 1000                                                      ;
    int     total  = 0                                                         ;
    int     from   = 0                                                         ;
    int     verify = 1                                                         ;
    chunk  = PT.Value(SC,PlanTable(Paraments),"Chunk" )                        ;
    from   = PT.Value(SC,PlanTable(Paraments),"Start" )                        ;
    verify = PT.Value(SC,PlanTable(Paraments),"Verify")                        ;
    Q = SC.sql.SelectFrom                                                      (
          "uuid"                                                               ,
          PlanTable(MajorUuid)                                                 ,
          QString("where type = %1").arg(Types::Picture)                     ) ;
    SqlLoopNow ( SC , Q )                                                      ;
      Xuids << SC . Uuid ( 0 )                                                 ;
    SqlLoopErr ( SC , Q )                                                      ;
    SqlLoopEnd ( SC , Q )                                                      ;
    total = SC.count(PlanTable(PictureUuid))                                   ;
    if (total>0)                                                               {
      int pIndex    = 0                                                        ;
      ProgressValue = from                                                     ;
      plan -> Start ( ProgressId , &ProgressValue , &ProgressRun )             ;
      plan -> Talk  ( tr("Start building picture thumbs"       ) )             ;
      while (!done)                                                            {
        Q = SC.sql.SelectFrom("count(*)",PlanTable(PictureUuid))               ;
        if (SC.Fetch(Q)) total = SC.Int(0)                                     ;
        plan -> setRange ( ProgressId , 0 , total )                            ;
        ProgressValue = from                                                   ;
        Uuids . clear ( )                                                      ;
        Q = SC.sql.SelectFrom                                                  (
              "id,uuid"                                                        ,
              PlanTable(PictureUuid)                                           ,
              QString("where id > %1 %2 %3"                                    )
              .arg(from                                                        )
              .arg(SC.OrderByAsc("id")                                         )
              .arg(SC.sql.Limit(0,chunk)                                       )
            )                                                                  ;
        SqlLoopNow ( SC , Q )                                                  ;
          from = SC . Int  ( 0 )                                               ;
          uuid = SC . Uuid ( 1 )                                               ;
          Uuids << uuid                                                        ;
        SqlLoopErr ( SC , Q )                                                  ;
        SqlLoopEnd ( SC , Q )                                                  ;
        for (int i=0;ProgressRun && i<Uuids.count();i++)                       {
          QImage * I                                                           ;
          QImage * T                                                           ;
          bool     v = false                                                   ;
          ProgressValue++                                                      ;
          pIndex++                                                             ;
          uuid = Uuids[i]                                                      ;
          if (verify==1)                                                       {
            I = Picture(SC,uuid)                                               ;
            if (IsNull(I)) DeleteThumb(SC,uuid) ; else                         {
              T = Thumb(SC,uuid)                                               ;
              if (IsNull(T))                                                   {
                UpdateThumb ( SC , uuid )                                      ;
                T = Thumb(SC,uuid)                                             ;
                if (NotNull(T))                                                {
                  delete T                                                     ;
                  v = true                                                     ;
                }                                                              ;
              } else                                                           {
                delete T                                                       ;
                v = true                                                       ;
              }                                                                ;
              delete I                                                         ;
            }                                                                  ;
          } else                                                               {
            Q = SC.sql.SelectFrom                                              (
                  "id"                                                         ,
                  PlanTable(ThumbUuid)                                         ,
                  SC.WhereUuid(uuid)                                         ) ;
            if (!SC.Fetch(Q))                                                  {
              I = Picture(SC,uuid)                                             ;
              if (IsNull(I)) DeleteThumb(SC,uuid) ; else                       {
                T = Thumb(SC,uuid)                                             ;
                if (IsNull(T))                                                 {
                  UpdateThumb ( SC , uuid )                                    ;
                  T = Thumb(SC,uuid)                                           ;
                  if (NotNull(T))                                              {
                    delete T                                                   ;
                    v = true                                                   ;
                  }                                                            ;
                } else                                                         {
                  delete T                                                     ;
                  v = true                                                     ;
                }                                                              ;
                delete I                                                       ;
              }                                                                ;
            } else v = true                                                    ;
          }                                                                    ;
          if (v)                                                               {
            Q = SC.sql.SelectFrom                                              (
                  "id"                                                         ,
                  PlanTable(PictureOrders)                                     ,
                  SC.WhereUuid(uuid)                                         ) ;
            if (!SC.Fetch(Q))                                                  {
              SC.insertUuid(PlanTable(PictureOrders),uuid,"uuid")              ;
            }                                                                  ;
          } else                                                               {
            Q = SC.sql.DeleteFrom(PlanTable(PictureOrders),SC.WhereUuid(uuid)) ;
            SC . Query ( Q )                                                   ;
          }                                                                    ;
          if (verify==1)                                                       {
            Q = SC.sql.SelectFrom                                              (
                  "id,uuid,type"                                               ,
                  PlanTable(MajorUuid)                                         ,
                  SC.WhereUuid(uuid)                                         ) ;
            if (SC.Fetch(Q))                                                   {
              SUID muuid = SC.Uuid(1)                                          ;
              int  mtype = SC.Int (2)                                          ;
              if (muuid!=uuid)                                                 {
                plan->Debug                                                    (
                  30                                                           ,
                  tr("Strange picture uuid error [%1 != %2]"                   )
                  .arg(uuid).arg(muuid)                                      ) ;
              } else
              if (mtype!=Types::Picture)                                       {
                plan->Debug                                                    (
                  15                                                           ,
                  tr("Picture uuid %1 has mismatch %2 type"                    )
                  .arg(uuid).arg(mtype)                                      ) ;
              }                                                                ;
            } else                                                             {
              SC.insertUuid(PlanTable(MajorUuid),uuid,Types::Picture)          ;
              plan->Debug(30,tr("Add missing picture uuid = %1").arg(uuid))    ;
            }                                                                  ;
            Q = SC.sql.Update(PlanTable(PictureUuid),SC.WhereUuid(uuid),1,"id");
            SC . Prepare ( Q             )                                     ;
            SC . Bind    ( "id" , pIndex )                                     ;
            SC . Exec    (               )                                     ;
            if (Xuids.contains(uuid))                                          {
              int index = Xuids.indexOf(uuid)                                  ;
              Xuids.takeAt(index)                                              ;
            }                                                                  ;
          }                                                                    ;
        }                                                                      ;
        if (Uuids.count()<=0) done = true                                      ;
        if (!ProgressRun    ) done = true                                      ;
      }                                                                        ;
    }                                                                          ;
    if (ProgressRun && Xuids.count()>0)                                        {
      if (Xuids.count()<1000)                                                  {
        SUID xuid                                                              ;
        foreach (xuid,Xuids)                                                   {
          plan->Debug(30,tr("Missing picture uuid %1 found").arg(xuid))        ;
        }                                                                      ;
      }                                                                        ;
    }                                                                          ;
    SC.close()                                                                 ;
  }                                                                            ;
  SC.remove()                                                                  ;
  plan -> Finish ( ProgressId                           )                      ;
  plan -> Talk   ( tr("Finish building picture thumbs") )                      ;
  deleteLater    (                                      )                      ;
}

bool N::ThumbBuilder::startup(void)
{
  ProgressId    = plan->Progress(tr("Picture thumbs"),tr("%v/%m pictures")) ;
  ProgressValue = 0                                                         ;
  ProgressRun   = true                                                      ;
  start ( )                                                                 ;
  return true                                                               ;
}
