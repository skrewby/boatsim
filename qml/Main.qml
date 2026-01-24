import QtQuick
import QtQuick.Window
import QtQuick.Controls

Window {
    width: 1920
    height: 1080
    visible: true
    title: "Boat Simulator"

    Rectangle {
        anchors.fill: parent
        color: "#000000"

        Column {
            anchors.centerIn: parent
            spacing: 10

            Button {
                anchors.horizontalCenter: parent.horizontalCenter
                text: "Run NMEA"
                onClicked: boatController.runNMEA()
            }
        }
    }
}
