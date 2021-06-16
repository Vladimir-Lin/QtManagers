#include <qtmanagers.h>

#ifdef QT_OPENGL_LIB

typedef struct               {
  void           * editor    ;
  N::Universe    * universe  ;
  N::Universe    * attach    ;
  N::SceneObject * scene     ;
  N::SceneObject * mounting  ;
} PrivateVideoFunctionBinder ;

N::PickView:: PickView ( QObject * parent , Plan * p )
            : PickItem (           parent ,        p )
{
  WidgetClass   ;
  Configure ( ) ;
}

N::PickView:: PickView ( QObject * parent , Plan * p , SceneDestructor * des )
            : PickItem (           parent ,        p ,                   des )
{
  WidgetClass   ;
  Configure ( ) ;
}

N::PickView::~PickView (void)
{
}

void N::PickView::Configure(void)
{
  QPointF      PadPoint ( 40 , 40                            ) ;
  ControlPoint color                                           ;
  color  = ControlPoint ( 1.00 , 1.00 , 1.0 , 0.95           ) ;
  addColor              (   60 , color                       ) ;
  color  = ControlPoint ( 0.60 , 0.80 , 0.9 , 0.10           ) ;
  addColor              (   61 , color                       ) ;
  color  = ControlPoint ( 0.50 , 0.70 , 0.3 , 0.25           ) ;
  addColor              (   62 , color                       ) ;
  color  = ControlPoint ( 0.70 , 0.30 , 0.5 , 0.35           ) ;
  addColor              (   63 , color                       ) ;
  //////////////////////////////////////////////////////////////
  QObject::setProperty ( "Type"        , "PickView"          ) ;
  setVariable          ( "Type"        , "PickView"          ) ;
  //////////////////////////////////////////////////////////////
  setVariable          ( "Back"        , false               ) ;
  setVariable          ( "Side"        , false               ) ;
  setVariable          ( "BackColor"   , 60                  ) ;
  setVariable          ( "SideColor"   , 61                  ) ;
  setVariable          ( "PadColor"    , 62                  ) ;
  setVariable          ( "BorderColor" , 63                  ) ;
  setVariable          ( "PadPoint"    , PadPoint            ) ;
  setVariable          ( "PadAlign"    , "Top"               ) ;
  //////////////////////////////////////////////////////////////
  setVariable          ( "Suicide"     , true                ) ;
  setVariable          ( "Items"       , 0                   ) ;
  setVariable          ( "CurrentPageIndex" , 0              ) ;
  setVariable          ( "AutoRefresh" , false               ) ;
  setVariable          ( "ArrangeBy"   , "Icon"              ) ;
//  setVariable          ( "ArrangeBy"   , "Picture"           ) ;
//  setVariable          ( "ArrangeBy"   , "Language"          ) ;
//  setVariable          ( "ArrangeBy"   , "Names"             ) ;
  setVariable          ( "ScrollBy"    , "Current"           ) ;
//  setVariable          ( "ScrollBy"    , "Start"             ) ;
}

void N::PickView::run(int type,ThreadData * data)
{
  ItemRunner ( type , data ) ;
}

// PickView::ItemRunner => 10001 , 10002 , 10003 , 10004 , 10011, 73002, 98011, 98012, 851225, 851226, 851227, 851228, 851229, 851230, 851231, 851232
bool N::PickView::ItemRunner(int type,ThreadData * data)
{
  switch ( type )                          {
    case 10001                             :
      LoadItems      (                   ) ;
    return true                            ;
    case 10002                             :
      ArrangeItems   (                   ) ;
    return true                            ;
    case 10003                             :
      CleanItems     (                   ) ;
    return true                            ;
    case 10004                             :
      ReportDetails  (                   ) ;
    return true                            ;
    case 10005                             :
      endDrag        (                   ) ;
    return true                            ;
    case 73002                             :
      RenameItem     ( data -> Arguments ) ;
    return true                            ;
    case 10011                             :
    case 98011                             :
    case 98012                             :
      LoadItems      (                   ) ;
      CleanItems     (                   ) ;
      ArrangeItems   (                   ) ;
      start          ( 10004             ) ;
    return true                            ;
    case CiosMenuId  ( 85 , 12 , 25      ) :
      AssignIcon     ( data -> Arguments ) ;
    return true                            ;
    case CiosMenuId  ( 85 , 12 , 26      ) :
      CountGroups    ( data -> Arguments ) ;
    return true                            ;
    case CiosMenuId  ( 85 , 12 , 27      ) :
      CountGroup     ( data -> Arguments ) ;
    return true                            ;
    case CiosMenuId  ( 85 , 12 , 28      ) :
      AppendDivision ( data -> Arguments ) ;
    return true                            ;
    case CiosMenuId  ( 85 , 12 , 29      ) :
      DeleteGroup    ( data -> Arguments ) ;
    return true                            ;
    case CiosMenuId  ( 85 , 12 , 30      ) :
      DetachGroup    ( data -> Arguments ) ;
    return true                            ;
    case CiosMenuId  ( 85 , 12 , 31      ) :
      JoinGroup      ( data -> Arguments ) ;
    return true                            ;
    case CiosMenuId  ( 85 , 12 , 32      ) :
      MoveGroup      ( data -> Arguments ) ;
    return true                            ;
  }                                        ;
  return false                             ;
}

void N::PickView::renderBox(void)
{
  QString PadAlign = Value ( "PadAlign" ) . toString ( )   ;
  QPointF PadPoint = Value ( "PadPoint" ) . toPointF ( )   ;
  //////////////////////////////////////////////////////////
  QtGL::PushMatrix       (                               ) ;
  QtGL::Disable          ( GL_CULL_FACE                  ) ;
  QtGL::Disable          ( GL_LINE_SMOOTH                ) ;
  QtGL::Disable          ( GL_POLYGON_SMOOTH             ) ;
  //////////////////////////////////////////////////////////
  if                     ( Value ( "Back" ) . toBool ( ) ) { // Back
    assumeColor          ( "BackColor"                   ) ;
    CubeVertex           ( 100 , minimum , maximum       ) ;
  }                                                        ;
  //////////////////////////////////////////////////////////
  if                     ( Value ( "Side" ) . toBool ( ) ) { // Side
    assumeColor          ( "SideColor"                   ) ;
    for                  ( int i = 102 ; i <= 105 ; i++  ) {
      CubeVertex         (   i , minimum , maximum       ) ;
    }                                                      ;
  }                                                        ;
  //////////////////////////////////////////////////////////
  assumeColor            ( "PadColor"                    ) ;
  if ( "Top"    == PadAlign )                              {
    double y = maximum . y - PadPoint . y ( )              ;
    QtGL::Quadrilateral  (                               ) ;
    QtGL::Vertex3d       ( minimum . x , y , maximum . z ) ;
    QtGL::Vertex3d       ( maximum . x , y , maximum . z ) ;
    QtGL::Vertex3d       ( maximum . x , y , minimum . z ) ;
    QtGL::Vertex3d       ( minimum . x , y , minimum . z ) ;
    QtGL::End            (                               ) ;
  }                                                        ;
  if ( "Bottom" == PadAlign )                              {
    double y = minimum . y + PadPoint . y ( )              ;
    QtGL::Quadrilateral  (                               ) ;
    QtGL::Vertex3d       ( minimum . x , y , maximum . z ) ;
    QtGL::Vertex3d       ( maximum . x , y , maximum . z ) ;
    QtGL::Vertex3d       ( maximum . x , y , minimum . z ) ;
    QtGL::Vertex3d       ( minimum . x , y , minimum . z ) ;
    QtGL::End            (                               ) ;
  }                                                        ;
  if ( "Left"   == PadAlign )                              {
    double x = minimum . x + PadPoint . x ( )              ;
    QtGL::Quadrilateral  (                               ) ;
    QtGL::Vertex3d       ( x , minimum . y , maximum . z ) ;
    QtGL::Vertex3d       ( x , maximum . y , maximum . z ) ;
    QtGL::Vertex3d       ( x , maximum . y , maximum . z ) ;
    QtGL::Vertex3d       ( x , minimum . y , maximum . z ) ;
    QtGL::End            (                               ) ;
  }                                                        ;
  if ( "Right"  == PadAlign )                              {
    double x = maximum . x - PadPoint . x ( )              ;
    QtGL::Quadrilateral  (                               ) ;
    QtGL::Vertex3d       ( x , minimum . y , maximum . z ) ;
    QtGL::Vertex3d       ( x , maximum . y , maximum . z ) ;
    QtGL::Vertex3d       ( x , maximum . y , maximum . z ) ;
    QtGL::Vertex3d       ( x , minimum . y , maximum . z ) ;
    QtGL::End            (                               ) ;
  }                                                        ;
  //////////////////////////////////////////////////////////
  assumeColor            ( "BorderColor"                 ) ;
  SceneObject::renderBox (                               ) ;
  if ( "Top"    == PadAlign )                              {
    double y = maximum . y - PadPoint . y ( )              ;
    QtGL::Loop           (                               ) ;
    QtGL::Vertex3d       ( minimum . x , y , maximum . z ) ;
    QtGL::Vertex3d       ( maximum . x , y , maximum . z ) ;
    QtGL::Vertex3d       ( maximum . x , y , minimum . z ) ;
    QtGL::Vertex3d       ( minimum . x , y , minimum . z ) ;
    QtGL::End            (                               ) ;
  }                                                        ;
  if ( "Bottom" == PadAlign )                              {
    double y = minimum . y + PadPoint . y ( )              ;
    QtGL::Loop           (                               ) ;
    QtGL::Vertex3d       ( minimum . x , y , maximum . z ) ;
    QtGL::Vertex3d       ( maximum . x , y , maximum . z ) ;
    QtGL::Vertex3d       ( maximum . x , y , minimum . z ) ;
    QtGL::Vertex3d       ( minimum . x , y , minimum . z ) ;
    QtGL::End            (                               ) ;
  }                                                        ;
  if ( "Left"   == PadAlign )                              {
    double x = minimum . x + PadPoint . x ( )              ;
    QtGL::Loop           (                               ) ;
    QtGL::Vertex3d       ( x , minimum . y , maximum . z ) ;
    QtGL::Vertex3d       ( x , maximum . y , maximum . z ) ;
    QtGL::Vertex3d       ( x , maximum . y , maximum . z ) ;
    QtGL::Vertex3d       ( x , minimum . y , maximum . z ) ;
    QtGL::End            (                               ) ;
  }                                                        ;
  if ( "Right"  == PadAlign )                              {
    double x = maximum . x - PadPoint . x ( )              ;
    QtGL::Loop           (                               ) ;
    QtGL::Vertex3d       ( x , minimum . y , maximum . z ) ;
    QtGL::Vertex3d       ( x , maximum . y , maximum . z ) ;
    QtGL::Vertex3d       ( x , maximum . y , maximum . z ) ;
    QtGL::Vertex3d       ( x , minimum . y , maximum . z ) ;
    QtGL::End            (                               ) ;
  }                                                        ;
  //////////////////////////////////////////////////////////
  QtGL::Enable           ( GL_CULL_FACE                  ) ;
  QtGL::PopMatrix        (                               ) ;
}

QImage * N::PickView::FetchThumb(SqlConnection & SC,SUID p,QString replacement)
{
  QImage       * t = NULL                    ;
  PictureManager PM ( plan )                 ;
  ////////////////////////////////////////////
  if ( p > 0 )                               {
    QImage * r = PM . Thumb ( SC , p )       ;
    if ( IsNull ( r ) )                      {
      t = new QImage ( replacement )         ;
    } else                                   {
      t = toIcon ( r , QSize ( 128 , 128 ) ) ;
      delete r                               ;
    }                                        ;
  } else                                     {
    t   = new QImage ( replacement )         ;
  }                                          ;
  ////////////////////////////////////////////
  return t                                   ;
}

void N::PickView::BoxChanged(bool hover)
{
  qint64 total = Value ( "Items"       ) . toLongLong ( ) ;
  bool   r     = Value ( "AutoRefresh" ) . toBool     ( ) ;
  ArrangeTitle      (         )                           ;
  RearrangeItems    (         )                           ;
  ArrangeScrollBars (         )                           ;
  UpdateScrollPages ( total   )                           ;
  ArrangePads       (         )                           ;
  if                ( ! hover )                           {
    if ( r ) start  ( 10011   )                           ;
  }                                                       ;
}

void N::PickView::FinalizeSelection(void)
{
  ItemSelections ( ) ;
}

void N::PickView::ArrangeItems(void)
{
  QString arrangeBy   = Value ( "ArrangeBy"   ) . toString ( ) ;
  QString replaceIcon = Value ( "ReplaceIcon" ) . toString ( ) ;
  //////////////////////////////////////////////////////////////
  if ( "Icon"    == arrangeBy )                                {
    if ( replaceIcon . length ( ) <= 0 ) return                ;
    ArrangeItems    ( replaceIcon )                            ;
  }                                                            ;
  //////////////////////////////////////////////////////////////
  if ( "Picture" == arrangeBy )                                {
    if ( replaceIcon . length ( ) <= 0 ) return                ;
    ArrangePictures ( replaceIcon )                            ;
  }                                                            ;
  //////////////////////////////////////////////////////////////
  if ( "Language" == arrangeBy )                               {
    if ( replaceIcon . length ( ) <= 0 ) return                ;
    ArrangeLanguage ( replaceIcon )                            ;
  }                                                            ;
  //////////////////////////////////////////////////////////////
  if ( "Names"    == arrangeBy )                               {
    if ( replaceIcon . length ( ) <= 0 ) return                ;
    ArrangeNames    ( replaceIcon )                            ;
  }                                                            ;
}

void N::PickView::ArrangeItems(QString replaceIcon)
{
  GroupItems     GI ( plan                   )                  ;
  SqlConnection  SC ( plan -> sql            )                  ;
  ///////////////////////////////////////////////////////////////
  setVariable       ( "CurrentPageIndex" , 0 )                  ;
  verticalIndex   = Value ( "StartId" ) . toInt ( )             ;
  horizontalIndex = Value ( "StartId" ) . toInt ( )             ;
  ///////////////////////////////////////////////////////////////
  int dtype = GroupType ( 2 )                                   ;
  GI . AutoMap    = true                                        ;
  GI . GroupTable = GI . LookTable                              (
                      dtype                                     ,
                      Types::Picture                            ,
                      Groups::Icon                            ) ;
  ///////////////////////////////////////////////////////////////
  if ( SC . open ( FunctionString ) )                           {
    UUIDs              U      = RealItems                       ;
    int                c      = columnCount  ( )                ;
    int                r      = lineCount    ( )                ;
    SUID               u      = 0                               ;
    qint64             index  = 0                               ;
    qint64             z      = 0                               ;
    ProgressButton   * pb     = ProgressTool         ( )        ;
    ProgressReporter * pr     = new ProgressReporter ( )        ;
    bool               drop   = false                           ;
    /////////////////////////////////////////////////////////////
    pr -> total = U . count ( )                                 ;
    pr -> value = 0                                             ;
    pr -> setRunning  ( true )                                  ;
    pr -> Using       (      )                                  ;
    pb -> setReporter ( pr   )                                  ;
    ArrangePads       (      )                                  ;
    /////////////////////////////////////////////////////////////
    while ( ! drop )                                            {
      if ( 0 == pr -> state ) drop = true                       ;
      if ( ! drop )                                             {
        if ( 2 == pr -> state )                                 {
          Time::msleep ( 10 )                                   ;
          continue                                              ;
        }                                                       ;
        pr -> state = 1                                         ;
      }                                                         ;
      if ( ! drop ) drop = ! ForUuid ( u , index , U )          ;
      if ( ! drop )                                             {
        QString  n = FetchName ( SC , u )                       ;
        SUID     p = GI . FindSecond                            (
                       SC                                       ,
                       u                                        ,
                       dtype                                    ,
                       Types::Picture                           ,
                       Groups::Icon                             ,
                       SC . OrderByAsc  ( "position" )          ,
                       SC . sql . Limit ( 0 , 1      )        ) ;
        QImage * t = FetchThumb   ( SC                          ,
                                    p                           ,
                                    replaceIcon               ) ;
        /////////////////////////////////////////////////////////
        SelectItem * si = NewItem ( n , t , QSize ( 128,128 ) ) ;
        si -> setVariable         ( "UUID" , u                ) ;
        ArrangeItem               ( si -> node , z , c , r    ) ;
        z ++                                                    ;
        pr -> value = z                                         ;
        RealToView [ u ] = si -> node                           ;
      }                                                         ;
    }                                                           ;
    /////////////////////////////////////////////////////////////
    pr -> Leaving                 (                           ) ;
    ClosePad                      ( pb -> node                ) ;
    /////////////////////////////////////////////////////////////
    UpdateScrollPages             ( U . count ( )             ) ;
    BoxChanged                    ( true                      ) ;
    /////////////////////////////////////////////////////////////
    SC . close                    (                           ) ;
  }                                                             ;
  SC   . remove                   (                           ) ;
}

void N::PickView::ArrangePictures(QString replaceIcon)
{
  int           c     = columnCount  ( )                        ;
  int           r     = lineCount    ( )                        ;
  qint64        total = Value ( "Items" ) . toLongLong ( )      ;
  SqlConnection SC ( plan -> sql )                              ;
  ///////////////////////////////////////////////////////////////
  if ( SC . open ( FunctionString ) )                           {
    UUIDs  U      = RealItems                                   ;
    SUID   u      = 0                                           ;
    qint64 index  = 0                                           ;
    qint64 z      = 0                                           ;
    ProgressButton   * pb     = ProgressTool         ( )        ;
    ProgressReporter * pr     = new ProgressReporter ( )        ;
    bool               drop   = false                           ;
    /////////////////////////////////////////////////////////////
    pr -> total = U . count ( )                                 ;
    pr -> value = 0                                             ;
    pr -> setRunning  ( true )                                  ;
    pr -> Using       (      )                                  ;
    pb -> setReporter ( pr   )                                  ;
    ArrangePads       (      )                                  ;
    /////////////////////////////////////////////////////////////
    while ( ! drop )                                            {
      if ( 0 == pr -> state ) drop = true                       ;
      if ( ! drop )                                             {
        if ( 2 == pr -> state )                                 {
          Time::msleep ( 10 )                                   ;
          continue                                              ;
        }                                                       ;
        pr -> state = 1                                         ;
      }                                                         ;
      if ( ! drop ) drop = ! ForUuid ( u , index , U )          ;
      if ( ! drop )                                             {
        QImage * t = FetchThumb ( SC                            ,
                                  u                             ,
                                  replaceIcon                 ) ;
        /////////////////////////////////////////////////////////
        SelectItem * si = NewIcon ( t , QSize ( 128 , 128 )   ) ;
        si -> setVariable         ( "UUID" , u                ) ;
        ArrangeItem               ( si -> node , z , c , r    ) ;
        RealToView [ u ] = si -> node                           ;
        /////////////////////////////////////////////////////////
        z ++                                                    ;
        pr -> value = z                                         ;
      }                                                         ;
    }                                                           ;
    /////////////////////////////////////////////////////////////
    pr -> Leaving (            )                                ;
    ClosePad      ( pb -> node )                                ;
    /////////////////////////////////////////////////////////////
    UpdateScrollPages             ( total                     ) ;
    BoxChanged                    ( true                      ) ;
    SC . close                    (                           ) ;
  }                                                             ;
  SC   . remove                   (                           ) ;
}

void N::PickView::ArrangeLanguage(QString replaceIcon)
{
  GroupItems     GI ( plan                   )                  ;
  SqlConnection  SC ( plan -> sql            )                  ;
  ///////////////////////////////////////////////////////////////
  setVariable       ( "CurrentPageIndex" , 0 )                  ;
  verticalIndex   = Value ( "StartId" ) . toInt ( )             ;
  horizontalIndex = Value ( "StartId" ) . toInt ( )             ;
  ///////////////////////////////////////////////////////////////
  int dtype = GroupType ( 2 )                                   ;
  GI . AutoMap    = true                                        ;
  GI . GroupTable = GI . LookTable                              (
                      dtype                                     ,
                      Types::Picture                            ,
                      Groups::Icon                            ) ;
  ///////////////////////////////////////////////////////////////
  if ( SC . open ( FunctionString ) )                           {
    UUIDs              U      = RealItems                       ;
    int                L      = 1871                            ;
    int                c      = columnCount  ( )                ;
    int                r      = lineCount    ( )                ;
    SUID               u      = 0                               ;
    qint64             index  = 0                               ;
    qint64             z      = 0                               ;
    ProgressButton   * pb     = ProgressTool         ( )        ;
    ProgressReporter * pr     = new ProgressReporter ( )        ;
    bool               drop   = false                           ;
    /////////////////////////////////////////////////////////////
    pr -> total = U . count ( )                                 ;
    pr -> value = 0                                             ;
    pr -> setRunning  ( true )                                  ;
    pr -> Using       (      )                                  ;
    pb -> setReporter ( pr   )                                  ;
    ArrangePads       (      )                                  ;
    /////////////////////////////////////////////////////////////
    L = GI . usingLanguage                                      (
          SC                                                    ,
          FirstUuid()                                           ,
          GroupType ( 1 )                                       ,
          plan->LanguageId                                    ) ;
    if ( L <= 0 ) L = plan->LanguageId                          ;
    setVariable ( "LanguageId" , L )                            ;
    /////////////////////////////////////////////////////////////
    while ( ! drop )                                            {
      if ( 0 == pr -> state ) drop = true                       ;
      if ( ! drop )                                             {
        if ( 2 == pr -> state )                                 {
          Time::msleep ( 10 )                                   ;
          continue                                              ;
        }                                                       ;
        pr -> state = 1                                         ;
      }                                                         ;
      if ( ! drop ) drop = ! ForUuid ( u , index , U )          ;
      if ( ! drop )                                             {
        QString  n = FetchAsynName ( SC , u , L )               ;
        SUID     p = GI . FindSecond                            (
                       SC                                       ,
                       u                                        ,
                       dtype                                    ,
                       Types::Picture                           ,
                       Groups::Icon                             ,
                       SC . OrderByAsc  ( "position" )          ,
                       SC . sql . Limit ( 0 , 1      )        ) ;
        QImage * t = FetchThumb   ( SC                          ,
                                    p                           ,
                                    replaceIcon               ) ;
        /////////////////////////////////////////////////////////
        SelectItem * si = NewItem ( n , t , QSize ( 128,128 ) ) ;
        si -> setVariable         ( "UUID" , u                ) ;
        ArrangeItem               ( si -> node , z , c , r    ) ;
        z ++                                                    ;
        pr -> value = z                                         ;
        RealToView [ u ] = si -> node                           ;
      }                                                         ;
    }                                                           ;
    /////////////////////////////////////////////////////////////
    pr -> Leaving                 (                           ) ;
    ClosePad                      ( pb -> node                ) ;
    /////////////////////////////////////////////////////////////
    UpdateScrollPages             ( U . count ( )             ) ;
    BoxChanged                    ( true                      ) ;
    /////////////////////////////////////////////////////////////
    SC . close                    (                           ) ;
  }                                                             ;
  SC   . remove                   (                           ) ;
}

void N::PickView::ArrangeNames(QString replaceIcon)
{
  GroupItems     GI ( plan                   )                  ;
  SqlConnection  SC ( plan -> sql            )                  ;
  ///////////////////////////////////////////////////////////////
  setVariable       ( "CurrentPageIndex" , 0 )                  ;
  verticalIndex   = Value ( "StartId" ) . toInt ( )             ;
  horizontalIndex = Value ( "StartId" ) . toInt ( )             ;
  ///////////////////////////////////////////////////////////////
  int dtype = GroupType ( 2 )                                   ;
  GI . AutoMap    = true                                        ;
  GI . GroupTable = GI . LookTable                              (
                      dtype                                     ,
                      Types::Picture                            ,
                      Groups::Icon                            ) ;
  ///////////////////////////////////////////////////////////////
  if ( SC . open ( FunctionString ) )                           {
    UUIDs              U      = RealItems                       ;
    int                L      = 1871                            ;
    int                c      = columnCount  ( )                ;
    int                r      = lineCount    ( )                ;
    SUID               u      = 0                               ;
    qint64             index  = 0                               ;
    qint64             z      = 0                               ;
    ProgressButton   * pb     = ProgressTool         ( )        ;
    ProgressReporter * pr     = new ProgressReporter ( )        ;
    bool               drop   = false                           ;
    /////////////////////////////////////////////////////////////
    pr -> total = U . count ( )                                 ;
    pr -> value = 0                                             ;
    pr -> setRunning  ( true )                                  ;
    pr -> Using       (      )                                  ;
    pb -> setReporter ( pr   )                                  ;
    ArrangePads       (      )                                  ;
    /////////////////////////////////////////////////////////////
    L = GI . usingLanguage                                      (
          SC                                                    ,
          FirstUuid()                                           ,
          GroupType ( 1 )                                       ,
          plan->LanguageId                                    ) ;
    if ( L <= 0 ) L = plan->LanguageId                          ;
    setVariable ( "LanguageId" , L )                            ;
    /////////////////////////////////////////////////////////////
    while ( ! drop )                                            {
      if ( 0 == pr -> state ) drop = true                       ;
      if ( ! drop )                                             {
        if ( 2 == pr -> state )                                 {
          Time::msleep ( 10 )                                   ;
          continue                                              ;
        }                                                       ;
        pr -> state = 1                                         ;
      }                                                         ;
      if ( ! drop ) drop = ! ForUuid ( u , index , U )          ;
      if ( ! drop )                                             {
        QString  n = FetchNames ( SC , u )                      ;
        SUID     p = GI . FindSecond                            (
                       SC                                       ,
                       u                                        ,
                       dtype                                    ,
                       Types::Picture                           ,
                       Groups::Icon                             ,
                       SC . OrderByAsc  ( "position" )          ,
                       SC . sql . Limit ( 0 , 1      )        ) ;
        QImage * t = FetchThumb   ( SC                          ,
                                    p                           ,
                                    replaceIcon               ) ;
        /////////////////////////////////////////////////////////
        SelectItem * si = NewItem ( n , t , QSize ( 128,128 ) ) ;
        si -> setVariable         ( "UUID" , u                ) ;
        ArrangeItem               ( si -> node , z , c , r    ) ;
        z ++                                                    ;
        pr -> value = z                                         ;
        RealToView [ u ] = si -> node                           ;
      }                                                         ;
    }                                                           ;
    /////////////////////////////////////////////////////////////
    pr -> Leaving                 (                           ) ;
    ClosePad                      ( pb -> node                ) ;
    /////////////////////////////////////////////////////////////
    UpdateScrollPages             ( U . count ( )             ) ;
    BoxChanged                    ( true                      ) ;
    /////////////////////////////////////////////////////////////
    SC . close                    (                           ) ;
  }                                                             ;
  SC   . remove                   (                           ) ;
}

void N::PickView::RearrangeItems(void)
{
  UUIDs U = CurrentItems         ;
  PickItem::RearrangeItems ( U ) ;
}

void N::PickView::UpdateScrollPages(qint64 total)
{
  QString scrollBy = Value ( "ScrollBy" ) . toString ( ) ;
  ////////////////////////////////////////////////////////
  if ( "Current" == scrollBy )                           {
    ScrollByCurrent ( total )                            ;
  }                                                      ;
  ////////////////////////////////////////////////////////
  if ( "Start"   == scrollBy )                           {
    ScrollByStart   ( total )                            ;
  }                                                      ;
}

void N::PickView::ScrollByStart(qint64 total)
{
  qint64  vi      = Value ( "StartId" ) . toLongLong ( ) ;
  QString arrange = Value ( "Arrange" ) . toString   ( ) ;
  if ( ( "Top/Bottom" == arrange )                      ||
       ( "Bottom/Top" == arrange )                     ) {
    UpdateVScroll ( total )                              ;
    verticalIndex = vi                                   ;
  }                                                      ;
  if ( ( "Left/Right" == arrange )                      ||
       ( "Right/Left" == arrange )                     ) {
    UpdateHScroll ( total )                              ;
    horizontalIndex = vi                                 ;
  }                                                      ;
}

void N::PickView::ScrollByCurrent(qint64 total)
{
  QString arrange = Value ( "Arrange" ) . toString ( ) ;
  qint64 vi                                            ;
  if ( ( "Top/Bottom" == arrange )                    ||
       ( "Bottom/Top" == arrange )                   ) {
    vi = verticalIndex                                 ;
    UpdateVScroll ( total )                            ;
    verticalIndex = vi                                 ;
  }                                                    ;
  if ( ( "Left/Right" == arrange )                    ||
       ( "Right/Left" == arrange )                   ) {
    vi = horizontalIndex                               ;
    UpdateHScroll ( total )                            ;
    horizontalIndex = vi                               ;
  }                                                    ;
}

bool N::PickView::LoadItems(void)
{
  QString access = Value ( "Access" ) . toString ( ) . toLower ( ) ;
  if ( "full"      == access ) return LoadFull      ( )            ;
  if ( "divisions" == access ) return LoadDivisions ( )            ;
  if ( "limits"    == access ) return LoadLimits    ( )            ;
  if ( "first"     == access ) return LoadFirst     ( )            ;
  if ( "second"    == access ) return LoadSecond    ( )            ;
  return false                                                     ;
}

bool N::PickView::LoadFull(void)
{
  bool          correct = false                              ;
  qint64        total   = 0                                  ;
  UUIDs         U                                            ;
  SqlConnection SC ( plan -> sql )                           ;
  ////////////////////////////////////////////////////////////
  if ( SC . open ( FunctionString ) )                        {
    total = SC . count ( MainTable ( ) )                     ;
    if ( SC . Uuids ( U                                      ,
                      MainTable ( )                          ,
                      "uuid"                                 ,
                      SC . OrderBy                           (
                        SortItem                   ( )       ,
                        (Qt::SortOrder)SortingType ( ) ) ) ) {
      correct = true                                         ;
    }                                                        ;
    SC . close  ( )                                          ;
  }                                                          ;
  SC   . remove ( )                                          ;
  ////////////////////////////////////////////////////////////
  LockAt   ( "Operate" )                                     ;
  if ( correct )                                             {
    RealItems = U                                            ;
  } else                                                     {
    RealItems . clear ( )                                    ;
  }                                                          ;
  setVariable ( "Items" , total )                            ;
  UnlockAt ( "Operate" )                                     ;
  ////////////////////////////////////////////////////////////
  return correct                                             ;
}

bool N::PickView::LoadDivisions(void)
{
  bool          correct = false                   ;
  UUIDs         U                                 ;
  SqlConnection SC ( plan -> sql )                ;
  /////////////////////////////////////////////////
  if ( SC . open ( FunctionString ) )             {
    if ( LoadGroups ( SC                          ,
                      GroupType   ( 1 )           ,
                      U                           ,
                      SortingType (   )           ,
                      SortItem    (   )       ) ) {
      correct = true                              ;
    }                                             ;
    SC . close  (                               ) ;
  }                                               ;
  SC   . remove (                               ) ;
  /////////////////////////////////////////////////
  LockAt   ( "Operate" )                          ;
  if ( correct )                                  {
    RealItems = U                                 ;
  } else                                          {
    RealItems . clear ( )                         ;
  }                                               ;
  setVariable ( "Items" , RealItems . count ( ) ) ;
  UnlockAt ( "Operate" )                          ;
  /////////////////////////////////////////////////
  return correct                                  ;
}

bool N::PickView::LoadLimits(void)
{
  bool          correct = false                                   ;
  qint64        startId = Value ( "StartId" ) . toLongLong ( )    ;
  qint64        column  = columnCount ( )                         ;
  qint64        lines   = lineCount   ( )                         ;
  qint64        page    = column * lines                          ;
  qint64        total   = 0                                       ;
  UUIDs         U                                                 ;
  SqlConnection SC ( plan -> sql )                                ;
  /////////////////////////////////////////////////////////////////
  if ( SC . open ( FunctionString ) )                             {
    total = SC . count ( MainTable ( ) )                          ;
    if ( SC . Uuids ( U                                           ,
                      MainTable ( )                               ,
                      "uuid"                                      ,
                      QString ( "%1 %2"                           )
                      . arg ( SC . OrderBy                        (
                              SortItem                   ( )      ,
                              (Qt::SortOrder)SortingType ( )    ) )
                      . arg ( SC . sql . Limit ( startId , page ) )
                                                              ) ) {
      correct = true                                              ;
    }                                                             ;
    SC . close  ( )                                               ;
  }                                                               ;
  SC   . remove ( )                                               ;
  /////////////////////////////////////////////////////////////////
  LockAt   ( "Operate" )                                          ;
  if ( correct )                                                  {
    RealItems = U                                                 ;
  } else                                                          {
    RealItems . clear ( )                                         ;
  }                                                               ;
  setVariable ( "Items" , total )                                 ;
  UnlockAt ( "Operate" )                                          ;
  /////////////////////////////////////////////////////////////////
  return correct                                                  ;
}

bool N::PickView::LoadFirst(void)
{
  UUIDs         U                                                         ;
  GroupItems    GI ( plan        )                                        ;
  SqlConnection SC ( plan -> sql )                                        ;
  qint64        total = 0                                                 ;
  /////////////////////////////////////////////////////////////////////////
  int t1 = GroupType ( 1 )                                                ;
  int t2 = GroupType ( 2 )                                                ;
  GI . AutoMap    = true                                                  ;
  GI . GroupTable = GI . LookTable                                        (
                      t1                                                  ,
                      t2                                                  ,
                      RelationType ( )                                  ) ;
  /////////////////////////////////////////////////////////////////////////
  if ( SC . open ( FunctionString ) )                                     {
    int    column  = columnCount  ( )                                     ;
    int    lines   = lineCount    ( )                                     ;
    int    page    = column * lines                                       ;
    qint64 startId = Value ( "StartId" ) . toLongLong ( )                 ;
    ///////////////////////////////////////////////////////////////////////
    total = GI . Count                                                    (
              SC                                                          ,
              FirstUuid ( )                                               ,
              t1                                                          ,
              t2                                                          ,
              RelationType ( )                                          ) ;
    ///////////////////////////////////////////////////////////////////////
    U = GI . Subordination                                                (
          SC                                                              ,
          FirstUuid ( )                                                   ,
          t1                                                              ,
          t2                                                              ,
          RelationType ( )                                                ,
          SC . OrderBy ( SortItem ( ) , (Qt::SortOrder) SortingType ( ) ) ,
          SC . sql . Limit ( startId , page )                           ) ;
    ///////////////////////////////////////////////////////////////////////
    SC . close  ( )                                                       ;
  }                                                                       ;
  SC   . remove ( )                                                       ;
  /////////////////////////////////////////////////////////////////////////
  LockAt      ( "Operate"       )                                         ;
  RealItems = U                                                           ;
  setVariable ( "Items" , total )                                         ;
  UnlockAt    ( "Operate"       )                                         ;
  /////////////////////////////////////////////////////////////////////////
  return true                                                             ;
}

bool N::PickView::LoadSecond(void)
{
  UUIDs         U                                                         ;
  GroupItems    GI ( plan        )                                        ;
  SqlConnection SC ( plan -> sql )                                        ;
  qint64        total = 0                                                 ;
  /////////////////////////////////////////////////////////////////////////
  int t1 = GroupType ( 1 )                                                ;
  int t2 = GroupType ( 2 )                                                ;
  GI . AutoMap    = true                                                  ;
  GI . GroupTable = GI . LookTable                                        (
                      t1                                                  ,
                      t2                                                  ,
                      RelationType ( )                                  ) ;
  /////////////////////////////////////////////////////////////////////////
  if ( SC . open ( FunctionString ) )                                     {
    int    column  = columnCount  ( )                                     ;
    int    lines   = lineCount    ( )                                     ;
    int    page    = column * lines                                       ;
    qint64 startId = Value ( "StartId" ) . toLongLong ( )                 ;
    ///////////////////////////////////////////////////////////////////////
    total = GI . Count                                                    (
              SC                                                          ,
              SecondUuid ( )                                              ,
              t1                                                          ,
              t2                                                          ,
              RelationType ( )                                          ) ;
    ///////////////////////////////////////////////////////////////////////
    U = GI . GetOwners                                                    (
          SC                                                              ,
          SecondUuid ( )                                                  ,
          t1                                                              ,
          t2                                                              ,
          RelationType ( )                                                ,
          SC . OrderBy ( SortItem ( ) , (Qt::SortOrder) SortingType ( ) ) ,
          SC . sql . Limit ( startId , page )                           ) ;
    ///////////////////////////////////////////////////////////////////////
    SC . close  ( )                                                       ;
  }                                                                       ;
  SC   . remove ( )                                                       ;
  /////////////////////////////////////////////////////////////////////////
  LockAt      ( "Operate"       )                                         ;
  RealItems = U                                                           ;
  setVariable ( "Items" , total )                                         ;
  UnlockAt    ( "Operate"       )                                         ;
  /////////////////////////////////////////////////////////////////////////
  return true                                                             ;
}

void N::PickView::AppendDivision(VarArgs & args)
{
  QString name = Value ( "NewGroup" ) . toString ( )    ;
  SUID    u    = AppendGroup ( name , GroupType ( 3 ) ) ;
  if ( u <= 0 )                                         {
    Alert ( Error )                                     ;
    return                                              ;
  }                                                     ;
  start ( 10011 )                                       ;
}

void N::PickView::RenameItem(VarArgs & args)
{
  if ( args . count ( ) < 3  ) return         ;
  /////////////////////////////////////////////
  SUID    nuid = args [ 0 ] . toULongLong ( ) ;
  SUID    uuid = args [ 1 ] . toULongLong ( ) ;
  QString name = args [ 2 ] . toString    ( ) ;
  /////////////////////////////////////////////
  if ( nuid              <= 0 ) return        ;
  if ( uuid              <= 0 ) return        ;
  if ( name . length ( ) <= 0 ) return        ;
  /////////////////////////////////////////////
  SelectItem * item                           ;
  TextObject * text                           ;
  item = ChildOf<SelectItem> ( nuid )         ;
  if ( IsNull ( item )        ) return        ;
  text = TextItem ( item )                    ;
  if ( IsNull ( text )        ) return        ;
  /////////////////////////////////////////////
  bool correct = false                        ;
  SqlConnection SC ( plan -> sql )            ;
  if ( SC.open(FunctionString))               {
    int language = plan -> LanguageId         ;
    if ( Variables . contains("LanguageId") ) {
      language = Value("LanguageId").toInt () ;
    }                                         ;
    correct = SC . assureName                 (
      PlanTable(Names)                        ,
      uuid                                    ,
      language                                ,
      name                                  ) ;
    SC . close  ( )                           ;
  }                                           ;
  SC   . remove ( )                           ;
  /////////////////////////////////////////////
  if ( correct )                              {
    text -> setText ( name )                  ;
  } else                                      {
    Alert ( Error )                           ;
  }                                           ;
}

void N::PickView::CountGroups(VarArgs & args)
{
  SqlConnection  SC ( plan -> sql            )                  ;
  ///////////////////////////////////////////////////////////////
  if ( SC . open ( FunctionString ) )                           {
    UUIDs              U     = CurrentItems                     ;
    SUID               u     = 0                                ;
    qint64             index = 0                                ;
    qint64             z     = 0                                ;
    ProgressButton   * pb    = ProgressTool         ( )         ;
    ProgressReporter * pr    = new ProgressReporter ( )         ;
    bool               drop  = false                            ;
    /////////////////////////////////////////////////////////////
    pr -> total = U . count ( )                                 ;
    pr -> value = 0                                             ;
    pr -> setRunning  ( true )                                  ;
    pr -> Using       (      )                                  ;
    pb -> setReporter ( pr   )                                  ;
    ArrangePads       (      )                                  ;
    /////////////////////////////////////////////////////////////
    while ( ! drop )                                            {
      if ( 0 == pr -> state ) drop = true                       ;
      if ( ! drop )                                             {
        if ( 2 == pr -> state )                                 {
          Time::msleep ( 10 )                                   ;
          continue                                              ;
        }                                                       ;
        pr -> state = 1                                         ;
      }                                                         ;
      if ( ! drop ) drop = ! ForUuid ( u , index , U )          ;
      if ( ! drop )                                             {
        SelectItem * item = ChildOf<SelectItem> ( u )           ;
        if ( NotNull ( item ) )                                 {
          IconObject * icon = IconItem ( item )                 ;
          if ( NotNull ( icon ) )                               {
            SUID g = item -> Value ( "UUID" ) . toULongLong ( ) ;
            if ( g >= 0 )                                       {
              qint64 total = CountMembers ( SC , g )            ;
              if ( total > 0 )                                  {
                QString mt                                      ;
                QString tt                                      ;
                mt = Value ( "ItemCount" ) . toString ( )       ;
                tt = QString(mt).arg(total)                     ;
                icon -> setVariable ( "ToolTip" , tt )          ;
              }                                                 ;
            }                                                   ;
          }                                                     ;
        }                                                       ;
        z ++                                                    ;
        pr -> value = z                                         ;
      }                                                         ;
    }                                                           ;
    /////////////////////////////////////////////////////////////
    pr -> Leaving                 (                           ) ;
    ClosePad                      ( pb -> node                ) ;
    /////////////////////////////////////////////////////////////
    SC . close                    (                           ) ;
  }                                                             ;
  SC   . remove                   (                           ) ;
  ///////////////////////////////////////////////////////////////
  Alert                           ( Done                      ) ;
}

void N::PickView::CountGroup(VarArgs & args)
{
  if ( args . count ( ) <  2 ) return         ;
  /////////////////////////////////////////////
  SUID nuid = args [ 0 ] . toULongLong ( )    ;
  SUID uuid = args [ 1 ] . toULongLong ( )    ;
  /////////////////////////////////////////////
  if ( nuid             <= 0 ) return         ;
  if ( uuid             <= 0 ) return         ;
  /////////////////////////////////////////////
  SelectItem * item                           ;
  IconObject * icon                           ;
  item = ChildOf<SelectItem> ( nuid )         ;
  if ( IsNull ( item )        ) return        ;
  icon = IconItem ( item )                    ;
  if ( IsNull ( icon )        ) return        ;
  /////////////////////////////////////////////
  bool   correct = false                      ;
  qint64 total   = 0                          ;
  SqlConnection SC ( plan -> sql )            ;
  if ( SC.open(FunctionString))               {
    total   = CountMembers ( SC , uuid )      ;
    correct = ( total > 0 )                   ;
    SC . close  ( )                           ;
  }                                           ;
  SC   . remove ( )                           ;
  /////////////////////////////////////////////
  if ( correct )                              {
    QString mt                                ;
    QString tt                                ;
    mt = Value ( "ItemCount" ) . toString ( ) ;
    tt = QString(mt).arg(total)               ;
    icon -> setVariable ( "ToolTip" , tt )    ;
    Alert ( Done  )                           ;
  } else                                      {
    Alert ( Error )                           ;
  }                                           ;
}

void N::PickView::DeleteGroup(VarArgs & args)
{
  if ( args . count ( ) <  2 ) return         ;
  /////////////////////////////////////////////
  SUID nuid = args [ 0 ] . toULongLong ( )    ;
  SUID uuid = args [ 1 ] . toULongLong ( )    ;
  /////////////////////////////////////////////
  if ( nuid             <= 0 ) return         ;
  if ( uuid             <= 0 ) return         ;
  /////////////////////////////////////////////
  SqlConnection SC ( plan -> sql )            ;
  if ( SC.open(FunctionString))               {
    QString Q                                 ;
    QString W                                 ;
    W = QString("where `first` = %1 or `second` = %2 ").arg(uuid).arg(uuid) ;
    Q = SC . sql . DeleteFrom                 (
          PlanTable(Groups)                   ,
          W                                 ) ;
    SC . Query ( Q )                          ;
    W = QString("where `uuid` = %1 ").arg(uuid) ;
    Q = SC . sql . DeleteFrom                 (
          PlanTable(Names)                    ,
          W                                 ) ;
    SC . Query ( Q )                          ;
    Q = SC . sql . DeleteFrom                 (
          PlanTable(Divisions)                ,
          W                                 ) ;
    SC . Query ( Q )                          ;
    Q = SC . sql . DeleteFrom                 (
          PlanTable(MajorUuid)                ,
          W                                 ) ;
    SC . Query ( Q )                          ;
    SC . close  ( )                           ;
  }                                           ;
  SC   . remove ( )                           ;
  /////////////////////////////////////////////
  start ( 10011 )                             ;
}

void N::PickView::DetachGroup(VarArgs & args)
{
  if ( args . count ( ) <  2 ) return      ;
  //////////////////////////////////////////
  SUID nuid = args [ 0 ] . toULongLong ( ) ;
  SUID uuid = args [ 1 ] . toULongLong ( ) ;
  //////////////////////////////////////////
  if ( nuid             <= 0 ) return      ;
  if ( uuid             <= 0 ) return      ;
  //////////////////////////////////////////
  SelectItem * item                        ;
  item = ChildOf<SelectItem> ( nuid )      ;
  if ( IsNull ( item )        ) return     ;
  //////////////////////////////////////////
//  QString M                                ;
//  QString nx                               ;
//  nx = item->Value("ItemName").toString()  ;
//  M  = tr("Detach [%1] pictures").arg(nx)  ;
//  plan -> showMessage ( M )                ;
#pragma message("showMessage to status bar is now having problem with thread")
  //////////////////////////////////////////
  bool correct = false                     ;
  SqlConnection SC ( plan -> sql )         ;
  if ( SC.open(FunctionString))            {
    correct = DetachMembers ( SC , uuid )  ;
    SC . close  ( )                        ;
  }                                        ;
  SC   . remove ( )                        ;
  //////////////////////////////////////////
  if ( correct )                           {
    Alert ( Done  )                        ;
  } else                                   {
    Alert ( Error )                        ;
  }                                        ;
}

void N::PickView::JoinGroup(VarArgs & args)
{
  UUIDs U                                       ;
  SUID  g = args . first ( ) . toULongLong ( )  ;
  if ( g <= 0 )                                 {
    Alert ( Error )                             ;
    return                                      ;
  }                                             ;
  args . takeFirst ( )                          ;
  for (int i = 0 ; i < args . count ( ) ; i++ ) {
    U << args [ i ] . toULongLong ( )           ;
  }                                             ;
  if ( U . count ( ) <= 0 )                     {
    Alert ( Error )                             ;
    return                                      ;
  }                                             ;
  ///////////////////////////////////////////////
  SqlConnection SC ( plan -> sql )              ;
  GroupItems    GI ( plan        )              ;
  if ( SC . open ( FunctionString )           ) {
    GI . AutoMap    = true                      ;
    GI . GroupTable = GI . LookTable            (
                   GroupType    ( 4 )           ,
                   GroupType    ( 5 )           ,
                   RelationType (   )         ) ;
    GI . Join                                   (
      SC                                        ,
      g                                         ,
      GroupType    ( 4 )                        ,
      GroupType    ( 5 )                        ,
      RelationType (   )                        ,
      0                                         ,
      U                                         ,
      true                                    ) ;
    SC . close   ( )                            ;
  }                                             ;
  SC   . remove  ( )                            ;
  ///////////////////////////////////////////////
  Alert ( Done )                                ;
}

void N::PickView::MoveGroup(VarArgs & args)
{
  UUIDs U                                       ;
  SUID  g = args . first ( ) . toULongLong ( )  ;
  args . takeFirst ( )                          ;
  for (int i = 0 ; i < args . count ( ) ; i++ ) {
    U << args [ i ] . toULongLong ( )           ;
  }                                             ;
  if ( U . count ( ) <= 0 )                     {
    Alert ( Error )                             ;
    return                                      ;
  }                                             ;
  Alert ( Action )                              ;
  ///////////////////////////////////////////////
  UUIDs X = RealItems                           ;
  UUIDs J                                       ;
  RMAPs P                                       ;
  SUID  u                                       ;
  foreach ( u , U )                             {
    if ( X . contains ( u ) )                   {
      int ix = X . indexOf ( u )                ;
      if ( ix >= 0 )                            {
        X . takeAt ( ix )                       ;
        J << u                                  ;
      }                                         ;
    }                                           ;
  }                                             ;
  ///////////////////////////////////////////////
  if ( g <= 0 )                                 {
    X << J                                      ;
  } else                                        {
    int ix = X . indexOf ( g )                  ;
    if ( ix < 0 ) ix = 0                        ;
    foreach ( u , J )                           {
      X . insert ( ix , u )                     ;
      ix ++                                     ;
    }                                           ;
  }                                             ;
  ///////////////////////////////////////////////
  if ( Qt::AscendingOrder  == SortingType ( ) ) {
    int Id = 1                                  ;
    foreach ( u , X )                           {
      P [ u ] = Id                              ;
      Id ++                                     ;
    }                                           ;
  } else
  if ( Qt::DescendingOrder == SortingType ( ) ) {
    int Id = X .count ( )                       ;
    foreach ( u , X )                           {
      P [ u ] = Id                              ;
      Id --                                     ;
    }                                           ;
  }                                             ;
  ///////////////////////////////////////////////
  SqlConnection SC ( plan -> sql )              ;
  if ( SC . open ( FunctionString )           ) {
    QString Q                                   ;
    foreach ( u , X )                           {
      int Id = P [ u ]                          ;
      Q = SC . sql . Update                     (
            PlanTable ( Divisions )             ,
            SC . WhereUuid ( u )                ,
            1                                   ,
            "position"                        ) ;
      SC . Prepare ( Q               )          ;
      SC . Bind    ( "position" , Id )          ;
      SC . Exec    (                 )          ;
    }                                           ;
    SC . close   ( )                            ;
  }                                             ;
  SC   . remove  ( )                            ;
  ///////////////////////////////////////////////
  RealItems = X                                 ;
  J . clear ( )                                 ;
  foreach ( u , X ) J << RealToView [ u ]       ;
  LockAt   ( "Operate" )                        ;
  CurrentItems = J                              ;
  UnlockAt ( "Operate" )                        ;
  ///////////////////////////////////////////////
  BoxChanged ( true )                           ;
  Alert      ( Done )                           ;
}

void N::PickView::AssignIcon(VarArgs & args)
{
  UUIDs U                                       ;
  SUID  g = args . first ( ) . toULongLong ( )  ;
  if ( g <= 0 )                                 {
    Alert ( Error )                             ;
    return                                      ;
  }                                             ;
  args . takeFirst ( )                          ;
  for (int i = 0 ; i < args . count ( ) ; i++ ) {
    U << args [ i ] . toULongLong ( )           ;
  }                                             ;
  if ( U . count ( ) <= 0 )                     {
    Alert ( Error )                             ;
    return                                      ;
  }                                             ;
  ///////////////////////////////////////////////
  SqlConnection SC ( plan->sql )                ;
  GroupItems    GI ( plan      )                ;
  if ( SC . open ( FunctionString ) )           {
    GI . AutoMap    = true                      ;
    GI . GroupTable = GI . LookTable            (
                        GroupType ( 4 )         ,
                        Types  :: Picture       ,
                        Groups :: Icon        ) ;
    GI . Delegate                               (
      SC                                        ,
      g                                         ,
      U [ 0 ]                                   ,
      GroupType ( 4 )                           ,
      Types  :: Picture                         ,
      Groups :: Icon                          ) ;
    SC.close  (                               ) ;
  }                                             ;
  SC . remove (                               ) ;
  ///////////////////////////////////////////////
  BoxChanged ( true )                           ;
  Alert      ( Done )                           ;
}

qint64 N::PickView::CountMembers(SqlConnection & SC,SUID u)
{
  GroupItems GI ( plan )           ;
  GI . AutoMap    = true           ;
  GI . GroupTable = GI . LookTable (
           GroupType    ( 4 )      ,
           GroupType    ( 5 )      ,
           RelationType (   )    ) ;
  return GI . Count                (
           SC                      ,
           u                       ,
           GroupType    ( 4 )      ,
           GroupType    ( 5 )      ,
           RelationType (   )    ) ;
}

bool N::PickView::DetachMembers(SqlConnection & SC,SUID u)
{
  GroupItems GI ( plan )                   ;
  GI . AutoMap    = true                   ;
  GI . GroupTable = GI . LookTable         (
                      GroupType    ( 4 )   ,
                      GroupType    ( 5 )   ,
                      RelationType (   ) ) ;
  GI . FullDetach                          (
                      SC                   ,
                      u                    ,
                      GroupType    ( 4 )   ,
                      GroupType    ( 5 )   ,
                      RelationType (   ) ) ;
  return true                              ;
}

bool N::PickView::ReportDetails(void)
{
  return false ;
}

void N::PickView::MountName(MenuManager & mm,SelectItem * item)
{
  if ( IsNull ( item ) ) return                      ;
  QString name                                       ;
  name = item -> Value ( "ItemName" ) . toString ( ) ;
  mm   . add          ( 0 , name )                   ;
  mm   . addSeparator (          )                   ;
}

void N::PickView::MountUuid(MenuManager & mm,QMenu * me,SelectItem * item)
{
  if ( IsNull ( item ) ) return                      ;
  if ( IsNull ( me   ) ) return                      ;
  SUID        puid = 0                               ;
  puid  = item -> Value ( "UUID" ) . toULongLong ( ) ;
  if ( puid <= 0       ) return                      ;
  QLineEdit * utem = NULL                            ;
  QString     s                                      ;
  utem  = new QLineEdit ( )                          ;
  s     = QString::number(puid)                      ;
  utem -> setText      ( s                         ) ;
  mm    . add          ( me , 1 , utem             ) ;
}

void N::PickView::NameEditor(MenuManager & mm,QMenu * menu,SelectItem * item,QLineEdit ** line)
{
  if ( IsNull ( item ) ) return                                     ;
  QString itemName = item -> Value ( "ItemName" ) . toString    ( ) ;
  SUID    itemUuid = item -> Value ( "UUID"     ) . toULongLong ( ) ;
  QString uuidName = QString::number(itemUuid)                      ;
  ///////////////////////////////////////////////////////////////////
  (*line)  = new QLineEdit (                                 )      ;
  (*line) -> setText       ( itemName                        )      ;
  (*line) -> setToolTip    ( tr("Name for %1").arg(uuidName) )      ;
  ///////////////////////////////////////////////////////////////////
  if ( IsNull ( menu ) )                                            {
    mm . add          (        0 , (*line) )                        ;
  } else                                                            {
    mm . add          ( menu , 0 , (*line) )                        ;
  }                                                                 ;
}

void N::PickView::ModifyName(SelectItem * item,QLineEdit * line)
{
  if ( IsNull ( item ) ) return                                     ;
  if ( IsNull ( line ) ) return                                     ;
  ///////////////////////////////////////////////////////////////////
  QString newName  = line -> text  (            )                   ;
  QString itemName = item -> Value ( "ItemName" ) . toString    ( ) ;
  ///////////////////////////////////////////////////////////////////
  if ( newName == itemName ) return                                 ;
  ///////////////////////////////////////////////////////////////////
  SUID    itemUuid = item -> Value ( "UUID"     ) . toULongLong ( ) ;
  VarArgs args                                                      ;
  ///////////////////////////////////////////////////////////////////
  args << item -> node                                              ;
  args << itemUuid                                                  ;
  args << newName                                                   ;
  start ( 73002 , args )                                            ;
}

bool N::PickView::prepare(void)
{
  Finish      (                                  ) ;
  Dual        (                                  ) ;
  addSequence (  7 , SceneObject::RenderChildren ) ;
  addSequence ( 11 , SceneObject::RenderBox      ) ;
  addSequence ( 15 , SceneObject::RenderFaces    ) ;
  return true                                      ;
}

bool N::PickView::startup(void)
{
  start ( 10011 ) ;
  return true     ;
}

int N::PickView::CallBinding(SceneObject * s,QVariants & args)
{
  if ( ! userobjs . contains ( "BindFunctions" ) ) return 0 ;
  PrivateVideoFunctionBinder * pvfb                         ;
  pvfb = new PrivateVideoFunctionBinder ( )                 ;
  ::memcpy ( pvfb                                           ,
             userobjs [ "BindFunctions" ]                   ,
             sizeof(PrivateVideoFunctionBinder)           ) ;
  pvfb -> mounting = s                                      ;
  int r = CallDynamic ( "BindFunctions"                     ,
                        this                                ,
                        pvfb                                ,
                        args                              ) ;
  delete pvfb                                               ;
  return r                                                  ;
}

void N::PickView::PushCommand(int cmd,QString n,SUID u)
{
  VarArgs args                             ;
  args << cmd                              ;
  args << u                                ;
  args << n                                ;
  args << GroupType    ( 2 )               ;
  args << GroupType    ( 3 )               ;
  args << GroupType    ( 4 )               ;
  args << GroupType    ( 5 )               ;
  args << FirstUuid    (   )               ;
  args << SecondUuid   (   )               ;
  args << RelationType (   )               ;
  args << node                             ;
  args << UniverseUuid (   )               ;
  args << Value ( "Suicide" ) . toBool ( ) ;
  CallRequests ( args )                    ;
}

void N::PickView::PushCommand(int cmd,SelectItem * item)
{
  if ( IsNull ( item ) ) return                              ;
  QString n = item -> Value ( "ItemName" ) . toString    ( ) ;
  SUID    g = item -> Value ( "UUID"     ) . toULongLong ( ) ;
  PushCommand ( cmd , n , g )                                ;
}

#endif
