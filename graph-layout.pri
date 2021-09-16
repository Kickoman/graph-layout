!isEmpty(GRAPH_LAYOUT_PRI_INCLUDED): error("graph-layout.pri is already included")
GRAPH_LAYOUT_PRI_INCLUDED = 1

# Include dependencies
include($$PWD/libs/GraphGeometry/GraphGeometry.pri)
include($$PWD/libs/kmisc/kmisc.pri)

#include qml
RESOURCES +=    \
    $$PWD/graph-layout-qml.qrc  \
    $$PWD/graph-layout-assets.qrc

INCLUDEPATH += $$PWD/include

CONFIG(debug, debug|release): {
    GRAPH_LAYOUT_OUTPUT = "$$PWD/build/debug"
} else: {
    GRAPH_LAYOUT_OUTPUT = "$$PWD/build/release"
}

message("libgraph-layout will be looked at: $$GRAPH_LAYOUT_OUTPUT/bin")
LIBS += -L"$$GRAPH_LAYOUT_OUTPUT/bin/" -lgraph-layout
