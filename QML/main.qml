import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12
import "./MainEnvComponents/"

ApplicationWindow
{
    width: 1080
    height: 720
    visible: true
    title: qsTr("PMessenger")
    Connections
    {
        target: netHandler
        function onEntryNetMessageArrived(net_msg)
        {
            if (net_msg["successful"])
            {
                mainEnv.visible = true
                introEnv.destroy()
            }
        }
    }


    MainEnv
    {
        id: mainEnv
        visible: true
        anchors.fill: parent
    }

    IntroEnv
    {
        id: introEnv
        anchors.fill: parent
        visible: true
    }

}
