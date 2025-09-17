import QtQuick 2.0
import QtQuick.Window 2.0
import QtQuick.Controls 2.0

ApplicationWindow
{
    id: rootWindow
    width: 555
    height: 115
    visible: true
    color: "transparent"
    flags: Qt.FramelessWindowHint | Qt.WindowStaysOnTopHint | Qt.Tool

    property bool isClockVisible: true
    property bool isFixedPosition: false

    property int dragStartX: 0
    property int dragStartY: 0

    MouseArea 
    {
        id: moveArea
        anchors.fill: parent
        acceptedButtons: Qt.LeftButton
        enabled: !rootWindow.isFixedPosition

        onPressed: 
        {
            dragStartX = mouse.x
            dragStartY = mouse.y
        }
        onPositionChanged: 
        {
            if (mouse.buttons & Qt.LeftButton) 
            {
                rootWindow.x += mouse.x - dragStartX
                rootWindow.y += mouse.y - dragStartY
            }
        }
    }

    MouseArea 
    {
        anchors.fill: parent
        acceptedButtons: Qt.RightButton
        
        onClicked: function(mouseEvent) 
        {
            if (mouseEvent.button === Qt.RightButton) 
            {
                listWidgetWindow.visible = true
                listWidgetWindow.requestActivate()
                listWidgetWindow.x = mouseEvent.x + rootWindow.x
                listWidgetWindow.y = mouseEvent.y + rootWindow.y
            }
        }
    }

    Text 
    {
        id: timeText
        anchors.centerIn: parent
        text: "00:00:00"
        font.pixelSize: 144
        font.bold: true
        color: "white"
        style: Text.Outline
        styleColor: "black"
    }

    Timer 
    {
        interval: 1000
        running: true
        repeat: true
        
        onTriggered: 
        {
            var date = new Date()
            var h = date.getHours().toString().padStart(2, '0')
            var m = date.getMinutes().toString().padStart(2, '0')
            var s = date.getSeconds().toString().padStart(2, '0')
            timeText.text = h + ":" + m + ":" + s
        }
    }

    Window 
    {
        id: listWidgetWindow
        width: 235
        height: 140
        title: "Меню"
        visible: false
        flags: Qt.FramelessWindowHint | Qt.WindowStaysOnTopHint | Qt.Tool
        color: "transparent"

        Rectangle 
        {
            anchors.fill: parent
            color: "#AA2C2C2E"
            radius: 10
            border.color: "#AA444446"
            border.width: 1

            ListView 
            {
                id: listView
                anchors.fill: parent
                anchors.margins: 8
                spacing: 0
                clip: true

                model: ListModel 
                {
                    ListElement { name: "Назад";           action: "back" }
                    ListElement { name: "Скрыть";         action: "hide_or_show" }
                    ListElement { name: "Зафиксирован";   action: "toggle_fix" }
                    ListElement { name: "Удалить";        action: "delete" }
                }

                delegate: Item 
                {
                    width: listView.width
                    height: 30

                    Rectangle 
                    {
                        anchors.fill: parent
                        color: mouseArea.containsMouse ? "#AA3A3A3C" : "transparent"
                    }
                    Rectangle 
                    {
                        visible: index < listView.count - 1
                        anchors.bottom: parent.bottom
                        width: parent.width
                        height: 1
                        color: "#808080"
                    }

                    Image 
                    {
                        id: iconImage
                        anchors.left: parent.left; anchors.leftMargin: 15
                        anchors.verticalCenter: parent.verticalCenter
                        width: 20; height: 20
                        source: 
                        {
                            if (action === "back")              return "../res/back.png"
                            else if (action === "hide_or_show") return rootWindow.isClockVisible ? "../res/hide.png" : "../res/show.png"
                            else if (action === "toggle_fix")   return rootWindow.isFixedPosition ? "../res/block_move.png" : "../res/move.png"
                            else if (action === "delete")       return "../res/delete.png"
                            else                                return ""
                        }

                        fillMode: Image.PreserveAspectFit
                        visible: source !== ""
                    }

                    Text 
                    {
                        anchors.right: parent.right; anchors.rightMargin: 15
                        anchors.verticalCenter: parent.verticalCenter
                    
                        text: 
                        {
                            if (action === "hide_or_show")    return rootWindow.isClockVisible ? "Скрыть" : "Показать"
                            else if (action === "toggle_fix") return rootWindow.isFixedPosition ? "Зафиксирован" : "Не зафиксирован"
                            else                              return name
                        }

                        color: name === "Удалить" ? "#FF6B6B" : "#FFFFFF"
                        font.pixelSize: 16
                        font.bold: true
                    }

                    MouseArea 
                    {
                        id: mouseArea
                        anchors.fill: parent
                        hoverEnabled: true
                    
                        onClicked: 
                        {
                            if (action === "back") 
                            {
                                listWidgetWindow.visible = false
                            }
                            else if (action === "hide_or_show") 
                            {
                                listWidgetWindow.visible = false
                                rootWindow.isClockVisible = !rootWindow.isClockVisible
                                timeText.visible = rootWindow.isClockVisible
                            }
                            else if (action === "toggle_fix") 
                            {
                                listWidgetWindow.visible = false
                                rootWindow.isFixedPosition = !rootWindow.isFixedPosition
                                moveArea.enabled = !rootWindow.isFixedPosition
                            }
                            else if (action === "delete") 
                            {
                                if (typeof process_helper !== "undefined") 
                                {
                                    process_helper.kill_process()
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}