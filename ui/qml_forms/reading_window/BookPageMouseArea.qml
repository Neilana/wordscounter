import QtQuick 2.7
import QtQuick.Window 2.2


/* BookPageMouseArea - contains MouseArea for claculating current position on the page
and finding a word under the mouse.
Properties:
    pageTextArea - TextArea with text from the current page
    mouseDropArea - area where the NewWordWindow can be dropped
    listView - ListView where book pages are displayed */
Item {
    // property Window newWordWin
    property var pageTextArea
    property var mouseDropArea
    property var listView
    property var newWordSnapped

    property alias bookMouse: bookPageMouseArea222

    visible: (pageTextArea !== null)

    anchors.fill: parent
    MouseArea {
        id: bookPageMouseArea222
        anchors.fill: parent
        hoverEnabled: true

        propagateComposedEvents: true

        onWheel: {
            // FIXME: checks for index exists
            if (wheel.angleDelta.y > 0)
                listView.positionViewAtIndex(listView.currentIndex - 1,
                                             ListView.SnapPosition)
            if (wheel.angleDelta.y < 0)
                listView.positionViewAtIndex(listView.currentIndex + 1,
                                             ListView.SnapPosition)
        }
        property var notesWin
        function hahaClicked(mouse) {
            {
                if (mouse.source === Qt.MouseEventNotSynthesized)
                    hoverEnabled = true
                else
                    // FIXME: NOT DURE
                    selectWordUnderMouse(mouse)

                var clickedText = pageTextArea.getFormattedText(
                            pageTextArea.selectionStart,
                            pageTextArea.selectionEnd)

                var wasLink = QmlCppInterface.isLink(clickedText)

                if (!wasLink) {
                    var wasImage = QmlCppInterface.isImage(clickedText)
                    if (!wasImage) {

                        //newWordSnapped.word.text = pageTextArea.selectedText
                        QmlCppInterface.wordClicked(pageTextArea.selectedText)
                    }
                } else // wasLinks
                {
                    var wasNote = QmlCppInterface.isNote(clickedText)
                    if (wasNote) {
                        if (notesWin == null) {
                            var component2
                            component2 = Qt.createComponent(
                                        "./notes/NotesWindow.qml")
                            notesWin = component2.createObject(rootWindow)
                        }
                        notesWin.show()
                    } else //not note
                    {
                        var link = QmlCppInterface.extractLink(clickedText)
                        Qt.openUrlExternally(link)
                    }
                }
                mouse.accepted = false
            }
        }
        onClicked: hahaClicked(mouse)

        function hahaPosition(mouse) {
            //console.log("HERE CHANGED")
            if (mouse.source === Qt.MouseEventNotSynthesized) {
                hoverEnabled = true
                if (!pressed)
                    selectWordUnderMouse(mouse)
            } else
                hoverEnabled = false

            // for keyboard navigation
            listView.focus = true
        }
        // если мышка (для поддержки норм реакци на тач-скрина)
        //onPositionChanged: hahaPosition(mouseX, mouseY)
        onPositionChanged: hahaPosition(mouse)

        function selectWordUnderMouse(mouse) {
            var mpos = mapToItem(pageTextArea, mouseX, mouseY)
            var wordPos = pageTextArea.positionAt(mpos.x, mpos.y)
            pageTextArea.cursorPosition = wordPos
            pageTextArea.selectWord()
        }
    }
}
