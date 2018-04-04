import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3
import QtQuick.Window 2.2
import QtQuick.Dialogs 1.2

import "../style"
import "../custom_items"
import "../menu_pages"
import "../windows_and_forms"

Item {
    property Window parentWindow

    property alias borderRectangle: borderRectangle

    width: parent.width
    height: parent.height

    Rectangle {
        id: borderRectangle
        color: "#00000000"
        anchors.rightMargin: 0
        anchors.bottomMargin: 0
        anchors.leftMargin: 0
        anchors.topMargin: 0
        z: 2
        visible: true
        clip: false
        border.color: Style.borderColor
        anchors.fill: parent
        border.width: Style.borderWidth

        MouseArea {
            id: mouseArea

            width: 20
            height: 20

            anchors.right: parent.right
            anchors.rightMargin: 0

            anchors.bottom: parent.bottom
            anchors.bottomMargin: 0

            property variant clickPos: "1,1"
            onPressed: {
                clickPos = Qt.point(mouse.x, mouse.y)
            }
            onReleased: {

                    QmlCppInterface.setPageSize(
                                bookPageWindow.pageView.width - 15,
                                bookPageWindow.pageView.height - 30)

            }
            onPositionChanged: {
                var delta = Qt.point(mouse.x - clickPos.x, mouse.y - clickPos.y)
                if (parentWindow != null) {
                    if (parentWindow.width + delta.x > 600)
                        parentWindow.width += delta.x
                    if (parentWindow.height + delta.y > 400)
                        parentWindow.height += delta.y
                }
            }
        }
    }
}
