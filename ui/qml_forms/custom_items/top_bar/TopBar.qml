import QtQuick 2.7
import QtQuick.Layouts 1.3
import QtQuick.Window 2.2

import "../../style"
import "../../custom_items"

Item {
    id: topBarItem

    property Window parentWindow
    property QtObject container

    property alias closeArea: closeButton.mouseArea
    property alias minButton: minButton
    property alias maxButton: maxButton
    property alias closeButton: closeButton
    property alias programStatusRect: programStatusRectangle

    property alias mainMenuBarButton: mainMenuBarButton
    property alias readingBarButton: readingBarButton

    property alias barMouseArea: barMouseArea

    width: parent.width
    height: 25

    anchors.right: parent.right
    anchors.left: parent.left
    anchors.top: parent.top

    anchors.rightMargin: Style.borderWidth
    anchors.leftMargin: Style.borderWidth
    anchors.topMargin: Style.borderWidth

    Rectangle {
        id: barMovingRectangle

        anchors.fill: parent
        color: Style.topBarBackgroundColor

        MouseArea {
            id: barMouseArea

            property var clickPos
            anchors.fill: parent

            onPressed: {
                clickPos = {
                    x: mouse.x,
                    y: mouse.y
                }
            }

            onPositionChanged: {
                container.x = mousePosition.cursorPos().x - clickPos.x
                container.y = mousePosition.cursorPos().y - clickPos.y
            }
        }
    } // [barMovingRectangle]

    RowLayout {
        id: barRowLayout

        anchors.right: parent.right
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        z: 3
        spacing: 0

        Rectangle {
            id: programStatusRectangle

            height: 15
            width: childrenRect.width

            anchors.right: minButton.left
            anchors.rightMargin: 10
            //            anchors.bottomMargin: 2
            color: "transparent"
            opacity: 0.5
            z: 3

            Layout.maximumHeight: 25
            Layout.maximumWidth: 200
            Layout.fillHeight: true

            Text {
                property var programStatus: QmlCppInterface.programStatus

                anchors.verticalCenter: parent.verticalCenter

                text: "^__^"
                color: Style.textColor

                font.family: Style.tableFont.name
                font.pixelSize: 16

                onProgramStatusChanged: text = programStatus
            }
        } // [programStatusRectangle]

        SmallTopButton {
            id: minButton

            Layout.fillHeight: true

            imageSource: "/icons/minimize.png"
            mouseArea.onClicked: {
                parentWindow.showMinimized()
            }
        } // [minButton]

        SmallTopButton {
            id: maxButton

            Layout.fillHeight: true

            imageSource: "/icons/fullscreen.png"
            mouseArea.onClicked: {
                if (parentWindow.visibility != Window.Maximized) {
                    parentWindow.showMaximized()
                    imageSource = "/icons/windowscreen.png"
                } else {
                    parentWindow.visibility = Window.Windowed
                    imageSource = "/icons/fullscreen.png"
                }
            }
        } // [maxButton]

        SmallTopButton {
            id: closeButton

            imageSource: "/icons/close.png"

            Layout.fillHeight: true
        } // [closeButton]
    } // [RowLayout] three top right buttons

    // top menu buttons
    RowLayout {
        id: menuBarLayout

        anchors.left: parent.left
        anchors.top: parent.top
        anchors.bottom: parent.bottom

        spacing: 0

        ImageTextButton {
            id: mainMenuBarButton

            height: parent.height
            width: 110

            text: "Home"
            image: "/icons/home.png"

            area.onClicked: {
                rootStackLayout.currentIndex = 0
            }
            rectangleColor: {
                if (area.containsMouse)
                    return Style.topBarHighlightHoverColor
                if (rootStackLayout.currentIndex == 0)
                    return Style.topSelectedColor
                return Style.topBarBackgroundColor
            }
        }

        ImageTextButton {
            id: readingBarButton

            height: parent.height
            width: 110

            text: "Book"
            image: "/icons/openBook_big.png"

            area.onClicked: {
                // !!! readingWindow.view.focus = true
                rootStackLayout.currentIndex = 1
            }
            rectangleColor: {
                if (area.containsMouse)
                    return Style.topBarHighlightHoverColor
                if (rootStackLayout.currentIndex == 1)
                    return Style.topSelectedColor
                return Style.topBarBackgroundColor
            }
        }
    } // [menuBarLayout]
}
