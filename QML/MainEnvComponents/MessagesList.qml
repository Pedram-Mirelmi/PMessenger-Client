import QtQuick 2.12
import QtQuick.Controls 2.14
import QtQuick.Window 2.14
import QtQuick.Layouts 2.14
Rectangle
{
    id: chatComp
    ColumnLayout
    {
        anchors.fill: parent
        Rectangle
        {
            id: chatHeader
            Layout.fillWidth: true
            height: 30
            color: "steelblue"
            Label
            {
                id: contactName
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: parent.left
                anchors.leftMargin: 5
//                text: chatsModel.currChatTitle
            }
        }

        ListView
        {
            id: chatList
            clip: true
            spacing: 5
            Layout.fillWidth: true
            Layout.fillHeight: true
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


        Frame
        {
            Layout.fillWidth: true
            Layout.fillHeight: false

            Layout.maximumHeight:  100
            id: frame
            RowLayout {
                anchors.fill: parent
                TextArea
                {
                    id: messageField
                    Layout.fillWidth: true
                    Layout.maximumHeight: 100
                    placeholderText: "Write a message ..."
                    wrapMode: TextArea.Wrap
                }

                Button {
                    id: sendButton
                    text: "Send"
                    enabled: messageField.length > 0
                    onClicked:
                    {
                        console.log("user sent ", messageField.text)
                        newChatMsgSent(messageField.text, chatComp.env_id)
                        messageField.clear()
                    }
                }
            }

        }
    }

}
