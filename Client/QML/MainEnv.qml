import QtQuick 2.14
import QtQuick.Controls 2.14
import QtQuick.Window 2.14
import QtQuick.Layouts 2.14

import "./MainEnvComponents/"
import "./Dialogs/"

Item
{
    id: root
    anchors.fill: parent
    property int user_id: -1
    Connections
    {
        target: backend
    }

    SplitView
    {
        anchors.fill: parent
        ContactsComponent
        {
            onOpenNewChatdialog: newChatDialog.open()
            id: contactsComp
        }

        ChatEnv
        {
            id: chatComp
        }

    }
}
