import QtQuick 2.0

import "../style"

Item {
    width: 23
    height: 400

    property alias imageSource: image.source
    property alias area: mouseArea

    Rectangle {
        id: rectangle
        //color: mouseArea.containsMouse ? Style.longButtonColor : Style.backgroundColor
        color: mouseArea.containsMouse ? Style.topBarHighlightHoverColor : Style.longButtonColor
        anchors.fill: parent

        Image {
            id: image
            //height: 100
            anchors.top: parent.top
            anchors.topMargin: parent.height / 2 - image.height / 2

            anchors.right: parent.right
            anchors.rightMargin: 2
            anchors.left: parent.left
            anchors.leftMargin: 2
            fillMode: Image.PreserveAspectFit
            source: "/icons/rightArrow.png"
        }

        MouseArea {
            id: mouseArea
            hoverEnabled: true
            anchors.fill: parent
        }
    }
}
