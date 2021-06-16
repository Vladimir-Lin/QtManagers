#include <qtmanagers.h>

N::PaletteBuilder:: PaletteBuilder ( QObject * parent , Plan * p )
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

N::PaletteBuilder::~PaletteBuilder(void)
{
}

void N::PaletteBuilder::run(void)
{
  QString     ConnectName = QString::number(rand())                     ;
  plan -> setRange  ( ProgressId , 0 , 100                       )      ;
  plan -> Start     ( ProgressId , &ProgressValue , &ProgressRun )      ;
  SqlConnection  SC ( plan->sql                                  )      ;
  if (SC.open("nPaletteBuilder",ConnectName))                           {
    QString Q                                                           ;
    UUIDs   Uuids                                                       ;
    SUID    uuid                                                        ;
    bool    done  = false                                               ;
    int     chunk = 1000                                                ;
    int     total = Count(SC)                                           ;
    int     from  = 0                                                   ;
    if (total>0)                                                        {
      ProgressValue = from                                              ;
      plan -> Start      ( ProgressId , &ProgressValue , &ProgressRun ) ;
      plan -> Talk       ( tr("Start building picture palettes"     ) ) ;
      while (!done)                                                     {
        total = Count(SC)                                               ;
        plan -> setRange ( ProgressId , 0 , total                     ) ;
        ProgressValue = from                                            ;
        Uuids . clear ( )                                               ;
        Q = SC.sql.SelectFrom                                           (
              "id,uuid"                                                 ,
              PlanTable(PictureUuid)                                    ,
              QString("where id > %1 order by id asc limit 0,%2"        )
              .arg(from                                                 )
              .arg(chunk                                                )
            )                                                           ;
        SqlLoopNow ( SC , Q )                                           ;
          from = SC . Int  ( 0 )                                        ;
          uuid = SC . Uuid ( 1 )                                        ;
          Uuids << uuid                                                 ;
        SqlLoopErr ( SC , Q )                                           ;
        SqlLoopEnd ( SC , Q )                                           ;
        for (int i=0;ProgressRun && i<Uuids.count();i++)                {
          SUID pcid = 0                                                 ;
          QImage * I                                                    ;
          ProgressValue++                                               ;
          uuid = Uuids[i]                                               ;
          Q = SC.sql.SelectFrom                                         (
                "colormap" , PlanTable(PicturePalettes)                 ,
                QString("where picture = %1").arg(uuid)               ) ;
          if (SC.Fetch(Q)) pcid = SC . Uuid ( 0 )                       ;
          if (pcid<=0)                                                  {
            I = Picture(SC,uuid)                                        ;
            if (NotNull(I))                                             {
              assurePalette(SC,uuid,*I)                                 ;
              delete I                                                  ;
            }                                                           ;
          }                                                             ;
        }                                                               ;
        if (Uuids.count()<=0) done = true                               ;
        if (!ProgressRun    ) done = true                               ;
      }                                                                 ;
    }                                                                   ;
    SC.close()                                                          ;
  }                                                                     ;
  SC.remove()                                                           ;
  plan -> Finish ( ProgressId                             )             ;
  plan -> Talk   ( tr("Finish building picture palettes") )             ;
  deleteLater    (                                        )             ;
}

bool N::PaletteBuilder::startup(void)
{
  ProgressId    = plan->Progress(tr("Picture palettes"),tr("%v/%m pictures")) ;
  ProgressValue = 0                                                           ;
  ProgressRun   = true                                                        ;
  start ( )                                                                   ;
  return true                                                                 ;
}
