import QtQuick 2.7
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

import "../../style"
import "../../custom_items"

Item {
    RowLayout {
        id: buttonsLayout2
        z: 3

        anchors.left: parent.left
        anchors.right: parent.right
        anchors.rightMargin: 10
        anchors.leftMargin: 0
        LabelButton {
            text.text: "Contents"

            borderRect.border.width: 0
            Layout.fillWidth: true

            text.color: {
                if (area.containsMouse)
                    return "#D35400" //orange2
                if (contentsBookmarksLayout.currentIndex == 0)
                    return Style.textColor
                return Style.mainMenuSelectedColor
            }

            area.onClicked: {
                contentsBookmarksLayout.currentIndex = 0
            }
        }

        LabelButton {
            text.text: "Bookmarks"

            borderRect.border.width: 0
            Layout.fillWidth: true
            text.color: {
                if (area.containsMouse)
                    return "#D35400" //orange2
                if (contentsBookmarksLayout.currentIndex == 1)
                    return Style.textColor
                return Style.mainMenuSelectedColor
            }

            area.onClicked: {
                contentsBookmarksLayout.currentIndex = 1
            }
        }
    }

    // border on the right of contents menu
    Rectangle {
        z: 3
        width: 1
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        color: Style.topSelectedColor
    }

    Rectangle {
        z: 3
        height: 1
        anchors.right: parent.right
        anchors.left: parent.left
        anchors.top: buttonsLayout2.bottom

        color: Style.topSelectedColor
    }

    // menu content
    StackLayout {
        id: contentsBookmarksLayout
        //anchors.fill: parent
        anchors.top: buttonsLayout2.bottom
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.left: parent.left

        currentIndex: 0

        ContentsItem {
            anchors.fill: parent
        }

        BookmarksItem {
            anchors.fill: parent
        }
    }
}
