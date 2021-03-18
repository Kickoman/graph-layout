import QtQuick 2.0
import "Graph"

GraphNode {
    id: root

    height: 70
    width: 70
    Rectangle {
        anchors.fill: parent

        color: "#1687a7"

        radius: 30

        Text {
            text: model.data
            anchors.centerIn: parent
        }

        Text {
            text: "{" + root.x + "; " + root.y + "}"
            anchors.bottom: parent.bottom
        }
    }
}
