NAME         = QtManagers
TARGET       = $${NAME}

QT           = core
QT          += gui
QT          += network
QT          += sql
QT          += script
QT          += positioning
QT          += widgets
QT          += opengl
QT          += printsupport
QT          += multimedia
QT          += multimediawidgets
QT          += opengl
QT          += QtCUDA
QT          += QtOpenCV
QT          += Essentials
QT          += QtCalendar
QT          += QtGMP
QT          += QtGSL
QT          += QtAlgebra
QT          += QtDiscrete
QT          += QtFFT
QT          += Mathematics
QT          += QtFuzzy
QT          += QtFLP
QT          += QtFoundation
QT          += QtGeometry
QT          += QtGadgets
QT          += QtWorld
QT          += QtComponents

load(qt_build_config)
load(qt_module)

INCLUDEPATH += $${PWD}/../../include/$${NAME}

HEADERS     += $${PWD}/../../include/$${NAME}/qtmanagers.h

include ($${PWD}/Retrievers/Retrievers.pri)
include ($${PWD}/Images/Images.pri)
include ($${PWD}/Gadgets/Gadgets.pri)
include ($${PWD}/Snaps/Snaps.pri)
include ($${PWD}/Painters/Painters.pri)
include ($${PWD}/TimeWidgets/TimeWidgets.pri)
include ($${PWD}/Text/Text.pri)
include ($${PWD}/Managers/Managers.pri)
include ($${PWD}/Multimedia/Multimedia.pri)
include ($${PWD}/Languages/Languages.pri)
include ($${PWD}/Scene/Scene.pri)
include ($${PWD}/Blobs/Blobs.pri)

OTHER_FILES += $${PWD}/../../include/$${NAME}/headers.pri

include ($${PWD}/../../doc/Qt/Qt.pri)

TRNAME       = $${NAME}
include ($${PWD}/../../Translations.pri)


CONFIG(debug,debug|release) {
  LIBS      += -lzlibd
  LIBS      += -llibpngd
  LIBS      += -llibjpegd
  LIBS      += -ltiffd
} else {
  LIBS      += -lzlib
  LIBS      += -llibpng
  LIBS      += -llibjpeg
  LIBS      += -ltiff
}

# LIBS        += -lippicvmt

win32 {

OPENCVVER    = 310

CONFIG(debug,debug|release) {
  CONFIG(static,static|shared) {
    LIBS    += -lIlmImfd
    LIBS    += -llibjasperd
    LIBS    += -llibwebpd
#    LIBS    += -lopencv_ts$${OPENCVVER}d
    LIBS    += -lopencv_world$${OPENCVVER}d
  } else {
#    LIBS    += -lopencv_ts$${OPENCVVER}d
    LIBS    += -lopencv_world$${OPENCVVER}d
  }
} else {
  CONFIG(static,static|shared) {
    LIBS    += -lIlmImf
    LIBS    += -llibjasper
    LIBS    += -llibwebp
#    LIBS    += -lopencv_ts$${OPENCVVER}
    LIBS    += -lopencv_world$${OPENCVVER}
  } else {
#    LIBS    += -lopencv_ts$${OPENCVVER}
    LIBS    += -lopencv_world$${OPENCVVER}
  }
}

}
