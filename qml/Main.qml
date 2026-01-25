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

            Text {
                anchors.horizontalCenter: parent.horizontalCenter
                color: "#aaaaaa"
                text: "No CAN interfaces found"
                visible: (boatController?.CanInterfaces ?? []).length === 0
            }

            Row {
                spacing: 10
                anchors.horizontalCenter: parent.horizontalCenter
                visible: (boatController?.CanInterfaces ?? []).length > 0

                ComboBox {
                    id: canInterfaceCombo
                    width: 200
                    model: boatController?.CanInterfaces ?? []
                    enabled: !(boatController?.Connected ?? true)
                }

                Button {
                    text: (boatController?.Connected ?? false) ? "Connected" : "Connect"
                    enabled: !(boatController?.Connected ?? true) && canInterfaceCombo.currentText !== ""
                    onClicked: boatController?.ConnectToInterface(canInterfaceCombo.currentText)
                }
            }

            Text {
                anchors.horizontalCenter: parent.horizontalCenter
                color: "#ff6666"
                text: boatController?.ErrorMessage ?? ""
                visible: (boatController?.ErrorMessage ?? "") !== ""
            }

            Button {
                anchors.horizontalCenter: parent.horizontalCenter
                text: (boatController?.Sending ?? false) ? "Stop" : "Start"
                enabled: boatController?.Connected ?? false
                onClicked: (boatController?.Sending ?? false) ? boatController?.Stop() : boatController?.Start()
            }
        }
    }
}
