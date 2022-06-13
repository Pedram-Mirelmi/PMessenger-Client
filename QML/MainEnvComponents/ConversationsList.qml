import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Window 2.12
import QtQuick.Layouts 1.12
import "../Dialogs/"
import "../TinyComponents/"

Rectangle
{
    id: conversationsComp
    Rectangle
    {
        id: conversationsHeader
        width: parent.width
        height: 50
        anchors.top: parent.top
        color: "#2b6aad"
        Label
        {
            id: mainTitle
            anchors.left: parent.left
            color: "#0a2d47"
            text: "P-Messenger"
            font.bold: true
            font.pixelSize: 25
            anchors.verticalCenter: parent.verticalCenter
            anchors.leftMargin: 5
        }

        GeneralButton
        {
            id: contactsFooter
            anchors.right: parent.right
            height: parent.height
            width: parent.height
            buttonColor: "#2b6aad"
            buttonEnteredColor: "#407dbd"
            buttonPressedColor: "#4c85c2"
            onButtonClicked: mainSearchDiolog.open()
            Image
            {
                id: searchButton
                width: parent.width / 2
                height: parent.height / 2
                source: "qrc:/images/search.png"
                anchors.centerIn: parent
            }
        }
    }

    Rectangle
    {
        id: splitter
        color: "#0f5196"
        anchors.top: conversationsHeader.bottom
        width: parent.width
        height: 5
    }

    Rectangle
    {
        anchors.top: splitter.bottom
        anchors.bottom: parent.bottom
        width: parent.width
        gradient: Gradient
        {
            GradientStop {position: 0.0; color: "#2b6aad"}
            GradientStop {position: 1.0; color: "#22548a"}
        }
        ListView
        {
            id: conversationsView
            //            anchors.top: contactsHeader.bottom
            //            anchors.bottom: parent.bottom
            //            width: parent.width
            anchors.fill: parent
            clip: true
            model: chatsModel


            delegate: Component
            {
                Item
                {
                    width: parent.width
                    height: 50
//                    color: chatsModel.currChatIndex === model.index ? "#0492b5" : "#2b6aad"

                    MouseArea
                    {
                        anchors.fill: parent
                        cursorShape: Qt.PointingHandCursor
                        onClicked:
                        {
                            mainApp.openExistingChatEnv(model.env_id, model.is_pending)
                            chatsModel.currChatIndex = model.index
                        }
                    }
                    Label
                    {
                        id: contactLabelId
                        text: model.title
                        font.pointSize: 20
                    }
                }
            }
        }
    }
}
