import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Window 2.12
import QtQuick.Layouts 2.12
import "../TinyComponents/"

Rectangle
{
    id: messagesComp
    color: "#021c38"
    ColumnLayout
    {
        anchors.fill: parent
        spacing: 0
        Rectangle
        {
            id: chatHeader
            Layout.fillWidth: true
            height: 50
            color: "#023061"
            Label
            {
                id: contactName
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: parent.left
                anchors.leftMargin: 5
                text: chatsModel.currChatTitle
                color: "#54abf7"
                font.bold: true
                font.pixelSize: 25
            }
        }

        Rectangle
        {
            Layout.fillWidth: true
            Layout.fillHeight: true
            gradient: Gradient
            {
                GradientStop {position: 0.0; color: "#2b6aad"}
                GradientStop {position: 1.0; color: "#22548a"}
            }
            Label
            {
                text: "Start chatting!"
                font.pixelSize: 20
                visible: chatList.count === 0
                anchors.centerIn: parent
            }
            ListView
            {
                id: chatList
                anchors.fill: parent
                anchors.topMargin: 5
                clip: true
                spacing: 5
                model: messagesModel
                delegate: Row
                {
                    id: messageDelegate
                    readonly property bool byMe: model.owner_id === mainWindow.user_id

                    anchors.right: byMe ? chatList.contentItem.right : undefined
                    anchors.left: !byMe ? chatList.contentItem.left : undefined
                    anchors.leftMargin : 10
                    anchors.rightMargin : 10
                    Rectangle
                    {

                        anchors.margins: 10
                        width: Math.min(messageText.contentWidth + messageText.anchors.margins, 3/4 * chatList.width)
                        height: messageText.implicitHeight + 2*messageText.anchors.margins
//                        width: parent.width
                        color: messageDelegate.byMe ? "#0460b5" : "#3e6c96"

                        Label
                        {
                            id: messageText
                            anchors.centerIn: parent
                            anchors.margins: 12
                            text: model.message_text
//                            wrapMode: Label.Wrap
                        }
                        border.color: messageDelegate.byMe ? "#022b52" : "#263440"
                        border.width: 2
                    }

                }
            }
        }


        RowLayout
        {
            Layout.fillWidth: true
            MultiLineTextInput
            {
                id: messageInputField
                Layout.fillWidth: true
                backGroundColor: "#021c38"
                placeHolderString: "Write a message..."
            }

            GeneralButton
            {
                id: sendButton
                enabled: messageInputField.text.length > 0
                onButtonClicked:
                {
                    mainApp.sendNewTextMessage(messagesModel.curr_env_id, messageInputField.text)
                    messageInputField.clear()
                }
                Label
                {
                    text: "Send"
                    anchors.centerIn: parent
                }
            }

        }
    }

}
