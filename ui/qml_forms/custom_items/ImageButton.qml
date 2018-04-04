import QtQuick 2.7

import "../style"

Item {
    height: 40
    width: 100

    property alias image: image
    property bool wasPressed: false

    Rectangle {
        id: button

        anchors.fill: parent

        color: {
            if (wasPressed)
                return Style.mainMenuSelectedColor
            if (mouseArea.containsMouse)
                return Style.highlightHoverMenuColor
            return Style.backgroundColor
        }

        Image {
            id: image

            anchors.fill: parent

            sourceSize.width: 0
            fillMode: Image.PreserveAspectFit
            source: "/icons/home.png"
        }

        MouseArea {
            id: mouseArea

            anchors.fill: parent

            hoverEnabled: true
            propagateComposedEvents: true

            // иначе мышка будет "съедать" события и автоматеческое переключение tabButton'ов не будет работать
            onClicked: mouse.accepted = false
            onPressed: mouse.accepted = false
            onReleased: mouse.accepted = false
            onDoubleClicked: mouse.accepted = false
            onPositionChanged: mouse.accepted = false
            onPressAndHold: mouse.accepted = false
        }
    }
}
