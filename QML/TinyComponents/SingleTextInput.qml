import QtQuick 2.0
import QtQuick.Controls 2.12
Rectangle
{
    id: myTextInput
    height: 40
    width: 200
    property string fontColor: "#dcf2fa"
    property string backGroundColor: "#113340"
    property string placeHolderStringColor: "#7c7d7d"
    property string placeHolderString: "Enter something here ..."
    property int fontPixelSize: textInput.font.pixelSize
    property string text: textInput.text
    color: backGroundColor

    MouseArea
    {
        anchors.fill: parent
        cursorShape: Qt.IBeamCursor
    }

    TextInput
    {
        id: textInput
        selectByMouse: true
        anchors.fill: myTextInput
        clip: true
        font.pixelSize: 7*parent.height/8
        color: myTextInput.fontColor
        selectionColor: "#2d524a"


        Text
        {
            anchors.fill: parent
            text: myTextInput.placeHolderString
            font.pixelSize: parent.font.pixelSize
            color: myTextInput.placeHolderStringColor
            visible: !textInput.text
        }
    }
}
