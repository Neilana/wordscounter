import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3

import "../style"
import "../custom_items"
import "../menu_pages"
import "../windows_and_forms"

Item {
    height: 40
    width: 100

    property alias text: text1.text
    property alias area: mouseArea
    property alias borderRect: rectangle

    property bool wasPressed: false

    Button {
        id: button
        anchors.fill: parent
        padding: 0

        Rectangle {
            id: rectangle
            //color: mouseArea.containsMouse ? Style.highlightHoverMenuColor : Style.backgroundColor
            color: {
                if (wasPressed)
                    return Style.mainMenuSelectedColor
                if (mouseArea.containsMouse)
                    return Style.highlightHoverMenuColor
                return Style.backgroundColor
            }

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
                //font.family: "/fonts/Cantarell-Regular.ttf"
                //font.family: "/fonts/LeagueSpartan-Bold"
                font.family: Style.subMenuFont.name
                color: Style.textColor
                horizontalAlignment: Text.AlignHCenter
                font.pixelSize: parent.width / 5

                fontSizeMode: Text.Fit
                //                Layout.minimumWidth: 60
                Layout.columnSpan: 1
                Layout.rowSpan: 1
                Layout.fillWidth: true
                Layout.fillHeight: true
                //Layout.preferredHeight: 23
                //Layout.preferredWidth: 47
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
    }
}
