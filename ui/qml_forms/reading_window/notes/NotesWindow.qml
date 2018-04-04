import QtQuick 2.7
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import QtQuick.Controls.Styles 1.4
import QtQuick.Window 2.2

import "../../style"
import "../../custom_items"
import "../../windows_and_forms"

Window {
    id: notesWindow

    // property alias barArea: notesWindowBar.barMouseArea
    // property alias wordForm: wordForm
    signal closeMe

    property var globalPosition
    property var openedNote: QmlCppInterface.openedNote

    // update coordinates when the window was moved
    function updateGlobalPos() {
        globalPosition = mapToGlobal(0, 0)
        notesWindowBar.container.x = x
        notesWindowBar.container.y = y
    }

    height: 300
    width: 400

    flags: Qt.FramelessWindowHint
    //flags: Qt.Window | Qt.WindowStaysOnTopHint
    color: Style.backgroundColor

    Rectangle {
        anchors.left: parent.left
        anchors.leftMargin: 2

        anchors.right: parent.right
        anchors.rightMargin: 2

        anchors.top: notesWindowBar.bottom
        anchors.topMargin: 0

        anchors.bottom: parent.bottom
        anchors.bottomMargin: 2

        color: Style.backgroundColor

        TextArea {
            anchors.fill: parent
            wrapMode: Text.Wrap
            readOnly: true
            textFormat: TextEdit.RichText
            //text: QmlCppInterface.getTableOfnotes()
            //text: "note teeext hereee"
            text: openedNote
        }
    }

    // Bar on the top of the window that allows to move and close the NewWordWindow
    TopBar {
        id: notesWindowBar

        // для cursor position
        container: notesWindow

        //closeArea.onClicked: notesWindow.closeMe()
        closeArea.onClicked: notesWindow.close()

        mainMenuBarButton.visible: false
        readingBarButton.visible: false
        minButton.visible: false
        maxButton.visible: false

        height: 20

        anchors.topMargin: 2
        anchors.leftMargin: 2
        anchors.rightMargin: 2
        barMouseArea.onPositionChanged: {
            globalPosition = mapToGlobal(0, 0)
        }
    } // [TopBar]

    // Just border around the window
    BorderRectangle {
        borderRectangle.border.width: 2
    }
}
