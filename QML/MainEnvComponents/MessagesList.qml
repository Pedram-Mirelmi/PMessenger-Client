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
                clip: true
                spacing: 5
                model: messagesModel
                delegate: Component
                {
                    Rectangle
                    {
                        anchors.right: model.owner_id === mainWindow.user_id ? chatList.contentItem.right : undefined
                        anchors.left: model.owner_id !== mainWindow.user_id ? chatList.contentItem.left : undefined
                        anchors.margins: 10
                        width: Math.min(messageText.implicitWidth + messageText.anchors.margins, 3/4 * chatList.width)
                        height: messageText.implicitHeight + 2*messageText.anchors.margins
                        color: model.owner_id === mainWindow.user_id ? "#0460b5" : "#3e6c96"

                        Label
                        {
                            id: messageText
                            anchors.fill: parent
                            anchors.margins: 12
                            text: model.message_text
                            wrapMode: Label.Wrap
                        }

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
