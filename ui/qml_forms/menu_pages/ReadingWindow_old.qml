import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import QtQuick.Window 2.2
import QtQuick.Dialogs 1.2
import QtQuick.Controls.Styles 1.4

import "../style"
import "../custom_items"
import "../menu_pages"
import "../windows_and_forms"

Item {
    property Window newWordWin
    id: item1
    width: 800
    height: 400

    property alias textArea: textArea
    property alias view: view
    property alias mouseDropAreaRight: mouseDropAreaRight

    signal clickedClosed

    ScrollView {
        ScrollBar.vertical.policy: ScrollBar.AsNeeded

        ScrollBar.vertical.width: 15
        ScrollBar.vertical.interactive: true

        z: -1
        id: view
        clip: true

        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.top: parent.top

        anchors.rightMargin: 5
        anchors.topMargin: 5
        anchors.leftMargin: 5
        anchors.bottomMargin: 5

        TextArea {
            id: textArea
            text: QmlCppInterface.bookText()
            font.pointSize: 11
            font.family: Style.readingFont.name
            wrapMode: Text.WordWrap
            readOnly: true
            textFormat: Text.RichText
            selectedTextColor: Style.textColor
            selectionColor: Style.topBarBackgroundColor
        }

        /*
    ListView {
        id: view

        width: 600
        height: 620

        clip: true
        orientation: ListView.Horizontal
        snapMode: ListView.SnapToItem
        //anchors.fill: parent
        model: bookModel
        delegate: Rectangle {
            color: "transparent"
            id: myPageDelegate2
            //anchors.fill: parent
            width: view.width
            height: view.height
            TextArea {
                id: textArea
                anchors.fill: parent
                textFormat: TextEdit.RichText

                //!!!
                //text: modelData
                text: display

                wrapMode: Text.Wrap
                horizontalAlignment: TextEdit.AlignJustify
            }

            // Page number
            Text {
                height: 20
                anchors.bottom: parent.bottom
                anchors.horizontalCenter: parent.horizontalCenter
                color: "grey"
                text: index
            }
        }*/
        MouseArea {
            id: readingMouseArea
            anchors.fill: parent
            onClicked: {

                if (mouse.source === Qt.MouseEventNotSynthesized)
                    hoverEnabled = true
                selectWordUnderMouse()
                if (newWordWin == null) {
                    var component
                    component = Qt.createComponent(
                                "/windows_and_forms/NewWordWindow.qml")
                    newWordWin = component.createObject(rootWindow)

                    newWordWin.show()
                    newWordWin.closeMe.connect(closeNewWord)
                    newWordWin.snapMe.connect(snapNewWord)
                    newWordWin.n1.word.text = textArea.selectedText

                    mouseDropAreaRight.updateGlobalPos()
                } else {
                    // if not slided
                    newWordWin.n1.word.text = textArea.selectedText
                    newWordSnapped.word.text = textArea.selectedText

                    if (newWordSnapped.state == "") {
                        newWordWin.hide()
                        newWordWin.showNormal()
                    }
                }
            }
            function closeNewWord() {
                newWordWin.close()
            }

            function snapNewWord() {
                newWordSnapped.state = "snapped"
                newWordSnapped.word.text = newWordWin.n1.word.text
            }
            // если мышка (для поддержки норм реакци на тач-скрина)
            hoverEnabled: true

            onPositionChanged: {

                if (mouse.source === Qt.MouseEventNotSynthesized) {
                    hoverEnabled = true
                    selectWordUnderMouse()
                } else
                    hoverEnabled = false
            }
            function selectWordUnderMouse() {
                var wordPos = textArea.positionAt(mouseX, mouseY)
                textArea.cursorPosition = wordPos
                textArea.selectWord()
            }
        }

        //        focus: true

        //        // 2 - чтобы пропускать запятые, точки и прочее (при перемещении влево работает плохо)
        //        Keys.onLeftPressed: {
        //            console.log("move left")
        //            textArea.cursorPosition = textArea.selectionStart - 2
        //            textArea.selectWord()
        //        }
        //        Keys.onRightPressed: {
        //            console.log("move right")
        //            textArea.cursorPosition = textArea.selectionEnd + 2
        //            textArea.selectWord()
        //        }
        //        Keys.onUpPressed: {
        //            console.log("move up")
        //            var rect = textArea.positionToRectangle(textArea.selectionEnd)

        //            console.log(rect.height)
        //            if (rect.y - rect.height >= 0) {
        //                textArea.cursorPosition = textArea.positionAt(
        //                            rect.x, rect.y - rect.height / 2)
        //                textArea.selectWord()
        //            }
        //        }
        //        Keys.onDownPressed: {
        //            console.log("move down")
        //            var rect = textArea.positionToRectangle(textArea.selectionStart)

        //            console.log(rect.height)
        //            //if (rect.y + rect.height <= textArea.contentHeight)
        //            {
        //                textArea.cursorPosition = textArea.positionAt(
        //                    rect.x, rect.y + rect.height + rect.height / 2)
        //                textArea.selectWord()
        //            }
        //        }
    }

    //!!!
    //}
    MouseArea {
        function updateGlobalPos() {
            globalPosition = mapToGlobal(0, 0)

            //var globalPosition = mapToGlobal(testMouseDrop.x, testMouseDrop.y)
            //console.log("For system: " + globalPosition)
            if (newWordWin != null) {
                newWordWin.dropRect.x = mouseDropAreaRight.globalPosition.x
                newWordWin.dropRect.y = mouseDropAreaRight.globalPosition.y
                newWordWin.dropRect.width = mouseDropAreaRight.width
                newWordWin.dropRect.height = mouseDropAreaRight.height
                //console.log("Hey!!! " + newWordWin.dropRect)
            }
        }
        property var globalPosition

        onClicked: updateGlobalPos()
        Component.onCompleted: updateGlobalPos()

        width: 100

        anchors.top: parent.top
        anchors.topMargin: 0
        anchors.right: parent.right

        anchors.rightMargin: 0
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 0
        hoverEnabled: true

        id: mouseDropAreaRight
        onPositionChanged: {
            if (!pressed) {
                // во избежания ситуации, когда перетаскиваешь окно и мышка пересекает область
                var pos = mapToItem(textArea, mouseX, mouseY)
                var wordPos = textArea.positionAt(pos.x, pos.y)
                textArea.cursorPosition = wordPos
                textArea.selectWord()

                //console.log("Hovered! " + pos)
            }
        }
        Rectangle {
            id: testRectangleRight

            anchors.fill: parent
            color: Style.topBarBackgroundColor

            opacity: 0

            states: [
                State {
                    when: // x
                          (newWordWin.globalPosition.x >= newWordWin.dropRect.x
                           && newWordWin.globalPosition.x
                           <= newWordWin.dropRect.x + newWordWin.dropRect.width)
                          && // y
                          (newWordWin.globalPosition.y >= newWordWin.dropRect.y
                           && newWordWin.globalPosition.y <= newWordWin.dropRect.y
                           + newWordWin.dropRect.height) && // not slided
                          (newWordSnapped.state == "")

                    PropertyChanges {
                        target: testRectangleRight
                        opacity: 0.6
                    }
                }
            ]
        }
    }

    MouseArea {
        visible: false
        width: newWordSnapped.width / 2
        anchors.top: parent.top
        anchors.topMargin: newWordMouseArea.height

        anchors.right: parent.right

        // scroll bar
        anchors.rightMargin: 15
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 0

        drag.target: newWordSnapped
        z: 3
        drag.axis: Drag.XAxis

        drag.minimumX: longButtonRight.x - newWordSnapped.width
        drag.maximumX: longButtonRight.x
        hoverEnabled: true
        propagateComposedEvents: true

        id: touchArea
        onPositionChanged: {
            if (!pressed && newWordSnapped.state == "slided") {
                // во избежания ситуации, когда перетаскиваешь окно и мышка пересекает область
                var pos = mapToItem(textArea, mouseX, mouseY)
                var wordPos = textArea.positionAt(pos.x, pos.y)
                textArea.cursorPosition = wordPos
                textArea.selectWord()
            }
        }

        onPressed: {
            newWordSnapped.anchors.left = undefined
            newWordSnapped.anchors.right = undefined
            console.log("PRESSSSSED " + mouse.source)
        }

        /*onClicked:{
        if (newWordSnapped.state == "contains_slided") {

                        //var wordX = textArea.width - longButtonRight.width
                        //      - touchArea.width + touchArea.mouseX
                        var wordX = dropAreaRight.x + touchArea.x + touchArea.mouseX
                        var wordY = newWordBar.height + touchArea.mouseY

                        var wordPos = textArea.positionAt(wordX, wordY)
                        textArea.cursorPosition = wordPos
                        textArea.selectWord()
                        dragDropItem.newWord.word.text = textArea.selectedText
                    } else {
                        console.log("QWE")
                        //newWord.word.selectAll()
                        // newWord.word.focus = true
                        // HURRRAYYY!!!!!!!!!!!!!!
                        var item = newWord.columnLayout.childAt(touchArea.mouseX,
                                                                touchArea.mouseY)
                        if (item !== null)
                            // == textinput
                            item.textInput.focus = true
                        else // == buttons
                        {
                            var buttonX = newWord.buttonsLayout.x + (touchArea.width - touchArea.mouseX)
                            var buttonY = newWord.buttonsLayout.y + (touchArea.height - touchArea.mouseY)

                            item = newWord.buttonsLayout.childAt(buttonX, buttonY)
                            if (item !== null)
                                item.onClicked()
                        }
                        // touchArea.mouse.accepted = false
                    }

        } */
        onReleased: {
            if (newWordSnapped.x >= longButtonRight.x - newWordSnapped.width * 0.5)
                newWordSnapped.state = "slided"
            else
                newWordSnapped.state = "snapped"

            var item = newWordSnapped.columnLayout.childAt(mouseX, mouseY)
            if (item !== null)
                // == textinput
                item.textInput.focus = true
            //            else // == buttons
            //            {

            //                //var buttonX = newWordSnapped.buttonsLayout.x + (width - mouseX)
            //                //var buttonY = newWordSnapped.buttonsLayout.y + (height - mouseY)
            //                item = newWordSnapped.buttonsLayout.childAt(mouseX, mouseY)
            //                if (item !== null)
            //                    item.onClicked()
            //            }
            //            mouse.accepted = false
            console.log("Drag x: " + drag.minimumX + " " + drag.maximumX)
        }
    }

    NewWord {
        width: 280
        transitions: [

            Transition {
                from: "moving_snapped"
                to: ""

                //reversible: true
                AnchorAnimation {
                    duration: 1000
                }
                NumberAnimation {
                    //                id: anim
                    target: view
                    properties: "anchors.rightMargin"
                    easing.type: Easing.Linear
                    duration: 1000
                }
            },
            Transition {
                from: "snapped"
                to: "slided"

                reversible: true
                AnchorAnimation {
                    duration: 1000
                }
                NumberAnimation {
                    //                id: anim
                    target: view
                    properties: "anchors.rightMargin"
                    easing.type: Easing.Linear
                    duration: 1000
                }
            },
            Transition {
                from: ""
                to: "snapped"
                reversible: true
                //             smoothly reanchor myRect and move into new position
                AnchorAnimation {
                    duration: 1000
                }
                NumberAnimation {
                    //                id: anim
                    target: view
                    properties: "anchors.rightMargin"
                    easing.type: Easing.Linear
                    duration: 1000
                }
            },
            Transition {
                from: "moving_snapped2"
                to: "slided"
                reversible: false
                //             smoothly reanchor myRect and move into new position
                AnchorAnimation {
                    duration: 500
                }
                NumberAnimation {
                    //                id: anim
                    target: view
                    properties: "anchors.rightMargin"
                    easing.type: Easing.Linear
                    duration: 500
                }
            },
            Transition {
                from: "moving_snapped2"
                to: "snapped"
                reversible: false
                //             smoothly reanchor myRect and move into new position
                AnchorAnimation {
                    duration: 500
                }
                NumberAnimation {
                    //                id: anim
                    target: view
                    properties: "anchors.rightMargin"
                    easing.type: Easing.Linear
                    duration: 500
                }
            }
        ]
        id: newWordSnapped
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.left: item1.right
        handImageButton.visible: true
        Drag.active: newWordMouseArea.drag.active || touchArea.drag.active
        visible: false
        //Drag.hotSpot.x: newWordSnapped.width / 2 - 20
        //Drag.hotSpot.y: 0
        MouseArea {
            id: newWordMouseArea
            width: newWordSnapped.width
            height: newWordSnapped.handImageButton.height
            z: 4

            anchors.top: parent.top

            drag.target: newWordSnapped
            drag.threshold: 30

            propagateComposedEvents: true
            onReleased: {
                if (newWordSnapped.state == "moving_snapped") {
                    var newPos = mapToGlobal(mouse.x, mouse.y)
                    newWordWin.setX(newPos.x)
                    newWordWin.setY(newPos.y)
                    newWordWin.updateGlobalPos()
                    newWordWin.show()

                    newWordSnapped.state = ""
                }
            }
            //onPositionChanged:
        }
        onWidthChanged: console.log(
                            "Width changed: " + width + ", state: " + state)
        states: [
            State {
                name: "moving_snapped"
                when: newWordMouseArea.drag.active //|| touchArea.drag.active
                extend: "snapped"
                AnchorChanges {
                    target: newWordSnapped

                    anchors.top: undefined
                    anchors.bottom: undefined
                    anchors.left: undefined
                    anchors.right: undefined
                }
                AnchorChanges {
                    target: view
                    anchors.right: parent.right
                }

                PropertyChanges {
                    target: view
                    anchors.rightMargin: newWordSnapped.width + longButtonRight.width
                }

                PropertyChanges {
                    target: longButtonRight
                    visible: false
                }

                PropertyChanges {
                    target: newWordSnapped
                    opacity: 0.7
                    anchors.centerIn: undefined
                    anchors.fill: undefined
                    visible: true
                }
            },

            State {
                name: "moving_snapped2"
                when: touchArea.drag.active
                extend: "snapped"
                AnchorChanges {
                    target: newWordSnapped

                    anchors.top: undefined
                    anchors.bottom: undefined
                    anchors.left: undefined
                    anchors.right: undefined
                }
                PropertyChanges {
                    target: newWordSnapped
                    visible: true
                }
            },

            State {
                name: "snapped"

                AnchorChanges {
                    target: view
                    anchors.right: newWordSnapped.left
                }
                PropertyChanges {
                    target: view
                    anchors.rightMargin: 0
                }

                PropertyChanges {
                    target: newWordSnapped
                    visible: true
                    width: 280
                }
                AnchorChanges {
                    target: newWordSnapped
                    anchors.left: undefined
                    anchors.right: longButtonRight.left
                }
                PropertyChanges {
                    target: longButtonRight
                    visible: true
                    imageSource: "/icons/rightArrow.png"
                }
                PropertyChanges {
                    target: touchArea
                    width: newWordSnapped.width - 10 // for scrollbar
                    visible: true
                    anchors.bottomMargin: newWordSnapped.buttonsLayout.height
                }
            },

            State {
                name: "slided"
                PropertyChanges {
                    target: newWordSnapped
                    visible: true
                }
                AnchorChanges {
                    target: newWordSnapped
                    anchors.right: undefined
                    anchors.left: longButtonRight.right
                }
                PropertyChanges {
                    target: longButtonRight
                    visible: true
                    imageSource: "/icons/leftArrow.png"
                }

                PropertyChanges {
                    target: view
                    anchors.rightMargin: 5 + longButtonRight.width
                }

                PropertyChanges {
                    target: touchArea
                    width: newWordSnapped.width / 2
                    visible: true
                }
            }
        ]
    }

    LongVerticalButton {
        id: longButtonRight
        visible: false
        z: 3
        anchors.right: parent.right
        anchors.rightMargin: 0
        anchors.top: parent.top
        anchors.topMargin: 0
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 0

        imageSource: "/icons/rightArrow.png"

        area.onClicked: {
            if (newWordSnapped.state == "slided")
                newWordSnapped.state = "snapped"
            else
                newWordSnapped.state = "slided"
        }
    }
}
