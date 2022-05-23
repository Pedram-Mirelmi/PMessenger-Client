import QtQuick 2.14
import QtQuick.Controls 2.14
import QtQuick.Window 2.14
import QtQuick.Layouts 2.14

import "MainEnvComponents/"
//import "MainEnvComponents/MessagesList.qml"
import "Dialogs/"

Rectangle
{
    property int user_id: -1
    color: "red"
    NewChatDialog
    {
        id: newChatDialog
    }

    SplitView
    {
        anchors.fill: parent
        ConversationsList
        {
            id: chatsComp
            SplitView.preferredWidth: 250
            SplitView.minimumWidth: 200
            SplitView.maximumWidth: 400
        }

        MessagesList
        {
            id: messagesList
        }

    }


}
