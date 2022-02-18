import QtQuick 2.14
import QtQuick.Window 2.14
import QtQuick.Controls 2.14
import "./MainEnvComponents/"

ApplicationWindow
{
    width: 1080
    height: 720
    visible: true
    title: qsTr("P-Messenger")
    Connections
    {
        target: netConn
        function onEntryNetMessageArrived(net_msg)
        {
            if (net_msg["successful"])
            {
                console.log("hereeeee", net_msg)
                mainEnv.visible = true
                introEnv.destroy()
            }
        }
    }


    Image
    {
        id: mainBackGround
        anchors.fill: parent
        source: "qrc:/images/AppBackground.jpg"
    }

    MainEnv
    {
        id: mainEnv
        visible: false
    }

    IntroEnv
    {
        id: introEnv
//        visible: false
    }

}
