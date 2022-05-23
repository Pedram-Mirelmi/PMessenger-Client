import QtQuick 2.14
import QtQuick.Controls 2.14
import QtQuick.Layouts 1.14
import "./TinyComponents/"
Rectangle
{
    id: introEnv
    Image
    {
        id: mainBackGround
        anchors.fill: parent
        source: "qrc:/images/BlueBackGround.jpg"
    }
    Label
    {
        id: title
        text: "PMessenger!"
        font.pointSize: parent.height / 15
        color: Qt.color("#5ca3ab")
        anchors
        {
            top: parent.top
            topMargin: 40
            horizontalCenter: parent.horizontalCenter
        }
    }


    ColumnLayout
    {
        spacing: 50
        anchors.centerIn: parent
        Label
        {
            text: netConn.netConnected ? "All Good!" : "Connecting ..."
            color: Qt.color("#5c9196")
            Layout.alignment: Qt.AlignHCenter
            font.pixelSize: 2*title.font.pixelSize/3
        }

        GridLayout
        {
            columns: 2
            Label
            {
                text: "Username: "
                font.pixelSize: usernameField.fontPixelSize
            }
            SingleTextInput
            {
                id: usernameField
                placeHolderString: ""
                height: 30
            }

            Label
            {
                text: "Password:"
                font.pixelSize: usernameField.fontPixelSize
            }

            SingleTextInput
            {
                id: passwordField
                placeHolderString: ""
                height: 30
            }

        }
        RowLayout
        {

            GeneralButton
            {
                id: loginButton
                Layout.fillWidth: true
                height: 25
                width: 90
                onButtonClicked:
                {
                    netConn.sendLoginReq(usernameField.text, passwordField.text);
                }
                Text
                {
                    anchors.centerIn: parent
                    color: "white"
                    text: "Login"
                }
            }
            GeneralButton
            {
                id: registerButton
                Layout.fillWidth: true
                height: 25
                width: 100
                onButtonClicked:
                {
                    netConn.sendRegisterReq(usernameField.text, passwordField.text);
                }
                Text
                {
                    anchors.centerIn: parent
                    color: "white"
                    text: "Login"
                }

            }
        }
    }


    Item
    {
        id: connectionStatus
        width: 40
        height: width
        Image
        {
            id: icon
            anchors.fill: parent
            source: netConn.netConnected ? "qrc:/images/connected.png" : "qrc:/images/disconnected.png"
        }
    }
}
