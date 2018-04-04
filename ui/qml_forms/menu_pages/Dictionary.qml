import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3

import "../style"
import "../custom_items"
import "../menu_pages"
import "../windows_and_forms"

Item {
    id: dictionaryItem
    width: 400
    height: 400

    SwipeView {
        id: dictionarySwipeView
        anchors.left: columnLayout.right
        anchors.leftMargin: 0
        anchors.top: parent.top
        anchors.topMargin: 0
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 0
        anchors.right: parent.right
        anchors.rightMargin: 0
        orientation: Qt.Vertical

        DictionaryKnown {
            id: dictionaryKnown
            clip: true
        }

        DictionaryIgnored {
            id: dictionaryIgnored
            clip: true
        }

        //        LibraryFavourite {
        //            id: libraryFavourite
        //        }
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
            id: knownButton
            text: "Known"
            Layout.fillWidth: true
            Layout.fillHeight: true
            image: "/icons/knownWords.png"
            rectangleColor: {
                if (area.containsMouse)
                    return Style.highlightHoverMenuColor
                if (dictionarySwipeView.currentIndex == 0)
                    return Style.subMenuSelectedColor
                return Style.backgroundColor
            }
            area.onClicked: {
                dictionarySwipeView.currentIndex = 0
            }
        }

        ImageTextButton {
            id: ignoredButton
            text: "Ignored"
            Layout.fillWidth: true
            Layout.fillHeight: true
            image: "/icons/ignoredWords.png"

            rectangleColor: {
                if (area.containsMouse)
                    return Style.highlightHoverMenuColor
                if (dictionarySwipeView.currentIndex == 1)
                    return Style.subMenuSelectedColor

                return Style.backgroundColor
            }
            area.onClicked: {
                dictionarySwipeView.currentIndex = 1
            }
        }

        //        ImageTextButton {
        //            id: favouriteButton
        //            text: "Favourites"
        //            Layout.fillWidth: true
        //            Layout.fillHeight: true
        //            image: "/icons/favourite.png"
        //            rectangleColor: {
        //                if (area.containsMouse)
        //                    return Style.highlightHoverMenuColor
        //                if (dictionarySwipeView.currentIndex == 2)
        //                    return Style.mainMenuSelectedColor

        //                return Style.backgroundColor
        //            }
        //            area.onClicked: {
        //                dictionarySwipeView.currentIndex = 2
        //            }
        // }
    }
}
