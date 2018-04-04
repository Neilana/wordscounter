import QtQuick 2.7
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import QtQuick.Dialogs 1.0

import "../style"
import "../custom_items"
import "../menu_pages"
import "../windows_and_forms"
import "../reading_window"
import "../reading_window/contents"

Item {
    id: booksLibraryItem

    signal openBookById(int id)

    width: 400
    height: 400
    objectName: "booksLibraryItem"

    RowLayout {
        id: tableMenuRow
        spacing: 0

        anchors.right: parent.right
        anchors.rightMargin: 0
        anchors.left: parent.left
        anchors.leftMargin: 0
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 0

        ImageTextButton {
            id: imageTextButton1
            text: "Add"
            image: "/icons/plus.png"
        }

        ImageTextButton {
            id: imageTextButton11
            text: "Read"
            anchors.left: imageTextButton1.right
            anchors.leftMargin: 0
            image: "/icons/openBook_small.png"

            area.onClicked: {
                QmlCppInterface.setPageSize(
                            bookPageWindow.pageView.initialWidth - 15,
                            bookPageWindow.pageView.height - 30)

                // open book
                QmlCppInterface.openBookById(booksGridView.currentItem.bookId)
                rootStackLayout.currentIndex = 1
                bookPageWindow.pageView.view.currentIndex = 0
            }
        }

        ImageTextButton {
            id: imageTextButton2
            text: "Edit"
            anchors.left: imageTextButton11.right
            anchors.leftMargin: 0
            image: "/icons/edit.png"
        }
        ImageTextButton {
            id: imageTextButton3
            text: "Love"
            anchors.left: imageTextButton2.right
            anchors.leftMargin: 0
            image: "/icons/addToFavs.png"
        }

        ImageTextButton {
            id: imageTextButton4
            text: "Delete"
            anchors.left: imageTextButton3.right
            anchors.leftMargin: 0
            image: "/icons/minus.png"
            area.onClicked: {
                // delete book
                QmlCppInterface.deleteBookById(booksGridView.currentItem.bookId)
            }
        }
    }

    Rectangle {
        id: rectangle
        color: "#00000000"
        clip: true
        anchors.bottomMargin: 0
        anchors.bottom: tableMenuRow.top
        anchors.right: parent.right
        anchors.left: parent.left
        anchors.top: parent.top

        Component {
            id: bookDelegate
            Book {
                property var bookId
                property var bookPath
                bookId: model.Id
                bookPath: model.Path
                coverImage.source: "data:image/png;base64," + model.Image
                //width: 200
                width: booksGridView.cellWidth - 10
                authorText: Author
                titleText: Title
                // bookBorder.border.width: 10
                bookBorder.border.color: {
                    if (mouseArea1.containsMouse)
                        return Style.highlightHoverMenuColor

                    if (booksGridView.currentIndex == index)
                        return Style.highlightSelectedItemColor
                    return Style.borderColor
                }
                MouseArea {
                    anchors.fill: parent
                    hoverEnabled: true
                    id: mouseArea1

                    onClicked: {
                        print(bookId)
                        print(bookPath)
                        //parent.GridView.view.currentIndex = index
                        booksGridView.currentIndex = index
                    }
                }
            }
        }

        GridView {
            id: booksGridView
            //@@@anchors.fill: parent
            model: librarySqlModel
            delegate: bookDelegate
            cellWidth: 220
            //cellWidth: booksItem.width * 0.3
            cellHeight: cellWidth * 1.5 + 50
            ScrollBar.vertical: ScrollBar {
                width: 15
            }
        }
    }
}
