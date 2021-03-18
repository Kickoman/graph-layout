import QtQuick 2.0

Item {

    z: parent.z + 2
    x: isDragged ? x : translateXFromModel(model.x)
    y: isDragged ? y : translateYFromModel(model.y)

    property bool animated: getAnimated()

    function getAnimated() {
        return animatedX || animatedY
    }

    function translateXToModel(x) { return x + width / 2 }
    function translateYToModel(y) { return y + height / 2 }
    function translateXFromModel(x) { return x - width / 2 }
    function translateYFromModel(y) { return y - height / 2 }

    function updateModelX() { model.x = translateXToModel(x) }
    function updateModelY() { model.y = translateYToModel(y) }

    property bool animatedX: xAnimation.running
    property bool animatedY: yAnimation.running
    property bool isDragged: false

    onXChanged: if (isDragged) updateModelX()
    onYChanged: if (isDragged) updateModelY()
    onIsDraggedChanged: {
        if (!isDragged) {
            updateModelX()
            updateModelY()
        }
    }

    MouseArea {
        id: liverbirdMouseArea
        property real lastZ: parent.z
        anchors.fill: parent

        drag.target: parent
        drag.axis: Drag.XandYAxis

        onReleased: {
            parent.z = lastZ
            parent.isDragged = false
        }
        onPressed: {
            lastZ = parent.z
            parent.z = 1000
            parent.isDragged = true
        }
    }

    Behavior on x {
        enabled: !parent.isDragged
        SmoothedAnimation {
            id: xAnimation
            velocity: 100
        }
    }
    Behavior on y {
        enabled: !parent.isDragged
        SmoothedAnimation {
            id: yAnimation
            velocity: 100
        }
    }
}
