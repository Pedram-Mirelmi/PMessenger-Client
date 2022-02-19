import QtQuick 2.12
import QtQuick.Controls 2.14
import QtQuick.Window 2.14
import QtQuick.Layouts 1.14
import "../Dialogs/"

Rectangle
{
    id: chatsComp
    SplitView.preferredWidth: 150
    SplitView.minimumWidth: 150
    SplitView.maximumWidth: 400
    color: "lightblue"
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
                text: "P-Messenger"
            }

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
        id: conversationsView
        anchors.top: contactsHeader.bottom
        width: parent.width
        clip: true
        anchors.bottom: parent.bottom
        model: chatsModel


        delegate: Component
        {
            Rectangle
            {
                width: parent.width
                height: 50
//                anchors.fill: parent
                color: chatsModel.currChatIndex === model.index ? "steelblue" : "white"
                MouseArea
                {
                    anchors.fill: parent
                    onClicked:
                    {
                        console.log("clicked on " + model.title)
                        dataHolder.openExistingChatEnv(model.env_id, model.is_pending)
                        chatsModel.currChatIndex = model.index
                    }
                }

                Label
                {
                    id: contactLabelId
                    text: model.title
                    font.pointSize: 20
                }
                //                Label
                //                {
                //                    id: lastMessgeId
                //                    text: "some message ..."
                //                    font.pixelSize: 10
                //                    anchors.top: contactLabelId.bottom
                //                    anchors.topMargin: 5
                //                }

            }
        }
    }
}
