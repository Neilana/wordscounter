import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3
import QtQuick.Window 2.2

import "../style"
import "../custom_items"
import "../menu_pages"
import "../windows_and_forms"

Item {
    id: libraryItem
    width: 100
    height: 100
    SwipeView {
        id: librarySwipeView
        anchors.right: parent.right
        anchors.left: columnLayout.right
        anchors.top: parent.top
        anchors.bottom: parent.bottom

        orientation: Qt.Vertical

        LibraryAllBooks {
            id: libraryAllBooks
            clip: true
        }

        LibraryAdd {
            id: libraryAdd
            clip: true
        }

        LibraryFavourite {
            id: libraryFavourite
            clip: true
        }

        LibrarySearch {
            id: librarySearch
            clip: true
        }
    }

    ColumnLayout {
        id: columnLayout
        width: 110
        spacing: 0
        anchors.left: parent.left
        anchors.leftMargin: 0
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 0
        anchors.top: parent.top
        anchors.topMargin: 0

        ImageTextButton {
            id: allButton
            text: "All"
            Layout.fillWidth: true
            Layout.fillHeight: true
            image: "/icons/allBooks.png"
            rectangleColor: {
                if (area.containsMouse)
                    return Style.highlightHoverMenuColor
                if (librarySwipeView.currentIndex == 0)
                    return Style.subMenuSelectedColor
                return Style.backgroundColor
            }
            area.onClicked: {
                librarySwipeView.currentIndex = 0
            }
        }

        ImageTextButton {
            id: addButton
            text: "New"
            Layout.fillWidth: true
            Layout.fillHeight: true
            image: "/icons/addBook.png"

            rectangleColor: {
                if (area.containsMouse)
                    return Style.highlightHoverMenuColor
                if (librarySwipeView.currentIndex == 1)
                    return Style.subMenuSelectedColor

                return Style.backgroundColor
            }
            area.onClicked: {
                librarySwipeView.currentIndex = 1
            }
        }
        ImageTextButton {
            id: favouriteButton
            text: "Loved"
            Layout.fillWidth: true
            Layout.fillHeight: true
            image: "/icons/favourite.png"
            rectangleColor: {
                if (area.containsMouse)
                    return Style.highlightHoverMenuColor
                if (librarySwipeView.currentIndex == 2)
                    return Style.subMenuSelectedColor

                return Style.backgroundColor
            }
            area.onClicked: {
                librarySwipeView.currentIndex = 2
            }
        }

        ImageTextButton {
            id: searchButton
            text: "Search"
            Layout.fillWidth: true
            Layout.fillHeight: true
            image: "/icons/bookSearch.png"
            rectangleColor: {
                if (area.containsMouse)
                    return Style.highlightHoverMenuColor
                if (librarySwipeView.currentIndex == 3)
                    return Style.subMenuSelectedColor

                return Style.backgroundColor
            }
            area.onClicked: {
                librarySwipeView.currentIndex = 3
            }
        }
    }
}
