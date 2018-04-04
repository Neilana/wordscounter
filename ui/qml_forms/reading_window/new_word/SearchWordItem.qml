import QtQuick 2.7
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import QtQuick.Controls.Styles 1.4
import QtQuick.Window 2.2

import "../../style"
import "../../custom_items"
import "../../windows_and_forms"

Item {
    signal searchedWordClicked(string word, string definition, string example)

    signal searchResultsChanged()

    TextInput {
        property string placeholderText: "Type word..."
        id: wordText

        text: QmlCppInterface.searchedWord

        onTextChanged: searchResultsChanged()

        height: 20
        Text {
            text: wordText.placeholderText

            // FIXME : color into property
            color: "#c1bcb6"
            visible: !wordText.text
            font.family: Style.tableFont.name
            font.pixelSize: 16
        }

        color: Style.textColor
        font.pixelSize: 16
        clip: true

        //                anchors.fill: parent
        anchors.top: parent.top
        anchors.topMargin: 5

        anchors.left: parent.left
        anchors.leftMargin: 5

        anchors.right: parent.right
        anchors.rightMargin: 5

        //        anchors.bottom: parent.bottom
        //        anchors.bottomMargin: 5
        wrapMode: TextEdit.Wrap
        cursorVisible: false

        font.family: Style.tableFont.name

        selectByMouse: true

        //selectByKeyboard: true
        selectionColor: Style.topBarBackgroundColor
        //selectionColor: Style.topSelectedColor
        selectedTextColor: Style.textColor
    }

    // border
    Rectangle {
        height: 1
        anchors.right: parent.right
        anchors.left: parent.left

        anchors.top: wordText.bottom
        anchors.topMargin: 5
        //anchors.bottom: parent.bottom
        color: Style.topSelectedColor
        z: 3
    }

    HelloAnimalItem {
        anchors.centerIn: parent
        width: searchView.width * 0.9

        //anchors.right: parent.right
        //anchors.left: parent.left
        visible: searchView.count == 0
        opacity: 0.3
    }

    ListView {
        id: searchView

        anchors.top: wordText.bottom
        anchors.topMargin: 5

        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right

        clip: true

        model: searchWordModel
        delegate: searchWordDelegate

        section.property: "partOfSpeech"
        section.delegate: sectionHeading
    }

    // section
    Component {
        id: sectionHeading
        Rectangle {
            width: searchView.width
            height: childrenRect.height + 10
            color: Style.topBarBackgroundColor

            Text {
                anchors.top: parent.top
                anchors.topMargin: 5

                anchors.left: parent.left
                anchors.leftMargin: 5

                anchors.right: parent.right
                anchors.rightMargin: 5

                text: section
                font.bold: true
                font.pixelSize: 17
                font.family: Style.tableFont.name

                color: Style.textColor
                //font.capitalization: Font.SmallCaps
                font.capitalization: Font.AllUppercase
            }
        }
    }

    // delegate
    Component {
        id: searchWordDelegate

        Rectangle {
            color: mouseText.containsMouse ? Style.highlightHoverMenuColor : "transparent"
            height: {
                if (definitionText.text == "")
                    return 0

                if (exampleText.text !== "")
                    return definitionText.paintedHeight + exampleText.paintedHeight + 25
                else
                    return definitionText.paintedHeight + 20
            }
            //height: 100
            width: searchView.width

            Text {
                id: definitionText

                anchors.top: parent.top
                anchors.topMargin: 10

                anchors.left: parent.left
                anchors.leftMargin: 5

                anchors.right: parent.right
                anchors.rightMargin: 5

                wrapMode: Text.Wrap
                color: Style.textColor

                //anchors.verticalCenter: parent.verticalCenter
                font.pixelSize: Style.contentsFontPixelSize

                text: definition
                font.family: Style.tableFont.name
            }

            Text {
                id: exampleText

                anchors.top: definitionText.bottom
                anchors.topMargin: 5

                anchors.left: parent.left
                anchors.leftMargin: 5

                anchors.right: parent.right
                anchors.rightMargin: 5

                wrapMode: Text.Wrap

                // FIXME : color into property
                color: "#928989"

                //anchors.verticalCenter: parent.verticalCenter
                font.pixelSize: Style.contentsFontPixelSize
                font.italic: true

                text: example
                //                {
                //                    if (example == "")
                //                        return "[No example]"
                //                    else
                //                        return example
                //                }
                font.family: Style.tableFont.name
            }

            MouseArea {
                id: mouseText

                anchors.fill: parent
                //onClicked: QmlCppInterface.searchedWordClicked(index)
                onClicked: {
                    searchedWordClicked(word, definition, example)
                }
                hoverEnabled: true
            }
        }
    }
}
