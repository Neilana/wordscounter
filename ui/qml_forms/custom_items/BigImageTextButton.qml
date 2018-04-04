import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3

import "../style"
import "../custom_items"
import "../menu_pages"
import "../windows_and_forms"

Item {
    height: 70
    width: 100

    property alias image: image.source
    property alias buttonText: text1
    property alias area: mouseArea
    property alias rectangleColor: rectangle.color

    property bool wasPressed: false

    Button {
        id: button
        anchors.fill: parent
        padding: 0

        Rectangle {
            id: rectangle
            color: {
                if (mouseArea.containsMouse)
                    return Style.highlightHoverMenuColor
                if (wasPressed)
                    return Style.mainMenuSelectedColor
                return Style.backgroundColor
            }
            border.color: "#00000000"

            anchors.fill: parent

            Image {
                id: image
                anchors.bottom: text1.top
                anchors.bottomMargin: 3
                anchors.right: parent.right
                anchors.left: parent.left
                anchors.top: parent.top
                anchors.rightMargin: 3
                anchors.leftMargin: 3
                anchors.topMargin: 5
                sourceSize.width: 0
                fillMode: Image.PreserveAspectFit
                source: "/icons/openFile.png"
            }

            MouseArea {
                id: mouseArea
                anchors.fill: parent

                hoverEnabled: true
                propagateComposedEvents: true
                onClicked: wasPressed = !wasPressed
            }

            Text {
                id: text1
                text: qsTr("Text")
                font.capitalization: Font.MixedCase
                fontSizeMode: Text.VerticalFit
                bottomPadding: 0
                rightPadding: 0
                topPadding: 0
                font.letterSpacing: 1
                color: Style.textColor
                //  font.family: "/fonts/Cantarell-Regular.ttf"
                //font.family: spartanFont.name
                font.family: Style.mainMenuFont.name

                anchors.right: parent.right
                anchors.rightMargin: 3
                anchors.left: parent.left
                anchors.leftMargin: 3
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 3
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                //font.pixelSize: 14
                font.pixelSize: parent.height * 0.2
                visible: mouseArea.containsMouse
            }
        }
    }
}
