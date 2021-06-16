#include <qtmanagers.h>

typedef struct      {
  SUID   first      ;
  SUID   second     ;
  int    t1         ;
  int    t2         ;
  int    relation   ;
  int    position   ;
  int    reversal   ;
  double membership ;
} nGroupData        ;

N::RetrievePeriod:: RetrievePeriod   ( void )
                  : RetrieveCalendar (      )
{
}

N::RetrievePeriod:: RetrievePeriod   ( QObject * owner , Plan * p )
                  : RetrieveCalendar (           owner ,        p )
{
}

N::RetrievePeriod::~RetrievePeriod(void)
{
}

int N::RetrievePeriod::Type(void) const
{
  return 5 ;
}

QValidator * N::RetrievePeriod::Validator (void)
{
  return NULL ;
}

bool N::RetrievePeriod::ObtainCalendar (
       SqlConnection & SC              ,
       SUID            uuid            ,
       QString       & string          )
{
  if (conf.size()<=0                 ) return false    ;
  if (conf.size()!=sizeof(nGroupData)) return false    ;
  nGroupData * NGD = (nGroupData *)conf.data()         ;
  if ( NGD -> first    != 0                           &&
       NGD -> t1       != 0                           &&
       NGD -> t2       != 0                           &&
       NGD -> relation != 0                          ) {
    GroupItems     GI ( plan )                         ;
    HistoryManager HM ( plan )                         ;
    SUID           xuid  = 0                           ;
    SUID           euid  = 0                           ;
    TUID           stid  = 0                           ;
    TUID           edid  = 0                           ;
    UUIDs          TUIDs                               ;
    TUIDs = GI . Subordination                         (
                    SC                                 ,
                    uuid                               ,
                    NGD -> t1                          ,
                    NGD -> t2                          ,
                    NGD -> relation                    ,
                    SC   . OrderByAsc ( "position" ) ) ;
    if (TUIDs.count()>0)                               {
      xuid = HM . GetHistoryType                       (
               SC                                      ,
               TUIDs                                   ,
               StartType                             ) ;
      euid = HM . GetHistoryType                       (
               SC                                      ,
               TUIDs                                   ,
               EndType                               ) ;
      if (xuid>0) stid = HM . GetTime ( SC , xuid )    ;
      if (euid>0) edid = HM . GetTime ( SC , euid )    ;
      if (stid>0)                                      {
        if (edid<=0) edid = StarDate::now()            ;
        return setTimeString ( stid , edid , string  ) ;
      }                                                ;
    }                                                  ;
  }                                                    ;
  return false                                         ;
}

bool N::RetrievePeriod::setTimeString (
       TUID      stid                 ,
       TUID      edid                 ,
       QString & string               )
{
  QString AgeFormat   = Keywords [ 11 ]                          ;
  QString DayFormat   = Keywords [ 12 ]                          ;
  QString AfterFormat = Keywords [ 13 ]                          ;
  TUID    DTX                                                    ;
  if ( stid > edid )                                             {
    DTX  = stid - edid                                           ;
    DTX /= 86400                                                 ;
    string = QString(AfterFormat).arg(DTX)                       ;
    return true                                                  ;
  }                                                              ;
  ////////////////////////////////////////////////////////////////
  DTX  = edid - stid                                             ;
  DTX /= 86400                                                   ;
  if ( DTX > 365250 )                                            { /* bigger than 1000 years */
    string = QString(DayFormat).arg(DTX)                         ;
    return true                                                  ;
  }                                                              ;
  ////////////////////////////////////////////////////////////////
  StarDate DTSD                                                  ;
  StarDate EDSD                                                  ;
  DTSD . Stardate = stid                                         ;
  DTSD . locate ( )                                              ;
  EDSD . Stardate = edid                                         ;
  EDSD . locate ( )                                              ;
  ////////////////////////////////////////////////////////////////
  QDateTime dt    = DTSD . toLocalTime ( )                       ;
  QDateTime Now   = EDSD . toLocalTime ( )                       ;
  QDate     ND    = Now.date()                                   ;
  QDate     DD    = dt.date()                                    ;
  QDate     XD    = dt.date()                                    ;
  QDate     SD    = dt.date()                                    ;
  int       Secs  = dt.secsTo(Now)                               ;
  int       Days  = 0                                            ;
  int       Years = 0                                            ;
  bool      done  = false                                        ;
  bool      error = false                                        ;
  if (!dt.isValid() || !DD.isValid() || !XD.isValid())           {
    error = true                                                 ;
    done  = true                                                 ;
  }                                                              ;
  if (Secs<0) done = true                                        ;
  Years = ND.year() - DD.year() - 3                              ;
  if (Years<0)                                                   {
    Years = 0                                                    ;
    error = true                                                 ;
    done  = true                                                 ;
  }                                                              ;
  if (Years>0)                                                   {
    if (SD.month()==2 && SD.day()==29)                           {
      int  dxs  = DD.year() + Years                              ;
      bool skip = false                                          ;
      if ((dxs%400)==0) skip = false                        ; else
      if ((dxs%100)==0) skip = true                         ; else
      if ((dxs%  4)==0) skip = false                        ; else
                        skip = true                              ;
      if (skip)                                                  {
        XD.setDate(dxs,3,1)                                      ;
      } else                                                     {
        XD.setDate(dxs,2,29)                                     ;
      }                                                          ;
    } else                                                       {
      XD.setDate(DD.year()+Years,DD.month(),DD.day())            ;
    }                                                            ;
    DD = XD                                                      ;
  }                                                              ;
  while (!done)                                                  {
    if (Years>5000)                                              {
      error = true                                               ;
      done  = true                                               ;
    }                                                            ;
    if (SD.month()==2 && SD.day()==29)                           {
      int  dxs  = DD.year() + 1                                  ;
      bool skip = false                                          ;
      if ((dxs%400)==0) skip = false                        ; else
      if ((dxs%100)==0) skip = true                         ; else
      if ((dxs%  4)==0) skip = false                        ; else
                        skip = true                              ;
      if (skip)                                                  {
        XD.setDate(dxs,3,1)                                      ;
      } else                                                     {
        XD.setDate(dxs,2,29)                                     ;
      }                                                          ;
    } else                                                       {
      XD.setDate(DD.year()+1,DD.month(),DD.day())                ;
    }                                                            ;
    if (XD>ND)                                                   {
      Days = DD.daysTo(ND)                                       ;
      done = true                                                ;
    } else                                                       {
      Years ++                                                   ;
      DD = XD                                                    ;
    }                                                            ;
  }                                                              ;
  ////////////////////////////////////////////////////////////////
  string = QString(AgeFormat).arg(Years).arg(Days)               ;
  return true                                                    ;
}
