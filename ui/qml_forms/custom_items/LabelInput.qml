import QtQuick 2.7
import QtQuick.Layouts 1.3

import "../style"
import "../custom_items"
import "../menu_pages"
import "../windows_and_forms"

Item {
    width: 150
    height: 60

    property alias title: textTitle.text

    property alias textInput: textInput.text
    property alias textInputItem: textInput

    property alias textInputPlaceholder: textInput.placeholderText

    ColumnLayout {
        id: column
        anchors.fill: parent
        spacing: 0

        Rectangle {
            width: parent.width
            height: childrenRect.height + 10
            Layout.fillWidth: true
            color: Style.topBarBackgroundColor

            Text {
                id: textTitle

                anchors.top: parent.top
                anchors.topMargin: 5

                anchors.left: parent.left
                anchors.leftMargin: 5

                anchors.right: parent.right
                anchors.rightMargin: 5

                text: "Word"
                font.bold: true
                font.pixelSize: 17
                font.family: Style.tableFont.name

                color: Style.textColor
                //font.capitalization: Font.SmallCaps
                font.capitalization: Font.AllUppercase
            }
        }

        Rectangle {
            id: rectangle
            height: 40
            Layout.fillWidth: true
            Layout.fillHeight: true
            clip: true

            //color: Style.topBarBackgroundColor
            color: "transparent"

            //border.color: Style.topSelectedColor
            anchors.right: parent.right
            anchors.rightMargin: 0
            anchors.left: parent.left
            anchors.leftMargin: 0

            TextEdit {
                property string placeholderText: "Enter text here..."
                id: textInput

                Text {
                    text: textInput.placeholderText

                    // FIXME : color into property
                    color: "#c1bcb6"
                    visible: !textInput.text
                    font.family: Style.tableFont.name
                    font.pixelSize: 16
                }

                color: Style.textColor
                font.pixelSize: 16
                clip: true

                //                anchors.fill: parent
                anchors.top: parent.top
                anchors.topMargin: 5

                anchors.left: parent.left
                anchors.leftMargin: 5

                anchors.right: parent.right
                anchors.rightMargin: 5

                anchors.bottom: parent.bottom
                anchors.bottomMargin: 5

                wrapMode: TextEdit.Wrap
                cursorVisible: false

                font.family: Style.tableFont.name

                selectByMouse: true
                selectByKeyboard: true

                selectionColor: Style.topBarBackgroundColor
                //selectionColor: Style.topSelectedColor
                selectedTextColor: Style.textColor
            }
        }
    }
}
