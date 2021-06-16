#include <qtmanagers.h>

N::TeXPaintEngine:: TeXPaintEngine (const QString & f,TeXPaintDevice::Unit u)
                  : QPaintEngine   (QPaintEngine::AllFeatures               )
                  , fname          (f                                       )
                  , d_pixmap_index (1                                       )
                  , d_unit         (u                                       )
{
  d_pgf_mode             = false            ;
  d_open_scope           = false            ;
  d_gray_scale           = false            ;
  d_document_mode        = false            ;
  d_escape_text          = true             ;
  d_font_size            = true             ;
  d_clip_path            = QPainterPath()   ;
  d_current_color        = QColor      ()   ;
  d_pattern_color        = QColor      ()   ;
  d_horizontal_alignment = Qt::AlignHCenter ;
}

N::TeXPaintEngine::~TeXPaintEngine(void)
{
}

bool N::TeXPaintEngine::begin(QPaintDevice* p)
{
  setPaintDevice(p);
  file = new QFile(fname);
  if (file->open(QIODevice::WriteOnly)){
    QTextStream t(file);
    t.setCodec("UTF-8");
    if (d_document_mode){
      t << "\\documentclass{article}\n";
      t << "\\usepackage[left=0.2cm,top=0.1cm,right=0.2cm,nohead,nofoot]{geometry}\n";

      if (d_pgf_mode){
        t << "\\usepackage{pgf}\n";
        t << "\\usepgflibrary{patterns}\n";
      } else {
        t << "\\usepackage{tikz}\n";
        t << "\\usetikzlibrary{patterns}\n";
      }

      t << "\\usepackage{ulem}\n";//used for striked out fonts (\sout command)
      t << "\\begin{document}\n";
    }

    QString pictureEnv = "\\begin{tikzpicture}{0";
    if (d_pgf_mode)
      pictureEnv = "\\begin{pgfpicture}{0";

    QString u = unit();
    t << pictureEnv + u + "}{0" + u + "}{";
    t << QString::number(p->width()) + u + "}{";
    t << QString::number(p->height()) + u + "}\n";

    if (!d_pgf_mode){
      QPainterPath path;
      path.addRect(QRect(0, 0, p->width(), p->height()));
      t << "\t\\clip" + tikzPath(path);
    }
    return true;
  }

  delete file;
  return false;
}

bool N::TeXPaintEngine::end()
{
  QTextStream t(file);
  t.setCodec("UTF-8");
  if (d_open_scope)
    t << endScope();

  if (d_pgf_mode)
    t << "\\end{pgfpicture}\n";
  else
    t << "\\end{tikzpicture}\n";

  if (d_document_mode)
    t << "\\end{document}\n";

  file->close();
  return true;
}

void N::TeXPaintEngine::updateState(const QPaintEngineState &)
{
}

void N::TeXPaintEngine::drawPoints ( const QPointF * points, int pointCount )
{
  if (emptyStringOperation())
    return;

  QMatrix m = painter()->worldMatrix();
  double lw = painter()->pen().widthF();

  QString s = QString::null;
  if (addNewPenColor()){
    d_current_color = painter()->pen().color();
    s = color(d_current_color);
  }

  for (int i = 0; i < pointCount; i++){
    QPointF p = m.map(points[i]);
    if (d_pgf_mode){
      QString path = pgfPoint(convertPoint(p));
      path += pgfPoint(QPointF(lw, lw)) + "\n";
      s += "\\pgfrect[fill]" + path;
    } else {
      QString path = tikzPoint(convertPoint(p));
      path += " rectangle " + tikzPoint(convertPoint(QPointF(p.x() + lw, p.y() + lw))) + ";\n";
      s += "\\fill " + path;
    }
  }

  writeToFile(s);
}

void N::TeXPaintEngine::drawLines ( const QLineF * lines, int lineCount )
{
  if (painter()->pen().style() == Qt::NoPen)
    return;

  QString s;
  for (int i = 0; i < lineCount; i++){
    QPainterPath path(lines[i].p1());
    path.lineTo(lines[i].p2());

    s += drawShape(Line, this->path(path));
  }

  writeToFile(s);
}

void N::TeXPaintEngine::drawPolygon ( const QPointF * points, int pointCount, PolygonDrawMode mode )
{
  if (emptyStringOperation())
    return;

  QVector<QPointF> pts;
  for (int i = 0; i < pointCount; i++)
    pts << points[i];

  QPainterPath path;
  path.addPolygon(QPolygonF(pts));
  if (mode != QPaintEngine::PolylineMode)
    path.closeSubpath ();

  QString s;;
  if (mode != QPaintEngine::PolylineMode){
    path.closeSubpath ();
    s += drawShape(Polygon, this->path(path));
  } else
    s += drawShape(Polyline, this->path(path));

  writeToFile(s);
}

void N::TeXPaintEngine::drawTextItem ( const QPointF & p, const QTextItem & textItem )
{
  QString s = QString::null;
  if (addNewPenColor()){
    s = color(painter()->pen().color());
    d_current_color = painter()->pen().color();
  }

  QMatrix m = painter()->worldMatrix();

  s += "\\pgftext[";

  QPointF origin = p;
  switch(d_horizontal_alignment){
    case Qt::AlignLeft:
      s += "left";
    break;

    case Qt::AlignHCenter:
    case Qt::AlignJustify:
      origin = QPointF(p.x() + 0.5*textItem.width(), p.y());
      s += "center";
    break;

    case Qt::AlignRight:
      origin = QPointF(p.x() + textItem.width(), p.y());
      s += "right";
    break;

    default:
    break;
  }

  s += ", base, at=";
  s += pgfPoint(convertPoint(m.map(origin)));

  if (painter()->transform().isRotating ()){
    double angle = 180.0/M_PI*acos(m.m11());
    if (m.m11() != 0.0 && m.m12() > 0)
      angle = -angle;
    s += ",rotate=" + QString::number(angle);
  }
  s += "]{";

  QFont f = textItem.font();
  if (d_font_size)
    s += "\\fontsize{" + QString::number(int(f.pointSizeF())) + "}{0}\\selectfont{";

  if (f.underline())
    s += "\\underline{";
  if (f.italic())
    s += "\\textit{";
  if (f.bold())
    s += "\\textbf{";
  if (f.strikeOut())
    s += "\\sout{";

  QString text = textItem.text();
  text.remove(QRegExp("~\\"));
  if (d_escape_text){
    text.replace("$", "\\$");
    text.replace("_", "\\_");
    text.replace("{", "\\{");
    text.replace("}", "\\}");
    text.replace("^", "\\^");
    text.replace("&", "\\&");
    text.replace("%", "\\%");
    text.replace("#", "\\#");
  }

  s += text;
  if (d_font_size)
    s += "}";

  if (f.italic())
    s += "}";
  if (f.bold())
    s += "}";
  if (f.underline())
    s += "}";
  if (f.strikeOut())
    s += "}";

  s += "}\n";

  writeToFile(s);
}

void N::TeXPaintEngine::drawRects ( const QRectF * rects, int rectCount )
{
  if (emptyStringOperation())
    return;

  QString s;
  for (int i = 0; i < rectCount; i++){
    QPainterPath path;
    path.addPolygon(QPolygonF(rects[i]));
    s += drawShape(Path, this->path(path));
  }

  writeToFile(s);
}

void N::TeXPaintEngine::drawEllipse ( const QRectF & rect )
{
  if (emptyStringOperation())
    return;

  QPointF p = painter()->worldMatrix().map(rect.bottomLeft());

  QString path;
  if (d_pgf_mode){
    path = pgfPoint(convertPoint(QPointF(p.x() + 0.5*rect.width(), p.y() - 0.5*rect.height())));
    path += pgfPoint(QPointF(0, 0.5*rect.height()*resFactorY()));
    path += pgfPoint(QPointF(0.5*rect.width()*resFactorX(), 0)) + "\n";
  } else {
    path = tikzPoint(convertPoint(QPointF(p.x() + 0.5*rect.width(), p.y() - 0.5*rect.height())));
    path += " ellipse (";
    QString u = unit();
    path += QString::number(0.5*rect.width()*resFactorX()) + u + " and ";
    path += QString::number(0.5*rect.height()*resFactorY()) + u + ");\n";
  }

  writeToFile(drawShape(Ellipse, path));
}

void N::TeXPaintEngine::drawPath ( const QPainterPath & path )
{
  if (emptyStringOperation())
    return;

  writeToFile(drawShape(Path, this->path(path)));
}

QString N::TeXPaintEngine::drawPgfShape(Shape shape, const QString & path)
{
  if (path.isEmpty())
    return QString::null;

  QString stroke_command = path + "\\pgfstroke\n";
  QString fill_command = path + "\\pgffill\n";
  switch(shape){
    case Line:
    case Polygon:
    case Polyline:
    case Path:
    case Points:
    break;

    case Rect:
      stroke_command = "\\pgfrect[stroke]" + path;
      fill_command = "\\pgfrect[fill]" + path;
    break;

    case Ellipse:
      stroke_command = "\\pgfellipse[stroke]" + path;
      fill_command = "\\pgfellipse[fill]" + path;
    break;
  }

  QString s = QString::null;
  if (shape != Line && shape != Polyline && painter()->brush().style() != Qt::NoBrush){
    // fill the background
    s += pgfBrush(painter()->brush());
    s += fill_command;
  }

  if (painter()->pen().style() != Qt::NoPen){// draw the contour
    s += pgfPen(painter()->pen());
    s += stroke_command;
  }

  return s;
}

QString N::TeXPaintEngine::drawShape(Shape shape, const QString & path)
{
  if (d_pgf_mode)
    return drawPgfShape(shape, path);

  return drawTikzShape(shape, path);
}

QString N::TeXPaintEngine::drawTikzShape(Shape shape, const QString & path)
{
  QString s = QString::null;
  if (path.isEmpty())
    return s;

  if (shape != Line && shape != Polyline && painter()->brush().style() != Qt::NoBrush)
    // fill the background
    s += tikzBrush(painter()->brush()) + path;
  if (painter()->pen().style() != Qt::NoPen)// draw the contour
    s += tikzPen(painter()->pen()) + path;

  return s;
}

void N::TeXPaintEngine::drawImage(const QRectF & r, const QImage & image, const QRectF & sr, Qt::ImageConversionFlags flags)
{
  drawPixmap(QPixmap::fromImage(image, flags).copy(sr.toAlignedRect()), r);
}

void N::TeXPaintEngine::drawPixmap(const QRectF &r, const QPixmap &pm, const QRectF &sr)
{
  drawPixmap(pm.copy(sr.toAlignedRect()), r);
}

void N::TeXPaintEngine::drawPixmap(const QPixmap &pix, const QRectF &r)
{
  QFileInfo fi(*file);

  QString base = fi.baseName() + "-images-";
  base += QDateTime::currentDateTime().toString("ddMMyy-hhmmss");
  if (!fi.dir().exists(base)){
    if (!fi.dir().mkdir(base))
      return;
  }

  QString name = fi.dir().absolutePath();
  name += "/" + base + "/image" + QString::number(d_pixmap_index);
  name = QDir::cleanPath(name);
  if (!pix.save(name + ".png", "PNG"))
    return;

  d_pixmap_index++;

  QTextStream t(file);
  t.setCodec("UTF-8");

  t << "\\pgfputat";
  t << pgfPoint(convertPoint(painter()->worldMatrix().map(r.bottomLeft())));
  t << "{\\pgfimage[interpolate=false,width=";

  QString u = unit();
  t << QString::number(r.width()*resFactorX()) + u + ",height=";
  t << QString::number(r.height()*resFactorY()) + u + "]{";
  t << name;
  t << "}}\n";
}

void N::TeXPaintEngine::setUnit(TeXPaintDevice::Unit u)
{
  d_unit = u;
}

void N::TeXPaintEngine::setGrayScale(bool on)
{
  d_gray_scale = on;
}
void N::TeXPaintEngine::setOutputMode(TeXPaintDevice::OutputMode mode)
{
  d_pgf_mode = (mode == TeXPaintDevice::Pgf) ? true : false;
}

void N::TeXPaintEngine::setDocumentMode(bool on)
{
  d_document_mode = on;
}

void N::TeXPaintEngine::setEscapeTextMode(bool on)
{
  d_escape_text = on;
}

void N::TeXPaintEngine::exportFontSizes(bool on)
{
  d_font_size = on;
}

void N::TeXPaintEngine::setTextHorizontalAlignment(Qt::Alignment alignment)
{
  d_horizontal_alignment = alignment;
}

QString N::TeXPaintEngine::clipPath()
{
  if (painter()->hasClipping()){
    QPainterPath path = painter()->clipPath().simplified();
    if (path.elementCount() > 1000)//latex has a limited main memory size
      return QString::null;

    if (d_pgf_mode)
      return pgfPath(path) + "\\pgfclip\n";
    else
      return "\\clip" + tikzPath(path);
  }
  return QString::null;
}

QString N::TeXPaintEngine::defineColor(const QColor& c, const QString& name)
{
  QString col = "\\definecolor{" + name + "}{";
  if (d_gray_scale){
    col += "gray}{";
    double gray = qGray(c.rgb())/255.0;
    col += QString::number(gray) + "}\n";
  } else {
    col += "rgb}{";
    col += QString::number(c.redF()) + ",";
    col += QString::number(c.greenF()) + ",";
    col += QString::number(c.blueF()) + "}\n";
  }
  return col;
}

QString N::TeXPaintEngine::tikzBrush(const QBrush& brush)
{
  QString options = QString::null;
  if (addNewPatternColor()){
    options = defineColor(brush.color(), "c");
    d_pattern_color = brush.color();
  }

  options += "\\fill [pattern color=c, pattern=";

  switch(brush.style()){
    case Qt::NoBrush:
      return QString::null;
    break;

    case Qt::SolidPattern:
    {
      QString s = QString::null;
      if (addNewBrushColor()){
        d_current_color = brush.color();
        s = color(brush.color());
      }

      s += "\\fill";

      double alpha = painter()->brush().color().alphaF();
      if(alpha > 0.0 && alpha < 1.0)
        s += "[opacity=" + QString::number(alpha) + "]";

      return s;
    }
    break;

    case Qt::Dense1Pattern:
    case Qt::Dense2Pattern:
    case Qt::Dense3Pattern:
    case Qt::Dense4Pattern:
      options += "crosshatch dots";
    break;

    case Qt::Dense5Pattern:
    case Qt::Dense6Pattern:
    case Qt::Dense7Pattern:
      options += "dots";
    break;

    case Qt::HorPattern:
      options += "horizontal lines";
    break;
    case Qt::VerPattern:
      options += "vertical lines";
    break;
    case Qt::CrossPattern:
      options += "grid";
    break;
    case Qt::BDiagPattern:
      options += "north east lines";
    break;
    case Qt::FDiagPattern:
      options += "north west lines";
    break;
    case Qt::DiagCrossPattern:
      options += "crosshatch";
    break;

    case Qt::LinearGradientPattern:
    {
      const QLinearGradient *qtgradient = (const QLinearGradient *)brush.gradient();
      QGradientStops stops = qtgradient->stops();

      QString lc = defineColor(stops.first().second, "lc");
      QString rc = defineColor(stops.last().second, "rc");

      QMatrix m = painter()->worldMatrix();
      QPointF sp = m.map(qtgradient->start());
      QPointF ep = m.map(qtgradient->finalStop());

      options = lc + rc + "\\fill [";
      options += "left color=lc, ";
      options += "right color=rc, ";
      options += "shading angle=" + QString::number(-QLineF(sp, ep).angle());
    }
    break;

    case Qt::RadialGradientPattern:
    {
      const QRadialGradient *qtgradient = (const QRadialGradient *)brush.gradient();
      QGradientStops stops = qtgradient->stops();

      QString colors;
      int count = stops.count();
      colors += defineColor(stops[0].second, "c1");
      colors += defineColor(stops[count - 1].second, "c2");

      /*for (int i = 0; i < count; i++){
        QGradientStop stop = stops[i];
        colors += defineColor(stop.second, "c" + QString::number(i + 1));
      }*/

      options = colors + "\\fill [";
      options += "inner color=c1, ";
      options += "outer color=c2, ";
      options += "shading=radial";
      qWarning("QTeXEngine: Uncentered Qt::RadialGradientPattern with more than two colors not supported.");

    }
    break;

    case Qt::ConicalGradientPattern:
    {
      qWarning("QTeXEngine: Qt::ConicalGradientPattern is not supported.");
      return QString::null;
    }
    break;

    default:
    break;
  }
  return options + "]";
}

QString N::TeXPaintEngine::pgfBrush(const QBrush& brush)
{
  QString s = QString::null;
  QColor c = brush.color();
  QString col = defineColor(c, "c");
  QString command = "\\pgfsetfillpattern{";
  switch(brush.style()){
    case Qt::NoBrush:
    break;

    case Qt::SolidPattern:
      s += color(c);
    break;

    case Qt::Dense1Pattern:
    case Qt::Dense2Pattern:
    case Qt::Dense3Pattern:
    case Qt::Dense4Pattern:
      s += col + command + "crosshatch dots}{c}\n";
    break;

    case Qt::Dense5Pattern:
    case Qt::Dense6Pattern:
    case Qt::Dense7Pattern:
      s += col + command + "dots}{c}\n";
    break;

    case Qt::HorPattern:
      s += col + command + "horizontal lines}{c}\n";
    break;
    case Qt::VerPattern:
      s += col + command + "vertical lines}{c}\n";
    break;
    case Qt::CrossPattern:
      s += col + command + "grid}{c}\n";
    break;
    case Qt::BDiagPattern:
      s += col + command + "north east lines}{c}\n";
    break;
    case Qt::FDiagPattern:
      s += col + command + "north west lines}{c}\n";
    break;
    case Qt::DiagCrossPattern:
      s += col + command + "crosshatch}{c}\n";
    break;

    default:
    break;
  }
  return s;
}

QString N::TeXPaintEngine::path(const QPainterPath & path)
{
  if (path.isEmpty ())
    return QString::null;

  if (d_pgf_mode)
    return pgfPath(path);

  return tikzPath(path);
}

QString N::TeXPaintEngine::pgfPath(const QPainterPath & path)
{
  QString s = QString::null;
  int points = path.elementCount();
  QMatrix m = painter()->worldMatrix();
  int curvePoints = 0;
  for (int i = 0; i < points; i++){
    QPainterPath::Element el = path.elementAt(i);
    QPointF p = m.map(QPointF(el.x, el.y));

    switch(el.type){
      case QPainterPath::MoveToElement:
        s += "\\pgfmoveto" + pgfPoint(convertPoint(p)) + "\n";
      break;

      case QPainterPath::LineToElement:
        s += "\\pgflineto" + pgfPoint(convertPoint(p)) + "\n";
      break;

      case QPainterPath::CurveToElement:
        s += "\\pgfcurveto" + pgfPoint(convertPoint(p));
        curvePoints = 0;
      break;

      case QPainterPath::CurveToDataElement:
        s += pgfPoint(convertPoint(p));
        curvePoints++;
        if (curvePoints == 2)
          s += "\n";
      break;
    }
  }
  return s;
}

QString N::TeXPaintEngine::tikzPath(const QPainterPath & path)
{
  QString s = QString::null;
  if (path.isEmpty())
    return s;

  int points = path.elementCount();
  QMatrix m = painter()->worldMatrix();
  int curvePoints = 0;
  for (int i = 0; i < points; i++){
    QPainterPath::Element el = path.elementAt(i);
    QPointF p = m.map(QPointF(el.x, el.y));
    switch(el.type){
      case QPainterPath::MoveToElement:
        s += tikzPoint(convertPoint(p));
      break;

      case QPainterPath::LineToElement:
        s += " -- " + tikzPoint(convertPoint(p));
      break;

      case QPainterPath::CurveToElement:
        s += " .. controls " + tikzPoint(convertPoint(p));
        curvePoints = 0;
      break;

      case QPainterPath::CurveToDataElement:
        curvePoints++;
        if (curvePoints == 1)
          s += " and " + tikzPoint(convertPoint(p));
        else if (curvePoints == 2)
          s += " .. " + tikzPoint(convertPoint(p));
      break;
    }
  }
  return s + ";\n";
}

QPointF N::TeXPaintEngine::convertPoint( const QPointF& p)
{
  return QPointF(resFactorX()*p.x(), paintDevice()->height() - resFactorY()*p.y());
}

double N::TeXPaintEngine::unitFactor()
{
  double factor = 1.0;
  switch (d_unit){
    case TeXPaintDevice::pt:
      factor = 72.27;
    break;
    case TeXPaintDevice::bp:
      factor = 72;
    break;
    case TeXPaintDevice::mm:
      factor = 25.4;
    break;
    case TeXPaintDevice::cm:
      factor = 2.54;
    break;
    case TeXPaintDevice::in:
    case TeXPaintDevice::ex:
    case TeXPaintDevice::em:
    break;
  }
  return factor;
}

double N::TeXPaintEngine::resFactorX()
{
  return unitFactor()/(double)paintDevice()->logicalDpiX();
}

double N::TeXPaintEngine::resFactorY()
{
  return unitFactor()/(double)paintDevice()->logicalDpiY();
}

QString N::TeXPaintEngine::pgfPoint( const QPointF& p)
{
  QString u = unit();
  QString s = "{\\pgfpoint{" + QString::number(p.x());
  s += u + "}{" + QString::number(p.y()) + u + "}}";
  return s;
}

QString N::TeXPaintEngine::tikzPoint(const QPointF & p)
{
  QString u = unit();
  QString s = "(" + QString::number(p.x());
  s += u + "," + QString::number(p.y()) + u + ")";
  return s;
}

QString N::TeXPaintEngine::color( const QColor& col)
{
  QString s = "\\color[";
  if (d_gray_scale){
    s += "gray]{";
    double gray = qGray(col.rgb())/255.0;
    s += QString::number(gray) + "}\n";
  } else {
    s += "rgb]{";
    s += QString::number(col.redF()) + ",";
    s += QString::number(col.greenF()) + ",";
    s += QString::number(col.blueF()) + "}\n";
  }

  return s;
}

QString N::TeXPaintEngine::pgfPen(const QPen& pen)
{
  QString s = QString::null;
  if (pen.style() == Qt::NoPen)
    return s;

  s += color(pen.color());
  s += "\\pgfsetlinewidth{" + QString::number(painter()->pen().widthF()) + "pt}\n";

  QString aux = "\\pgfsetdash{";
  QString term = "}{0cm}\n";

  double space_length = 0.08*pen.widthF();
  double dot_length = 0.3*space_length;
  double dash_length = 1.5*space_length;

  QString dash = "{" + QString::number(dash_length) + "cm}";
  QString dot = "{" + QString::number(dot_length) + "cm}";
  QString space = "{" + QString::number(space_length) + "cm}";

  switch (pen.style()){
    case Qt::SolidLine:
    break;
    case Qt::DashLine:
      s += aux + dash + space + term;
    break;
    case Qt::DotLine:
      s += aux + dot + space + term;
    break;
    case Qt::DashDotLine:
      s += aux + dash + space + dot + space + term;
    break;
    case Qt::DashDotDotLine:
      s += aux + dash + space + dot + space + dot + space + term;
    break;

    case Qt::CustomDashLine:
    {
      s += aux;
      QVector<qreal> pattern = pen.dashPattern();
      int count = pattern.count();
      QString u = unit();
      for (int i = 0; i < count; i++)
        s += "{" + QString::number(pattern[i]) + u + "}";

      s += term;
      break;
    }

    default:
      break;
  }

  switch (pen.joinStyle()){
    case Qt::MiterJoin:
      s += "\\pgfsetmiterjoin\n";
      //s += "\\pgfsetmiterlimit{" + QString::number(pen.miterLimit()) + "pt}\n";
    break;
    case Qt::BevelJoin:
      s += "\\pgfsetbeveljoin\n";
    break;
    case Qt::RoundJoin:
      s += "\\pgfsetroundjoin\n";
    break;
    case Qt::SvgMiterJoin:
      s += "\\pgfsetmiterjoin\n";
    break;
    default:
      break;
  }

  switch (pen.capStyle()){
    case Qt::FlatCap:
      s += "\\pgfsetrectcap\n";
    break;
    case Qt::SquareCap:
      s += "\\pgfsetrectcap\n";
    break;
    case Qt::RoundCap:
      s += "\\pgfsetroundcap\n";
    break;
    default:
      break;
  }

  return s;
}

QString N::TeXPaintEngine::tikzPen(const QPen& pen)
{
  if (pen.style() == Qt::NoPen)
    return QString::null;

  QString col = QString::null;
  if (addNewPenColor()){
    col = color(pen.color());
    d_current_color = pen.color();
  }

  QString options = "[line width=";
  options += QString::number(painter()->pen().widthF()) + "pt, ";

  double space_length = 0.08*pen.widthF();
  double dot_length = 0.3*space_length;
  double dash_length = 1.5*space_length;

  QString dash = "on " + QString::number(dash_length) + "cm";
  QString dot = "on " + QString::number(dot_length) + "cm";
  QString space = " off " + QString::number(space_length) + "cm";

  QString aux = "dash pattern=";
  QString term = ", dash phase=0pt, ";
  switch (pen.style()){
    case Qt::SolidLine:
    break;
    case Qt::DashLine:
      options += aux + dash + space + term;
    break;
    case Qt::DotLine:
      options += aux + dot + space + term;
    break;
    case Qt::DashDotLine:
      options += aux + dash + space + dot + space + term;
    break;
    case Qt::DashDotDotLine:
      options += aux + dash + space + dot + space + dot + space + term;
    break;

    case Qt::CustomDashLine:
    {
      options += aux;
      QVector<qreal> pattern = pen.dashPattern();
      int count = pattern.count();
      QString u = unit();
      for (int i = 0; i < count; i++){
        QString s = "on ";
        if (i%2)
          s = " off ";
        options += s + QString::number(pattern[i]) + u;
      }

      options += term;
      break;
    }

    default:
      break;
  }

  options += "line join=";
  switch (pen.joinStyle()){
    case Qt::MiterJoin:
      options += "miter, ";
    break;
    case Qt::BevelJoin:
      options += "bevel, ";
    break;
    case Qt::RoundJoin:
      options += "round, ";
    break;
    case Qt::SvgMiterJoin:
      options += "miter, ";
    break;
    default:
      break;
  }

  options += "line cap=";
  switch (pen.capStyle()){
    case Qt::FlatCap:
      options += "rect]";
    break;
    case Qt::SquareCap:
      options += "rect]";
    break;
    case Qt::RoundCap:
      options += "round]";
    break;
    default:
      break;
  }

  return col + "\\draw" + options;
}

QString N::TeXPaintEngine::indentString(const QString& s)
{
  QStringList lst = s.split("\n", QString::SkipEmptyParts);
  for(int i = 0; i < lst.count(); i++)
    lst[i].prepend("\t");

  return lst.join("\n") + "\n";
}

QString N::TeXPaintEngine::beginScope()
{
  QString s = "\\begin{scope}\n";
  if (d_pgf_mode)
    s = "\\begin{pgfscope}\n";

  if (painter()->hasClipping()){
    QString clip = clipPath();
    if (!clip.isEmpty())
      s += indentString(clip);
  }

  d_pattern_color = QColor();
  d_current_color = QColor();
  return s;
}

QString N::TeXPaintEngine::endScope()
{
  if (d_pgf_mode)
    return "\\end{pgfscope}\n";

  return "\\end{scope}\n";
}

void N::TeXPaintEngine::writeToFile(const QString& s)
{
  QTextStream t(file);
  t.setCodec("UTF-8");

  if (d_pgf_mode){
    t << beginScope();
    t << indentString(s);
    t << endScope();
    return;
  }

  QString scope;
  if (d_open_scope)
    scope = endScope();

  if (changedClipping()){
    scope += beginScope();
    scope += indentString(s);
    t << scope;

    d_open_scope = true;

    if (painter()->hasClipping())
      d_clip_path = painter()->clipPath();
    else
      d_clip_path = QPainterPath();
  } else
    t << indentString(s);
}

bool N::TeXPaintEngine::emptyStringOperation()
{
  if ((painter()->brush().style() == Qt::NoBrush ||
    (painter()->brush().color().alpha() == 0)) &&
    painter()->pen().style() == Qt::NoPen)
    return true;

  return false;
}

bool N::TeXPaintEngine::changedClipping()
{
  QPainterPath clipPath = QPainterPath();
  if (painter()->hasClipping()){
    if (painter()->clipPath().elementCount() > 1000)
      return false;
    clipPath = painter()->clipPath();
  }

  if (clipPath != d_clip_path)
    return true;

  return false;
}

bool N::TeXPaintEngine::addNewPatternColor()
{
  Qt::BrushStyle style = painter()->brush().style();
  if (style <= Qt::SolidPattern || style >= Qt::LinearGradientPattern)
    return false;

  if (!d_pattern_color.isValid() ||
    d_pattern_color != painter()->brush().color())
    return true;

  return false;
}

bool N::TeXPaintEngine::addNewBrushColor()
{
  if (!d_current_color.isValid() || changedClipping() ||
    d_current_color.name() != painter()->brush().color().name())
    return true;

  return false;
}

bool N::TeXPaintEngine::addNewPenColor()
{
  if (!d_current_color.isValid() ||
    (changedClipping() && painter()->brush().style() == Qt::NoBrush) ||
    d_current_color.name() != painter()->pen().color().name())
    return true;

  return false;
}

QString N::TeXPaintEngine::unit()
{
  switch (d_unit){
    case TeXPaintDevice::pt:
      return "pt";
    break;
    case TeXPaintDevice::bp:
      return "bp";
    break;
    case TeXPaintDevice::mm:
      return "mm";
    break;
    case TeXPaintDevice::cm:
      return "cm";
    break;
    case TeXPaintDevice::in:
      return "in";
    break;
    case TeXPaintDevice::ex:
      return "ex";
    break;
    case TeXPaintDevice::em:
      return "em";
    break;
  }
  return "pt";
}
