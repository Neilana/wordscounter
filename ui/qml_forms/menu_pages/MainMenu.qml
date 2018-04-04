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

    SwipeView {
        id: mainSwipeView

        anchors.leftMargin: 0
        anchors.rightMargin: 0
        anchors.topMargin: 0
        anchors.bottomMargin: 0

        anchors.top: mainMenuLayout.bottom
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.left: parent.left

        //currentIndex: tabBar1.currentIndex
        Library {
            id: library
            clip: true
        }

        Dictionary {
            id: dictionary
            clip: true
        }

        Users {
            id: users
            clip: true
        }
    }
    RowLayout {
        id: mainMenuLayout

        z: 0
        spacing: 0
        anchors.leftMargin: 0
        anchors.rightMargin: 0
        anchors.topMargin: 0

        //anchors.bottomMargin: Style.borderWidth

        //anchors.top: topBar.bottom
        anchors.right: parent.right
        //anchors.bottom: parent.bottom
        anchors.left: parent.left

        BigImageTextButton {
            id: libraryButton
            buttonText.text: "Library"
            buttonText.visible: area.containsMouse
                                || mainSwipeView.currentIndex == 0
            Layout.fillWidth: true
            image: "/icons/library.png"
            rectangleColor: {
                if (area.containsMouse)
                    return Style.highlightHoverMenuColor
                if (mainSwipeView.currentIndex == 0)
                    return Style.mainMenuSelectedColor
                return Style.backgroundColor
            }

            area.onClicked: {
                mainSwipeView.currentIndex = 0
            }
        }
        BigImageTextButton {
            id: dictionaryButton
            buttonText.text: "Dictionary"
            buttonText.visible: area.containsMouse
                                || mainSwipeView.currentIndex == 1

            Layout.fillWidth: true
            image: "/icons/dictionary.png"
            rectangleColor: {
                if (area.containsMouse)
                    return Style.highlightHoverMenuColor
                if (mainSwipeView.currentIndex == 1)
                    return Style.mainMenuSelectedColor
                return Style.backgroundColor
            }
            area.onClicked: {
                mainSwipeView.currentIndex = 1
            }
        }
        BigImageTextButton {
            id: usersButton
            buttonText.text: "Users"
            buttonText.visible: area.containsMouse
                                || mainSwipeView.currentIndex == 2
            Layout.fillWidth: true
            image: "/icons/users.png"
            rectangleColor: {
                if (area.containsMouse)
                    return Style.highlightHoverMenuColor
                if (mainSwipeView.currentIndex == 2)
                    return Style.mainMenuSelectedColor
                return Style.backgroundColor
            }
            area.onClicked: {
                mainSwipeView.currentIndex = 2
            }
        }
    }


    //    Rectangle {
    //        id: menuBorder
    //        height: 1
    //        color: Style.innerBorderColor
    //        // opacity: 0.2
    //        z: 4
    //        anchors.right: parent.right
    //        anchors.rightMargin: 0
    //        anchors.left: parent.left
    //        anchors.leftMargin: 0
    //        anchors.top: mainMenuLayout.bottom
    //        anchors.topMargin: 0
    //    }

    //bottom border
}
