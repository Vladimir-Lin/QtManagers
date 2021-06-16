#include <qtmanagers.h>

N::TeXPaintDevice:: TeXPaintDevice (const QString & fileName,const QSize & s,Unit u)
                  : QPaintDevice   (                                               )
{
  d_size = s;

  if (!d_size.isValid())
    d_size = QSize(500, 400);

    engine = new TeXPaintEngine(fileName, u);
}

N::TeXPaintDevice::~TeXPaintDevice(void)
{
  delete engine ;
}

QPaintEngine * N::TeXPaintDevice::paintEngine () const
{
  return engine;
}

#ifdef QT_PRINTSUPPORT_LIB
void N::TeXPaintDevice::setColorMode(QPrinter::ColorMode mode)
{
  engine->setGrayScale(mode == QPrinter::GrayScale);
}
#endif

void N::TeXPaintDevice::setOutputMode(OutputMode mode)
{
  engine->setOutputMode(mode);
}

void N::TeXPaintDevice::setUnit(Unit u)
{
  engine->setUnit(u);
}

void N::TeXPaintDevice::setSize(const QSize& s)
{
  d_size = s;
}

void N::TeXPaintDevice::setDocumentMode(bool on)
{
  engine->setDocumentMode(on);
}

void N::TeXPaintDevice::setEscapeTextMode(bool on)
{
  engine->setEscapeTextMode(on);
}

void N::TeXPaintDevice::exportFontSizes(bool on)
{
  engine->exportFontSizes(on);
}

void N::TeXPaintDevice::setTextHorizontalAlignment(Qt::Alignment alignment)
{
  engine->setTextHorizontalAlignment(alignment);
}

int N::TeXPaintDevice::metric ( PaintDeviceMetric metric ) const
{
  QDesktopWidget *desktop = QApplication::desktop();
  int dpi_x = desktop->logicalDpiX();
  int dpi_y = desktop->logicalDpiY();
  switch (metric){
    case QPaintDevice::PdmWidth:
      return d_size.width();
    case QPaintDevice::PdmHeight:
      return d_size.height();
    case QPaintDevice::PdmWidthMM:
      return int(25.4*d_size.width()/(double)dpi_x);
    case QPaintDevice::PdmHeightMM:
      return int(25.4*d_size.height()/(double)dpi_y);
    case QPaintDevice::PdmNumColors:
      return 65536;//should it be millions?
    case QPaintDevice::PdmDepth:
      return 32;
    case QPaintDevice::PdmDpiX:
    case QPaintDevice::PdmPhysicalDpiX:
      return dpi_x;
    case QPaintDevice::PdmDpiY:
    case QPaintDevice::PdmPhysicalDpiY:
      return dpi_y;
    default:
      qWarning ("N::TeXPaintDevice::Unknown metric asked");
      return 0;
  }
}
