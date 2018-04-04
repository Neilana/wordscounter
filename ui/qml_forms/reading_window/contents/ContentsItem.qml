import QtQuick 2.7
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

import "../../style"
import "../../custom_items"


/* TODO: nested list
https://blog.csdn.net/colouroo/article/details/44700357
https: //forum.qt.io/topic/66565/qml-expandable-menu-with-subitems/13
*/
Item {
    id: contentsItem

    height: 400
    width: 600

    Component {
        id: contactDelegate
        Rectangle {
            width: contentsView.width
            height: text.height + 10

            color: {
                if (mouseText.containsMouse)
                    return Style.highlightHoverMenuColor
                if (index == contentsView.currentIndex)
                    return Style.subMenuSelectedColor
                return "transparent"
            }
            Text {
                id: text

                anchors.left: parent.left
                anchors.leftMargin: 5

                anchors.right: parent.right
                anchors.rightMargin: 5

                anchors.verticalCenter: parent.verticalCenter

                wrapMode: Text.Wrap
                text: display
                color: Style.textColor
                font.pixelSize: Style.contentsFontPixelSize // !!!
                //font.family: "Open Sans"
                font.family: Style.tableFont.name
                font.capitalization: Font.SmallCaps
                MouseArea {
                    id: mouseText

                    anchors.fill: parent
                    hoverEnabled: true

                    onClicked: {
                        QmlCppInterface.contentClicked(index)
                        contentsView.currentIndex = index
                    }
                }
            } // [Text]
        } // [Rectangle]
    } // [ContactsDelegate]

    ListView {
        id: contentsView

        model: contentsModel
        delegate: contactDelegate

        clip: true
        focus: true

        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.bottom: parent.bottom

        Layout.fillWidth: true
        Layout.fillHeight: true

        ScrollBar.vertical: ScrollBar {
        }
    }

    // background message if there's no table of contents
    EmptyItem {
        imageSource: "/icons/contents2.png"
        text: "Table of contents is empty."
        visible: contentsView.count == 0

        anchors.centerIn: parent
        width: parent.width
        opacity: 0.3
    }
}
