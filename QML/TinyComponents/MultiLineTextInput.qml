import QtQuick 2.12
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
    property int fontPixelSize: textArea.font.pixelSize
    property string text: textArea.text
    color: backGroundColor

    MouseArea
    {
        anchors.fill: parent
        cursorShape: Qt.IBeamCursor
    }

    ScrollView
    {
        id: view
        anchors.fill: parent
        TextArea
        {
            id: textArea
            color: myTextInput.fontColor
            background: Rectangle{color: myTextInput.backGroundColor}
            selectByMouse: true
            placeholderText: myTextInput.placeHolderString
            placeholderTextColor: myTextInput.placeHolderStringColor
        }
    }


}
