import QtQuick 2.14
import QtQuick.Window 2.14
import QtQuick.Controls 2.14
import "./MainEnvComponents/"

ApplicationWindow
{
    width: 1080
    height: 720
    visible: true
    title: qsTr("PMessenger")
    Connections
    {
        target: netConn
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
        visible: false
    }

}
