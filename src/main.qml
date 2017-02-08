import QtQuick 2.2
import QtQuick.Window 2.1

Window {
    id: mainWindow
    visible: true
    width: 800
    height: 800
    color: "#FFFF0000"

    MouseArea {
        anchors.fill: parent
        onClicked: Qt.quit()
    }

    Rectangle {
        id: testRect
        width: 100
        height: 100
        anchors.centerIn: parent
        color: "#FF0000FF"
    }
}