import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3
import QtQuick.Dialogs 1.0

import "../style"
import "../custom_items"
import "../menu_pages"
import "../windows_and_forms"

Item {
    id: item1
    width: 400
    height: 600

    TextButton {
        id: b1
        text: "Open file"
        area.onClicked: {
            //text1.text = QmlCppInterface.countAllUnknownWordsByBookId(0)
            fileDialog.open()
        }
    }
    Text {
        id: text1
        anchors.top: b1.bottom
        anchors.topMargin: 10
        text: ""
    }

    FileDialog {
        id: fileDialog
        title: "Please choose a file"

        folder: shortcuts.home
        onAccepted: {
            QmlCppInterface.addNewBookByFilename(fileDialog.fileUrl)
        }
        onRejected: {
            console.log("Canceled")
        }
        visible: false
    }
}
