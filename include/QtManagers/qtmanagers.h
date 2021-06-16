/****************************************************************************
 *                                                                          *
 * Copyright (C) 2001 ~ 2016 Neutrino International Inc.                    *
 *                                                                          *
 * Author : Brian Lin <lin.foxman@gmail.com>, Skype: wolfram_lin            *
 *                                                                          *
 ****************************************************************************/

#ifndef QT_MANAGERS_H
#define QT_MANAGERS_H

#include <QtComponents>
#include <QtWorld>

QT_BEGIN_NAMESPACE

#ifndef QT_STATIC
#    if defined(QT_BUILD_QTMANAGERS_LIB)
#      define Q_MANAGERS_EXPORT Q_DECL_EXPORT
#    else
#      define Q_MANAGERS_EXPORT Q_DECL_IMPORT
#    endif
#else
#    define Q_MANAGERS_EXPORT
#endif

namespace Ui                               {
class Q_MANAGERS_EXPORT nTimeScope         ;
class Q_MANAGERS_EXPORT nTextReplacement   ;
class Q_MANAGERS_EXPORT nTextProperties    ;
class Q_MANAGERS_EXPORT nContourController ;
class Q_MANAGERS_EXPORT nDataToCpp         ;
class Q_MANAGERS_EXPORT nBlobToCpp         ;
}

namespace N
{

class Q_MANAGERS_EXPORT RetrieveMembers    ;
class Q_MANAGERS_EXPORT RetrieveVariable   ;
class Q_MANAGERS_EXPORT RetrieveSexuality  ;
class Q_MANAGERS_EXPORT RetrieveCalendar   ;
class Q_MANAGERS_EXPORT RetrievePeriod     ;
class Q_MANAGERS_EXPORT FaceParaments      ;
class Q_MANAGERS_EXPORT HistogramBuilder   ;
class Q_MANAGERS_EXPORT PaletteBuilder     ;
class Q_MANAGERS_EXPORT PictureRecognizer  ;
class Q_MANAGERS_EXPORT ScreenCapturer     ;
class Q_MANAGERS_EXPORT ThumbBuilder       ;
class Q_MANAGERS_EXPORT PicturePlugin      ;
class Q_MANAGERS_EXPORT PictureManager     ;
class Q_MANAGERS_EXPORT PeopleManager      ;
class Q_MANAGERS_EXPORT TagsManager        ;
class Q_MANAGERS_EXPORT LearnSite          ;
class Q_MANAGERS_EXPORT CommonSeeker       ;
class Q_MANAGERS_EXPORT DataEntries        ;
class Q_MANAGERS_EXPORT EquipmentList      ;
class Q_MANAGERS_EXPORT FileBrowser        ;
class Q_MANAGERS_EXPORT ItemEditor         ;
class Q_MANAGERS_EXPORT ItemNameEditor     ;
class Q_MANAGERS_EXPORT ItemNames          ;
class Q_MANAGERS_EXPORT NamesEditor        ;
class Q_MANAGERS_EXPORT NameComplex        ;
class Q_MANAGERS_EXPORT RealityLists       ;
class Q_MANAGERS_EXPORT SnapView           ;
class Q_MANAGERS_EXPORT GroupView          ;
class Q_MANAGERS_EXPORT GroupList          ;
class Q_MANAGERS_EXPORT SubgroupLists      ;
class Q_MANAGERS_EXPORT SubgroupPictures   ;
class Q_MANAGERS_EXPORT TimeScope          ;
class Q_MANAGERS_EXPORT TimeAlignment      ;
class Q_MANAGERS_EXPORT MemberLists        ;
class Q_MANAGERS_EXPORT FontLists          ;
class Q_MANAGERS_EXPORT PenLists           ;
class Q_MANAGERS_EXPORT BrushLists         ;
class Q_MANAGERS_EXPORT GradientLists      ;
class Q_MANAGERS_EXPORT PainterLists       ;
class Q_MANAGERS_EXPORT TextCursor         ;
class Q_MANAGERS_EXPORT TextRelation       ;
class Q_MANAGERS_EXPORT TextSection        ;
class Q_MANAGERS_EXPORT TextBlock          ;
class Q_MANAGERS_EXPORT TextDocument       ;
class Q_MANAGERS_EXPORT TextReplacement    ;
class Q_MANAGERS_EXPORT TextProperties     ;
class Q_MANAGERS_EXPORT TeXPaintDevice     ;
class Q_MANAGERS_EXPORT TeXPaintEngine     ;
class Q_MANAGERS_EXPORT AudioLists         ;
class Q_MANAGERS_EXPORT ContourController  ;
class Q_MANAGERS_EXPORT SceneManager       ;
class Q_MANAGERS_EXPORT SceneCreator       ;
class Q_MANAGERS_EXPORT DataArray          ;
class Q_MANAGERS_EXPORT DataCharCommand    ;
class Q_MANAGERS_EXPORT DataArrayCommand   ;
class Q_MANAGERS_EXPORT BlobToCpp          ;
class Q_MANAGERS_EXPORT DataToCpp          ;
class Q_MANAGERS_EXPORT HexWidget          ;
class Q_MANAGERS_EXPORT HexEditor          ;
class Q_MANAGERS_EXPORT Execution          ;
class Q_MANAGERS_EXPORT PickView           ;
class Q_MANAGERS_EXPORT ViewBox            ;
class Q_MANAGERS_EXPORT ViewShelf          ;

typedef QList < FaceParaments > Phiz       ;

typedef SceneObject * ObjectCreator   (void * conf,QObject * parent) ;
typedef Universe    * UniverseCreator (void * conf,QObject * parent) ;

Q_MANAGERS_EXPORT void             ListGMT        (QComboBox * gmt) ;
Q_MANAGERS_EXPORT void             SetGMT         (QComboBox * gmt,int TimeZone) ;
Q_MANAGERS_EXPORT int              localGMT       (void) ;
Q_MANAGERS_EXPORT ItemNameEditor * EditLanguages  (QWidget * parent,Plan & plan) ;
Q_MANAGERS_EXPORT TreeWidgetItems  List           (Languages   & languages       ,
                                                   QTreeWidget * Tree            ,
                                                   int           column          ,
                                                   bool          processEvents ) ;
Q_MANAGERS_EXPORT NAMEs            LoadNames      (PurePlan    & plan       ,
                                                   UUIDs       & Uuids      ,
                                                   int           language ) ;
Q_MANAGERS_EXPORT QStringList      EventTypeNames (void) ;

namespace Plugins
{

Q_MANAGERS_EXPORT QList<SqlPlugin      *> LoadSqlPlugins (QDir & dir) ;
Q_MANAGERS_EXPORT QList<HunterPlugin   *> LoadHunters    (QDir & dir) ;
Q_MANAGERS_EXPORT QList<DocumentPlugin *> LoadDocuments  (QDir & dir) ;
Q_MANAGERS_EXPORT QList<PicturePlugin  *> LoadPictures   (QDir & dir) ;

Q_MANAGERS_EXPORT bool LoadSqls      (QDir                                    & dir       ,
                                      QList<SqlPlugin                      *> & SQLs      ,
                                      QMap <SqlPlugin *,QPluginLoader      *> & Loaders ) ;
Q_MANAGERS_EXPORT bool LoadHunters   (QDir                                    & dir       ,
                                      QList<HunterPlugin                   *> & Hunters   ,
                                      QMap <HunterPlugin   *,QPluginLoader *> & Loaders ) ;
Q_MANAGERS_EXPORT bool LoadDocuments (QDir                                    & dir       ,
                                      QList<DocumentPlugin                 *> & Documents ,
                                      QMap <DocumentPlugin *,QPluginLoader *> & Loaders ) ;
Q_MANAGERS_EXPORT bool LoadPictures  (QDir                                    & dir       ,
                                      QList<PicturePlugin                  *> & Pictures  ,
                                      QMap <PicturePlugin  *,QPluginLoader *> & Loaders ) ;

}

namespace Images
{

Q_MANAGERS_EXPORT QStringList ExtensionFilters (void) ;
Q_MANAGERS_EXPORT QString     ImageFilters     (void) ;

}

/*****************************************************************************
 *                                                                           *
 *                                 Retrievers                                *
 *                                                                           *
 *****************************************************************************/

class Q_MANAGERS_EXPORT RetrieveMembers : public DataRetriever
{
  public:

    QString GroupTable ;

    explicit RetrieveMembers       (void) ;
    explicit RetrieveMembers       (QObject * owner,Plan * p) ;
    virtual ~RetrieveMembers       (void) ;

    virtual int          Type      (void) const ;

    virtual QValidator * Validator (void) ;

    virtual bool         Obtain    (SqlConnection    & Connection  ,
                                    SUID               uuid        ,
                                    QString          & string    ) ;
    virtual bool         Obtain    (SqlConnection    & Connection  ,
                                    SUID               uuid        ,
                                    QTreeWidgetItem  * item      ) ;
    virtual bool         Obtain    (SqlConnection    & Connection  ,
                                    SUID               uuid        ,
                                    QTableWidgetItem * item      ) ;
    virtual bool         Obtain    (SqlConnection    & Connection  ,
                                    SUID               uuid        ,
                                    QListWidgetItem  * item      ) ;

    virtual QWidget    * Editor    (SqlConnection    & Connection  ,
                                    SUID               uuid        ,
                                    QString          & string    ) ;
    virtual QWidget    * Editor    (SqlConnection    & Connection  ,
                                    SUID               uuid        ,
                                    QTreeWidgetItem  * item      ) ;
    virtual QWidget    * Editor    (SqlConnection    & Connection  ,
                                    SUID               uuid        ,
                                    QTableWidgetItem * item      ) ;
    virtual QWidget    * Editor    (SqlConnection    & Connection  ,
                                    SUID               uuid        ,
                                    QListWidgetItem  * item      ) ;

    virtual void         setFirst  (SUID first ,int t1,int t2,int relation) ;
    virtual void         setSecond (SUID second,int t1,int t2,int relation) ;

  protected:

    virtual int          Counting  (SqlConnection    & Connection  ,
                                    SUID               uuid      ) ;

  private:


} ;

class Q_MANAGERS_EXPORT RetrieveVariable : public DataRetriever
{
  public:

    explicit RetrieveVariable      (void) ;
    explicit RetrieveVariable      (QObject * owner,Plan * p) ;
    virtual ~RetrieveVariable      (void) ;

    virtual int          Type      (void) const ;

    virtual QValidator * Validator (void) ;

    virtual bool         Obtain    (SqlConnection    & Connection  ,
                                    SUID               uuid        ,
                                    QString          & string    ) ;
    virtual bool         Obtain    (SqlConnection    & Connection  ,
                                    SUID               uuid        ,
                                    QTreeWidgetItem  * item      ) ;
    virtual bool         Obtain    (SqlConnection    & Connection  ,
                                    SUID               uuid        ,
                                    QTableWidgetItem * item      ) ;
    virtual bool         Obtain    (SqlConnection    & Connection  ,
                                    SUID               uuid        ,
                                    QListWidgetItem  * item      ) ;

    virtual QWidget    * Editor    (SqlConnection    & Connection  ,
                                    SUID               uuid        ,
                                    QString          & string    ) ;
    virtual QWidget    * Editor    (SqlConnection    & Connection  ,
                                    SUID               uuid        ,
                                    QTreeWidgetItem  * item      ) ;
    virtual QWidget    * Editor    (SqlConnection    & Connection  ,
                                    SUID               uuid        ,
                                    QTableWidgetItem * item      ) ;
    virtual QWidget    * Editor    (SqlConnection    & Connection  ,
                                    SUID               uuid        ,
                                    QListWidgetItem  * item      ) ;

  protected:

  private:

} ;

class Q_MANAGERS_EXPORT RetrieveSexuality : public DataRetriever
{
  public:

    QString GroupTable ;

    explicit RetrieveSexuality     (void) ;
    explicit RetrieveSexuality     (QObject * owner,Plan * p) ;
    virtual ~RetrieveSexuality     (void) ;

    virtual int          Type      (void) const ;

    virtual QValidator * Validator (void) ;

    virtual bool         Obtain    (SqlConnection    & Connection  ,
                                    SUID               uuid        ,
                                    QString          & string    ) ;
    virtual bool         Obtain    (SqlConnection    & Connection  ,
                                    SUID               uuid        ,
                                    QTreeWidgetItem  * item      ) ;
    virtual bool         Obtain    (SqlConnection    & Connection  ,
                                    SUID               uuid        ,
                                    QTableWidgetItem * item      ) ;
    virtual bool         Obtain    (SqlConnection    & Connection  ,
                                    SUID               uuid        ,
                                    QListWidgetItem  * item      ) ;

    virtual QWidget    * Editor    (SqlConnection    & Connection  ,
                                    SUID               uuid        ,
                                    QString          & string    ) ;
    virtual QWidget    * Editor    (SqlConnection    & Connection  ,
                                    SUID               uuid        ,
                                    QTreeWidgetItem  * item      ) ;
    virtual QWidget    * Editor    (SqlConnection    & Connection  ,
                                    SUID               uuid        ,
                                    QTableWidgetItem * item      ) ;
    virtual QWidget    * Editor    (SqlConnection    & Connection  ,
                                    SUID               uuid        ,
                                    QListWidgetItem  * item      ) ;

  protected:

    NAMEs Sexualities ;

    bool ObtainList                (SqlConnection    & Connection) ;
    bool ObtainSexuality           (SqlConnection    & Connection  ,
                                    SUID               uuid        ,
                                    QString          & name      ) ;

  private:

} ;

class Q_MANAGERS_EXPORT RetrieveCalendar : public DataRetriever
{
  public:

    QString GroupTable  ;
    int     StartType   ;
    int     EndType     ;

    explicit RetrieveCalendar      (void) ;
    explicit RetrieveCalendar      (QObject * owner,Plan * p) ;
    virtual ~RetrieveCalendar      (void) ;

    virtual int          Type      (void) const ;

    virtual QValidator * Validator (void) ;

    virtual void         setFirst  (SUID first ,int t1,int t2,int relation) ;
    virtual void         setSecond (SUID second,int t1,int t2,int relation) ;

    virtual bool         Obtain    (SqlConnection    & Connection  ,
                                    SUID               uuid        ,
                                    QString          & string    ) ;
    virtual bool         Obtain    (SqlConnection    & Connection  ,
                                    SUID               uuid        ,
                                    QTreeWidgetItem  * item      ) ;
    virtual bool         Obtain    (SqlConnection    & Connection  ,
                                    SUID               uuid        ,
                                    QTableWidgetItem * item      ) ;
    virtual bool         Obtain    (SqlConnection    & Connection  ,
                                    SUID               uuid        ,
                                    QListWidgetItem  * item      ) ;

    virtual QWidget    * Editor    (SqlConnection    & Connection  ,
                                    SUID               uuid        ,
                                    QString          & string    ) ;
    virtual QWidget    * Editor    (SqlConnection    & Connection  ,
                                    SUID               uuid        ,
                                    QTreeWidgetItem  * item      ) ;
    virtual QWidget    * Editor    (SqlConnection    & Connection  ,
                                    SUID               uuid        ,
                                    QTableWidgetItem * item      ) ;
    virtual QWidget    * Editor    (SqlConnection    & Connection  ,
                                    SUID               uuid        ,
                                    QListWidgetItem  * item      ) ;

  protected:

    TUID TimeZone ;

    virtual bool setTimeString     (TUID             tuid          ,
                                    QString          & string    ) ;
    virtual bool ObtainCalendar    (SqlConnection    & Connection  ,
                                    SUID               uuid        ,
                                    QString          & string    ) ;

  private:

} ;

class Q_MANAGERS_EXPORT RetrievePeriod : public RetrieveCalendar
{
  public:

    explicit RetrievePeriod        (void) ;
    explicit RetrievePeriod        (QObject * owner,Plan * p) ;
    virtual ~RetrievePeriod        (void) ;

    virtual int          Type      (void) const ;

    virtual QValidator * Validator (void) ;

  protected:

    virtual bool setTimeString     (TUID      stid     ,
                                    TUID      edid     ,
                                    QString & string ) ;
    virtual bool ObtainCalendar    (SqlConnection    & Connection  ,
                                    SUID               uuid        ,
                                    QString          & string    ) ;

  private:

} ;

/*****************************************************************************
 *                                                                           *
 *                            Facial Recognizer                              *
 *                                                                           *
 *****************************************************************************/

class Q_MANAGERS_EXPORT FaceParaments
{
  public:

    QRect            Face      ;
    QList<QRect>     LeftEye   ;
    QList<QRect>     RightEye  ;
    QList<QRect>     Mouth     ;
    QList<QRect>     Noses     ;
    QList<QRect>     LeftEar   ;
    QList<QRect>     RightEar  ;
    QList<QRect>     Glass     ;
    QList<QRect>     Body      ;
    QList<QRect>     UpperBody ;
    QList<QRect>     LowerBody ;
    QMap<int,QPoint> Points    ;

    explicit FaceParaments (void) ;
    virtual ~FaceParaments (void) ;

    bool   isIntact        (void) ;
    bool   isPartial       (void) ;
    double Possibility     (void) ;
    double Angle           (int rightEyeIndex = 0,int leftEyeIndex = 0) ;

  protected:

  private:

} ;

/*****************************************************************************
 *                                                                           *
 *                             Picture Manipulation                          *
 *                                                                           *
 *****************************************************************************/

class Q_MANAGERS_EXPORT PicturePlugin
{
  public:

    explicit  PicturePlugin         (void) { ; }
    virtual ~ PicturePlugin         (void) { ; }

    virtual QString   name          (void                        ) const = 0 ;
    virtual QString   identifier    (void                        ) const = 0 ;
    virtual QString   catalog       (void                        ) const = 0 ;
    virtual bool      hasGui        (void                        ) const = 0 ;
    virtual bool      canDock       (void                        ) const = 0 ;
    virtual bool      available     (void                        )       = 0 ;
    virtual int       instances     (void                        )       = 0 ;
    virtual bool      initialize    (Plan * plan                 )       = 0 ;
    virtual QObject * create        (QWidget * parent,Plan * plan)       = 0 ;
    virtual bool      docking       (QMainWindow * M,QObject * O )       = 0 ;
    virtual bool      startup       (QObject     * object        )       = 0 ;

} ;

class Q_MANAGERS_EXPORT PictureManager
{
  public:

    Plan      * PicturePlan ;
    QDir        Directory   ;
    QString     Filters     ;
    QStringList Extensions  ;

    explicit   PictureManager     (Plan * plan = NULL) ;
    virtual   ~PictureManager     (void);

    int        Count              (SqlConnection & Connection) ;
    int        Count              (SqlConnection & Connection  ,
                                   SUID            uuid        ,
                                   int             type      ) ;
    int        Count              (SqlConnection & Connection  ,
                                   SUID            uuid        ,
                                   int             type        ,
                                   int             relation  ) ;

    QString    SafeTable          (QString name) ;
    QString    PictureTable       (QString name) ;
    QString    ThumbTable         (QString name) ;
    QString    HistogramTable     (QString name) ;

    bool       Equal              (QImage & image   ,
                                   QSize    size  ) ;
    bool       Smaller            (QImage & image   ,
                                   QSize    size  ) ;

    int        Checksum           (QByteArray & data) ;

    QByteArray toColors           (QImage & Image) ;

    void       ColorImage         (QImage & image   ,
                                   QColor   color ) ;
    void       ColorIcon          (QIcon  & icon    ,
                                   QSize    size    ,
                                   QColor   color ) ;

    QIcon      Icon               (QImage * image   ,
                                   QSize    size  ) ;
    QImage     Icon               (QImage & image   ,
                                   QSize    size  ) ;
    bool       Icon               (QImage & image   ,
                                   QSize    size    ,
                                   QImage & target) ;

    QByteArray IntoPNG            (QImage     & image) ;
    bool       IntoPNG            (QImage     & image  ,
                                   QByteArray & PNG  ) ;

    QImage *   SafeImage          (QByteArray & Body       ,
                                   QByteArray & Format   ) ;
    QImage *   MimeImage          (const QMimeData * mime) ;

    QByteArray File               (QString      filename ) ;
    bool       File               (QString      filename   ,
                                   QByteArray & Data     ) ;

    bool       Dump               (QString      filename   ,
                                   QByteArray & Data     ) ;
    bool       Dump               (QDir       & directory  ,
                                   QString      filename   ,
                                   QByteArray & Data     ) ;

    bool       Import             (SqlConnection & Connection    ,
                                   QString         filename      ,
                                   SUID          & PictureUuid ) ;
    bool       Import             (SqlConnection & Connection    ,
                                   QString         filename      ,
                                   QByteArray    & Data          ,
                                   SUID          & PictureUuid ) ;

    bool       Import             (QString      filename    ) ;
    bool       Import             (QString      filename      ,
                                   SUID       & PictureUuid ) ;
    bool       Import             (QString      filename      ,
                                   QByteArray & Data        ) ;
    bool       Import             (QString      filename      ,
                                   QByteArray & Data          ,
                                   SUID       & PictureUuid ) ;
    bool       Import             (QImage     & image         ,
                                   SUID       & PictureUuid ) ;

    bool       PictureSuffix      (SqlConnection & Connection ,
                                   SUID            uuid       ,
                                   QString       & suffix   ) ;

    QString    Export             (SqlConnection & Connection ,
                                   QDir          & dir        ,
                                   SUID            uuid     ) ;
    bool       Export             (SqlConnection & Connection ,
                                   QString         filename   ,
                                   SUID            uuid     ) ;

    QSize      Geometry           (SqlConnection & Connection ,
                                   SUID            uuid     ) ;

    QImage *   Picture            (SUID uuid);
    QImage *   Picture            (SqlConnection & Connection ,
                                   SUID            uuid     ) ;
    QImage *   Thumb              (SUID uuid) ;
    QImage *   Thumb              (SqlConnection & Connection ,
                                   SUID            uuid     ) ;
    bool       AppendThumb        (SqlConnection & Connection ,
                                   SUID            uuid       ,
                                   QByteArray    & image      ,
                                   QByteArray    & thumb      ,
                                   QImage        & Original   ,
                                   QImage        & Small    ) ;
    bool       DeleteThumb        (SqlConnection & Connection ,
                                   SUID            uuid     ) ;
    bool       UpdateThumb        (SqlConnection & Connection ,
                                   SUID            uuid     ) ;

    SUID       Find               (SqlConnection & Connection   ,
                                   QByteArray    & image      ) ;
    int        ColorCount         (SqlConnection & Connection,SUID uuid) ;

    // OpenCV functions
    bool       Release            (IplImage * image) ;
    IplImage * toIplImage         (QImage   & image) ;
    IplImage * toIplImage         (QImage   & image,double angle) ;
    QImage   * toImage            (IplImage * image) ;
    IplImage * toGrey             (IplImage * image) ;
    IplImage * Smooth             (IplImage * image,int Mode,int p1,int p2) ;
    IplImage * Median             (IplImage * image,int p1 = 3,int p2 = 0) ;
    IplImage * Gaussian           (IplImage * image,int p1 = 3,int p2 = 3) ;
    IplImage * Blur               (IplImage * image,int p1 = 3,int p2 = 3) ;
    IplImage * Laplace            (IplImage * image            ,
                                   int        Aperture = 3   ) ;
    IplImage * Threshold          (IplImage * image            ,
                                   double     threshold        ,
                                   double     maxv             ,
                                   int        threstype      ) ;
    IplImage * Erode              (IplImage * image            ,
                                   int        Iterations = 1 ) ;
    IplImage * Dilate             (IplImage * image            ,
                                   int        Iterations = 1 ) ;

    void       UnsharpMask        (const IplImage * src        ,
                                         IplImage * dst        ,
                                   float            amount     ,
                                   float            radius     ,
                                   int              threshold  ,
                                   int              contrast ) ;
    void       AdjustContrast     (const IplImage * src        ,
                                         IplImage * dst        ,
                                   int              contrast ) ;

    QImage     UnsharpMask        (QImage & src        ,
                                   float    amount     ,
                                   float    radius     ,
                                   int      threshold  ,
                                   int      contrast ) ;
    QImage     AdjustContrast     (QImage & src,int contrast) ;

    // Groups Management
    bool       InsertGroup        (SqlConnection & Connection   ,
                                   SUID            first        ,
                                   SUID            second       ,
                                   int             t1           ,
                                   int             t2           ,
                                   int             relation   ) ;
    bool       InsertGroup        (SqlConnection & Connection   ,
                                   SUID            first        ,
                                   SUID            second       ,
                                   int             t1           ,
                                   int             t2           ,
                                   int             relation     ,
                                   double          membership ) ;
    bool       InsertGroup        (SqlConnection & Connection   ,
                                   SUID            first        ,
                                   SUID            second       ,
                                   int             t1           ,
                                   int             t2           ,
                                   int             relation     ,
                                   int             position   ) ;

    UUIDs      FindOwners         (SqlConnection & Connection ,
                                   SUID            uuid       ,
                                   int             t1         ,
                                   int             t2         ,
                                   int             relation ) ;

    // Rectangle Manager
    SUID       InsertPoint        (SqlConnection & Connection  ,
                                   QPoint          point     ) ;
    SUID       InsertRect         (SqlConnection & Connection  ,
                                   QRect           Rectangle ) ;
    bool       Rectangle          (SqlConnection & Connection  ,
                                   SUID            uuid        ,
                                   QRectF        & Rect      ) ;

    // Faces Manager
    SUID       InsertFace         (SqlConnection & Connection    ,
                                   SUID            picture       ,
                                   SUID            rectangle     ,
                                   double          possibility ) ;
    bool       InsertFaceParament (SqlConnection & Connection    ,
                                   SUID            face          ,
                                   QString         name          ,
                                   SUID            rectangle     ,
                                   SUID            point       ) ;
    UUIDs      FullFaces          (SqlConnection & Connection) ;
    UUIDs      BadFaces           (SqlConnection & Connection) ;
    UUIDs      FaceLimits         (SqlConnection & Connection  ,
                                   int             StartId     ,
                                   int             Limit     ) ;
    int        CountFaceDepot     (SqlConnection & Connection) ;

    // Palettes Manager
    void       Paint              (QImage  & image    ,
                                   Palette & palette  ,
                                   QSize     block  ) ;
    void       Paint              (QIcon   & icon     ,
                                   QSize     size     ,
                                   Palette & palette  ,
                                   QSize     block  ) ;
    bool       GetPalette         (SqlConnection & Connection ,
                                   SUID            uuid       ,
                                   Palette       & palette  ) ;
    bool       AttachPalette      (SqlConnection & Connection ,
                                   SUID            picture    ,
                                   SUID            palette  ) ;
    SUID       FindPalette        (SqlConnection & Connection ,
                                   Palette       & palette  ) ;
    SUID       AppendPalette      (SqlConnection & Connection ,
                                   Palette       & palette  ) ;
    bool       assurePalette      (SqlConnection & Connection ,
                                   SUID            uuid       ,
                                   Palette       & palette  ) ;
    bool       assurePalette      (SqlConnection & Connection ,
                                   SUID            uuid       ,
                                   QImage        & image    ) ;
    bool       assureHistogram    (SqlConnection & Connection ,
                                   SUID            uuid       ,
                                   QImage        & image    ) ;
    bool       assureVerify       (SqlConnection & Connection ,
                                   SUID            uuid       ,
                                   QByteArray    & Data     ) ;

    // Picture comparsion
    double     Similarity         (QImage & First    ,
                                   QImage & Second   ,
                                   int      Method ) ;

    SUID       Append             (SqlConnection & Connection ,
                                   QString         filename   ,
                                   QByteArray    & image    ) ;
    SUID       AppendPNG          (SqlConnection & Connection ,
                                   QByteArray    & image    ) ;

    // Handle Picture Depot
    bool       InsertPicture      (SqlConnection & Connection ,
                                   SUID            uuid       ,
                                   QByteArray    & image      ,
                                   QString       & Suffix     ,
                                   int             mimeid     ,
                                   int             checksum ) ;
    bool       PictureBlob        (SqlConnection & Connection ,
                                   SUID            uuid       ,
                                   QByteArray    & blob     ) ;
    bool       InsertBlob         (SqlConnection & Connection ,
                                   SUID            uuid       ,
                                   QByteArray    & blob     ) ;
    bool       UpdateBlob         (SqlConnection & Connection ,
                                   SUID            uuid       ,
                                   QByteArray    & blob     ) ;
    bool       AssureBlob         (SqlConnection & Connection ,
                                   SUID            uuid       ,
                                   QByteArray    & blob     ) ;
    bool       DeleteBlob         (SqlConnection & Connection ,
                                   SUID            uuid     ) ;
    bool       BlobExists         (SqlConnection & Connection ,
                                   SUID            uuid     ) ;

    bool       SkinMask           (QImage & Mask,QPolygonF & polygon) ;

    bool       SurfFeaturePoints  (QImage        & image                    ,
                                   FeaturePoints & points                   ,
                                   double          hessianThreshold = 400   ,
                                   int             octaves          =   3   ,
                                   int             octaveLayers     =   4 ) ;
    bool       SurfFeaturePoints  (QImage        & image                    ,
                                   QPolygonF     & points                   ,
                                   double          hessianThreshold = 400   ,
                                   int             octaves          =   3   ,
                                   int             octaveLayers     =   4 ) ;

    QImage     toGrey             (QImage & source) ;
    bool       toGrey             (QImage & source,QImage & grey ) ;
    bool       HorizontalSobel    (QImage & source,QImage & sobel) ;
    bool       HorizontalSobel    (QImage & source,QImage & sobel,int threshold) ;
    bool       VerticalSobel      (QImage & source,QImage & sobel) ;
    bool       VerticalSobel      (QImage & source,QImage & sobel,int threshold) ;

    bool       Stitch             (QImage        & result             ,
                                   QList<QImage> & images             ,
                                   double          confidence = 1.0 ) ;
    IplImage * RepairMask         (IplImage * source,int level = 168) ;
    bool       Repair             (QImage   & source                            ,
                                   QImage   & patch                             ,
                                   double     inpaintRange = 3                  ,
                                   int        flags        = CV_INPAINT_TELEA   ,
                                   int        leve         = 168              ) ;
    bool       Repair             (QImage   & source                            ,
                                   QImage   & patch                             ,
                                   IplImage * mask                              ,
                                   double     inpaintRange = 3                  ,
                                   int        flags        = CV_INPAINT_TELEA ) ;

    virtual bool ObtainSizes      (SqlConnection & Connection ,
                                   UUIDs         & Uuids      ,
                                   QList<QSize>  & SIZEs    ) ;
    virtual bool ObtainBySizes    (SqlConnection & Connection ,
                                   UUIDs         & Result     ,
                                   QList<QSize>  & SIZEs    ) ;
    virtual bool ObtainColors     (SqlConnection & Connection ,
                                   UUIDs         & Uuids      ,
                                   CUIDs         & COLORs   ) ;
    virtual bool ObtainByColors   (SqlConnection & Connection ,
                                   UUIDs         & Uuids      ,
                                   CUIDs         & COLORs   ) ;
    virtual bool ObtainPALs       (SqlConnection & Connection ,
                                   UUIDs         & Uuids      ,
                                   UUIDs         & PALs     ) ;
    virtual bool ObtainByPALs     (SqlConnection & Connection ,
                                   UUIDs         & Uuids      ,
                                   UUIDs         & PALs     ) ;
    virtual bool ObtainHistos     (SqlConnection & Connection ,
                                   UUIDs         & Uuids      ,
                                   ByteArrays    & HISTOs   ) ;
    virtual bool ObtainByHistos   (SqlConnection & Connection ,
                                   UUIDs         & Uuids      ,
                                   ByteArrays    & HISTOs   ) ;

  protected:

  private:

} ;

class Q_MANAGERS_EXPORT HistogramBuilder : public QObject
                                         , public Thread
                                         , public PictureManager
{
  Q_OBJECT
  public:

    explicit HistogramBuilder (QObject * parent = NULL,Plan * plan = NULL) ;
    virtual ~HistogramBuilder (void) ;

  protected:

    Plan * plan          ;
    int    ProgressId    ;
    qint64 ProgressValue ;
    bool   ProgressRun   ;

    virtual void run           (void) ;

  private:

  public slots:

    virtual bool startup       (void) ;

  protected slots:

  private slots:

  signals:

} ;

class Q_MANAGERS_EXPORT PaletteBuilder : public QObject
                                       , public Thread
                                       , public PictureManager
{
  Q_OBJECT
  public:

    explicit     PaletteBuilder (QObject * parent = NULL,Plan * plan = NULL) ;
    virtual     ~PaletteBuilder (void) ;

  protected:

    Plan * plan          ;
    int    ProgressId    ;
    qint64 ProgressValue ;
    bool   ProgressRun   ;

    virtual void run            (void) ;

  private:

  public slots:

    virtual bool startup        (void) ;

  protected slots:

  private slots:

  signals:

} ;

class Q_MANAGERS_EXPORT ThumbBuilder : public QObject
                                     , public Thread
                                     , public PictureManager
{
  Q_OBJECT
  public:

    explicit     ThumbBuilder (QObject * parent = NULL,Plan * plan = NULL) ;
    virtual     ~ThumbBuilder (void) ;

  protected:

    Plan * plan          ;
    int    ProgressId    ;
    qint64 ProgressValue ;
    bool   ProgressRun   ;

    virtual void run          (void) ;

  private:

  public slots:

    virtual bool startup      (void) ;

  protected slots:

  private slots:

  signals:

} ;

class Q_MANAGERS_EXPORT PictureRecognizer
{
  public:

    CvClassifiers Classifiers      ;
    bool          Flip             ; // Left - Right exchange
    QSize         FaceDetect       ;
    double        FaceScale        ;
    int           FaceNeighbors    ;
    int           FaceFlags        ;
    QSize         FeatureDetect    ;
    double        FeatureScale     ;
    int           FeatureNeighbors ;
    int           FeatureFlags     ;
    QSize         MinWindowSize    ;
    bool          AdjustWindow     ;
    BMAPs         Features         ;

    explicit             PictureRecognizer   (Plan * plan) ;
    virtual             ~PictureRecognizer   (void       ) ;

    static CvClassifiers Load                (NAMEs Pathes) ;
    Phiz                 Faces               (IplImage * image) ;

    QRectF               ShiftIn             (QRectF Image,QRect Feature) ;
    QRectF               ShiftOut            (QRectF Image,QRect Feature) ;
    QRect                GoldenRatio         (QRect  Face ,QSize Size) ;
    QRect                FlatRatio           (QRect  Face ,QSize Size) ;

    virtual bool         setFeature          (int feature,bool enable) ;
    virtual bool         isFeature           (int feature) ;

    virtual QRect        LeftEye             (QRect Face) ;
    virtual QRect        RightEye            (QRect Face) ;
    virtual QRect        Mouth               (QRect Face) ;
    virtual QRect        Nose                (QRect Face) ;

  protected:

    Plan * FilterPlan ;

    void                 SetImageROI         (IplImage * image,QRect Rect);
    CvSeq      *         DetectMajorObject   (IplImage * image,CvMemStorage * storage);
    CvSeq      *         DetectFeatureObject (IplImage * image,CvMemStorage * storage,int Classifier);
    QList<QRect>         Rectangles          (CvSeq    * items,QRect Shift) ;
    QList<QRect>         DetectFeatures      (IplImage * image,CvMemStorage * storage,int Classifier,QRect Face);

  private:

} ;

/*****************************************************************************
 *                                                                           *
 *                          Essential People Management                      *
 *                                                                           *
 *****************************************************************************/

class Q_MANAGERS_EXPORT PeopleManager
{
  public:

    Plan * PeoplePlan ;

    explicit    PeopleManager       (Plan * plan = NULL) ;
    virtual    ~PeopleManager       (void) ;

    UUIDs       Lists               (SqlConnection & Connection,QString options = "order by id asc") ;
    UUIDs       Surnames            (SqlConnection & Connection,QString options = "order by id asc") ;
    UUIDs       NameList            (SqlConnection & Connection,SUID Uuid) ;
    UUIDs       Group               (SqlConnection & Connection,SUID first,int t1,QString options = "order by position asc") ;
    QStringList Names               (SqlConnection & Connection,SUID Uuid) ;
    QString     PreferName          (SqlConnection & Connection,SUID Uuid) ;
    QString     Name                (SqlConnection & Connection,SUID Uuid) ;
    QString     Name                (SqlConnection & Connection,SUID Uuid,int language) ;
    bool        NameLanguage        (SqlConnection & Connection,SUID Uuid) ;
    SUID        Surname             (SqlConnection & Connection,SUID Uuid) ;
    bool        PeopleSurname       (SqlConnection & Connection,SUID surname,SUID name) ;
    int         PeopleComboBox      (SqlConnection & Connection,QComboBox * Search) ;
    bool        PersonHasSurname    (SqlConnection & Connection,SUID surname,UUIDs & people) ;
    bool        SurnameEquivalences (SqlConnection & Connection,SUID surname,UUIDs & equivalences) ;
    bool        SurnameCombo        (QComboBox     * combo     ,SqlConnection & Connection,int language,QString name) ;
    SUID        FindPeople          (SqlConnection & Connection,QString name) ;
    SUID        addSurname          (SqlConnection & Connection,int language,QString name) ;
    bool        addEqualSurname     (SqlConnection & Connection,SUID surname,SUID equal) ;
    bool        removeEqualSurname  (SqlConnection & Connection,SUID surname,SUID equal) ; // both side
    bool        detachEqualSurname  (SqlConnection & Connection,SUID surname,SUID equal) ; // one side
    SUID        addPeople           (SqlConnection & Connection,int language,QString name) ;
    bool        InsertNameMap       (SqlConnection & Connection,SUID Uuid,SUID Nuid) ;
    bool        AssignSexuality     (SqlConnection & Connection,SUID People,SUID Sexuality) ;
    SUID        PeopleGender        (SqlConnection & Connection,SUID People) ;
    UUIDs       SexualityCrowd      (SqlConnection & Connection,SUID Sexuality) ;
    UUIDs       HumanSexualities    (SqlConnection & Connection) ;
    bool        HumanSexualities    (SqlConnection & Connection,int language,NAMEs & names) ;

  protected:

  private:

} ;

/*****************************************************************************
 *                                                                           *
 *                                 Tags Manager                              *
 *                                                                           *
 *****************************************************************************/

class Q_MANAGERS_EXPORT TagsManager
{
  public:

    Plan * TagsPlan ;

    explicit TagsManager (Plan * plan = NULL) ;
    virtual ~TagsManager (void) ;

    UUIDs    Uuids       (SqlConnection & Connection,int Type) ;
    bool     toNames     (SqlConnection & connection,int vLanguageId,UUIDs & Uuids,NAMEs & names) ;
    SUID     NewTag      (SqlConnection & Connection,int Type) ;
    bool     Join        (SqlConnection & Connection,SUID tag,int ObjectType,const UUIDs & Uuids,bool progress = true) ;
    int      Count       (SqlConnection & Connection,SUID uuid) ;
    SUID     TagUuid     (SqlConnection & Connection,int Type,QString name,int language) ;
    bool     GetTags     (SqlConnection & Connection,UUIDs & U,QString name) ;

  protected:

  private:

} ;

// Normally, you copy this to your target widget.
// Try not to use the DefaultLearnSiteStrings directly and replace trXX into tr
#define DefaultLearnSiteStrings                                                  \
  Translations [ MsgParsing ] = trXX("Parsing %1"     )                         ;\
  Translations [ MsgUpdate  ] = trXX("Update %1"      )                         ;\
  Translations [ MsgFetch   ] = trXX("Fetch %1 header")                         ;\
  Translations [ MsgIMS     ] = trXX("Import %1 pages")                         ;\
  Translations [ MsgIMD     ] = trXX("Import URL : <%1>")                       ;\
  Translations [ MsgIMB     ] = trXX("Import %1 bytes into %2")                 ;\
  Translations [ MsgMMB     ] = trXX("Import multimedia file %1 bytes into %2") ;\
  Translations [ MsgHNH     ] = trXX("[%1] has no header")                      ;\
  Translations [ MsgRCI     ] = trXX("[%1] return code is %2")                  ;\
  Translations [ MsgURM     ] = trXX("URI is <%1>")                             ;\
  Translations [ MsgAMT     ] = trXX("Alter mimetype to %1")                    ;\
  Translations [ MsgDLC     ] = trXX("Downloading %1")                          ;\
  Translations [ MsgDLF     ] = trXX("Download %1 failure")                     ;\
  Translations [ MsgMSF     ] = trXX("%1 failure")                              ;\
  Translations [ MsgAPU     ] = trXX("Attach picture with url")                 ;\
  Translations [ MsgDBG     ] = trXX("Picture connected to %1")                 ;\
  Translations [ MsgIPM     ] = trXX("Importing picture"      )                 ;\
  Translations [ MsgIPC     ] = trXX("Importing picture <%1>" )                 ;\
  Translations [ MsgCIF     ] = trXX("Check image format")                      ;\
  Translations [ MsgWAG     ] = trXX("Web agent : <%1>")                        ;\
  Translations [ MsgRDL     ] = trXX("Redirect %1 to %2")                       ;\
  Translations [ MsgImage   ] = trXX("Image file: %1")                          ;\
  Translations [ MsgSuffix  ] = trXX("Suffix: %1")                              ;

class Q_MANAGERS_EXPORT LearnSite : public NetworkManager
                                  , public DocumentManager
                                  , public MimeManager
                                  , public XmlParser
{
  public:

    enum              {
      MsgParsing =  0 , // ("Parsing %1"     )
      MsgUpdate  =  1 , // ("Update %1"      )
      MsgFetch   =  2 , // ("Fetch %1 header")
      MsgIMS     =  3 , // ("Import %1 pages")
      MsgIMD     =  4 , // ("Import URL : <%1>")
      MsgIMB     =  5 , // ("Import %1 bytes into %2")
      MsgMMB     =  6 , // ("Import multimedia file %1 bytes into %2")
      MsgHNH     =  7 , // ("[%1] has no header")
      MsgRCI     =  8 , // ("[%1] return code is %2")
      MsgURM     =  9 , // ("URI is <%1>")
      MsgAMT     = 10 , // ("Alter mimetype to %1")
      MsgDLC     = 11 , // ("Downloading %1")
      MsgDLF     = 12 , // ("Download %1 failure")
      MsgMSF     = 13 , // ("%1 failure")
      MsgAPU     = 14 , // ("Attach picture with url")
      MsgDBG     = 15 , // ("Picture connected to %1")
      MsgIPM     = 16 , // ("Importing picture"      )
      MsgIPC     = 17 , // ("Importing picture <%1>" )
      MsgCIF     = 18 , // ("Check image format")
      MsgWAG     = 19 , // ("Web agent : <%1>")
      MsgRDL     = 20 , // ("Redirect %1 to %2")
      MsgImage   = 21 , // ("Image file: %1")
      MsgSuffix  = 22   // ("Suffix: %1")
    }                 ;

    NAMEs      Translations     ;
    int        StepDelay        ;
    int        MiniDelay        ;
    QByteArray DownloadedHeader ;

    explicit     LearnSite       (Plan * plan = NULL) ;
    virtual     ~LearnSite       (void) ;

    virtual void ResetParser     (void) ;

    virtual bool Initialize      (SqlConnection & Connection) ;

    virtual bool addVariable     (SqlConnection & Connection,SUID uuid,QString name,QString value) ;
    virtual bool deleteVariable  (SqlConnection & Connection,SUID uuid,QString name) ;
    virtual bool setReturnCode   (SqlConnection & Connection,SUID uuid,int retcode) ;
    virtual bool assignOriginate (SqlConnection & Connection,SUID uuid,SUID from) ;
    virtual bool ParseBody       (SqlConnection & SC,SUID uuid,QUrl & url,QByteArray & Body) ;
    virtual bool setPageMime     (SqlConnection & SC,SUID uuid,QString title,QString mime) ;
    virtual bool setBody         (SqlConnection & SC,SUID uuid,QByteArray & body) ;
    virtual bool setPageHeader   (SqlConnection & SC,SUID uuid,int retcode,QByteArray & header,QString & lastupdate) ;

    virtual QString LookForMime  (QString filename) ;

    virtual bool isImage         (QStringList & Filters,QString filename,QString message,QString debug) ;
    virtual bool isImage         (QStringList & Filters,QString filename) ;

    virtual bool Redirect        (SqlConnection & Connection ,
                                  SUID            uuid       ,
                                  QUrl          & source     ,
                                  QUrl          & location ) ;
    virtual bool ImportUrls      (SqlConnection & SC       ,
                                  SUID            uuid     ,
                                  QString         message  ,
                                  QString         debug  ) ;
    virtual bool AttachPicture   (SqlConnection & Connection   ,
                                  SUID            uuid         ,
                                  SUID            picture      ,
                                  QByteArray    & header       ,
                                  QString       & LastUpdate   ,
                                  QString       & MimeType     ,
                                  QString         message      ,
                                  QString         debug      ) ;
    virtual bool ImportPicture   (SqlConnection & Connection   ,
                                  QStringList   & ImageF       ,
                                  SUID            uuid         ,
                                  QUrl          & url          ,
                                  QString         filename     ,
                                  QByteArray    & header       ,
                                  QString       & lastUpdate   ,
                                  QString       & MIME         ,
                                  QString         importPicMsg ,
                                  QString         importPicCnt ,
                                  QString         message      ,
                                  QString         debug      ) ;
    virtual bool Associate       (SqlConnection & SC,SUID uuid,UUIDs & Uuids) ;
    virtual bool SaveAudio       (SqlConnection & SC          ,
                                  SUID            uuid        ,
                                  QUrl          & url         ,
                                  QByteArray    & Header      ,
                                  QByteArray    & Body        ,
                                  QString       & LastUpdate  ,
                                  QString       & MimeType  ) ;
    virtual bool SaveVideo       (SqlConnection & SC          ,
                                  SUID            uuid        ,
                                  QUrl          & url         ,
                                  QByteArray    & Header      ,
                                  QByteArray    & Body        ,
                                  QString       & LastUpdate  ,
                                  QString       & MimeType  ) ;
    virtual bool Download        (SqlConnection & SC,SUID uuid,QUrl url) ;

    virtual QString GetAgent     (QString debug = "") ;
    virtual QString parseTitle   (void) ;
    virtual QString Temporary    (QString filename) ;
    virtual bool    useProxy     (void) ;
    virtual QString ProxyAddress (void) ;
    virtual bool    setOptions   (Ftp & ftp) ;

    virtual bool    setEnabled   (int Id,bool enabled) ;
    virtual bool    isEnabled    (int Id) ;

  protected:

    int         ProgressId    ;
    qint64      ProgressValue ;
    bool        ProgressGo    ;
    BMAPs       Enablings     ;
    QByteArray  AnalyzeTitle  ;
    QList<QUrl> Links         ;
    QList<QUrl> Images        ;
    QStringList Hosts         ;
    QMutex      ImportPicLock ;
    QMutex      DownloadLock  ;

    virtual void AppendTitle     (QString title,QByteArray origin  ) ;
    virtual void AppendLink      (QString name,QUrl origin,QUrl url) ;
    virtual void AppendImage     (QString name,QUrl origin,QUrl url) ;

    virtual void shiftDelay      (void) ;
    virtual void stepDelay       (void) ;

    virtual bool TryGuess        (void) ;
    virtual bool IgnoreDump      (void) ;
    virtual bool TryReferer      (void) ;

    virtual int  HeaderTime      (void) ;
    virtual int  DownloadTime    (void) ;
    virtual int  ConnectTimeout  (void) ;

    virtual bool ImportHost      (QString host) ;
    virtual bool isAppendHost    (void) ;

    virtual bool RemoveImage     (void) ;

  private:

} ;

/*****************************************************************************
 *                                                                           *
 *                            Applied GUI enhancements                       *
 *                                                                           *
 *****************************************************************************/

class Q_MANAGERS_EXPORT ItemNames : public TreeWidget
                                  , public Object
{
  Q_OBJECT
  public:

    explicit            ItemNames       (StandardConstructor) ;
    virtual            ~ItemNames       (void);

    virtual QSize       sizeHint        (void) const ;

  protected:

    NAMEs       Usages     ;
    QPoint      dragPoint  ;
    bool        dropAction ;

    virtual bool        FocusIn         (void) ;
    virtual void        Configure       (void) ;

    virtual bool        hasItem         (void) ;
    virtual bool        startDrag       (QMouseEvent * event) ;
    virtual bool        fetchDrag       (QMouseEvent * event) ;
    virtual QMimeData * dragMime        (void) ;
    virtual void        dragDone        (Qt::DropAction dropIt,QMimeData * mime) ;
    virtual bool        finishDrag      (QMouseEvent * event) ;

    virtual bool        acceptDrop      (QWidget * source,const QMimeData * mime);
    virtual bool        dropNew         (QWidget * source,const QMimeData * mime,QPoint pos);
    virtual bool        dropNames       (nDeclDrops,const UUIDs & Uuids) ;

    virtual void        run             (int type,ThreadData * data) ;
    virtual bool        ItemRunner      (int type,ThreadData * data) ;

  private:

  public slots:

    virtual bool        startup         (void) ;

    virtual void        Reload          (void) ;

    virtual void        New             (void) ;
    virtual void        Delete          (void) ;
    virtual void        Paste           (void) ;
    virtual void        Eradicate       (void) ;

    virtual void        ItemUp          (void) ;
    virtual void        ItemDown        (void) ;

  protected slots:

    virtual void        removeOldItem   (void) ;
    virtual void        setNameItem     (QTreeWidgetItem * item) ;
    virtual void        setLanguageItem (QTreeWidgetItem * item) ;
    virtual void        setUsageItem    (QTreeWidgetItem * item) ;

    virtual void        editingFinished (void) ;
    virtual void        languageChanged (int index) ;
    virtual void        usageChanged    (int index) ;
    virtual void        itemFinalize    (void) ;

    virtual void        singleClicked   (QTreeWidgetItem * item,int column) ;
    virtual void        doubleClicked   (QTreeWidgetItem * item,int column) ;

  private slots:

  signals:

    void Update                         (nDeclWidget,SUID uuid) ;

} ;

class Q_MANAGERS_EXPORT ItemEditor : public TreeDock
{
  Q_OBJECT
  public:

    QString ItemTable ;
    int     ItemType  ;

    explicit      ItemEditor      (StandardConstructor) ;
    virtual      ~ItemEditor      (void) ;

    UUIDs         ItemUuids       (void) ;

  protected:

    virtual bool  FocusIn         (void) ;
    virtual void  Configure       (void) ;

    virtual bool  acceptDrop      (QWidget * source,const QMimeData * mime);
    virtual bool  dropNew         (QWidget * source,const QMimeData * mime,QPoint pos);
    virtual bool  dropText        (QWidget * source,QPointF pos,const QString & text);

    virtual UUIDs LoadUuids       (SqlConnection & connection) ;
    virtual void  LoadNames       (SqlConnection & connection,UUIDs & Uuids) ;
    virtual SUID  AppendUuid      (SqlConnection & connection);
    virtual bool  AppendName      (SqlConnection & connection,SUID uuid,QString name);

    virtual void  setCheckable    (QTreeWidgetItem * item) ;

    virtual void  run             (int Type,ThreadData * data) ;

    virtual QTreeWidgetItem * AppendItem (void) ;

  private:

  public slots:

    virtual bool  startup         (void) ;
    virtual bool  List            (void) ;
    virtual void  New             (void) ;
    virtual void  Rename          (void) ;
    virtual void  Paste           (void) ;
    virtual void  Paste           (QString text);
    virtual void  Export          (void) ;
    virtual void  Copy            (void) ;

  protected slots:

    virtual void  doubleClicked   (QTreeWidgetItem * item,int column) ;

    void          removeOldItem   (void) ;
    void          editingFinished (void) ;

  private slots:

  signals:

    void Update                  (nDeclWidget,SUID uuid) ;

} ;

class Q_MANAGERS_EXPORT NamesEditor : public ItemEditor
{
  Q_OBJECT
  public:

    explicit     NamesEditor (StandardConstructor) ;
    virtual     ~NamesEditor (void);

  protected:

    virtual void LoadNames   (SqlConnection & connection,UUIDs & Uuids) ;
    virtual bool AppendName  (SqlConnection & connection,SUID uuid,QString name);

  private:

  public slots:

  protected slots:

    virtual bool Menu        (QPoint pos) ;

  private slots:

  signals:

} ;

class Q_MANAGERS_EXPORT ItemNameEditor : public TableWidget
{
  Q_OBJECT
  public:

    explicit     ItemNameEditor   (StandardConstructor) ;
    virtual     ~ItemNameEditor   (void);

  protected:

    UUIDs          Uuids        ;
    CUIDs          Supports     ;
    QComboBox    * LanguageBox  ;
    QComboBox    * SearchText   ;
    int            EditIndex    ;
    QStringList    ItemNames    ;
    UUIDs          ItemUuids    ;
    CUIDs          ItemSupports ;

    virtual void contextMenuEvent (QContextMenuEvent * event) ;

    virtual void Configure        (void) ;
    virtual bool FocusIn          (void) ;

    virtual void run              (int type,ThreadData * data) ;

    bool load                     (UUIDs & Uuids,CUIDs & Supports);
    bool load                     (UUIDs & Uuids,int index);

    virtual void ToTraditional    (QTableWidgetItem * s,QTableWidgetItem * t) ;
    virtual void ToSimplified     (QTableWidgetItem * t,QTableWidgetItem * s) ;

  private:

  public slots:

    virtual void startup          (UUIDs & Uuids,CUIDs & Supports);
    virtual void startup          (QStringList & Names,UUIDs & Uuids,CUIDs & Supports);
    virtual void Refresh          (void) ;
    virtual void reload           (void) ;
    virtual void Search           (void) ;
    virtual void Search           (QString text) ;
    virtual void Find             (QString text) ;
    virtual void Export           (void) ;
    virtual bool Relocation       (void) ;
    virtual void TagFonts         (void) ;

  protected slots:

    virtual bool Menu             (QPoint pos) ;

    virtual void languageSection  (int index) ;
    virtual void itemSection      (int index) ;
    virtual void nameChanged      (int row,int column);
    void         languageSelected (int index) ;
    void         SearchReady      (void) ;

    virtual void LoadUuids        (void) ;
    virtual void LoadNames        (void) ;

  private slots:

  signals:

    void         setTags          (void) ;

} ;

class Q_MANAGERS_EXPORT NameComplex : public TreeWidget
                                    , public Ownership
{
  Q_OBJECT
  public:

    explicit            NameComplex     (StandardConstructor) ;
    virtual            ~NameComplex     (void) ;

    virtual QSize       sizeHint        (void) const ;

  protected:

    NAMEs  Usages     ;
    NAMEs  Maps       ;
    UUIDs  MapUuids   ;
    QPoint dragPoint  ;
    bool   dropAction ;
    bool   titled     ;

    virtual bool        FocusIn         (void) ;
    virtual void        Configure       (void) ;

    virtual bool        hasItem         (void) ;
    virtual bool        startDrag       (QMouseEvent * event) ;
    virtual bool        fetchDrag       (QMouseEvent * event) ;
    virtual QMimeData * dragMime        (void) ;
    virtual void        dragDone        (Qt::DropAction dropIt,QMimeData * mime) ;
    virtual bool        finishDrag      (QMouseEvent * event) ;

    virtual bool        acceptDrop      (QWidget * source,const QMimeData * mime);
    virtual bool        dropNew         (QWidget * source,const QMimeData * mime,QPoint pos);
    virtual bool        dropNames       (nDeclDrops,const UUIDs & Uuids) ;

    virtual void        run             (int type,ThreadData * data) ;

    virtual void        Reload          (ThreadData * data) ;
    virtual void        JoinMaps        (ThreadData * data) ;
    virtual void        Eradicate       (ThreadData * data) ;
    virtual void        FetchName       (SqlConnection & SC,ThreadData * data) ;
    virtual void        FetchNames      (SqlConnection & SC,ThreadData * data) ;

    virtual int         UpdateMaster    (SqlConnection & SC,int id,int language,QString name) ;
    virtual int         MapIndex        (QTreeWidgetItem * item) ;
    virtual bool        DeleteMaster    (SqlConnection & SC,int  id  ) ;
    virtual bool        DeleteAlias     (SqlConnection & SC,SUID uuid) ;

  private:

  public slots:

    virtual bool        startup         (void) ;

    virtual void        New             (void) ;
    virtual void        Delete          (void) ;
    virtual void        Paste           (void) ;
    virtual void        Eradicate       (void) ;

    virtual void        ItemUp          (void) ;
    virtual void        ItemDown        (void) ;
    virtual void        FlushIndex      (void) ;

  protected slots:

    virtual bool        Menu            (QPoint pos) ;

    virtual void        removeOldItem   (void) ;
    virtual void        setNameItem     (QTreeWidgetItem * item) ;
    virtual void        setLanguageItem (QTreeWidgetItem * item) ;
    virtual void        setUsageItem    (QTreeWidgetItem * item) ;
    virtual void        setMapsItem     (QTreeWidgetItem * item) ;

    virtual void        editingFinished (void) ;
    virtual void        languageChanged (int index) ;
    virtual void        usageChanged    (int index) ;
    virtual void        mapsChanged     (int index) ;
    virtual void        comboChanged    (int index,int column) ;
    virtual void        itemFinalize    (void) ;

    virtual void        singleClicked   (QTreeWidgetItem * item,int column) ;
    virtual void        doubleClicked   (QTreeWidgetItem * item,int column) ;

  private slots:

  signals:

    void                Restart         (void) ;
    void                Update          (nDeclWidget,SUID uuid) ;
} ;

class Q_MANAGERS_EXPORT DataEntries : public ItemEditor
{
  Q_OBJECT
  public:

    explicit     DataEntries (StandardConstructor) ;
    virtual     ~DataEntries (void) ;

  protected:

    virtual void Configure   (void) ;

  private:

  public slots:

  protected slots:

    virtual bool Menu        (QPoint pos) ;

  private slots:

  signals:

    void         Entries     (QString title,SUID uuid) ;

} ;

class Q_MANAGERS_EXPORT RealityLists : public TreeDock
                                     , public Group
{
  Q_OBJECT
  public:

    enum RealityEnumIDs                 {
      AlternateMenuID      = 12142719   ,
      DockingMenuID        = 12142720   ,
      RealityListMsgExport = 12250001   ,
      RealityListMsgFilter = 12250002   ,
      RealityListMsgPage   = 12250003 } ;

    explicit            RealityLists    (StandardConstructor) ;
    virtual            ~RealityLists    (void) ;

  protected:

    QPoint        dragPoint  ;
    bool          dropAction ;
    Qt::SortOrder sorting    ;

    virtual bool        FocusIn         (void) ;
    virtual void        Configure       (void) ;
    virtual void        InstallMsgs     (void) ;
    virtual void        InstalIcons     (void) ;

    virtual bool        hasItem         (void);
    virtual bool        startDrag       (QMouseEvent * event);
    virtual bool        fetchDrag       (QMouseEvent * event);
    virtual QMimeData * dragMime        (void);
    virtual void        dragDone        (Qt::DropAction dropIt,QMimeData * mime);
    virtual bool        finishDrag      (QMouseEvent * event);

    virtual bool        acceptDrop      (QWidget * source,const QMimeData * mime);
    virtual bool        dropNew         (QWidget * source,const QMimeData * mime,QPoint pos);
    virtual bool        dropText        (QWidget * source,QPointF pos,const QString & text);
    virtual bool        dropPeople      (nDeclWidget,QPointF pos,const UUIDs & Uuids) ;
    virtual bool        dropPictures    (nDeclWidget,QPointF pos,const UUIDs & Uuids) ;
    virtual bool        dropAlbums      (nDeclWidget,QPointF pos,const UUIDs & Uuids) ;
    virtual bool        dropVideos      (nDeclWidget,QPointF pos,const UUIDs & Uuids) ;
    virtual bool        dropModels      (nDeclWidget,QPointF pos,const UUIDs & Uuids) ;
    virtual bool        dropReality     (nDeclWidget,QPointF pos,const UUIDs & Uuids) ;

    virtual bool        dropObjects     (int t2,QPointF pos,const UUIDs & Uuids) ;

    virtual UUIDs       LoadUuids       (SqlConnection & connection) ;
    virtual void        LoadNames       (ThreadData * data,SqlConnection & connection,UUIDs & Uuids) ;
    virtual bool        JoinGroup       (SqlConnection & SC,SUID u) ;

    virtual void        List            (ThreadData * data) ;
    virtual void        Append          (ThreadData * data) ;
    virtual void        Join            (ThreadData * data) ;
    virtual void        Delete          (ThreadData * data) ;
    virtual void        DropIn          (ThreadData * data) ;
    virtual void        MoveTo          (ThreadData * data) ;
    virtual void        SetPositions    (ThreadData * data) ;

    virtual void        run             (int Type,ThreadData * data) ;

  private:

  public slots:

    virtual bool        startup         (void) ;
    virtual void        Insert          (void) ;
    virtual void        Delete          (void) ;
    virtual void        Rename          (void) ;
    virtual void        Paste           (void) ;
    virtual void        Paste           (QString text);
    virtual void        Export          (void) ;
    virtual void        doTranslations  (void) ;

  protected slots:

    virtual bool        Menu            (QPoint pos) ;
    virtual bool        LocalMenu       (QWidget * widget,QPoint pos) ;
    virtual void        SubjectMenu     (MenuManager     & mm   ,
                                         QMenu           * ma ) ;
    virtual bool        RunSubject      (MenuManager     & mm   ,
                                         QAction         * aa   ,
                                         QTreeWidgetItem * it ) ;
    virtual bool        RunSorting      (MenuManager     & Menu ,
                                         QAction         * aa ) ;

    virtual void        singleClicked   (QTreeWidgetItem * item,int column) ;
    virtual void        doubleClicked   (QTreeWidgetItem * item,int column) ;

    virtual void        editingFinished (void) ;

  private slots:

  signals:

    void                RealityMenu     (QWidget * widget,QPoint pos) ;
    void                FullNames       (SUID uuid) ;
    void                Lookup          (QString name,int t2,SUID uuid) ;
    void                Translations    (QString name,UUIDs & U) ;

} ;

/*****************************************************************************
 *                                                                           *
 *                            Applied GUI enhancements                       *
 *                                                                           *
 *****************************************************************************/

class Q_MANAGERS_EXPORT SnapView : public ListDock
                                 , public GroupItems
{
  Q_OBJECT
  public:

    explicit            SnapView        (StandardConstructor) ;
    virtual            ~SnapView        (void) ;

    virtual void        setGui          (QWidget * widget) ;

  protected:

    bool            Commanding      ;
    QTimer        * CommandSequence ;
    QPoint          dragPoint       ;
    ListWidgetItems Items           ;

    virtual void        Configure       (void) ;

    virtual bool        hasItem         (void) ;
    virtual bool        startDrag       (QMouseEvent * event) ;
    virtual bool        fetchDrag       (QMouseEvent * event) ;
    virtual QMimeData * dragMime        (void) = 0 ;
    virtual void        dragDone        (Qt::DropAction dropIt,QMimeData * mime) ;
    virtual bool        finishDrag      (QMouseEvent * event) ;

    virtual bool        acceptDrop      (QWidget * source,const QMimeData * mime) ;
    virtual bool        dropNew         (QWidget * source,const QMimeData * mime,QPoint pos) = 0 ;
    virtual bool        dropMoving      (QWidget * source,const QMimeData * mime,QPoint pos) = 0 ;
    virtual bool        dropAppend      (QWidget * source,const QMimeData * mime,QPoint pos) = 0 ;
    virtual bool        removeDrop      (void) ;

    virtual QString     MimeType        (const QMimeData * mime) = 0 ;
    virtual UUIDs       MimeUuids       (const QMimeData * mime,QString mimetype) = 0 ;

    QListWidgetItem *   uuidAt          (SUID uuid) ;
    virtual void        AnotherALERT    (int command) ;

    virtual bool        ReportDetails   (SqlConnection & Connection,QListWidgetItem * item,int flags) ;

  private:

  public slots:

    virtual bool        startup         (void) ;
    virtual void        pendingStartup  (void) ;
    virtual bool        Shutdown        (void) ;
    virtual bool        Relocation      (void) ;

    virtual void        NameThread      (int from,int to) ;
    virtual void        NameThread      (VarArgs & arguments) ;
    virtual void        NameThread      (void) ;
    virtual void        NameThreads     (int CommandId) ;
    virtual void        NamesThread     (int from,int to) ;
    virtual void        NamesThread     (VarArgs & arguments) ;
    virtual void        NamesThread     (void) ;
    virtual void        NamesThreads    (int CommandId) ;
    virtual void        ThumbsThread    (int start,int end) ;
    virtual void        ThumbsThread    (VarArgs & arguments) ;
    virtual void        ThumbsThread    (void) ;
    virtual void        GoThumbsThreads (int CommandId) ;
    virtual void        IconsThread     (Group group,int start,int end) ;
    virtual void        IconsThread     (VarArgs & arguments) ;
    virtual void        IconsThread     (Group group) ;
    virtual void        GoIconsThreads  (int CommandId,Group group) ;
    virtual void        JoinGroupThread (Group group,UUIDs & Uuids) ;
    virtual void        JoinOwnerThread (Group objectUuid,UUIDs & Uuids) ;
    virtual void        DetailsThread   (int flags) ;
    virtual void        IsCompleted     (void) ;
    virtual void        PreferLanguage  (SUID uuid,int type,int language) ;
    virtual void        PreferLanguage  (VarArgs & arguments) ;

    virtual bool        RunCommand      (VarArgs & arguments) ;

  protected slots:

    virtual void        StartCommand    (void) ;
    virtual void        CommandHandler  (void) ;

  private slots:

    void                setItemIcon     (QImage * image,QListWidgetItem * item) ;
    void                StartSequence   (void) ;
    void                StopSequence    (void) ;

  signals:

    void                SequenceStart   (void) ;
    void                SequenceStop    (void) ;
    void                TriggerCommand  (void) ;
    void                setIcon         (QImage * image,QListWidgetItem * item) ;

} ;

class Q_MANAGERS_EXPORT GroupView : public IconDock
                                  , public Object
                                  , public GroupItems
{
  Q_OBJECT
  public:

    typedef enum    {
      IconOnly  = 1 ,
      ThumbIcon = 2 }
      IconDisplay   ;

    QIcon       icon    ;
    IconDisplay display ;
    bool        textBar ;

    explicit            GroupView       (StandardConstructor) ;
    virtual            ~GroupView       (void) ;

    virtual void setGui                 (QWidget * widget) ;
    QListWidgetItem *   uuidAt          (SUID uuid) ;

  protected:

    bool            Commanding      ;
    QTimer        * CommandSequence ;
    bool            dropAction      ;
    QPoint          dragPoint       ;
    ListWidgetItems Items           ;

    virtual bool        FocusIn         (void);

    virtual void        Configure       (void);

    virtual bool        hasItem         (void);
    virtual bool        startDrag       (QMouseEvent * event);
    virtual bool        fetchDrag       (QMouseEvent * event);
    virtual QMimeData * dragMime        (void);
    virtual void        dragDone        (Qt::DropAction dropIt,QMimeData * mime);
    virtual bool        finishDrag      (QMouseEvent * event);

    virtual bool        acceptDrop      (QWidget * source,const QMimeData * mime);
    virtual bool        dropNew         (QWidget * source,const QMimeData * mime,QPoint pos);
    virtual bool        dropMoving      (QWidget * source,const QMimeData * mime,QPoint pos);
    virtual bool        dropAppend      (QWidget * source,const QMimeData * mime,QPoint pos);
    virtual bool        removeDrop      (void);

    virtual QString     MimeType        (const QMimeData * mime);
    virtual UUIDs       MimeUuids       (const QMimeData * mime,QString mimetype) ;

    virtual QString     ToolTipTotoal   (int total) ;
    virtual int         Counting        (SqlConnection & Connection,SUID uuid) ;
    virtual UUIDs       GroupUuids      (SqlConnection & Connection) ;
    QIcon               GetIcon         (SqlConnection & Connection,SUID uuid) ;
    virtual SUID        AppendGroup     (SqlConnection & Connection,QString name) ;

    virtual bool        dropText        (QWidget * source,QPointF pos,const QString & text) ;
    virtual bool        dropUrls        (QWidget * source,QPointF pos,const QList<QUrl> & urls) ;
    virtual bool        dropImage       (QWidget * source,QPointF pos,const QImage & image) ;
    virtual bool        dropHtml        (QWidget * source,QPointF pos,const QString & html) ;
    virtual bool        dropColor       (QWidget * source,QPointF pos,const QColor & color) ;
    virtual bool        dropTags        (QWidget * source,QPointF pos,const UUIDs & Uuids) ;
    virtual bool        dropPictures    (QWidget * source,QPointF pos,const UUIDs & Uuids) ;
    virtual bool        dropPeople      (QWidget * source,QPointF pos,const UUIDs & Uuids) ;
    virtual bool        dropVideos      (QWidget * source,QPointF pos,const UUIDs & Uuids) ;
    virtual bool        dropAlbums      (QWidget * source,QPointF pos,const UUIDs & Uuids) ;
    virtual bool        dropGender      (QWidget * source,QPointF pos,const SUID gender) ;
    virtual bool        dropDivisions   (QWidget * source,QPointF pos,const UUIDs & Uuids) ;

    virtual bool        setGroupIcon    (SUID uuid,const QImage & image) ;
    virtual bool        setGroupIcon    (SUID uuid,SUID picture) ;

    virtual bool        ReportDetails   (SqlConnection & Connection,QListWidgetItem * item,int flags) ;
    virtual int         EachType        (void) ;

    virtual void        run             (int Type,ThreadData * data) ;

  private:

  public slots:

    virtual bool        Shutdown        (void);
    virtual bool        Relocation      (void);

    virtual void        New             (void) ;
    virtual void        Rename          (void) ;
    virtual void        Delete          (void) ;
    virtual void        Rename          (QListWidgetItem * item) ;
    virtual void        Remove          (SUID uuid) ;
    virtual void        Remove          (UUIDs & Uuids) ;
    virtual void        Refresh         (QListWidgetItem * item);

    virtual bool        startup         (void) ;
    virtual bool        Reload          (void) ;
    virtual bool        Members         (void) ;

    virtual bool        JoinText        (SUID uuid,const QString & text) ;
    virtual bool        JoinUrls        (SUID uuid,const QList<QUrl> & urls) ;
    virtual bool        JoinImage       (SUID uuid,const QImage & image) ;
    virtual bool        JoinHtml        (SUID uuid,const QString & html) ;
    virtual bool        JoinColor       (SUID uuid,const QColor & color) ;
    virtual bool        JoinTags        (SUID uuid,const UUIDs & Uuids) ;
    virtual bool        JoinPictures    (SUID uuid,const UUIDs & Uuids) ;
    virtual bool        JoinPeople      (SUID uuid,const UUIDs & Uuids) ;
    virtual bool        JoinVideos      (SUID uuid,const UUIDs & Uuids) ;
    virtual bool        JoinAlbums      (SUID uuid,const UUIDs & Uuids) ;
    virtual bool        JoinGender      (SUID uuid,const SUID gender) ;
    virtual bool        JoinGroups      (SUID uuid,const UUIDs & Uuids) ;

    virtual void        GroupPositions  (RMAPs & Position,UUIDs & Uuids) ;
    virtual bool        MoveGroups      (SUID uuid,const UUIDs & Uuids) ;
    virtual bool        JoinGallery     (SUID uuid,const UUIDs & Uuids) ;
    virtual bool        JoinGallery     (SUID uuid,const QImage & image) ;

    virtual void        JoinGroupThread (Group group,UUIDs & Uuids) ;
    virtual void        JoinOwnerThread (Group objectUuid,UUIDs & Uuids) ;
    virtual void        DetailsThread   (int flags) ;
    virtual void        IsCompleted     (void) ;

    virtual bool        RunCommand      (VarArgs & arguments) ;

  protected slots:

    virtual void        StartCommand    (void) ;
    virtual void        CommandHandler  (void) ;

    virtual void        renameFinished  (void) ;
    virtual void        editingFinished (void) ;

    virtual bool        Menu            (QPoint pos) ;

    virtual void        setItemIcon     (QListWidgetItem * item,QImage * image) ;

  private slots:

    void                StartSequence   (void) ;
    void                StopSequence    (void) ;

  signals:

    void                SequenceStart   (void) ;
    void                SequenceStop    (void) ;
    void                TriggerCommand  (void) ;
    void                assignIcon      (QListWidgetItem * item,QImage * image) ;
    void                Translations    (QString title,UUIDs & Uuids) ;

} ;

class Q_MANAGERS_EXPORT GroupList : public TreeDock
                                  , public Object
                                  , public Relation
                                  , public GroupItems
{
  Q_OBJECT
  public:

    Qt::SortOrder Sorting ;

    explicit     GroupList           (StandardConstructor) ;
    virtual     ~GroupList           (void) ;

  protected:

    bool dropAction ;

    virtual void Configure           (void) ;
    virtual bool FocusIn             (void) ;

    virtual bool acceptDrop          (QWidget * source,const QMimeData * mime);
    virtual bool dropNew             (QWidget * source,const QMimeData * mime,QPoint pos) ;

    virtual bool dropPictures        (nDeclWidget,QPointF pos,const UUIDs   & Uuids   ) ;
    virtual bool dropPeople          (nDeclWidget,QPointF pos,const UUIDs   & Uuids   ) ;
    virtual bool dropAudios          (nDeclWidget,QPointF pos,const UUIDs   & Uuids   ) ;
    virtual bool dropVideos          (nDeclWidget,QPointF pos,const UUIDs   & Uuids   ) ;
    virtual bool dropAlbums          (nDeclWidget,QPointF pos,const UUIDs   & Uuids   ) ;
    virtual bool dropDivisions       (nDeclWidget,QPointF pos,const UUIDs   & Uuids   ) ;
    virtual bool dropURIs            (nDeclWidget,QPointF pos,const UUIDs   & Uuids   ) ;
    virtual bool dropBookmarks       (nDeclWidget,QPointF pos,const UUIDs   & Uuids   ) ;
    virtual bool dropShapes          (nDeclWidget,QPointF pos,const UUIDs   & Uuids   ) ;
    virtual bool dropMembers         (nDeclWidget,QPointF pos,const UUIDs   & Uuids   ) ;
    virtual bool dropSets            (nDeclWidget,QPointF pos,const UUIDs   & Uuids   ) ;
    virtual bool dropActions         (nDeclWidget,QPointF pos,const UUIDs   & Uuids   ) ;
    virtual bool dropDecision        (nDeclWidget,QPointF pos,const UUIDs   & Uuids   ) ;
    virtual bool dropCondition       (nDeclWidget,QPointF pos,const UUIDs   & Uuids   ) ;
    virtual bool dropExecution       (nDeclWidget,QPointF pos,const UUIDs   & Uuids   ) ;
    virtual bool dropSqlTable        (nDeclWidget,QPointF pos,const UUIDs   & Uuids   ) ;
    virtual bool dropDatabase        (nDeclWidget,QPointF pos,const UUIDs   & Uuids   ) ;
    virtual bool dropTask            (nDeclWidget,QPointF pos,const UUIDs   & Uuids   ) ;
    virtual bool dropNation          (nDeclWidget,QPointF pos,const UUIDs   & Uuids   ) ;
    virtual bool dropContour         (nDeclWidget,QPointF pos,const UUIDs   & Uuids   ) ;
    virtual bool dropManifold        (nDeclWidget,QPointF pos,const UUIDs   & Uuids   ) ;
    virtual bool dropSources         (nDeclWidget,QPointF pos,const UUIDs   & Uuids   ) ;
    virtual bool dropDocuments       (nDeclWidget,QPointF pos,const UUIDs   & Uuids   ) ;
    virtual bool dropEyes            (nDeclWidget,QPointF pos,const UUIDs   & Uuids   ) ;
    virtual bool dropHairs           (nDeclWidget,QPointF pos,const UUIDs   & Uuids   ) ;
    virtual bool dropKeywords        (nDeclWidget,QPointF pos,const UUIDs   & Uuids   ) ;
    virtual bool dropTerminologies   (nDeclWidget,QPointF pos,const UUIDs   & Uuids   ) ;
    virtual bool dropKnowledge       (nDeclWidget,QPointF pos,const UUIDs   & Uuids   ) ;
    virtual bool dropFields          (nDeclWidget,QPointF pos,const UUIDs   & Uuids   ) ;
    virtual bool dropKnowledgeBase   (nDeclWidget,QPointF pos,const UUIDs   & Uuids   ) ;
    virtual bool dropSqlColumn       (nDeclWidget,QPointF pos,const UUIDs   & Uuids   ) ;
    virtual bool dropUuids           (nDeclWidget,QPointF pos,const UUIDs   & Uuids   ) ;
    virtual bool dropCommodities     (nDeclWidget,QPointF pos,const UUIDs   & Uuids   ) ;
    virtual bool dropOrganizations   (nDeclWidget,QPointF pos,const UUIDs   & Uuids   ) ;
    virtual bool dropBlobs           (nDeclWidget,QPointF pos,const UUIDs   & Uuids   ) ;
    virtual bool dropVariables       (nDeclWidget,QPointF pos,const UUIDs   & Uuids   ) ;
    virtual bool dropTorrents        (nDeclWidget,QPointF pos,const UUIDs   & Uuids   ) ;
    virtual bool dropCameras         (nDeclWidget,QPointF pos,const UUIDs   & Uuids   ) ;
    virtual bool dropFaces           (nDeclWidget,QPointF pos,const UUIDs   & Uuids   ) ;
    virtual bool dropColorGroups     (nDeclWidget,QPointF pos,const UUIDs   & Uuids   ) ;
    virtual bool dropSetsAlgebras    (nDeclWidget,QPointF pos,const UUIDs   & Uuids   ) ;

    virtual bool dropObjects         (nDeclWidget,QPointF pos,const UUIDs & Uuids,int type) ;

    virtual void run                 (int Type,ThreadData * data) ;

  private:

  public slots:

    virtual bool startup             (void) ;
    virtual void List                (void) ;
    virtual void New                 (void) ;
    virtual void Paste               (void) ;
    virtual void Copy                (void) ;

    virtual void Paste               (QString text) ;

  protected slots:

    virtual void singleClicked       (QTreeWidgetItem * item,int column) ;
    virtual void doubleClicked       (QTreeWidgetItem * item,int column) ;
    virtual void editingFinished     (void) ;

    virtual bool Menu                (QPoint pos) ;

  private slots:

  signals:

    void Open                        (QString name,SUID uuid,int type) ;
    void Subgroups                   (QString name,SUID uuid,int type) ;

} ;

class Q_MANAGERS_EXPORT SubgroupLists : public GroupView
                                      , public Relation
{
  Q_OBJECT
  public:

    explicit        SubgroupLists (StandardConstructor) ;
    virtual        ~SubgroupLists (void) ;

    virtual QSize   sizeHint      (void) const ;

  protected:

    int     OrderBy   ;
    QString OrderItem ;

    virtual void    Configure     (void) ;

    virtual QString ToolTipTotoal (int total) ;
    virtual int     Counting      (SqlConnection & Connection,SUID uuid) ;
    virtual UUIDs   GroupUuids    (SqlConnection & Connection) ;
    virtual SUID    AppendGroup   (SqlConnection & Connection,QString name) ;

    virtual int     EachType      (void) ;

  private:

  public slots:

    virtual bool    JoinPictures  (SUID uuid,const UUIDs & Uuids) ;

  protected slots:

  private slots:

  signals:

} ;

class Q_MANAGERS_EXPORT SubgroupPictures : public SubgroupLists
{
  Q_OBJECT
  public:

    explicit     SubgroupPictures (StandardConstructor) ;
    virtual     ~SubgroupPictures (void) ;

  protected:

    virtual void Configure        (void) ;

  private:

  public slots:

    virtual void DeletePictures   (QListWidgetItem * it) ;

  protected slots:

    virtual bool Menu             (QPoint pos) ;

  private slots:

  signals:

    void         SeePictures      (SUID object,SUID subgroup,QString name) ;
    void         Constraints      (QString name,SUID uuid) ;
    void         RuleTables       (QString name,SUID uuid) ;

} ;

/*****************************************************************************
 *                                                                           *
 *                              File GUI enhancements                        *
 *                                                                           *
 *****************************************************************************/

class Q_MANAGERS_EXPORT FileBrowser : public Widget
{
  Q_OBJECT
  Q_PROPERTY(int gaps READ Gaps WRITE setGaps DESIGNABLE false)
  public:

    QFileSystemModel * model ;

    explicit FileBrowser     (StandardConstructor) ;
    virtual ~FileBrowser     (void);

    int setGaps              (int gap) ;
    int Gaps                 (void) ;

  protected:

    ComboBox    * path ;
    TreeView    * file ;
    HeaderView  * head ;
    QToolButton * tool ;
    int           gaps ;
    QFileInfo     last ;

    virtual void showEvent   (QShowEvent   * event) ;
    virtual void resizeEvent (QResizeEvent * event) ;
    virtual void closeEvent  (QCloseEvent  * event) ;

    virtual void Configure   (void) ;

  private:

  public slots:

    virtual void Relocation  (void) ;
    void         setRootPath (QString root) ;
    void         setDragDrop (QAbstractItemView::DragDropMode mode) ;

  protected slots:

    void fileClicked         (const QModelIndex & index) ;
    void menuClicked         (void) ;

    void headerMenu          (MenuManager & mm,QMenu   * menu  ) ;
    bool runHeaderMenu       (MenuManager & mm,QAction * action) ;

    virtual bool Menu        (QPoint pos) ;

  private slots:

  signals:

    void Lastest             (QString   path) ;
    void Clicked             (QFileInfo info) ;
    void Lastest             (QWidget * widget,QString   path) ;
    void Clicked             (QWidget * widget,QFileInfo info) ;

} ;

class Q_MANAGERS_EXPORT EquipmentList : public TreeDock
{
  Q_OBJECT
  public:

    explicit EquipmentList       (StandardConstructor) ;
    virtual ~EquipmentList       (void);

  protected:

    virtual void Configure       (void) ;
    virtual bool FocusIn         (void) ;

    virtual void run             (int type,ThreadData * data) ;
    virtual bool ItemRunner      (int type,ThreadData * data) ;
    virtual bool Load            (ThreadData * data) ;

  private:

  public slots:

    virtual bool startup         (void) ;
    virtual void List            (void) ;
    virtual void Insert          (void) ;

  protected slots:

    virtual bool Menu            (QPoint pos) ;
    virtual void doubleClicked   (QTreeWidgetItem * item,int column) ;
    virtual void editingFinished (void) ;

  private slots:

  signals:

} ;

/*****************************************************************************
 *                                                                           *
 *                                 Utilities                                 *
 *                                                                           *
 *****************************************************************************/

class Q_MANAGERS_EXPORT ScreenCapturer : public QThread
{
  Q_OBJECT
  public:

    explicit ScreenCapturer   (QObject * parent = 0);
    virtual ~ScreenCapturer   (void);

  protected:

    bool      running    ;
    int       Interval   ;
    int       MaxSeconds ;
    int       DelayStart ;
    QDir      Directory  ;
    QWidget * Widget     ;

    void run                  (void);

  private:

    QImage  LastestImage ;
    QImage  WindowImage  ;
    QPixmap LastestPixmap;
    QPixmap WindowPixmap ;

    virtual bool compare      (void);

  public slots:

    virtual void setInterval  (int interval) ;
    virtual void setMaxTime   (int maxSeconds) ;
    virtual void setDelay     (int delay) ;
    virtual void setDirectory (QDir dir) ;
    virtual void setWidget    (QWidget * widget) ;

    virtual void Start        (void);
    virtual void Stop         (void);

    virtual void Store        (QDateTime Time,QImage & Image);

  protected slots:

  private slots:

  signals:

} ;

/*****************************************************************************
 *                                                                           *
 *                                 Time Widgets                              *
 *                                                                           *
 *****************************************************************************/

class Q_MANAGERS_EXPORT TimeScope : public Dialog
{
  Q_OBJECT
  public:

    explicit     TimeScope    (StandardConstructor) ;
    virtual     ~TimeScope    (void) ;

    TUID         Start        (void) ;
    TUID         End          (void) ;
    TUID         dT           (void) ;

  protected:

    Ui::nTimeScope * ui ;

    virtual void Configure    (void) ;

  private:

  public slots:

    void         setStart     (TUID start) ;
    void         setEnd       (TUID end) ;
    void         setGap       (TUID gap) ;

  protected slots:

    void         startChanged (void) ;
    void         endChanged   (void) ;
    void         doneStart    (void) ;
    void         doneEnd      (void) ;

  private slots:

  signals:

} ;

class Q_MANAGERS_EXPORT TimeAlignment : public TableWidget
{
  Q_OBJECT
  public:

    explicit TimeAlignment (StandardConstructor) ;
    virtual ~TimeAlignment (void) ;

  protected:

    TUID  StartT ;
    TUID  EndT   ;
    TUID  dT     ;
    CUIDs Lists  ;

    virtual void Configure (void) ;

  private:

  public slots:

    virtual void Reflush   (void) ;
    virtual void addTime   (void) ;

  protected slots:

    virtual bool Menu      (QPoint pos) ;

  private slots:

  signals:

} ;

/*****************************************************************************
 *                                                                           *
 *                               Identity Widgets                            *
 *                                                                           *
 *****************************************************************************/

class Q_MANAGERS_EXPORT MemberLists : public TreeDock
{
  Q_OBJECT
  public:

    enum                        {
      AddMsg       = 79991101   ,
      DeleteMsg    = 79991102   ,
      HavingMenu   = 79991201   ,
      EnablingItem = 79991202 } ;

    explicit     MemberLists   (StandardConstructor) ;
    virtual     ~MemberLists   (void) ;

    virtual bool isEnabling    (SUID uuid) ;

  protected:

    virtual void Configure     (void) ;
    virtual bool FocusIn       (void) ;

    virtual void PrepareItem   (QTreeWidgetItem *item) ;

  private:

  public slots:

    virtual void Add           (void) ;
    virtual void Delete        (void) ;
    virtual void Delete        (SUID uuid) ;

    virtual void Append        (SUID uuid,QString name) ;
    virtual void Append        (SUID uuid,QString name,bool enable) ;

  protected slots:

    virtual bool Menu          (QPoint pos) ;

    virtual void singleClicked (QTreeWidgetItem * item,int column) ;
    virtual void doubleClicked (QTreeWidgetItem * item,int column) ;

  private slots:

  signals:

    void         AddMember     (void) ;
    void         DeleteMember  (SUID uuid) ;
    void         MemberPressed (SUID uuid) ;
    void         MemberClicked (SUID uuid) ;

} ;

/*****************************************************************************
 *                                                                           *
 *                               Painter Widgets                             *
 *                                                                           *
 *****************************************************************************/

class Q_MANAGERS_EXPORT FontLists : public TreeDock
{
  Q_OBJECT
  public:

    explicit            FontLists  (StandardConstructor) ;
    virtual            ~FontLists  (void) ;

  protected:

    QPoint dragPoint ;

    virtual void        Configure  (void) ;
    virtual bool        FocusIn    (void) ;

    virtual bool        hasItem    (void);
    virtual bool        startDrag  (QMouseEvent * event);
    virtual bool        fetchDrag  (QMouseEvent * event);
    virtual QMimeData * dragMime   (void);
    virtual void        dragDone   (Qt::DropAction dropIt,QMimeData * mime) ;
    virtual bool        finishDrag (QMouseEvent * event) ;

    virtual void        run        (int Type,ThreadData * data) ;
    virtual bool        ItemRunner (int Type,ThreadData * data) ;
    virtual void        Load       (ThreadData * data) ;

  private:

  public slots:

    virtual bool        startup    (void) ;
    virtual void        List       (void) ;
    virtual void        Insert     (void) ;
    virtual void        Delete     (void) ;

    virtual void        Update     (QString name,Font * font) ;

  protected slots:

    virtual bool        Menu       (QPoint pos) ;

  private slots:

  signals:

    void                Append     (QWidget * widget) ;
    void                EditFont   (QWidget * widget,QString name,Font * font) ;
    void                Export     (QWidget * widget,QString name,Font * font) ;

} ;

class Q_MANAGERS_EXPORT PenLists : public TreeDock
{
  Q_OBJECT
  public:

    explicit            PenLists   (StandardConstructor) ;
    virtual            ~PenLists   (void) ;

  protected:

    QPoint dragPoint ;

    virtual void        Configure  (void) ;
    virtual bool        FocusIn    (void) ;

    virtual bool        hasItem    (void);
    virtual bool        startDrag  (QMouseEvent * event);
    virtual bool        fetchDrag  (QMouseEvent * event);
    virtual QMimeData * dragMime   (void);
    virtual void        dragDone   (Qt::DropAction dropIt,QMimeData * mime);
    virtual bool        finishDrag (QMouseEvent * event);

    virtual void        run        (int Type,ThreadData * data) ;
    virtual bool        ItemRunner (int Type,ThreadData * data) ;
    virtual void        Load       (ThreadData * data) ;

  private:

  public slots:

    virtual bool        startup    (void) ;
    virtual void        List       (void) ;
    virtual void        Insert     (void) ;
    virtual void        Delete     (void) ;
    virtual void        Copy       (void) ;

    virtual void        Update     (QString name,Pen * pen) ;

  protected slots:

    virtual bool        Menu       (QPoint pos) ;

    virtual void        setIcon    (Pen & pen,QTreeWidgetItem * item) ;
    virtual void        assurePen  (QTreeWidgetItem * item,QByteArray * data) ;

  private slots:

  signals:

    void                assignPen  (QTreeWidgetItem * item,QByteArray * data) ;
    void                Append     (QWidget * widget) ;
    void                EditPen    (QWidget * widget,QString name,Pen * pen) ;
    void                Export     (QWidget * widget,QString name,Pen * pen) ;

} ;

class Q_MANAGERS_EXPORT BrushLists : public ListDock
{
  Q_OBJECT
  public:

    explicit            BrushLists  (StandardConstructor) ;
    virtual            ~BrushLists  (void) ;

  protected:

    QPoint dragPoint ;

    virtual void        Configure   (void) ;
    virtual bool        FocusIn     (void) ;

    virtual bool        hasItem     (void);
    virtual bool        startDrag   (QMouseEvent * event);
    virtual bool        fetchDrag   (QMouseEvent * event);
    virtual QMimeData * dragMime    (void);
    virtual void        dragDone    (Qt::DropAction dropIt,QMimeData * mime);
    virtual bool        finishDrag  (QMouseEvent * event);

    QIcon               getIcon     (Brush & brush) ;

    virtual void        run         (int Type,ThreadData * data) ;
    virtual bool        ItemRunner  (int Type,ThreadData * data) ;
    virtual void        Load        (ThreadData * data) ;

  private:

  public slots:

    virtual bool        startup     (void) ;
    virtual void        List        (void) ;
    virtual void        Insert      (void) ;
    virtual void        Delete      (void) ;
    virtual void        Copy        (void) ;

  protected slots:

    virtual bool        Menu        (QPoint pos) ;

    virtual void        assureBrush (QListWidgetItem * item,QByteArray * data) ;

  private slots:

  signals:

    void                assignBrush (QListWidgetItem * item,QByteArray * data) ;
    void                Append      (QWidget * widget) ;
    void                Export      (QWidget * widget,QString name,Brush * brush) ;

} ;

class Q_MANAGERS_EXPORT GradientLists : public ListDock
{
  Q_OBJECT
  public:

    explicit            GradientLists  (StandardConstructor) ;
    virtual            ~GradientLists  (void) ;

  protected:

    QPoint dragPoint ;

    virtual void        Configure      (void) ;
    virtual bool        FocusIn        (void) ;

    virtual bool        hasItem        (void) ;
    virtual bool        startDrag      (QMouseEvent * event) ;
    virtual bool        fetchDrag      (QMouseEvent * event) ;
    virtual QMimeData * dragMime       (void) ;
    virtual void        dragDone       (Qt::DropAction dropIt,QMimeData * mime) ;
    virtual bool        finishDrag     (QMouseEvent * event) ;

    QIcon               getIcon        (Gradient & gradient) ;

    virtual void        run            (int Type,ThreadData * data) ;
    virtual bool        ItemRunner     (int Type,ThreadData * data) ;
    virtual void        Load           (ThreadData * data) ;

  private:

  public slots:

    virtual bool        startup        (void) ;
    virtual void        List           (void) ;
    virtual void        Insert         (void) ;
    virtual void        Delete         (void) ;
    virtual void        Copy           (void) ;

  protected slots:

    virtual bool        Menu           (QPoint pos) ;

    virtual void        assureGradient (QListWidgetItem * item,QByteArray * data) ;

  private slots:

  signals:

    void                assignGradient (QListWidgetItem * item,QByteArray * data) ;
    void                Append         (QWidget * widget) ;
    void                Export         (QWidget * widget,QString name,Gradient * gradient) ;

} ;

class Q_MANAGERS_EXPORT PainterLists : public TreeDock
{
  Q_OBJECT
  public:

    explicit     PainterLists  (StandardConstructor) ;
    virtual     ~PainterLists  (void) ;

  protected:

    virtual void Configure     (void) ;
    virtual bool FocusIn       (void) ;

    virtual void run           (int Type,ThreadData * data) ;
    virtual bool ItemRunner    (int Type,ThreadData * data) ;
    virtual void Load          (ThreadData * data) ;

  private:

  public slots:

    virtual bool startup       (void) ;
    virtual void List          (void) ;

  protected slots:

    virtual bool Menu          (QPoint pos) ;

    virtual void RemoveOldItem (void) ;
    virtual void NameFinished  (void) ;

    virtual void singleClicked (QTreeWidgetItem * item,int column) ;
    virtual void doubleClicked (QTreeWidgetItem * item,int column) ;

  private slots:

  signals:

} ;

class Q_MANAGERS_EXPORT CommonSeeker : public QObject
{
  Q_OBJECT
  public:

    qint64 ProgressValue  ;
    bool   ProgressGo     ;

    explicit         CommonSeeker (QObject * parent = NULL) ;
    virtual         ~CommonSeeker (void) ;

    virtual bool     runnable     (void) const ;
    virtual QString  Temporary    (QString filename) ;
    virtual int      DownloadTime (void) const ;
    virtual int      ConnectTime  (void) const ;
    virtual bool     TrackingHttp (void) const ;
    virtual bool     skipJava     (void) const ;
    virtual Sql    & HunterSql    (void) ;
    virtual int      ID           (void) ;
    virtual qint64 & Value        (void) ;
    virtual bool   & Go           (void) ;
    virtual QString  ProxyAddress (void) const ;
    virtual int      ProxyPort    (void) const ;

  protected:

    bool    canRun         ;
    QDir    TemporaryPath  ;
    int     downloadTime   ;
    int     connectTime    ;
    bool    trackingHttp   ;
    bool    skipJavaScript ;
    Sql     Hunter         ;
    int     ProgressID     ;
    QString HostProxy      ;
    int     PortProxy      ;

  private:

  public slots:

    virtual void showMessage     (QString message) ;
    virtual void LookUrl         (QString url) ;
    virtual void setRunnable     (bool Run) ;
    virtual void setPath         (QString path) ;
    virtual void setDownloadTime (int downloadTime) ;
    virtual void setConnectTime  (int connectTime) ;
    virtual void setTrackingHttp (bool track) ;
    virtual void setJavaScript   (bool skip) ;
    virtual void setHunter       (Sql & sql) ;

    virtual void JavaChanged     (int state) ;
    virtual void TrackingChanged (int state) ;

    virtual void setCommonId     (int Id) ;
    virtual void setProxy        (const QString & text) ;

  protected slots:

  private slots:

    void         emitUrl         (QString url) ;
    void         emitMessage     (QString message) ;

  signals:

    void         privateUrl      (QString url) ;
    void         privateMessage  (QString message) ;
    void         sendUrl         (QString url) ;
    void         sendMessage     (QString message) ;

} ;

/*****************************************************************************
 *                                                                           *
 *                        Text arrangement Widgets                           *
 *                                                                           *
 *****************************************************************************/

class Q_MANAGERS_EXPORT TextCursor
{
  public:

    int                  Block       ;
    int                  Page        ;
    int                  Paragraph   ;
    int                  At          ;
    QPointF              Position    ;
    QRectF               Area        ;
    TextSection       *  Section     ;
    TextBlock         *  Full        ;
    TextBlock         *  Original    ;
    bool                 InputMethod ;
    bool                 Selected    ;
    QList<TextSection *> Selections  ;
    IMAPs                Clicks      ;
    QMap<int,QPointF   > Points      ;

    explicit TextCursor      (void) ;
             TextCursor      (const TextCursor & cursor) ;
    virtual ~TextCursor      (void) ;

    TextCursor & operator =  (const TextCursor & cursor) ;
    TextCursor & set         (const TextCursor & cursor) ;

    void     clear           (void) ;
    void     empty           (void) ;
    bool     isActivated     (void) ;
    QPointF  sectionAt       (void) ;

    void     clearSelection  (void) ;

    void     setPosition     (int id,QPointF pos) ;

    QPointF  Left            (void) ;
    QPointF  Right           (void) ;

    void     Anchor          (void) ;
    void     Last            (TextSection * section) ;
    void     Home            (TextSection * section) ;

    int      sectionLength   (void) ;

    void     Booking         (void) ;
    void     MarkStart       (void) ;
    void     MarkSelected    (void) ;
    void     MarkFirst       (void) ;
    void     MarkSecond      (void) ;
    bool     Select          (void) ;

    void     EnterIM         (void) ;
    void     LeaveIM         (void) ;

  protected:

  private:

} ;

class Q_MANAGERS_EXPORT TextRelation : public Object
{
  public:

    int     relation  ;
    VarArgs arguments ;

    explicit TextRelation     (void) ;
    explicit TextRelation     (SUID uuid,int type,int relation) ;
             TextRelation     (const TextRelation & relation) ;
    virtual ~TextRelation     (void) ;

    TextRelation & operator = (const TextRelation & relation) ;

  protected:

  private:

} ;

class Q_MANAGERS_EXPORT TextSection
{
  public:

    typedef enum          {
      PreviousSection = 1 ,
      NextSection     = 2 ,
      UpSection       = 3 ,
      DownSection     = 4 ,
      PreviousBlock   = 5 ,
      NextBlock       = 6 }
      SectionConnection   ;

    TextBlock            *  block       ;
    int                     index       ;
    QString                 section     ;
    bool                    arrangement ;
    bool                    selected    ;
    int                     start       ;
    int                     finish      ;
    Font                 *  font        ;
    Pen                  *  pen         ;
    Brush                *  brush       ;
    Pen                  *  paperPen    ;
    Brush                *  paperBrush  ;
    Pen                  *  selectPen   ;
    Brush                *  selectBrush ;
    QPointF                 position    ;
    QRectF                  area        ;
    QRectF                  region      ;
    int                     alignment   ;
    int                     attributes  ;
    bool                    dirty       ;
    QMap<int,TextSection *> linkages    ;
    TextRelation         *  relation    ;

    explicit TextSection           (void) ;
    explicit TextSection           (QString text,int index = -1) ;
    explicit TextSection           (TextBlock * block,QString text,int index = -1) ;
             TextSection           (const TextSection & section) ;
    virtual ~TextSection           (void) ;

    TextSection & operator =       (const TextSection & section) ;
    TextSection * clone            (void) ;
    void          Recover          (const TextSection & section) ;

    void          clear            (void) ;
    void          clearRendition   (void) ;
    int           length           (void) ;
    bool          intersects       (QPainterPath area) ;
    bool          contains         (QPainterPath area) ;
    bool          contains         (QPointF pos) ;
    bool          completeSelected (void) ;

    QRectF        Rectangle        (void) ;
    QRectF        Measure          (void) ;
    QRectF        Measure          (QString text) ;
    QPointF       Locate           (QPointF pos) ;
    QPointF       sectionAt        (int index) ;
    int           cursorAt         (QPointF pos) ;

    void          Render           (QPainter * painter) ;
    void          TextPainter      (QPainter * painter) ;
    void          SymbolPainter    (QPainter * painter) ;
    void          PaperPainter     (QPainter * painter) ;
    void          SelectedPainter  (QPainter * painter) ;
    void          SelectionPainter (QPainter * painter) ;
    void          RenderText       (QPainter * painter) ;
    void          RenderSymbol     (QPainter * painter) ;
    void          RenderSelection  (QPainter * painter) ;
    void          RenderEmpty      (QPainter * painter) ;

    void          Insert           (int index,QString text) ;
    void          Cut              (int index,int length) ;
    void          Delete           (int index) ;

    int           addLinkage       (int node,TextSection * section) ;
    TextSection * linkage          (int node) ;

    QRectF        TextRegion       (int position,int len) ;

  protected:

  private:

} ;

class Q_MANAGERS_EXPORT TextBlock
{
  public:

    typedef enum        {
      PreviousBlock = 1 ,
      NextBlock     = 2 }
      BlockConnection   ;

    int                   index       ;
    QString               body        ;
    bool                  dirty       ;
    bool                  selected    ;
    bool                  arrangement ;
    int                   start       ;
    int                   finish      ;
    QString               space       ;
    QPointF               position    ;
    QRectF                area        ;
    QList<TextSection  *> sections    ;
    QMap<int,TextBlock *> linkages    ;

    explicit       TextBlock       (void) ;
    explicit       TextBlock       (QString text) ;
                   TextBlock       (const TextBlock & block) ;
    virtual       ~TextBlock       (void) ;

    TextBlock   & operator =       (const TextBlock & block) ;
    TextSection * operator []      (int index) ;
    TextSection * NewSection       (QString text,int index = -1) ;
    TextBlock   * clone            (void) ;
    void          Recover          (const TextBlock & block) ;
    int           join             (TextSection * section) ;
    int           remove           (int from,int end) ;

    bool          completeSelected (void) ;
    int           count            (void) ;
    void          clear            (void) ;
    void          deleteSections   (void) ;
    void          ClearCR          (void) ;
    void          setSpace         (QString replacement) ;
    bool          intersects       (QPainterPath area) ;
    QString       Measure          (int index,Font & font,QRectF area) ;
    void          Collect          (void) ;
    QRectF        boundingRect     (QPointF position) ;
    QRectF        boundingRect     (void) ;
    QRectF        Rectangle        (void) ;

    TextSection * contains         (QPointF pos) ;

    int           addLinkage       (int node,TextBlock * block) ;
    TextBlock   * linkage          (int node) ;

    void          Insert           (TextCursor & cursor,QString text) ;
    void          Cut              (TextCursor & cursor,int length) ;
    void          Delete           (TextCursor & cursor) ;
    int           ParagraphAt      (TextCursor & cursor) ;

  protected:

  private:

} ;

class Q_MANAGERS_EXPORT TextDocument
{
  public:

    QByteArray           body         ;
    QString              lineFeed     ;
    QString              lineSplit    ;
    QString              encoding     ;
    QTextCodec        *  codec        ;
    int                  layout       ;
    bool                 selected     ;
    int                  start        ;
    int                  finish       ;
    bool                 dirty        ;
    QList<TextBlock   *> blocks       ;
    QList<TextSection *> sectionLists ;
    QList<TextSection *> opSections   ;
    QList<TextBlock   *> opBlocks     ;

    explicit TextDocument        (void) ;
    explicit TextDocument        (Plan * plan,SUID uuid) ;
             TextDocument        (const TextDocument & document) ;
    virtual ~TextDocument        (void) ;

    TextDocument & operator =    (const TextDocument & document) ;
    TextDocument & operator =    (QByteArray & body) ;
    TextBlock    * operator []   (int index) ;
    TextBlock    * operator []   (TextCursor & cursor) ;
    int  indexOf                 (TextBlock  * block) ;

    int  size                    (void) ;
    int  blockCount              (void) ;
    bool isDirty                 (void) ;
    void clear                   (void) ;
    void clean                   (void) ;
    void Clear                   (void) ;
    void deleteBlocks            (void) ;
    int  Prepare                 (void) ;
    int  toByteArray             (void) ;

    bool setEncoding             (QString name) ;
    void setLineFeed             (QString lf) ;
    void setLineSplit            (QString ls) ;

    bool loadPlainText           (Plan * plan,SUID uuid) ;
    bool loadFile                (Plan * plan,SUID uuid) ;
    bool setBody                 (QByteArray & body     ) ;

    void Collect                 (void) ;

    TextSection * contains       (QPointF pos) ;

    void EnumSections            (void) ;
    bool FindSection             (int & block,int & section,TextSection * text) ;

    TextSection * Previous       (TextSection * section) ;
    TextSection * Next           (TextSection * section) ;
    TextSection * Up             (TextSection * section) ;
    TextSection * Down           (TextSection * section) ;

    bool          Insert         (TextCursor  & cursor,QString text) ;
    bool          Paste          (TextCursor  & cursor,QString text) ;
    bool          Cut            (TextCursor  & cursor,int length = -1) ;
    bool          Delete         (TextCursor  & cursor) ;
    bool          Split          (TextCursor  & cursor) ;

    QRectF        boundingRect   (TextCursor  & cursor) ;
    bool          contains       (TextCursor  & cursor) ;
    int           length         (TextCursor  & cursor) ;
    TextBlock *   Block          (TextCursor  & cursor) ;
    void          setBlock       (TextCursor  & cursor,TextSection * Section) ;
    QString       Measure        (TextCursor  & cursor,Font & font,QRectF area) ;

    void          clearSelection (TextCursor  & cursor) ;
    QString       selectedText   (TextCursor  & cursor) ;
    void          Select         (TextCursor  & cursor  ,
                                  TextSection * start   ,
                                  int           sat     ,
                                  TextSection * finish  ,
                                  int           eat   ) ;
    void           SelectAll     (TextCursor  & cursor) ;
    void           Locate        (TextCursor  & cursor) ;

  protected:

  private:

} ;

class Q_MANAGERS_EXPORT TextReplacement : public Dialog
{
  Q_OBJECT
  public:

    explicit TextReplacement (StandardConstructor) ;
    virtual ~TextReplacement (void) ;

    QString sourceString     (void) ;
    QString replaceString    (void) ;

  protected:

    Ui::nTextReplacement * ui ;

  private:

  public slots:

    void setSource           (QString source) ;

  protected slots:

  private slots:

  signals:

} ;

class Q_MANAGERS_EXPORT TextProperties : public Dialog
{
  Q_OBJECT
  public:

    NAMEs Encodings ;
    UMAPs rEncode   ;

    explicit TextProperties (StandardConstructor) ;
    virtual ~TextProperties (void) ;

    int Encoding (void) ;
    int Language (void) ;

  protected:

    Ui::nTextProperties * ui ;

    virtual void Configure  (void) ;

  private:

  public slots:

    void setLanguage        (int language) ;
    void setEncoding        (int encoding) ;
    void setHidden          (int item    ) ;

  protected slots:

  private slots:

  signals:

} ;

/*****************************************************************************
 *                                                                           *
 *                                  TeX Widgets                              *
 *                                                                           *
 *****************************************************************************/

class Q_MANAGERS_EXPORT TeXPaintDevice : public QPaintDevice
{
  public:

    enum Unit {
      pt      ,
      bp      ,
      mm      ,
      cm      ,
      in      ,
      ex      ,
      em    } ;

    enum OutputMode {
      Tikz          ,
      Pgf         } ;

     TeXPaintDevice (const QString & fileName,const QSize & s = QSize(),Unit u = pt) ;
    ~TeXPaintDevice (void);

    virtual QPaintEngine * paintEngine (void) const ;

#ifdef QT_PRINTSUPPORT_LIB
    void setColorMode               (QPrinter::ColorMode mode) ;
#endif
    void setOutputMode              (OutputMode mode) ;
    void setUnit                    (Unit u) ;
    void setSize                    (const QSize & s) ;

    void setDocumentMode            (bool on = true) ;
    void setEscapeTextMode          (bool on = true) ;
    void exportFontSizes            (bool on = true) ;

    void setTextHorizontalAlignment (Qt::Alignment alignment) ;

  protected:

    virtual int metric ( PaintDeviceMetric ) const ;

  private:

    QSize            d_size ;
    TeXPaintEngine * engine ;

} ;

class Q_MANAGERS_EXPORT TeXPaintEngine : public QPaintEngine
{
  public:

     TeXPaintEngine(const QString&, TeXPaintDevice::Unit u = TeXPaintDevice::pt);
    ~TeXPaintEngine(void) ;

    virtual QPaintEngine::Type type(void) const { return QPaintEngine::User ; }

    virtual bool begin (QPaintDevice *) ;
    virtual bool end   (void) ;

    virtual void updateState  ( const QPaintEngineState & ) ;
    virtual void drawEllipse  ( const QRectF &);

    virtual void drawPoints   ( const QPointF * points, int pointCount ) ;
    virtual void drawLines    ( const QLineF * , int ) ;
    virtual void drawPath     ( const QPainterPath & path ) ;
    virtual void drawPolygon  ( const QPointF * , int , PolygonDrawMode ) ;
    virtual void drawTextItem ( const QPointF & , const QTextItem & ) ;
    virtual void drawRects    ( const QRectF * , int ) ;
    virtual void drawPixmap   ( const QRectF &, const QPixmap &, const QRectF &) ;
    virtual void drawImage    ( const QRectF &, const QImage &, const QRectF &, Qt::ImageConversionFlags) ;

    void setUnit                    (TeXPaintDevice::Unit u) ;
    void setOutputMode              (TeXPaintDevice::OutputMode mode) ;
    void setGrayScale               (bool on = true) ;
    void setDocumentMode            (bool on = true) ;
    void setEscapeTextMode          (bool on = true) ;
    void exportFontSizes            (bool on = true) ;
    void setTextHorizontalAlignment (Qt::Alignment alignment) ;

  private:

    enum Shape {
      Line     ,
      Polygon  ,
      Polyline ,
      Rect     ,
      Ellipse  ,
      Path     ,
      Points } ;

    bool    emptyStringOperation (void) ;

    QString unit            (void) ;
    double  unitFactor      (void) ;
    double  resFactorX      (void) ;
    double  resFactorY      (void) ;

    QString pgfPoint        (const QPointF & p) ;
    QString tikzPoint       (const QPointF & p) ;

    QPointF convertPoint    (const QPointF & p) ;
    QString color           (const QColor  & col) ;
    QString defineColor     (const QColor  & c,const QString& name) ;

    QString pgfPen          (const QPen    & pen) ;
    QString tikzPen         (const QPen    & pen) ;

    QString pgfBrush        (const QBrush  & brush) ;
    QString tikzBrush       (const QBrush  & brush) ;

    QString beginScope      (void) ;
    QString endScope        (void) ;

    QString clipPath        (void) ;
    bool    changedClipping (void) ;

    QString path            (const QPainterPath & path) ;
    QString pgfPath         (const QPainterPath & path) ;
    QString tikzPath        (const QPainterPath & path) ;

    QString drawShape       (Shape shape,const QString & path) ;
    QString drawPgfShape    (Shape shape,const QString & path) ;
    QString drawTikzShape   (Shape shape,const QString & path) ;

    void    drawPixmap      (const QPixmap & pix,const QRectF & p) ;
    void    writeToFile     (const QString & s) ;
    QString indentString    (const QString & s) ;

    bool addNewBrushColor   (void) ;
    bool addNewPatternColor (void) ;
    bool addNewPenColor     (void) ;

    QFile              * file                   ;
    QString              fname                  ;
    int                  d_pixmap_index         ;
    bool                 d_pgf_mode             ;
    bool                 d_open_scope           ;
    bool                 d_gray_scale           ;
    bool                 d_document_mode        ;
    bool                 d_escape_text          ;
    bool                 d_font_size            ;
    QPainterPath         d_clip_path            ;
    QColor               d_current_color        ;
    QColor               d_pattern_color        ;
    TeXPaintDevice::Unit d_unit                 ;
    Qt::Alignment        d_horizontal_alignment ;

} ;

/*****************************************************************************
 *                                                                           *
 *                Some classes for VCF (Very Complex Format) Items           *
 *                                                                           *
 *****************************************************************************/

class Q_MANAGERS_EXPORT AudioLists : public TreeDock
                                   , public Ownership
{
  Q_OBJECT
  public:

    explicit            AudioLists    (StandardConstructor) ;
    virtual            ~AudioLists    (void) ;

  protected:

    QPoint dragPoint ;

    virtual void        Configure     (void) ;
    virtual bool        FocusIn       (void) ;

    virtual bool        hasItem       (void) ;
    virtual bool        startDrag     (QMouseEvent * event) ;
    virtual bool        fetchDrag     (QMouseEvent * event) ;
    virtual QMimeData * dragMime      (void) ;
    virtual void        dragDone      (Qt::DropAction dropIt,QMimeData * mime) ;
    virtual bool        finishDrag    (QMouseEvent * event) ;

    virtual UUIDs       AudioUuids    (SqlConnection & Connection) ;

    virtual void        run           (int Type,ThreadData * data) ;
    virtual void        run           (void) ;
    virtual bool        ItemRunner    (int Type,ThreadData * data) ;

  private:

  public slots:

    virtual bool        startup       (void) ;
    virtual void        List          (void) ;
    virtual void        AppendChannel (QString name,SUID uuid) ;

  protected slots:

    virtual bool        Menu          (QPoint pos) ;
    virtual void        doubleClicked (QTreeWidgetItem * item,int column) ;

  private slots:

  signals:

    void                Edit          (QWidget * widget,SUID uuid) ;
    void                AddChannel    (QString name,SUID uuid,QString filename) ;

} ;

class Q_MANAGERS_EXPORT ContourController : public StackedWidget
                                          , public GraphicsManager
{
  Q_OBJECT
  public:

    VcfCanvas   * canvas  ;
    VcfContours * contour ;

    explicit ContourController  (StandardConstructor) ;
    virtual ~ContourController  (void);

  protected:

    QColor                   PenColor   ;
    QColor                   BrushColor ;
    Ui::nContourController * ui         ;

    QImage CreateSnap           (QSize size,QPointF center,Contour & contour) ;
    QIcon  IconColor            (QSize size,QColor color) ;

  private:

  public slots:

    virtual void Display        (void) ;
    virtual void Save           (void) ;
    virtual void Cancel         (void) ;

  protected slots:

    virtual void NameChanged    (QString text) ;
    virtual void Fill           (int state) ;
    virtual void Bounding       (int state) ;
    virtual void LineColor      (void) ;
    virtual void FillColor      (void) ;

  private slots:

  signals:

    void Finish      (void) ;

} ;

class Q_MANAGERS_EXPORT SceneManager
{
  public:

    Plan * ScenePlan ;

    explicit SceneManager  (Plan * plan = NULL);
    virtual ~SceneManager  (void);

    QString Name           (SqlConnection & Connection,SUID uuid,int language);
    bool    assureName     (SqlConnection & Connection,SUID uuid,int language,QString name);

    UUIDs   Matrices       (SqlConnection & Connection) ;
    SUID    updateMatrix   (SqlConnection & Connection,SUID uuid,Matrix & matrix) ;
    SUID    updateMatrix   (SqlConnection & Connection,SUID uuid,QString name,QString identifier,Matrix & matrix) ;
    Matrix  getMatrix      (SqlConnection & Connection,SUID uuid) ;

    UUIDs   Cameras        (SqlConnection & Connection) ;
    Camera  GetCamera      (SqlConnection & Connection,SUID uuid) ;
    bool    updateCamera   (SqlConnection & SC,Camera & camera,QImage & snap,QString identifier) ;

  protected:

  private:

} ;

class Q_MANAGERS_EXPORT SceneCreator
{
  public:

    explicit SceneCreator               (void) ;
    virtual ~SceneCreator               (void) ;

    static bool hasObject               (QString name) ;
    static bool hasUniverse             (QString name) ;

    static int  add                     (QString name,ObjectCreator   * creator) ;
    static int  add                     (QString name,UniverseCreator * creator) ;

    static SceneObject * createObject   (QString name,void * conf = NULL,QObject * parent = NULL) ;
    static SceneObject * createObject   (QString name,QByteArray & conf ,QObject * parent = NULL) ;
    static Universe    * createUniverse (QString name,void * conf = NULL,QObject * parent = NULL) ;
    static Universe    * createUniverse (QString name,QByteArray & conf ,QObject * parent = NULL) ;

  protected:

  private:

    static bool                            initialized ;
    static QMap<QString,ObjectCreator   *> objects     ;
    static QMap<QString,UniverseCreator *> universes   ;

    static void addDefaultCreator     (void) ;

} ;

/*****************************************************************************
 *                                                                           *
 *                                  BLOB Widgets                             *
 *                                                                           *
 *****************************************************************************/

class Q_MANAGERS_EXPORT DataArray
{
  public:

    explicit     DataArray          (void) ;
    virtual     ~DataArray          (void) ;

    QByteArray & data               (void) ;
    void         setData            (QByteArray data) ;
    void         setData            (File & File) ;

    int          addressOffset      (void) ;
    void         setAddressOffset   (int offset) ;
    int          addressWidth       (void) ;
    void         setAddressWidth    (int width) ;

    bool         dataChanged        (int i) ;
    QByteArray   dataChanged        (int i,int len) ;
    void         setDataChanged     (int i,bool state) ;
    void         setDataChanged     (int i,const QByteArray & state) ;

    int          realAddressNumbers (void) ;
    int          size               (void) ;

    QByteArray & insert             (int i,char ch) ;
    QByteArray & insert             (int i,const QByteArray & ba) ;

    QByteArray & remove             (int pos,int len) ;

    QByteArray & replace            (int index,char  ch) ;
    QByteArray & replace            (int index,const QByteArray & ba) ;
    QByteArray & replace            (int index,int   length,const QByteArray & ba) ;

    QChar        asciiChar          (int index) ;
    QString      toReadableString   (int start=0,int end=-1) ;

  protected:

    QByteArray Data               ;
    QByteArray ChangedData        ;
    int        AddressNumbers     ;
    int        AddressOffset      ;
    int        RealAddressNumbers ;
    int        OldSize            ;

  private:

} ;

class Q_MANAGERS_EXPORT DataCharCommand : public QUndoCommand
{
  public:

    enum { Id = 1234 } ;

    enum Cmd   {
      insert   ,
      remove   ,
      replace} ;

    int id(void) const { return Id; }

    explicit DataCharCommand                  (
               DataArray    * xData           ,
               Cmd            cmd             ,
               int            charPos         ,
               char           newChar         ,
               QUndoCommand * parent = NULL ) ;
    virtual ~DataCharCommand (void) ;

    virtual void undo        (void);
    virtual void redo        (void);
    virtual bool mergeWith   (const QUndoCommand *command);

  protected:

  private:

    Cmd         cmd        ;
    int         charPos    ;
    bool        wasChanged ;
    char        newChar    ;
    char        oldChar    ;
    DataArray * xData      ;

} ;

class Q_MANAGERS_EXPORT DataArrayCommand : public QUndoCommand
{
  public:

    enum { Id = 4321 } ;

    enum Cmd    {
      insert    ,
      remove    ,
      replace } ;

    int id(void) const { return Id; }

    explicit DataArrayCommand                       (
               DataArray    * xData                 ,
               Cmd            cmd                   ,
               int            baPos                 ,
               QByteArray     newBa  = QByteArray() ,
               int            len    = 0            ,
               QUndoCommand * parent = NULL       ) ;
    virtual ~DataArrayCommand (void) ;

    virtual void undo (void) ;
    virtual void redo (void) ;

  protected:

  private:

    Cmd         cmd        ;
    int         baPos      ;
    int         len        ;
    DataArray * xData      ;
    QByteArray  wasChanged ;
    QByteArray  newBa      ;
    QByteArray  oldBa      ;

} ;

class Q_MANAGERS_EXPORT BlobToCpp : public Dialog
{
  Q_OBJECT
  public:

    explicit BlobToCpp     (StandardConstructor) ;
    virtual ~BlobToCpp     (void) ;

  protected:

    Ui::nBlobToCpp * ui   ;
    QByteArray     * Data ;
    int              Type ;

    virtual void Configure (void) ;

  private:

  public slots:

    bool setData           (QByteArray * data,int type) ;
    bool Export            (void) ;
    void ExportCpp         (void) ;

  protected slots:

  private slots:

  signals:

} ;

class Q_MANAGERS_EXPORT DataToCpp : public Dialog
{
  Q_OBJECT
  public:

    explicit DataToCpp     (StandardConstructor) ;
    virtual ~DataToCpp     (void);

  protected:

    Ui::nDataToCpp * ui ;

    virtual void Configure (void) ;

  private:

  public slots:

    void DataSize          (int ) ;
    void DataSource        (void) ;
    void ExportCpp         (void) ;
    bool Export            (void) ;

  protected slots:

    void EditorChanged     (QString) ;

  private slots:

  signals:

} ;

class Q_MANAGERS_EXPORT HexWidget : public Widget
                                  , public Object
{
  Q_OBJECT
  public:

    QString Name ;

    explicit HexWidget                (QScrollArea * parent = NULL   ,
                                       Plan        * plan   = NULL ) ;
    virtual ~HexWidget                (void);

    void   setAddressAreaColor        (QColor const &color) ;
    QColor addressAreaColor           (void) ;

    void   setAddressOffset           (int offset) ;
    int    addressOffset              (void) ;

    void   setCursorPos               (int position) ;
    int    cursorPos                  (void) ;

    void   setData                    (const QByteArray & data) ;
    QByteArray data                   (void) ;

    void   setHighlightingColor       (const QColor & color) ;
    QColor highlightingColor          (void) ;

    void   setOverwriteMode           (bool overwriteMode) ;
    bool   overwriteMode              (void) ;

    void   setReadOnly                (bool readOnly) ;
    bool   isReadOnly                 (void) ;

    void   setSelectionColor          (const QColor & color) ;
    QColor selectionColor             (void) ;

    DataArray & xData                 (void) ;

    int  indexOf                      (const QByteArray & ba,int from = 0) ;
    int  lastIndexOf                  (const QByteArray & ba,int from = 0) ;
    void insert                       (int index,const QByteArray & ba) ;
    void insert                       (int index,char ch) ;
    void remove                       (int index,int len=1) ;

    void replace                      (int index,char ch) ;
    void replace                      (int index,const QByteArray & ba) ;
    void replace                      (int pos,int len,const QByteArray & after) ;

    void setAddressArea               (bool addressArea) ;
    void setAddressWidth              (int  addressWidth) ;
    void setAsciiArea                 (bool asciiArea) ;
    void setHighlighting              (bool mode) ;

    virtual void setFont              (const QFont & font) ;

    void undo                         (void) ;
    void redo                         (void) ;

    QString toReadableString          (void) ;
    QString selectionToReadableString (void) ;

  protected:

    virtual bool FocusIn              (void) ;
    virtual void Configure            (void) ;

    virtual void contextMenuEvent     (QContextMenuEvent * event) ;
    virtual void focusInEvent         (QFocusEvent * event) ;
    virtual void focusOutEvent        (QFocusEvent * event) ;
    virtual void keyPressEvent        (QKeyEvent   * event) ;
    virtual void mouseMoveEvent       (QMouseEvent * event) ;
    virtual void mousePressEvent      (QMouseEvent * event) ;
    virtual void paintEvent           (QPaintEvent * event) ;

    int  cursorPos                    (QPoint pos) ;
    void resetSelection               (int pos) ;
    void resetSelection               (void) ;
    void setSelection                 (int pos) ;
    int  getSelectionBegin            (void) ;
    int  getSelectionEnd              (void) ;

  private:

    void adjust                       (void) ;
    void ensureVisible                (void) ;

    QColor        _addressAreaColor  ;
    QColor        _highlightingColor ;
    QColor        _selectionColor    ;
    QScrollArea * _scrollArea        ;
    QTimer      * _cursorTimer       ;
    QUndoStack  * _undoStack         ;

    DataArray     _xData             ;

    bool          _blink             ;
    bool          _renderingRequired ;
    bool          _addressArea       ;
    bool          _asciiArea         ;
    bool          _highlighting      ;
    bool          _overwriteMode     ;
    bool          _readOnly          ;
    bool          _capital           ;

    int           _charWidth         ;
    int           _charHeight        ;
    int           _cursorX           ;
    int           _cursorY           ;
    int           _cursorPosition    ;
    int           _xPosAdr           ;
    int           _xPosHex           ;
    int           _xPosAscii         ;

    int           _selectionBegin    ;
    int           _selectionEnd      ;
    int           _selectionInit     ;

    int           _size              ;

  public slots:

    virtual void Save          (void) ;

  protected slots:

    virtual bool Menu          (QPoint pos) ;

  private slots:

    void updateCursor          (void) ;

  signals:

    void EventSignal           (int command) ;
    void currentAddressChanged (int address) ;
    void currentSizeChanged    (int size) ;
    void dataChanged           (void) ;
    void overwriteModeChanged  (bool state) ;

} ;

class Q_MANAGERS_EXPORT HexEditor : public ScrollArea
{
  Q_OBJECT
  Q_PROPERTY(QByteArray data READ data WRITE setData)
  Q_PROPERTY(int addressOffset READ addressOffset WRITE setAddressOffset)
  Q_PROPERTY(QColor addressAreaColor READ addressAreaColor WRITE setAddressAreaColor)
  Q_PROPERTY(int cursorPosition READ cursorPosition WRITE setCursorPosition)
  Q_PROPERTY(QColor highlightingColor READ highlightingColor WRITE setHighlightingColor)
  Q_PROPERTY(QColor selectionColor READ selectionColor WRITE setSelectionColor)
  Q_PROPERTY(bool overwriteMode READ overwriteMode WRITE setOverwriteMode)
  Q_PROPERTY(bool readOnly READ isReadOnly WRITE setReadOnly)
  Q_PROPERTY(QFont font READ font WRITE setFont)
  public:

    explicit      HexEditor                 (StandardConstructor) ;
    virtual      ~HexEditor                 (void) ;

    void          setName                   (QString name) ;
    void          setUuid                   (SUID uuid,int sourceType = Cpp::Undecided) ;
    bool          load                      (void) ;
    void          setData                   (const QByteArray & data) ;
    QByteArray    data                      (void) ;

    int           indexOf                   (const QByteArray & ba,int from = 0) const;
    int           lastIndexOf               (const QByteArray & ba,int from = 0) const;

    void          insert                    (int i,const QByteArray & ba);
    void          insert                    (int i,char ch);

    void          remove                    (int pos,int len = 1) ;
    void          replace                   (int pos,int len,const QByteArray & after) ;

    QString       toReadableString          (void) ;
    QString       selectionToReadableString (void) ;

    void          setAddressOffset          (int offset) ;
    int           addressOffset             (void) ;
    void          setCursorPosition         (int cusorPos) ;
    int           cursorPosition            (void) ;

    void          setAddressAreaColor       (const QColor & color) ;
    QColor        addressAreaColor          (void) ;
    void          setHighlightingColor      (const QColor & color) ;
    QColor        highlightingColor         (void) ;
    void          setSelectionColor         (const QColor & color) ;
    QColor        selectionColor            (void) ;
    void          setOverwriteMode          (bool) ;
    bool          overwriteMode             (void) ;

    void          setReadOnly               (bool) ;
    bool          isReadOnly                (void) ;

    const QFont & font                      (void) const ;
    void          setFont                   (const QFont &) ;

  protected:

    HexWidget   * qHexEdit_p ;
    QHBoxLayout * layout     ;
    QScrollArea * scrollArea ;

    virtual void Configure (void) ;

  private:

  public slots:

    void redo              (void) ;
    void undo              (void) ;
    void setAddressWidth   (int  addressWidth) ;
    void setAddressArea    (bool addressArea) ;
    void setAsciiArea      (bool asciiArea) ;
    void setHighlighting   (bool mode) ;

  protected slots:

  private slots:

  signals:

    void currentAddressChanged (int address) ;
    void currentSizeChanged    (int size) ;
    void dataChanged           (void) ;
    void overwriteModeChanged  (bool state) ;

} ;

class Q_MANAGERS_EXPORT Execution
{
  public:

    QString Name       ;
    QString Register   ;
    SUID    uuid       ;
    SUID    IconUuid   ;
    SUID    Platform   ;
    int     Identifier ;
    bool    Parallel   ;
    QIcon   Icon       ;
    QString Executable ;
    QString Environ    ;
    QString Paraments  ;

    explicit      Execution (void) ;
    virtual      ~Execution (void) ;

    virtual void  clear     (void) ;
    virtual bool  Run       (void) ;
    virtual UUIDs All       (Plan * plan,SqlConnection & Connection) ;
    virtual UUIDs All       (Plan * plan) ;
    virtual bool  Save      (Plan * plan,SqlConnection & Connection) ;
    virtual bool  Save      (Plan * plan) ;
    virtual bool  Load      (Plan * plan,SqlConnection & Connection,int     Identifier) ;
    virtual bool  Load      (Plan * plan,SqlConnection & Connection,SUID    uuid      ) ;
    virtual bool  Load      (Plan * plan,SqlConnection & Connection,QString name      ) ;
    virtual bool  Load      (Plan * plan,int     Identifier) ;
    virtual bool  Load      (Plan * plan,SUID    uuid      ) ;
    virtual bool  Load      (Plan * plan,QString name      ) ;

  protected:

  private:

} ;

class Q_MANAGERS_EXPORT PickView : public PickItem
{
  Q_OBJECT
  public:

    explicit         PickView          (QObject * parent,Plan * plan) ;
    explicit         PickView          (QObject * parent,Plan * plan,SceneDestructor * destructor) ;
    virtual         ~PickView          (void) ;

    virtual void     renderBox         (void) ;

  protected:

    UUIDs RealItems  ;
    SMAPs RealToView ;

    virtual void     Configure         (void) ;

    virtual void     run               (int type,ThreadData * data) ;
    virtual bool     ItemRunner        (int type,ThreadData * data) ;

    virtual QImage * FetchThumb        (SqlConnection & SC,SUID uuid,QString replacement) ;

    virtual void     BoxChanged        (bool hover) ;
    virtual void     FinalizeSelection (void) ;
    virtual void     ArrangeItems      (void) ;
    virtual void     ArrangeItems      (QString replaceIcon) ;
    virtual void     ArrangePictures   (QString replaceIcon) ;
    virtual void     ArrangeLanguage   (QString replaceIcon) ;
    virtual void     ArrangeNames      (QString replaceIcon) ;
    virtual void     RearrangeItems    (void) ;
    virtual void     UpdateScrollPages (qint64 total) ;
    virtual void     ScrollByStart     (qint64 total) ;
    virtual void     ScrollByCurrent   (qint64 total) ;

    virtual void     RenameItem        (VarArgs & args) ;
    virtual void     AppendDivision    (VarArgs & args) ;
    virtual void     DeleteGroup       (VarArgs & args) ;
    virtual void     DetachGroup       (VarArgs & args) ;
    virtual void     CountGroups       (VarArgs & args) ;
    virtual void     CountGroup        (VarArgs & args) ;
    virtual void     JoinGroup         (VarArgs & args) ;
    virtual void     MoveGroup         (VarArgs & args) ;
    virtual void     AssignIcon        (VarArgs & args) ;

    virtual qint64   CountMembers      (SqlConnection & SC,SUID uuid) ;
    virtual bool     DetachMembers     (SqlConnection & SC,SUID uuid) ;

  private:

  public slots:

    virtual bool     prepare           (void) ;
    virtual bool     startup           (void) ;

    virtual bool     LoadItems         (void) ;
    virtual bool     LoadFull          (void) ;
    virtual bool     LoadDivisions     (void) ;
    virtual bool     LoadLimits        (void) ;
    virtual bool     LoadFirst         (void) ;
    virtual bool     LoadSecond        (void) ;
    virtual bool     ReportDetails     (void) ;

    virtual void     PushCommand       (int cmd,QString name,SUID uuid) ;
    virtual void     PushCommand       (int cmd,SelectItem * item) ;
    virtual int      CallBinding       (SceneObject * s,QVariants & args) ;

  protected slots:

    virtual void     MountName         (MenuManager & mm,SelectItem * item) ;
    virtual void     MountUuid         (MenuManager & mm,QMenu * menu,SelectItem * item) ;
    virtual void     NameEditor        (MenuManager & mm,QMenu * menu,SelectItem * item,QLineEdit ** line) ;
    virtual void     ModifyName        (SelectItem * item,QLineEdit * line) ;

  private slots:

  signals:

} ;

class Q_MANAGERS_EXPORT ViewBox : public PickView
{
  Q_OBJECT
  public:

    explicit     ViewBox   (QObject * parent,Plan * plan) ;
    explicit     ViewBox   (QObject * parent,Plan * plan,SceneDestructor * destructor) ;
    virtual     ~ViewBox   (void) ;

  protected:

    virtual void Configure (void) ;

  private:

  public slots:

  protected slots:

  private slots:

  signals:

} ;

class Q_MANAGERS_EXPORT ViewShelf : public ViewBox
{
  Q_OBJECT
  public:

    explicit     ViewShelf (QObject * parent,Plan * plan) ;
    explicit     ViewShelf (QObject * parent,Plan * plan,SceneDestructor * destructor) ;
    virtual     ~ViewShelf (void) ;

  protected:

    virtual void Configure (void) ;

  private:

  public slots:

  protected slots:

  private slots:

  signals:

} ;

}

Q_DECLARE_METATYPE  ( N::Phiz         )
Q_DECLARE_METATYPE  ( N::DataArray    )
Q_DECLARE_METATYPE  ( N::Execution    )
Q_DECLARE_METATYPE  ( N::TextCursor   )
Q_DECLARE_METATYPE  ( N::TextRelation )
Q_DECLARE_METATYPE  ( N::TextSection  )
Q_DECLARE_METATYPE  ( N::TextBlock    )
Q_DECLARE_METATYPE  ( N::TextDocument )

Q_DECLARE_INTERFACE ( N::PicturePlugin , "com.neutrino.picture.filter" )

QT_END_NAMESPACE

#endif
