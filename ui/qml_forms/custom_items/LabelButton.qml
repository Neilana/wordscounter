import QtQuick 2.7
import QtQuick.Layouts 1.3

import "../style"

Item {
    height: 40
    width: 100

    property alias text: text1
    property alias area: mouseArea
    property alias borderRect: rectangle

    property bool wasPressed: false

    Rectangle {
        id: rectangle
        color: Style.backgroundColor
        anchors.fill: parent
        border.width: 1
        border.color: Style.buttonBorderColor
        Text {
            id: text1
            text: qsTr("Open file")
            font.capitalization: Font.AllUppercase
            anchors.top: parent.top
            anchors.topMargin: 2
            anchors.left: parent.left
            anchors.leftMargin: 2
            anchors.right: parent.right
            anchors.rightMargin: 2
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 2

            font.family: Style.subMenuFont.name

            color: {
                if (mouseArea.containsMouse)
                    return "#D35400" //orange2 FIXME
                if (wasPressed)
                    return Style.mainMenuSelectedColor
                return Style.textColor
            }

            horizontalAlignment: Text.AlignHCenter
            font.pixelSize: parent.width / 7

            fontSizeMode: Text.Fit

            Layout.columnSpan: 1
            Layout.rowSpan: 1
            Layout.fillWidth: true
            Layout.fillHeight: true

            verticalAlignment: Text.AlignVCenter
            padding: 0
        }

        MouseArea {
            id: mouseArea
            anchors.fill: parent

            hoverEnabled: true
            propagateComposedEvents: true

            //                // иначе мышка будет "съедать" события и автоматеческое переключение tabButton'ов не будет работать
            //                onClicked: mouse.accepted = false
            //                onPressed: mouse.accepted = false
            //                onReleased: mouse.accepted = false
            //                onDoubleClicked: mouse.accepted = false
            //                onPositionChanged: mouse.accepted = false
            //                onPressAndHold: mouse.accepted = false
        }
    }
    //}
}
