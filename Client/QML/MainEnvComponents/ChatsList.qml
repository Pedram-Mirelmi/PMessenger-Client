import QtQuick 2.12
import QtQuick.Controls 2.14
import QtQuick.Window 2.14
import QtQuick.Layouts 1.14

import "../Dialogs/"

Rectangle
{
    id: contactsComp
    SplitView.preferredWidth: 150
    SplitView.minimumWidth: 150
    SplitView.maximumWidth: 400
    color: "lightblue"
    signal openNewChatdialog()
    Rectangle
    {
        id: contactsHeader
        width: parent.width
        height: 30
        anchors.top: parent.top
        color: "steelblue"
        RowLayout
        {
            anchors.fill: parent
            Label
            {
                id: mainTitle
                Layout.fillHeight: true
                verticalAlignment: Qt.AlignVCenter
                Layout.leftMargin: 10
                text: "M-Messenger"
            }

//            Button
//            {
//                Label
//                {
//                    text: "test"
//                }
//                onClicked: mainApp.test()
//                onDoubleClicked: mainApp.test2()
//            }

            ToolButton
            {
                id: contactsFooter
                Layout.fillHeight: true
                Layout.alignment: Qt.AlignRight
                Layout.minimumWidth: 30
                Layout.maximumWidth: 30
                Image
                {
                    id: searchButton
                    source: "qrc:/images/search.png"
                    width: 25
                    height: 25
                    anchors.centerIn: parent
                }
                onClicked:
                {
                    newChatDialog.open()
                }
            }
        }

    }


    ListView
    {
        id: contactsList
        anchors.top: contactsHeader.bottom
        width: parent.width
        clip: true
        anchors.bottom: parent.bottom
        model: chatsModel


        delegate: Component
        {
            Frame
            {
                width: parent.width
                height: 50
                MouseArea
                {
                    anchors.fill: parent
                    onClicked:
                    {
                        console.log("clicked on", model.name)
                    }
                }

                Label
                {
                    id: contactLabelId
                    text: model.name
                    font.pointSize: 15
                }
                Label
                {
                    id: lastMessgeId
                    text: "some message ..."
                    font.pixelSize: 10
                    anchors.top: contactLabelId.bottom
                    anchors.topMargin: 5
                }
            }
        }
    }
}
