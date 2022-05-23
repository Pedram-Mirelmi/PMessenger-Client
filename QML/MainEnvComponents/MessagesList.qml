import QtQuick 2.12
import QtQuick.Controls 2.14
import QtQuick.Window 2.14
import QtQuick.Layouts 2.14
import "../TinyComponents/"

Rectangle
{
    id: chatComp
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
            Image
            {
                anchors.fill: parent
                id: chatBackGround
                source: "qrc:/images/BlueBackGround.jpg"
            }
            ListView
            {
                id: chatList
                clip: true
                spacing: 5
                model: messagesModel
                delegate: Component
                {
                    Rectangle
                    {
                        anchors.right: model.owner_id === 1 ? chatList.contentItem.right : undefined
                        anchors.left: model.owner_id !== 1 ? chatList.contentItem.left : undefined
                        anchors.margins: 10
                        width: Math.min(messageText.implicitWidth + messageText.anchors.margins, 3/4 * chatList.width)
                        height: messageText.implicitHeight + 2*messageText.anchors.margins
                        color: model.owner_id === 1 ? "lightblue" : "lightgrey"
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
                    console.log("user sent ", messageInputField.text)
                    //                        newChatMsgSent(messageField.text, chatComp.env_id)
                    //                        messageField.clear()
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
