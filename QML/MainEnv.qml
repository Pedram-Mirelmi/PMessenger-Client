import QtQuick 2.12
import QtQuick.Controls 2.14
import QtQuick.Window 2.12
import QtQuick.Layouts 2.12

import "MainEnvComponents/"
//import "MainEnvComponents/MessagesList.qml"
import "Dialogs/"

Rectangle
{
    property int user_id: -1
    color: "red"
    MainSearchDialog
    {
        id: mainSearchDiolog
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
