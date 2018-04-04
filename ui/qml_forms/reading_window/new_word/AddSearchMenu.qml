import QtQuick 2.7
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import QtQuick.Controls.Styles 1.4
import QtQuick.Window 2.2

import "../../style"
import "../../custom_items"

Item {
    property alias newWordForm: newWord

    width: newWord.width

    //menu buttons
    RowLayout {
        id: addSearchButtonsLayout

        width: 197
        z: 3

        anchors.top: parent.top
        anchors.left: parent.left
        anchors.leftMargin: (parent.width - width) / 2 + 6

        LabelButton {
            id: searchButton

            borderRect.border.width: 0
            Layout.fillWidth: true

            text.text: "Search"
            text.color: {
                if (area.containsMouse)
                    return "#D35400" //orange2
                if (addSearchLayout.currentIndex == 0)
                    return Style.textColor
                return Style.mainMenuSelectedColor
            }

            area.onClicked: {
                addSearchLayout.currentIndex = 0
            }

            SequentialAnimation {
                id: searchButtonAnimation

                loops: 2
                PropertyAnimation {
                    target: searchButton.text
                    properties: "color"
                    to: "#f9f5f1"
                    duration: 400
                }

                PropertyAnimation {
                    target: searchButton.text
                    properties: "color"
                    to: Style.mainMenuSelectedColor
                    duration: 400
                }
            } // [searchButtonAnimation]
        } // [searchButton]

        LabelButton {

            id: addButton

            Layout.fillWidth: true
            borderRect.border.width: 0

            text.text: "Add"
            text.color: {
                if (area.containsMouse)
                    return "#D35400" //orange2
                if (addSearchLayout.currentIndex == 1)
                    return Style.textColor
                return Style.mainMenuSelectedColor
            }
            area.onClicked: {
                addSearchLayout.currentIndex = 1
            }

            SequentialAnimation {
                id: addButtonAnimation

                loops: 2
                PropertyAnimation {
                    target: addButton.text
                    properties: "color"
                    to: "#f9f5f1"
                    duration: 400
                }

                PropertyAnimation {
                    target: addButton.text
                    properties: "color"
                    to: Style.mainMenuSelectedColor
                    duration: 400
                }
            } // [addButtonAnimation]
        } // [addButton]
    } // [addSearchButtonsLayout]

    // Borders
    Rectangle {
        height: 1
        anchors.right: parent.right
        anchors.left: parent.left
        anchors.top: addSearchButtonsLayout.bottom
        //anchors.bottom: parent.bottom
        color: Style.topSelectedColor
        z: 3
    }

    Rectangle {
        z: 3
        width: 1
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        color: Style.topSelectedColor
    }

    // menu content
    StackLayout {
        id: addSearchLayout

        anchors.top: addSearchButtonsLayout.bottom
        anchors.right: parent.right
        anchors.bottom: parent.bottom

        currentIndex: 0

        SearchWordItem {
            id: searchWordId
            anchors.fill: parent
        }

        NewWord {
            id: newWord

            anchors.right: parent.right
            anchors.top: parent.top
            anchors.bottom: parent.bottom
        }

        Component.onCompleted: {
            searchWordId.searchedWordClicked.connect(updateNewWordForm)
            searchWordId.searchResultsChanged.connect(blinkSearchButton)
        }

        function updateNewWordForm(word, definition, example) {
            newWord.word = word
            newWord.definition = definition
            newWord.example = example

            addButtonAnimation.start()
        }

        function blinkSearchButton() {
            if (addSearchLayout.currentIndex !== 0)
                searchButtonAnimation.start()
        }
    }
}
