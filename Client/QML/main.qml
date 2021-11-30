import QtQuick 2.14
import QtQuick.Window 2.14
import QtQuick.Controls 2.14

import "./MainEnvComponents/"

ApplicationWindow
{
    width: 640
    height: 480
    visible: true
    title: qsTr("P-Messenger")
    Connections
    {
        target: backend
        function onEnterResponseCaugth(outcome, note)
        {
            console.log("hereeeee")
            mainEnv.visible = true
            introEnv.destroy()
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
    }

}
