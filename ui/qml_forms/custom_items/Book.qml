import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3

import "../style"
import "../custom_items"
import "../menu_pages"
import "../windows_and_forms"

Item {
    id: bookItem
    width: 500
    //height: 850
    height: width * 1.5 + 50

    property alias coverImage: coverImage
    property alias authorText: authorText.text
    property alias titleText: titleText.text

    property alias bookBorder: bookBorder.borderRectangle
    BorderRectangle {
        id: bookBorder
        //visible: mouseArea.containsMouse
        //z: 3
        borderRectangle.border.color: mouseArea.containsMouse ? Style.highlightHoverMenuColor : Style.borderColor
    }
    ColumnLayout {
        anchors.rightMargin: Style.borderWidth
        anchors.leftMargin: Style.borderWidth
        anchors.bottomMargin: Style.borderWidth
        anchors.topMargin: Style.borderWidth
        spacing: 0
        anchors.fill: parent
        Image {
            id: coverImage
            x: 0
            //width: 300
            //height: 600
            Layout.minimumHeight: 4
            Layout.fillHeight: true
            Layout.fillWidth: true

            anchors.top: parent.top
            anchors.topMargin: 0
            fillMode: Image.Stretch
            source: "../icons/bookExample.jpg"
        }

        Text {
            id: authorText
            x: 0
            //width: 200
            height: 25

            text: qsTr("Author")
            wrapMode: Text.WrapAtWordBoundaryOrAnywhere
            font.bold: true
            font.pointSize: bookItem.width * 0.06

            Layout.minimumHeight: 25
            Layout.maximumHeight: 25
            //Layout.fillHeight: true
            Layout.fillWidth: true

            anchors.top: coverImage.bottom
            anchors.topMargin: 0

            fontSizeMode: Text.Fit
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            font.family: Style.tableFont.name
            color: Style.textColor

            elide: Text.ElideRight
            padding: 5
        }

        Text {
            id: titleText
            x: 0
            //width: 200
            height: 25
            elide: Text.ElideRight

            //FIXME: fixed heights
            text: qsTr("Title")
            wrapMode: Text.WrapAtWordBoundaryOrAnywhere
            font.pointSize: bookItem.width * 0.06
            renderType: Text.QtRendering

            Layout.maximumHeight: 25
            Layout.minimumHeight: 25
            //Layout.fillHeight: true
            Layout.fillWidth: true

            font.weight: Font.Normal
            font.family: Style.tableFont.name
            color: Style.textColor

            anchors.top: authorText.bottom
            anchors.topMargin: 0

            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            fontSizeMode: Text.VerticalFit

            padding: 5
        }
    }

    MouseArea {
        id: mouseArea
        anchors.fill: parent
        hoverEnabled: true
    }
}
