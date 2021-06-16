#include <qtmanagers.h>

QStringList N::Images::ExtensionFilters (void)
{
  QStringList       filters                      ;
  QList<QByteArray> IR                           ;
  IR = QImageReader::supportedImageFormats()     ;
  for (int i=0;i<IR.count();i++)                 {
    QString LC = QString::fromUtf8(IR[i])        ;
    filters << QString("*.%1").arg(LC.toLower()) ;
    filters << QString("*.%1").arg(LC.toUpper()) ;
  }                                              ;
  return filters                                 ;
}

QString N::Images::ImageFilters (void)
{
  QString     filter                             ;
  QStringList filters                            ;
  QStringList items                              ;
  QList<QByteArray> IR                           ;
  IR = QImageReader::supportedImageFormats()     ;
  for (int i=0;i<IR.count();i++)                 {
    QString LC = QString::fromUtf8(IR[i])        ;
    filters << QString("*.%1").arg(LC.toLower()) ;
    filters << QString("*.%1").arg(LC.toUpper()) ;
    items   << QString("%1 (*.%2 *.%3)"          )
               .arg(LC                           )
               .arg(LC.toLower()                 )
               .arg(LC.toUpper()               ) ;
  }                                              ;
  filter  = QObject::tr("All image formats (%1)" )
                    .arg(filters.join(" ")     ) ;
  filter += ";;"                                 ;
  filter += items.join(";;")                     ;
  return filter                                  ;
}
