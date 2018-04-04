import QtQuick 2.4

Item {
    width: 400
    height: 400

    TextEdit {
        id: textEdit
        x: 8
        y: 8
        width: 80
        height: 20
        text: qsTr("Settings")
        font.pixelSize: 12
    }
}
