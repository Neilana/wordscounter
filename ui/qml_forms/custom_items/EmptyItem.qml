import QtQuick 2.7

import "../style"

Item {

    property alias imageSource: imageId.source
    property alias text: textId.text
    width: 100
    height: 200

    Image {
        anchors.horizontalCenter: parent.horizontalCenter
        width: parent.width * 0.4

        id: imageId

        horizontalAlignment: Image.AlignHCenter
        verticalAlignment: Image.AlignVCenter

        source: "/icons/bookmarks2.png"

        fillMode: Image.PreserveAspectFit
    }

    Text {
        id: textId

        anchors.top: imageId.bottom
        anchors.topMargin: 10
        width: parent.width

        text: "No bookmarks here."

        wrapMode: Text.Wrap

        color: Style.textColor
        font.pixelSize: Style.contentsFontPixelSize
        font.family: Style.tableFont.name

        horizontalAlignment: Text.AlignHCenter
    }
}
