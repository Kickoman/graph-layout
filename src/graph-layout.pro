TEMPLATE = lib
QT += core
CONFIG += c++11
TARGET = graph-layout

HEADERS += \
    ../include/graphcalculator.h   \
    ../include/graphlayout.h       \
    ../include/graphlineproxymodel.h   \
    ../include/graphnodesproxymodel.h  \
    ../include/igraph.h

SOURCES += \
    graphcalculator.cpp \
    graphlayout.cpp     \
    graphlineproxymodel.cpp \
    graphnodesproxymodel.cpp    \

include(../libs/GraphGeometry/GraphGeometry.pri)
include(../libs/kmisc/kmisc.pri)

CONFIG(debug, debug|release): {
    OUTPUT_DIR = "$$PWD/../build/debug"
} else: {
    OUTPUT_DIR = "$$PWD/../build/release"
}

OBJECTS_DIR = "$$OUTPUT_DIR/obj/"
MOC_DIR     = "$$OUTPUT_DIR/moc/"
RCC_DIR     = "$$OUTPUT_DIR/res/"
UI_DIR      = "$$OUTPUT_DIR/ui/"
DESTDIR     = "$$OUTPUT_DIR/bin/"

message("libgraph-layout library (.so) will be placed at: $$DESTDIR")

INCLUDEPATH += ../include/
