import QtQuick 2.14
import QtQuick.Controls 2.14
import QtQuick.Layouts 1.14

Item
{
    id: rootId
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
                    backend.sendLoginReq(usernameField.text, passwordField.text);
                }
            }
            Button
            {
                id: registerButton
                text: "Register"
                onClicked:
                {
                    backend.sendRegisterReq(usernameField.text, passwordField.text);
                }
            }
        }
    }
}
