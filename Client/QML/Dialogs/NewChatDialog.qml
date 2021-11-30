import QtQuick 2.14
import QtQuick.Controls 2.14
import QtQuick.Window 2.14
import QtQuick.Layouts 1.14


Dialog
{
    id: customeDialog
    width: 400
    height: 600
    signal searchedFor(string phrase)
    background: Rectangle
    {
        anchors.fill: parent
        color: "steelblue"
    }
    anchors.centerIn: parent
    Frame
    {
        anchors.fill: parent
        ColumnLayout
        {
            anchors.fill: parent
            spacing: 10
            Label
            {
                text: "Start new chat"
                Layout.alignment: Qt.AlignCenter

                font.pointSize: 30
            }

            Column
            {
                spacing: 5
                Layout.fillWidth: true
                Label
                {
                    text: "Search for a user"
                }
                TextField
                {
                    id: searchField
                    width: parent.width
                    placeholderText: "Enter a username here"
                    font.pointSize: 15
                }

            }

            Button
            {
                Layout.alignment: Qt.AlignCenter
                text: "Search"
                onClicked:
                {
                    searchedFor(searchField.text)
                }
            }
            Frame
            {
                Layout.fillHeight: true
                Layout.fillWidth: true
                ListView
                {
                    id: resutList
                    anchors.fill: parent
                    clip: true
                    model: ListModel
                    {
                        ListElement
                        {
                            username: "Minil"
                            name: "Mina"
                        }
                        ListElement
                        {
                            username: "Minillll"
                            name: "Minaaa"
                        }
                    }
                    delegate: Component
                    {
                        Rectangle
                        {
                            width: resutList.width
                            height: inputComp.implicitHeight
                            color: "lightblue"
                            ColumnLayout
                            {
                                id: inputComp
                                anchors.fill: parent
                                spacing: 10
                                Label
                                {
                                    Layout.fillWidth: true
                                    Layout.fillHeight: true
                                    text: model.name
                                    font.pointSize: 20
                                }
                                Label
                                {
                                    Layout.fillWidth: true
                                    Layout.fillHeight: true
                                    text: '@' + model.username
                                    font.pointSize: 10
                                    color: "steelblue"
                                }
                            }

                            MouseArea
                            {
                                anchors.fill: parent
                                onClicked:
                                {
                                    console.log("clicked on ", model.username)
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}
