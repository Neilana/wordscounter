import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3
import QtQuick.Window 2.2

import "./style"
import "./custom_items"
import "./custom_items/top_bar"
import "./menu_pages"
import "./windows_and_forms"
import "./reading_window"

ApplicationWindow {
    property Window exitDialog
    property bool bookOpened: QmlCppInterface.bookOpened

    id: rootWindow

    width: 1000
    height: 660

    visible: true
    title: qsTr("Words Counter")
    color: Style.backgroundColor
    flags: Qt.FramelessWindowHint | Qt.WindowMinimizeButtonHint | Qt.Window

    TopBar {
        id: topBar

        signal moved

        // custom properties
        parentWindow: rootWindow
        container: rootWindow
        barMouseArea.onPositionChanged: topBar.moved()
        readingBarButton.visible: bookOpened
        programStatusRect.visible: true

        closeArea.onClicked: {
            if (exitDialog === null) {
                var component
                //var exitDialog
                component = Qt.createComponent(
                            "/windows_and_forms/ExitDialog.qml")
                exitDialog = component.createObject(rootWindow)
                if (exitDialog !== null) {
                    exitDialog.clickedYes.connect(closeWindow)
                    exitDialog.clickedNo.connect(closeExitDialog)
                }
            } else {
                exitDialog.hide()
                exitDialog.showNormal()
            }
        }

        function closeWindow() {
            rootWindow.close()
        }
        function closeExitDialog() {
            exitDialog.close()
            exitDialog = null
        }
    }

    StackLayout {
        id: rootStackLayout

        anchors.right: parent.right
        anchors.left: parent.left
        anchors.bottom: parent.bottom
        anchors.top: topBar.bottom

        anchors.rightMargin: Style.borderWidth
        anchors.leftMargin: Style.borderWidth
        anchors.bottomMargin: Style.borderWidth
        anchors.topMargin: 0

        clip: true

        MainMenu {
            id: mainMenu
            anchors.fill: parent
            clip: true
        }

        ReadingWindow {
            id: bookPageWindow
            anchors.fill: parent
        }
    }

    BorderRectangle {
        parentWindow: rootWindow
    }
}
