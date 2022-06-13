import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12
import "./MainEnvComponents/"

ApplicationWindow
{
    id: mainWindow
    width: 1080
    height: 720
    visible: true
    title: qsTr("PMessenger")
    property int user_id: 0
    Connections
    {
        target: netHandler
        function onEntryNetMessageArrived(net_msg)
        {
            if (net_msg["successful"])
            {
                mainEnv.visible = true
                introEnv.destroy()
                mainWindow.user_id = net_msg["user_info"]["user_id"]
            }
        }
    }


    MainEnv
    {
        id: mainEnv
        visible: false
        anchors.fill: parent
    }

    IntroEnv
    {
        id: introEnv
        anchors.fill: parent
        visible: true
    }

}
