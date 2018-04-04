import QtQuick 2.7
import QtQuick.Layouts 1.3

import "../../style"
import "../../custom_items"


/* NewWord - form for adding a new word into dictionary */
Item {
    height: 600
    width: 280

    // aliases for givin an access to NewWord form's text fields and buttons
    property alias word: wordInput.textInput
    property alias definition: definitionInput.textInput
    property alias example: exampleInput.textInput
    property alias synonyms: synonymsInput.textInput

    property alias columnLayout: columnLayout
    property alias buttonsLayout: rowLayout

    property alias confirmButton: confirmButton.area
    property alias ignoreButton: ignoreButton.area

    Rectangle {
        id: rectangle

        anchors.fill: parent
        color: Style.backgroundColor

        // Labels and text fields for adding a new words
        ColumnLayout {
            id: columnLayout

            anchors.right: parent.right
            anchors.left: parent.left
            anchors.bottom: rowLayout.top
            anchors.top: parent.top

            spacing: 0

            LabelInput {
                id: wordInput

                title: "Word"
                textInputPlaceholder: "Type word..."

                Layout.fillWidth: true
            }
            LabelInput {
                id: definitionInput

                title: "Definition"
                textInputPlaceholder: "Type definition..."

                height: 200
                Layout.fillHeight: true
                Layout.fillWidth: true
            }
            LabelInput {
                id: exampleInput

                title: "Example"
                textInputPlaceholder: "Type example..."
                textInputItem.color: "#928989"
                textInputItem.font.italic: true

                height: 200
                Layout.fillHeight: true
                Layout.fillWidth: true
            }
            LabelInput {
                id: synonymsInput

                title: "Synonyms"
                textInputPlaceholder: "Type synonyms..."

                Layout.fillWidth: true
            }
        } // [ColumnLayout]

        // Buttons for actions with entered word
        RowLayout {
            id: rowLayout

            spacing: 25

            anchors.bottom: parent.bottom
            anchors.bottomMargin: 0
            anchors.left: parent.left
            anchors.leftMargin: (parent.width - width) / 2

            ImageTextButton {
                id: confirmButton

                text: "Add"
                textColor: area.containsMouse ? "#D35400" : Style.textColor
                textSize: 197 / 14

                image: area.containsMouse ? "/icons/confirm_light.png" : "/icons/confirm.png"
                rectangleColor: Style.backgroundColor

                Layout.fillHeight: true
                Layout.fillWidth: true

                area.onClicked: QmlCppInterface.addWord(word,
                                                        definition, example)
            }

            ImageTextButton {
                id: ignoreButton

                text: "Ignore"
                textColor: area.containsMouse ? "#D35400" : Style.textColor
                textSize: 197 / 14

                image: area.containsMouse ? "/icons/ignore_light.png" : "/icons/ignore.png"
                rectangleColor: Style.backgroundColor

                Layout.fillHeight: true
                Layout.fillWidth: true
            }
        } // [RowLayout]
    } // [rectangle]
} // [Item]
