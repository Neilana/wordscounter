import QtQuick 2.7

import "../style"

Item {
    width: 100
    height: 200

    property var animalId: QmlCppInterface.generateHelloAnimalNumber()
    Image {
        anchors.horizontalCenter: parent.horizontalCenter
        width: parent.width * 0.4

        id: animalImage

        horizontalAlignment: Image.AlignHCenter
        verticalAlignment: Image.AlignVCenter

        source: "/background_animals/" + QmlCppInterface.getHelloAnimalImagePath(
                    animalId)

        fillMode: Image.PreserveAspectFit
    }

    Text {
        anchors.top: animalImage.bottom
        anchors.topMargin: 10
        width: parent.width

        text: "Nothing to show here.\nBut here is happy little "
              + QmlCppInterface.getHelloAnimalName(
                  animalId) + " to say you hello."

        wrapMode: Text.Wrap

        color: Style.textColor
        font.pixelSize: Style.contentsFontPixelSize
        font.family: Style.tableFont.name

        horizontalAlignment: Text.AlignHCenter
    }
}
