import QtQuick 2.12
import QtQuick.Controls 2.14
import QtQuick.Window 2.14
import QtQuick.Layouts 2.14
Rectangle
{
    id: chatComp
    property int contact_id: -1
    property string contact_name: ""
    property string contact_username: ""
    property int env_id: -1
    signal newChatMsgSent(string msg_text, int env_id)

    ColumnLayout
    {
        id: layouttt
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
                text: chatComp.contact_name
            }
        }

        ListView
        {
            id: chatList
            clip: true
            spacing: 5
            Layout.fillWidth: true
            Layout.fillHeight: true
            model: ListModel
            {
                id: chatModel
                ListElement
                {
                    sender_id: 1
                    time: "asdfasd"
                    message_text: "Hello love!"

                }
                ListElement
                {
                    sender_id: 2
                    time: "like... NOW!"
                    message_text: "Hello love!"

                }
            }

            delegate: Component
            {
                Rectangle
                {
                    anchors.right: model.sender_id === 1 ? chatList.contentItem.right : undefined
                    anchors.left: model.sender_id !== 1 ? chatList.contentItem.left : undefined
                    anchors.margins: 10
                    width: Math.min(messageText.implicitWidth + messageText.anchors.margins, 3/4 * chatList.width)
                    height: messageText.implicitHeight + 2*messageText.anchors.margins
                    color: model.sender_id === 1 ? "lightblue" : "lightgrey"
                    Label
                    {
                        id: messageText
                        anchors.fill: parent
                        anchors.margins: 12
                        text: "long text long text long text long text long text long text long text long text long text long text long text long text long text long text long text long text long text long text long text long text long text"
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
