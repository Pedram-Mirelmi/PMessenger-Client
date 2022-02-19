import QtQuick 2.14
import QtQuick.Controls 2.14
import QtQuick.Layouts 1.14

Item
{
    anchors.fill: parent

    Column
    {
        anchors.centerIn: parent
        Label
        {
            id: title
            text: "Welcome!"
            font.pointSize: 30
//            color: Qt.color("#FF00695c")
        }

        GridLayout
        {
            columns: 2
            columnSpacing: 50
            width: parent.width

            Label
            {
                text: "Username: "
            }
            TextField
            {
                id: usernameField
                placeholderText: "Your username here ..."
            }

            Label
            {
                text: "Password: "
            }

            TextField
            {
                id: passwordField
                placeholderText: "Your password here"
            }

            Button
            {
                id: loginButton
                text: "Login"
                onClicked:
                {
                    netConn.sendLoginReq(usernameField.text, passwordField.text);
                }
            }
            Button
            {
                id: registerButton
                text: "Register"
                onClicked:
                {
                    netConn.sendRegisterReq(usernameField.text, passwordField.text);
                }
            }
        }
    }
    Item {
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
