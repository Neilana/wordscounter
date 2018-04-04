import QtQuick 2.7
import QtQuick.Window 2.2

import "../style"
import "../custom_items"
import "../custom_items/top_bar"


/* ExitDialog - a typical window displaying a dialog for exiting the application */
Window {
    signal clickedYes
    signal clickedNo

    id: exitDialog

    width: 300
    height: 180

    visible: true
    flags: Qt.FramelessWindowHint
    //flags: Qt.Window | Qt.WindowStaysOnTopHint
    color: Style.backgroundColor

    // Just a simple border around the window
    BorderRectangle {
    }

    // Top bar displaying only close button and allowing to move the ExitDialog window
    TopBar {
        parentWindow: exitDialog
        container: exitDialog

        height: 20

        closeArea.onClicked: exitDialog.clickedNo()
        minButton.visible: false
        maxButton.visible: false

        mainMenuBarButton.visible: false
        readingBarButton.visible: false
    }

    // Question to ask
    Text {
        id: questionText
        text: qsTr("Close me?")

        x: 105
        y: 57

        color: Style.textColor
        font.pixelSize: 20
        font.family: Style.subMenuFont.name
    }

    // Answers
    TextButton {
        id: noButton
        text: "Nope"

        x: 172
        y: 101
        width: 70

        anchors.verticalCenterOffset: 0
        anchors.verticalCenter: yesButton.verticalCenter
        area.onClicked: {
            // Emit the new clicked signal here:
            exitDialog.clickedNo()
        }
    } // [TextButton: No]

    TextButton {
        id: yesButton
        text: "Yes"

        x: 58
        y: 102
        width: 70

        area.onClicked: {
            // Emit the new clicked signal here:
            exitDialog.clickedYes()
        }
    } // [TextButton: Yes]
}
