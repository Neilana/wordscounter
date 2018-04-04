import QtQuick 2.7
import QtQuick.Layouts 1.3

import "../style"
import "../custom_items"

Item {
    width: 100
    height: 40

    property alias image: image.source
    property alias text: text1.text
    property alias textSize: text1.font.pixelSize

    property alias textColor: text1.color
    property alias area: mouseArea
    property alias rectangleColor: rectangle.color

    property bool wasPressed: false

    Rectangle {
        id: rectangle

        anchors.fill: parent

        color: {
            if (mouseArea.containsMouse)
                return Style.highlightHoverMenuColor
            return Style.backgroundColor
        }

        MouseArea {
            id: mouseArea

            anchors.fill: parent
            hoverEnabled: true
            onClicked: wasPressed = !wasPressed
        }

        RowLayout {
            anchors.rightMargin: 5
            anchors.leftMargin: 5
            anchors.fill: parent
            spacing: 2

            Image {
                id: image

                source: "../icons/openFile.png"
                fillMode: Image.PreserveAspectFit

                Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                Layout.fillWidth: true
                Layout.fillHeight: true
                Layout.minimumWidth: 30
            }

            Text {
                id: text1

                elide: Text.ElideNone
                wrapMode: Text.WordWrap
                color: Style.textColor
                textFormat: Text.PlainText
                horizontalAlignment: Text.AlignHCenter

                font.family: Style.subMenuFont.name
                font.pixelSize: parent.width * 0.13
                font.capitalization: Font.AllUppercase
                fontSizeMode: Text.FixedSize

                Layout.minimumWidth: 60
                Layout.columnSpan: 1
                Layout.rowSpan: 1
                Layout.fillWidth: true
                Layout.fillHeight: true

                verticalAlignment: Text.AlignVCenter
                padding: 0
            }
        } // [RowLayout]
    } // [Rectangle]
} // Item
