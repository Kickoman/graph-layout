import QtQuick 2.6
import QtQuick.Controls 2.15
import "Graph"
import QtQuick.Shapes 1.12

Rectangle {
    id: root
    width: 1200
    height: 600

    property var graphLayout: null

    Header {
        id: header
        height: 50
        z: 10
        anchors {
            top: parent.top
            right: parent.right
            left: parent.left
        }

        border.width: 0
        color: "#276678"
        title: "QmlApplication"
    }

    Rectangle {
        id: workplace
        color: "#d3e0ea"

        anchors {
            top: header.bottom
            right: controlsScroll.left
            left: parent.left
            bottom: parent.bottom
        }

        ZoomableField {
            id: field
            canvasBackgroundColor: "#f6f5f5"

            anchors.fill: parent

            GraphView {
                id: graphView
                model: graphLayout
//                nodeDelegate: RoundGraphNode { }
                nodeDelegate: RectangeGraphNode { }
            }

            onDoubleClicked: controlsScroll.toggleVisibility()
        }

        Text {
            id: debugText
            text: "Actual canvas size: " + field.width + "x" + field.height +
                  "\nZoom: " + field.zoom +
                  "\nFlickable size: " + field.width + "x" + field.height +
                  "\nPosition: " + field.contentX + ", " + field.contentY +
                  "\nContent size:" + field.contentWidth + "x" + field.contentHeight

            width: 300
            height: 100
            z: 10
            anchors.bottom: parent.bottom
            anchors.left: parent.left
        }

        Slider {
            visible: controlsScroll.visible
            id: zoomSlider
            from: 0.1
            value: field.zoom
            to: 10
            stepSize: 0.1
            onValueChanged: field.setZoom(value, 0, 0)
        }
    }

    ScrollView {
        id: controlsScroll

        width: 300
        clip: true

        function toggleVisibility() {
            if (visible) {
                width = 0
                visible = false
            } else {
                width = 300
                visible = true
            }
        }

        anchors {
            top: header.bottom
            right: parent.right
            bottom: parent.bottom
        }

        contentWidth: 300
        contentHeight: controls.height
        ScrollBar.vertical.policy: ScrollBar.AlwaysOn
        ToolbarControls {
            id: controls

            z: 10

            backgroundColor: "#1687a7"
            border.width: 0

            anchors.fill: parent
            height: 1000


            onGraphRearrangeRequested: {
                graphLayout.setRepulsiveForce(controls.repulsiveFormula)
                graphLayout.setAttractiveForce(controls.attractiveFormula)
                graphLayout.setFrameSize(field.canvasWidth, field.canvasHeight)
                graphLayout.setNodeSize(graphView.getNodeWidth(), graphView.getNodeHeight())
                graphLayout.recalculatePositions()
            }

            onGraphRandomizeRequested: {
                graphLayout.setRandomPositions()
            }
        }
    }
}
