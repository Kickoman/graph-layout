import QtQuick 2.0
import QtQml.Models 2.12
import QtQml 2.0

// Main Field
Rectangle {
    id: root
    color: "gray"
    anchors.fill: parent

    property alias nodeDelegate: nodesRepeater.delegate
    property alias edgeDelegate: linesRepeater.delegate
    property var model: null

    function getNodeX(index) {
        let item = nodesRepeater.itemAt(index);
        if (item === null)
            return 0;
        return item.x + item.width / 2;
    }

    function getNodeY(index) {
        let item = nodesRepeater.itemAt(index);
        if (item === null)
            return 0;
        return item.y + item.height / 2;
    }

    function getNodeWidth() {
        let width = 0;
        if (nodesRepeater.count > 0)
            width = nodesRepeater.itemAt(0).width
        return width
    }

    function getNodeHeight() {
        let height = 0;
        if (nodesRepeater.count > 0)
            height = nodesRepeater.itemAt(0).height
        return height
    }

    Grid {
        id: mainGrid
        anchors.fill: parent

        property int squareSize: 50

        spacing: 0
        rows: parent.height / squareSize
        columns: parent.width / squareSize

        Repeater {
            model: mainGrid.rows * mainGrid.columns

            Rectangle {
                width: mainGrid.squareSize
                height: mainGrid.squareSize

                function getRowNumber() { return Math.floor(index / mainGrid.columns) }
                function getColNumber() { return index - (mainGrid.columns * getRowNumber()) }

                function getColor() {
                    var rowNumber = getRowNumber()
                    var colNumber = getColNumber()

                    if (rowNumber % 2 == 0)
                        return colNumber % 2 ? "#f6f5f5" : "#d3e0ea"
                    return colNumber % 2 ? "#d3e0ea" : "#f6f5f5"
                }

                color: getColor()
            }
        }
    }

    Repeater {
        id: nodesRepeater
        model: root.model.nodesModel
        onCountChanged: {
            linesRepeater.model = null
            linesRepeater.model = root.model.edgesModel
        }
    }

    Repeater {
        id: linesRepeater
        model: root.model.edgesModel
        delegate: GraphLine {
            z: parent.z + 1
            startX: getNodeX(model.startNodeIndex)
            startY: getNodeY(model.startNodeIndex)
            finishX: getNodeX(model.finishNodeIndex)
            finishY: getNodeY(model.finishNodeIndex)
        }
    }

    onHeightChanged: model.setFrameSize(root.width, root.height)
    onWidthChanged: model.setFrameSize(root.width, root.height)
}
