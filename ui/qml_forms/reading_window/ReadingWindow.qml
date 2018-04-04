import QtQuick 2.7
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4

import "../custom_items"
import "../reading_window"
import "../style"
import "../reading_window/contents"
import "../reading_window/new_word"


/* ReadingWindow - window responsible for:
    - displaying a book (via custom item PageView). Displays only one page at a time;
    - displaying a form for adding a new unknown word into dictionary (via custom item
    NewWordWindow) by clicking on the word;
    - different movement operations for the NewWordWindow (dropping, sliding, unsnapping). */
Item {
    id: readingWindow

    property alias pageView: pageView
    property alias newWordSnapped: newWordSnapped.newWordForm

    property int widthMargin: newWordSnapped.width + longButtonRight.width

    width: 800
    height: 400

    BusyIndicator {
        running: pageView.view.count == 0
        anchors.centerIn: pageView
    }

    // current page / loading progress
    //    ProgressBar {

    //        //value: pageView.view.currentIndex + 10
    //        //maximumValue: pageView.view.count + 15

    //        //   value: QmlCppInterface.currentEndPos
    //        // maximumValue: QmlCppInterface.lastEndPos

    //        minimumValue: 0

    //        x: 100
    //        y: 100
    //        style: ProgressBarStyle {
    //            background: Rectangle {
    //                radius: 2
    //                color: "lightgray"
    //                border.color: "gray"
    //                border.width: 1
    //                implicitWidth: 200
    //                implicitHeight: 24
    //            }
    //            progress: Rectangle {
    //                color: "lightsteelblue"
    //                border.color: "steelblue"
    //            }
    //        }
    //    }

    // Book pages
    PageView {
        id: pageView
        property var initialWidth: parent.width - (newWordSnapped.width + contentsItem.width
                                                   + longButtonRight.width * 2 + 15)

        visible: QmlCppInterface.bookOpened

        // custom properties
        snappedWord: newWordSnapped

        // common properties
        anchors.right: newWordSnapped.visible ? newWordSnapped.left : longButtonRight.left

        anchors.bottom: parent.bottom
        anchors.left: contentsItem.visible ? contentsItem.right : longButtonLeft.right

        anchors.top: parent.top

        anchors.rightMargin: 5
        anchors.topMargin: 5
        anchors.leftMargin: 5
        anchors.bottomMargin: 5

        //        onWidthChanged: {
        //            QmlCppInterface.setPageSize(width, height - 30)
        //        }
    }

    AddSearchMenu {
        id: newWordSnapped

        anchors.right: longButtonRight.left
        //anchors.right: newWordSnapped.visible ? newWordSnapped.left : longButtonRight.left
        anchors.rightMargin: 0

        anchors.top: parent.top
        anchors.topMargin: 0

        anchors.bottom: parent.bottom
        anchors.bottomMargin: 0
    }

    // Button that allows slide/unslide NewWordSnapped form on the right side
    LongVerticalButton {
        id: longButtonRight

        // custom properties
        imageSource: newWordSnapped.visible ? "/icons/rightArrow.png" : "/icons/leftArrow.png"

        // common properties
        visible: true
        z: 3

        anchors.right: parent.right
        anchors.rightMargin: 0
        anchors.top: parent.top
        anchors.topMargin: 0
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 0

        area.onClicked: {
            newWordSnapped.visible = !newWordSnapped.visible
        }
    }

    //   Button that allows slide/unslide CONTENTS win
    LongVerticalButton {
        id: longButtonLeft

        imageSource: contentsItem.visible ? "/icons/leftArrow.png" : "/icons/rightArrow.png"

        visible: true
        z: 3

        anchors.left: parent.left
        anchors.leftMargin: 0

        anchors.top: parent.top
        anchors.topMargin: 0

        anchors.bottom: parent.bottom
        anchors.bottomMargin: 0

        area.onClicked: {
            contentsItem.visible = !contentsItem.visible
        }
    } // [LongVerticalButton]

    ContentsBookmarksMenu {
        id: contentsItem

        width: 197

        anchors.bottom: parent.bottom
        anchors.left: longButtonLeft.right
        anchors.top: parent.top
    } // [ContentsBookmarksMenu]
} // [Item]
