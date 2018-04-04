import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3

import "../style"
import "../custom_items"
import "../menu_pages"
import "../windows_and_forms"

Item {
    id: usersItem
    width: 400
    height: 400

    SwipeView {
        id: usersSwipeView
        anchors.left: columnLayout.right
        anchors.leftMargin: 0
        anchors.top: parent.top
        anchors.topMargin: 0
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 0
        anchors.right: parent.right
        anchors.rightMargin: 0
        orientation: Qt.Vertical

        UsersChose {
            id: usersChose
        }

        UsersNew {
            id: usersNew
        }
                UsersSettings {
                    id: usersSettings
                }

    }
    ColumnLayout {
        id: columnLayout
        width: 100
        spacing: 0
        anchors.left: parent.left
        anchors.leftMargin: 0
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 0
        anchors.top: parent.top
        anchors.topMargin: 0

        ImageTextButton {
            id: choseButton
            text: "Login"
            Layout.fillWidth: true
            Layout.fillHeight: true
            image: "/icons/knownWords.png"
            rectangleColor: {
                if (area.containsMouse)
                    return Style.highlightHoverMenuColor
                if (usersSwipeView.currentIndex == 0)
                    return Style.subMenuSelectedColor
                return Style.backgroundColor
            }
            area.onClicked: {
                usersSwipeView.currentIndex = 0
            }
        }

        ImageTextButton {
            id: newButton
            text: "New"
            Layout.fillWidth: true
            Layout.fillHeight: true
            image: "/icons/ignoredWords.png"

            rectangleColor: {
                if (area.containsMouse)
                    return Style.highlightHoverMenuColor
                if (usersSwipeView.currentIndex == 1)
                    return Style.subMenuSelectedColor

                return Style.backgroundColor
            }
            area.onClicked: {
                usersSwipeView.currentIndex = 1
            }
        }

        ImageTextButton {
            id: settingsButton
            text: "Settings"
            Layout.fillWidth: true
            Layout.fillHeight: true
            image: "/icons/favourite.png"
            rectangleColor: {
                if (area.containsMouse)
                    return Style.highlightHoverMenuColor
                if (usersSwipeView.currentIndex == 2)
                    return Style.subMenuSelectedColor

                return Style.backgroundColor
            }
            area.onClicked: {
                usersSwipeView.currentIndex = 2
            }
        }
    }
}
