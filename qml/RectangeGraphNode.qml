import QtQuick 2.0
import "Graph"

GraphNode {
    id: root

    height: 100
    width: 250

    Rectangle {
        anchors.fill: parent

        color: "#1687a7"
        radius: 5

        Text {
            text: model.data
            anchors.fill: parent
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            fontSizeMode: Text.Fit
            minimumPixelSize: 5
            font.pixelSize: 50
        }
    }
}
