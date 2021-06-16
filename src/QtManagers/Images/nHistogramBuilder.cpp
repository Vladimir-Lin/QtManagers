#include <qtmanagers.h>

N::HistogramBuilder:: HistogramBuilder ( QObject * parent , Plan * p )
                    : QObject          (           parent            )
                    , Thread           (                             )
                    , PictureManager   (                           p )
                    , plan             (                           p )
                    , ProgressId       ( 0                           )
                    , ProgressValue    ( 0                           )
                    , ProgressRun      ( true                        )
{
  WidgetClass ;
}

N::HistogramBuilder::~HistogramBuilder(void)
{
}

void N::HistogramBuilder::run(void)
{
  QString     ConnectName = QString::number(rand())                     ;
  plan -> setRange  ( ProgressId , 0 , 100                       )      ;
  plan -> Start     ( ProgressId , &ProgressValue , &ProgressRun )      ;
  SqlConnection  SC ( plan->sql                                  )      ;
  if (SC.open("nDownloadPictures",ConnectName))                         {
    QString Q                                                           ;
    UUIDs   Uuids                                                       ;
    SUID    uuid                                                        ;
    bool    done  = false                                               ;
    int     chunk = 1000                                                ;
    int     total = Count(SC)                                           ;
    int     from  = 0                                                   ;
    if (total>0)                                                        {
      Q = SC.sql.SelectFrom                                             (
            "uuid" , PlanTable(PictureColors)                           ,
            "order by id desc limit 0,1"                              ) ;
      if (SC.Fetch(Q))                                                  {
        uuid = SC.Uuid(0)                                               ;
        Q    = SC.sql.SelectFrom                                        (
                 "id"                                                   ,
                 PlanTable(PictureUuid)                                 ,
                 SC.WhereUuid(uuid)                                   ) ;
        if (SC.Fetch(Q)) from = SC.Int(0)                               ;
      }                                                                 ;
      ProgressValue = from                                              ;
      plan -> Start      ( ProgressId , &ProgressValue , &ProgressRun ) ;
      plan -> Talk       ( tr("Start building picture histograms"   ) ) ;
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
          QImage * I                                                    ;
          ProgressValue++                                               ;
          uuid = Uuids[i]                                               ;
          I = Picture(SC,uuid)                                          ;
          if (NotNull(I))                                               {
            QImage * V = new QImage()                                   ;
            *V = I->convertToFormat(QImage::Format_ARGB32)              ;
            delete I                                                    ;
            assureHistogram ( SC , uuid , *V )                          ;
            delete V                                                    ;
          }                                                             ;
        }                                                               ;
        if (Uuids.count()<=0) done = true                               ;
        if (!ProgressRun    ) done = true                               ;
      }                                                                 ;
    }                                                                   ;
    SC.close()                                                          ;
  }                                                                     ;
  SC.remove()                                                           ;
  plan -> Finish     ( ProgressId                                 )     ;
  plan->Talk(tr("Histogram analysis finished"))                         ;
  deleteLater ( )                                                       ;
}

bool N::HistogramBuilder::startup(void)
{
  ProgressId    = plan->Progress(tr("Color histogram"),tr("%v/%m pictures")) ;
  ProgressValue = 0                                                          ;
  ProgressRun   = true                                                       ;
  start ( )                                                                  ;
  return true                                                                ;
}
