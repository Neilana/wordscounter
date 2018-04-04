import QtQuick 2.6
import QtQuick.Window 2.2
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.2
import QtQuick.Layouts 1.3

import "../style"
import "../custom_items"
import "../menu_pages"
import "../windows_and_forms"

Item {
    width: 400
    height: 400
    id: ignoredItem

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
            id: imageTextButton2
            text: "Edit"
            anchors.left: imageTextButton1.right
            anchors.leftMargin: 0
            image: "/icons/edit.png"
        }
        ImageTextButton {
            id: imageTextButton3
            text: "Delete"
            anchors.left: imageTextButton2.right
            anchors.leftMargin: 0
            image: "/icons/minus.png"
        }
    }

    TableView {
        sortIndicatorColumn: 0
        sortIndicatorVisible: false
        frameVisible: true

        anchors.right: parent.right
        anchors.rightMargin: 0
        anchors.left: parent.left
        anchors.leftMargin: 0
        anchors.bottom: tableMenuRow.top
        anchors.bottomMargin: 0
        anchors.topMargin: 0
        anchors.top: parent.top

        style: TableViewStyle {
            alternateBackgroundColor: Style.topBarBackgroundColor
            backgroundColor: Style.backgroundColor
            activateItemOnSingleClick: true
            textColor: Style.textColor
        }
        TableViewColumn {
            role: "Word"
            title: "Word"
            //   width: 100
        }

        model: ignoredSqlModel

        headerDelegate: Rectangle {
            height: textItem.implicitHeight + 10
            width: textItem.implicitWidth
            color: Style.highlightHoverMenuColor
            Text {
                id: textItem
                anchors.fill: parent
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: styleData.textAlignment
                text: styleData.value
                wrapMode: Text.WordWrap

                anchors.bottomMargin: 5
                anchors.topMargin: 5
                anchors.leftMargin: 5

                elide: Text.ElideRight
                //renderType: Text.NativeRendering
                color: Style.textColor
                font.family: Style.mainMenuFont.name
                font.pixelSize: 17
            }
            Rectangle {
                anchors.right: parent.right
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 1
                anchors.topMargin: 1
                width: 1
                color: "#ccc"
            }
        }

        itemDelegate: Rectangle {
            id: itemRect
            color: "#00000000"
            anchors.fill: parent

            //height: 70
            //  height: textItem1.impliciHeight + 4
            //  width: 40
            //height: 100
            Text {
                //width: 20
                //height: 100
                id: textItem1
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: styleData.textAlignment

                text: styleData.value
                anchors.right: parent.right
                anchors.rightMargin: 0
                anchors.top: parent.top
                anchors.topMargin: 0
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 0
                anchors.left: parent.left
                anchors.leftMargin: 5

                elide: Text.ElideRight
                wrapMode: Text.WordWrap
                color: Style.textColor
                //                //renderType: Text.NativeRendering
                font.family: Style.tableFont.name
                font.pixelSize: 14
            }

            //            Component.onCompleted: {
            //                if (styleData.column === 1) {
            //                    var buf = rowsHeights
            //                    buf[styleData.row] = parseInt(styleData.value.text)

            //                    rowsHeights = buf
            //                    console.log(textItem1.implicitWidth)
            //                    console.log(rowsHeights)
            //                }
            //            }
        }
        rowDelegate: Rectangle {
            id: rowId
            height: 30
            //height: rowsHeights[styleData.row]
            //color: "#00000000"
            color: {
                if (styleData.selected)
                    return Style.highlightSelectedItemColor
                if (styleData.alternate)
                    return Style.topBarBackgroundColor
                return Style.backgroundColor
            }
        }
    }
}
