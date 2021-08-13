import QtQuick 2.0
import "Graph"

GraphNode {
    id: root

    height: 50
    width: 100

    Rectangle {
        anchors.fill: parent

        color: "#E7DFC6"
        radius: 5

        border.color: "#131B23"

        Text {
            text: model.data
            color: "#131B23"
            anchors.fill: parent
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            fontSizeMode: Text.Fit
            minimumPixelSize: 5
            font.pixelSize: 14
        }
    }
}
