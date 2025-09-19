import QtQuick 2.0
import QtQuick.Window 2.0
import QtQuick.Controls 2.0

ApplicationWindow
{
    id: rootWindow
    width: 280
    height: 79 + 27
    visible: true
    color: "transparent"
    flags: Qt.FramelessWindowHint | Qt.Tool

    Text 
    {
        id: cpu
        text: "Load CPU"
        x: 0
        y: 0
        font.pixelSize: 25
        color: "white"
        style: Text.Outline
    }
    
    Text 
    {
        id: gpu
        text: "Load GPU"
        x: 0
        y: 27
        font.pixelSize: 25
        color: "white"
        style: Text.Outline
    }

    Text 
    {
        id: rom
        text: "Load ROM"
        x: 0
        y: 52
        font.pixelSize: 25
        color: "white"
        style: Text.Outline
    }

    Text 
    {
        id: ram
        text: "Load RAM"
        x: 0
        y: 79
        font.pixelSize: 25
        color: "white"
        style: Text.Outline
    }

    Timer 
    {
        interval: 1000
        running: true
        repeat: true
        
        onTriggered: 
        {
            cpu.text = "CPU:  " + processor.cpu();
            gpu.text = "GPU:  " + processor.gpu();
            rom.text = "ROM: "  + processor.rom();
            ram.text = "RAM: "  + processor.ram();
        }
    }

    MouseArea 
    {
        id: moveArea
        anchors.fill: parent
        acceptedButtons: Qt.LeftButton

        property int drag_start_x: 0
        property int drag_start_y: 0

        onPressed: 
        {
            drag_start_x = mouse.x
            drag_start_y = mouse.y
        }
        onPositionChanged: 
        {
            if (mouse.buttons & Qt.LeftButton) 
            {
                rootWindow.x += mouse.x - drag_start_x
                rootWindow.y += mouse.y - drag_start_y
            }
        }
    }
}