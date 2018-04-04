import QtQuick 2.7
import QtQuick.Layouts 1.3

import "../../style"

Item {
    property alias imageSource: imageId.source
    property alias mouseArea: mouseAreaId

    width: 25
    height: 25

    Rectangle {
        id: minButton
        anchors.fill: parent
        color: mouseAreaId.containsMouse ? Style.topBarHighlightHoverColor : Style.topBarBackgroundColor

        MouseArea {
            id: mouseAreaId

            anchors.fill: parent

            hoverEnabled: true
            propagateComposedEvents: true
        }

        Image {
            id: imageId

            anchors.fill: parent

            anchors.rightMargin: 2
            anchors.leftMargin: 2
            anchors.bottomMargin: 2
            anchors.topMargin: 2

            sourceSize.width: 0
            fillMode: Image.PreserveAspectFit
            source: "/icons/minimize.png"
        }
    } // [Rectangle]
} // [Item]
