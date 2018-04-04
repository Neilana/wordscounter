import QtQuick 2.7
import QtQuick.Layouts 1.3

import "../style"
import "../custom_items"
import "../menu_pages"
import "../windows_and_forms"

Item {
    width: 150
    height: 50

    property alias title: textTitle.text
    property alias textInput: textInput
    ColumnLayout {
        id: column
        anchors.fill: parent

        Text {
            id: textTitle
            height: 20
            color: Style.textColor
            text: qsTr("Text")
            font.capitalization: Font.AllUppercase
            fontSizeMode: Text.VerticalFit
            verticalAlignment: Text.AlignVCenter
            Layout.fillWidth: true

            anchors.right: parent.right
            anchors.rightMargin: 3
            anchors.left: parent.left
            anchors.leftMargin: 3

            font.bold: true
            font.pixelSize: 14
            font.family: Style.tableFont.name
        }

        Rectangle {
            id: rectangle
            height: 20
            Layout.fillWidth: true
            Layout.fillHeight: true
            color: Style.topBarBackgroundColor
            clip: true
            border.color: Style.topSelectedColor

            anchors.right: parent.right
            anchors.rightMargin: 0
            anchors.left: parent.left
            anchors.leftMargin: 0

            TextEdit {
                id: textInput
                color: Style.textColor
                font.pixelSize: 16
                clip: true

                anchors.fill: parent

                cursorVisible: false
                font.family: Style.tableFont.name
            }
        }
    }
}
