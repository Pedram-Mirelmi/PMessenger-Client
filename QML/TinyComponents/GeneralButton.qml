import QtQuick 2.12

Rectangle
{
    id: generalButton
    property string buttonColor: "#0d9e7f"
    property string buttonEnteredColor: "#0a8c70"
    property string buttonPressedColor: "#067d63"

    signal buttonClicked()
    width: 80
    height: 30

    radius: 5
    color: buttonColor
    MouseArea
    {
        anchors.fill: parent
        hoverEnabled: true
        onEntered: generalButton.color = generalButton.buttonEnteredColor
        onPressed: generalButton.color = generalButton.buttonPressedColor
        onReleased: generalButton.color = generalButton.buttonColor
        onExited: generalButton.color = generalButton.buttonColor
        onClicked: generalButton.buttonClicked()
    }
}
