import QtQuick 2.7
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

import "../style"


/* PageView - area for displaying a single book page and allowing to scroll to other pages.
 Pages are loaded into ListView model as a HTML-formated strings (QStringList) */
Item {
    property alias view: view
    property alias bookPageMouseArea: bookPageMouseArea

    property var snappedWord
    property var pageIndex: QmlCppInterface.pageIndex

    width: 600
    height: 620

    onPageIndexChanged: {
        view.currentIndex = pageIndex
    }

    /* ListView - displays book pages one by one */
    ListView {
        onCurrentIndexChanged: {
            QmlCppInterface.pageIndex = currentIndex
        }

        id: view

        model: bookModel
        contentWidth: parent.width
        contentHeight: parent.height

        anchors.fill: parent
        clip: true
        orientation: ListView.Horizontal
        snapMode: ListView.SnapOneItem // can scroll only one page at time

        //snapMode: ListView.SnapToItem // can scroll several pages with one flick
        highlightRangeMode: ListView.StrictlyEnforceRange // !!! OMFG HOW LONG I'VE BEEN LOOKING FOR YOU
        highlightMoveDuration: 2 // AND YOU !!!!1111 // moving with keyboard

        delegate: Rectangle {
            property alias textArea: textArea

            //property alias pageNumber: pageNumber
            width: view.width
            height: view.height

            color: "transparent"

            // Page text
            TextArea {
                id: textArea

                textFormat: TextEdit.RichText
                text: display
                wrapMode: Text.Wrap
                horizontalAlignment: TextEdit.AlignJustify
                readOnly: true
                selectedTextColor: Style.textColor
                selectionColor: Style.topBarBackgroundColor

                anchors.fill: parent
            }

            // page number/percents
            //            RowLayout {
            //                height: 20
            //                anchors.bottom: parent.bottom
            //                anchors.horizontalCenter: parent.horizontalCenter
            //                TextInput {
            //                    color: "grey"
            //                    selectedTextColor: Style.textColor
            //                    selectionColor: Style.topBarBackgroundColor
            //                    font.family: Style.readingFont.name
            //                    //text: ""
            //                    inputMask: "00%"
            //                    onAccepted: {
            //                        QmlCppInterface.loadPageByPercents(text)
            //                    }
            //                    // text: QmlCppInterface.percents
            //                    onFocusChanged: {
            //                        if (focus)
            //                            selectAll()
            //                    }
            //                }
            // Page number
            //                Text {
            //                    id: pageNumber
            //                    color: "grey"
            //                    text: index
            //                    //text: "/ " + QmlCppInterface.percents + "%"
            //                    font.family: Style.readingFont.name
            //                }
            // }
        } // [Rectangle delegate end]

        /* BookPageMouseArea - a mouse area for text. It allows to watch the mouse and highlight
        text under the mouse in the TextAre on the currently displayed page in ListView */
        BookPageMouseArea {
            id: bookPageMouseArea

            //visible: false
            pageTextArea: view.currentItem.textArea
            listView: view
            newWordSnapped: snappedWord

            anchors.left: parent.left
            anchors.right: parent.right
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 20
        }
    } // [ListView end]
} // [PageView end]
