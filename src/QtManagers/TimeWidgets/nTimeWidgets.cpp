#include <Common>

#ifdef XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

///////////////////////////////////////////////////

nDateTimeDisplay:: nDateTimeDisplay (QWidget *             parent)
                :  nLabel           (                      parent) ,
                   currentDateTime  (QDateTime::currentDateTime()) ,
                   dateTimeFormat   ("yyyy/MM/dd hh:mm:ss"       ) ,
                   keepUpdate       (true                        ) ,
                   localTime        (true                        )

{
  QObject::connect(
    &TIMER , SIGNAL (timeout   ()) ,
    this   , SLOT   (UpdateTime())
  );
  TIMER.setInterval(1000);
  if (keepUpdate) TIMER.start();
}

nDateTimeDisplay::~nDateTimeDisplay (void)
{
}

QDateTime nDateTimeDisplay::CurrentDateTime(void)
{
  return currentDateTime;
}

void nDateTimeDisplay::setDateTime(QDateTime currentDT)
{
  currentDateTime = currentDT;
  update();
}

bool nDateTimeDisplay::KeepUpdate(void)
{
  return keepUpdate;
}

void nDateTimeDisplay::setKeepUpdate(bool enable)
{
  keepUpdate = enable;
  if (keepUpdate) TIMER.start(1000);
             else TIMER.stop();
  update();
}

bool nDateTimeDisplay::LocalTime(void)
{
  return localTime;
}

void nDateTimeDisplay::setLocalTime(bool enable)
{
  localTime = enable;
  update();
}

QString nDateTimeDisplay::DateTimeFormat(void)
{
  return dateTimeFormat;
}

void nDateTimeDisplay::setDateTimeFormat(QString format)
{
  dateTimeFormat = format;
}

bool nDateTimeDisplay::paintText(QPaintEvent * event)
{
  QPainter p(this);
  QRect    rt = rect();
  QString  t  = currentDateTime.toString(dateTimeFormat);
  QRect    st,tt;

  p.setPen   (palette().color(QPalette::Window));
  p.setBrush (palette().color(QPalette::Window));
  p.drawRect (rt);

  st.setX      (2            );
  st.setY      (2            );
  st.setWidth  (rt.width ()-2);
  st.setHeight (rt.height()-2);
  tt.setX      (0            );
  tt.setY      (0            );
  tt.setWidth  (rt.width ()-2);
  tt.setHeight (rt.height()-2);

  if (UseShadow) {
    p.setPen     (ShadowColor     );
    p.drawText   (st,alignment(),t);
  };
  p.setPen     (palette().color(QPalette::WindowText));
  p.drawText   (tt,alignment(),t);

  return true;
}

void nDateTimeDisplay::UpdateTime(void)
{
  if (localTime)
    currentDateTime = QDateTime::currentDateTime();
  else
    currentDateTime = currentDateTime.addMSecs(1000);
  update();
}

///////////////////////////////////////////////////

nTodayReport:: nTodayReport(QWidget *      parent)
            :  nWidget     (               parent) ,
               Format      ("yyyy/MM/dd hh:mm:ss") ,
               fontOk      (false                )
{
  QObject::connect(
    &TIMER , SIGNAL(timeout()) ,
    this   , SLOT  (repaint())
  );
  TIMER.start(1000);
}

nTodayReport::~nTodayReport(void)
{
}

void nTodayReport::paintEvent(QPaintEvent * event)
{
  QString        T       = QDateTime::currentDateTime().toString(Format);
  QFont          FONT    = font();
  QFontMetrics   FM      = fontMetrics();
  QRect          RT      = event->rect();
  QRect          WR      = rect();
  bool           justfit = false ;
  QSize          S               ;
  if (RT.width()==0 || RT.height()==0) return;
  FONT.setBold(true);
  if (!fontOk) {
    S  = FM.size(Qt::TextSingleLine,T);
    if (S.width ()>WR.width ()) justfit = true;
    if (S.height()>WR.height()) justfit = true;
    if (!justfit) {
      int pt = FONT.pointSize();
      FONT.setPointSize(pt+1);
      setFont(FONT);
    } else {
      int pt = FONT.pointSize();
      FONT.setPointSize(pt-1);
      setFont(FONT);
      fontOk = true;
    };
  };
  QPainter p(this);
  p.setBrush(QColor(255,255,255));
  p.setPen  (QColor(255,255,255));
  p.drawRect(WR);
  p.setPen  (QColor(0,0,255));
  p.setFont (FONT);
  p.drawText(WR,Qt::AlignVCenter|Qt::AlignHCenter,T);
}

void nTodayReport::closeEvent(QCloseEvent * event)
{
  TIMER.stop();
  event->accept();
}

void nTodayReport::setFormat(QString format)
{
  Format = format;
}

///////////////////////////////////////////////////
nTimePanel:: nTimePanel   (QWidget *      parent)
          :  nLabel       (               parent) ,
             DateColor    (QColor(0,0,255)      ) ,
             TimeColor    (QColor(255,0,0)      ) ,
             TimeInterval (1000                 ) ,
             UpdateTime   (true                 ) ,
             margins      (QMargins(40,20,40,10))
{
  UseShadow   = true;
  currentTime = QDateTime::currentDateTime();
  updateTimer.setInterval(TimeInterval);
  QObject::connect(
    &updateTimer , SIGNAL (timeout      ()) ,
    this         , SLOT   (updateDisplay())
  );
  if (UpdateTime) updateTimer.start();
}

nTimePanel::~nTimePanel(void)
{
}

QDateTime nTimePanel::currentDateTime(void)
{
  return currentTime;
}

void nTimePanel::setDateTime(QDateTime dt)
{
  currentTime = dt;
  update();
}

QFont nTimePanel::dateFont(void)
{
  return DateFont;
}

void nTimePanel::setDateFont(QFont font)
{
  DateFont = font;
  update();
}

QColor nTimePanel::dateColor(void)
{
  return DateColor;
}

void nTimePanel::setDateColor(QColor color)
{
  DateColor = color;
  update();
}

QFont nTimePanel::timeFont(void)
{
  return TimeFont;
}

void nTimePanel::setTimeFont(QFont font)
{
  TimeFont = font;
  update();
}

QColor nTimePanel::timeColor(void)
{
  return TimeColor;
}

void nTimePanel::setTimeColor(QColor color)
{
  TimeColor = color;
  update();
}

bool nTimePanel::updateTime(void)
{
  return UpdateTime;
}

void nTimePanel::setUpdateTime(bool updatetime)
{
  UpdateTime = updatetime;
  if (UpdateTime) updateTimer.start();
             else updateTimer.stop ();
  update();
}

int nTimePanel::timeInterval(void)
{
  return TimeInterval;
}

void nTimePanel::setTimeInterval(int interval)
{
  TimeInterval = interval;
  updateTimer.setInterval(interval);
  update();
}

bool nTimePanel::syncLocal(void)
{
  return SyncLocal;
}

void nTimePanel::setSyncLocal(bool Sync)
{
  SyncLocal = Sync;
  update();
}

void nTimePanel::updateDisplay(void)
{
  if (SyncLocal)
    currentTime = QDateTime::currentDateTime();
  else
    currentTime = currentTime.addMSecs(TimeInterval);
  update();
}

bool nTimePanel::paintText(QPaintEvent * event)
{
  QPainter p(this);
  QRect    rt    = rect();
  QString  S     = text();
  QString  D     = currentTime.toString("yyyy/MM/dd");
  QString  T     = currentTime.toString("hh:mm:ss");
  QRect    st,tt;
  int      w,h;
  QFontMetrics fm = fontMetrics();

  p.setPen     (palette().color(QPalette::Window));
  p.setBrush   (palette().color(QPalette::Window));
  p.drawRect   (rt);

  st.setX      (4);
  st.setY      (4);
  st.setWidth  (fm.width(S));
  st.setHeight (fm.height());
  p.setFont    (font());
  p.setPen     (palette().color(QPalette::WindowText));
  p.drawText   (st,alignment(),S);

  w = width () - margins.left() - margins.right ();
  h = height() - margins.top () - margins.bottom();
  h/= 2;

  st.setX      (margins.left()+2);
  st.setY      (margins.top ()+2);
  st.setWidth  (w);
  st.setHeight (h);
  tt.setX      (margins.left()  );
  tt.setY      (margins.top ()  );
  tt.setWidth  (w);
  tt.setHeight (h);

  p.setFont    (DateFont);
  if (UseShadow) {
    p.setPen     (ShadowColor     );
    p.drawText   (st,alignment(),D);
  };
  p.setPen     (DateColor);
  p.drawText   (tt,alignment(),D);

  st.setX      (margins.left()+  2);
  st.setY      (margins.top ()+h+2);
  st.setWidth  (w);
  st.setHeight (h);
  tt.setX      (margins.left()    );
  tt.setY      (margins.top ()+h  );
  tt.setWidth  (w);
  tt.setHeight (h);

  p.setFont    (TimeFont);
  if (UseShadow) {
    p.setPen     (ShadowColor     );
    p.drawText   (st,alignment(),T);
  };
  p.setPen     (TimeColor);
  p.drawText   (tt,alignment(),T);

  return true;
}

//////////////////////////////////////////////////////////
nCalendarObject::nCalendarObject (QGraphicsItem * parent)
               : QGraphicsItem   (                parent)
{
}

nCalendarObject::~nCalendarObject(void)
{
}

QRectF nCalendarObject::boundingRect(void) const
{
  QRectF RF;
  RF.setX(x());
  RF.setY(y());
  RF.setWidth (20.0f);
  RF.setHeight(20.0f);
  return RF;
}

//////////////////////////////////////////////////////////
nCalendarTimeBar::nCalendarTimeBar  (QGraphicsItem * parent)
                : nGraphicsRectItem (                parent) ,
                  GridColor         (QColor(240,160,160)   ) ,
                  BackgroundColor   (QColor(216,255,240)   ) ,
                  BarColor          (QColor(240,224,224)   ) ,
                  BarBorder         (QColor(128,128,224)   )
{
  Initialize();
}

nCalendarTimeBar::~nCalendarTimeBar(void)
{
}

void nCalendarTimeBar::setDirection(int Direction)
{
  TDirection = Direction ;
}

void nCalendarTimeBar::setSize(int Width,int Height)
{
  TWidth  = Width  ;
  THeight = Height ;
  setRect (0.0f,0.0f,TWidth,THeight);
}

void nCalendarTimeBar::Initialize(void)
{
  Bar        = NULL   ;
  TimeLine   = NULL   ;
  TimeText   = NULL   ;
  TDirection = 0      ;
  TWidth     = 200.0f ;
  THeight    =  20.0f ;
  setBrush (BackgroundColor         );
  setPen   (GridColor               );
  setRect  (0.0f,0.0f,TWidth,THeight);
  setDrawStyle(ButtonStyle);
}

void nCalendarTimeBar::setMode(ShowMode MODE)
{
  QGraphicsLineItem * LINE;
  Mode = MODE;
  switch (Mode) {
    case Today:
      Bar = new nGraphicsRectItem(this);
      Bar->setBrush     (BarColor );
      Bar->setPen       (BarBorder);
      Bar->setZValue    (0.3f     );
      Bar->setOpacity   (0.5f     );
      Bar->setVisible   (true     );
      Bar->setDrawStyle (ButtonStyle);
      for (int i=1;i<24;i++) {
        LINE = new QGraphicsLineItem(this);
        LINE->setPen    (GridColor);
        LINE->setZValue (0.5f);
        LINE->setOpacity(0.5f);
        GRID << LINE;
      };
    break;
    case Week:
    break;
    case Month:
    break;
    case Year:
    break;
  };
}

void nCalendarTimeBar::Update(void)
{
  QTime TT(0,0,0,0);
  QTime CT = QTime::currentTime();
  int   DT = TT.secsTo(CT);
  qreal dt = DT;
  QRectF RF = rect();
  QRectF TF = RF;
  switch (Mode) {
    case Today:
      dt  = RF.width();
      dt *= DT;
      dt /= 86400;
      TF.setY     (3            );
      TF.setHeight(RF.height()-6);
      TF.setWidth (dt           );
      Bar->setRect(TF);
      for (int i=0;i<GRID.size();i++) {
        int x = (i+1) * RF.width() / 24.0f;
        GRID[i]->setLine(x,0.0f,x,RF.height());
      };
    break;
    case Week:
    break;
    case Month:
    break;
    case Year:
    break;
  };
}

bool nCalendarTimeBar::isInside(QPointF p)
{
  QTime   T(0,0,0,0);
  QRectF  TF;
  QRectF  RF = rect();
  QPointF PF = mapFromScene(p);
  if (PF.x()<0          ) return false;
  if (PF.y()<0          ) return false;
  if (PF.x()>RF.width ()) return false;
  if (PF.y()>RF.height()) return false;
  return true;
}

QTime nCalendarTimeBar::getTime(QPointF p)
{
  QTime   T(0,0,0,0);
  QRectF  TF;
  QRectF  RF = rect();
  QPointF PF = mapFromScene(p);
  T = T.addSecs((int)((PF.x()*86400.0f)/RF.width()));
  return T;
}

void nCalendarTimeBar::ShowTime(QPointF p)
{
  QTime   T(0,0,0,0);
  QRectF  TF;
  QRectF  RF = rect();
  QPointF PF = mapFromScene(p);
  if (TimeLine==NULL) {
    TimeLine = new QGraphicsLineItem(this);
    TimeLine->setOpacity(0.5f);
    TimeLine->setZValue (0.7f);
    TimeLine->setPen    (QColor(  0,  0,255));
  };
  if (TimeText==NULL) {
    TimeText = new QGraphicsSimpleTextItem(this);
    TimeText->setOpacity(0.5f);
    TimeText->setZValue (0.4f);
    TimeText->setPen    (QColor(0,0,0));
  };
  T = T.addSecs((int)((PF.x()*86400.0f)/RF.width()));
  switch (Mode) {
    case Today:
      TimeLine->setLine(p.x(),0.0f,p.x(),RF.height());
      TimeText->setText(T.toString("hh:mm:ss"));
      TF = TimeText->boundingRect();
      if ((TF.width()+p.x()+8)>=RF.width()) TimeText->setPos(RF.width()-TF.width()-4,(RF.height()-TF.height())/2);
                                       else TimeText->setPos(p.x()+4                ,(RF.height()-TF.height())/2);
    break;
    case Week:
    break;
    case Month:
    break;
    case Year:
    break;
  };
}

#endif
