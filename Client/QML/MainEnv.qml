import QtQuick 2.14
import QtQuick.Controls 2.14
import QtQuick.Window 2.14
import QtQuick.Layouts 2.14

import "MainEnvComponents/"
//import "MainEnvComponents/MessagesList.qml"
import "Dialogs/"

Item
{
    anchors.fill: parent
    property int user_id: -1

    NewChatDialog
    {
        id: newChatDialog
    }

    SplitView
    {
        anchors.fill: parent

        ChatsList
        {
            id: chatsComp
        }

        MessagesList
        {
            id: messagesList
        }

    }


}
