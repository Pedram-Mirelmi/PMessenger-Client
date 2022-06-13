import QtQuick 2.14
import QtQuick.Controls 2.14
import QtQuick.Window 2.14
import QtQuick.Layouts 1.14
import "../TinyComponents/"
Dialog
{
    id: mainSearchDiolog
    width: 400
    height: 600
    background: Rectangle
    {
        anchors.fill: parent
        color: "#22548a"
    }
    Connections
    {
        target: netHandler
        function onUsernameSearchResultArrived(search_result)
        {
            console.log("search result received!")
            for (let i = 0; i < search_result.length; i++)
            {
                let user_info = search_result[i];
                resultListModel.append({"user_id": user_info["user_id"],
                                        "username": user_info["username"],
                                        "name": user_info["name"]});
            }
        }
    }
    onClosed: resultListModel.clear()

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
                SingleTextInput
                {
                    id: searchField
                    width: parent.width
                    placeHolderString: "Enter a username here"
                    fontPixelSize: 15
                }

            }

            GeneralButton
            {
                Layout.alignment: Qt.AlignCenter
                Label
                {
                    text: "Search"
                    anchors.centerIn: parent
                }
                onButtonClicked:
                {
                    resultListModel.clear()
                    netHandler.sendUsernameSearchReq(searchField.text)
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
                        id: resultListModel
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
                                    console.log("clicked on " + model.name)
                                    db.tryToInsertUser({"user_id": model.user_id,
                                                        "username": model.username,
                                                        "name": model.name})
                                    mainApp.openPrivateChatWith(model.user_id, model.name)
                                    mainSearchDiolog.close()
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}
