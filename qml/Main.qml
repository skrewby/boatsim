import QtQuick
import QtQuick.Window
import QtQuick.Controls
import QtQuick.Layouts

Window {
    width: 800
    height: 600
    visible: true
    title: "Boat Simulator"

    component ValueSpinBox: SpinBox {
        property int decimals: 2
        property real realValue: 0
        property real multiplier: 100
        property real realStep: 1.0

        implicitWidth: 120

        from: 0
        to: 2147483647
        value: realValue * multiplier
        stepSize: realStep * multiplier
        editable: true

        onValueModified: realValue = value / multiplier

        textFromValue: function(value) {
            return (value / multiplier).toFixed(decimals)
        }

        valueFromText: function(text) {
            return parseFloat(text) * multiplier
        }

        background: Rectangle { color: "#333"; border.color: "#555"; radius: 3 }
        contentItem: TextInput {
            text: parent.textFromValue(parent.value)
            color: "#eee"
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            inputMethodHints: Qt.ImhFormattedNumbersOnly
        }
    }

    Rectangle {
        anchors.fill: parent
        color: "#1a1a1a"

        ColumnLayout {
            anchors.fill: parent
            anchors.margins: 20
            spacing: 20

            RowLayout {
                Layout.fillWidth: true
                spacing: 10

                ComboBox {
                    id: canInterfaceCombo
                    Layout.preferredWidth: 150
                    implicitHeight: 20
                    model: boatController?.CanInterfaces ?? []
                    enabled: !(boatController?.Connected ?? true)

                    background: Rectangle {
                        color: "#333"
                        border.color: "#555"
                        radius: 4
                        implicitHeight: 20
                    }

                    contentItem: Text {
                        text: canInterfaceCombo.displayText
                        color: "#eee"
                        verticalAlignment: Text.AlignVCenter
                        leftPadding: 8
                    }
                }

                Button {
                    text: (boatController?.Connected ?? false) ? "Connected" : "Connect"
                    enabled: !(boatController?.Connected ?? true) && canInterfaceCombo.currentText !== ""
                    onClicked: boatController?.ConnectToInterface(canInterfaceCombo.currentText)
                    implicitHeight: 20

                    background: Rectangle {
                        color: parent.enabled ? "#4a4a4a" : "#333"
                        border.color: "#666"
                        radius: 4
                        implicitHeight: 20
                    }

                    contentItem: Text {
                        text: parent.text
                        color: parent.enabled ? "#eee" : "#888"
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                    }
                }

                Button {
                    text: (boatController?.Sending ?? false) ? "Stop" : "Start"
                    enabled: boatController?.Connected ?? false
                    onClicked: (boatController?.Sending ?? false) ? boatController?.Stop() : boatController?.Start()
                    implicitHeight: 20

                    background: Rectangle {
                        color: parent.enabled ? ((boatController?.Sending ?? false) ? "#8b4444" : "#448b44") : "#333"
                        border.color: "#666"
                        radius: 4
                        implicitHeight: 20
                    }

                    contentItem: Text {
                        text: parent.text
                        color: parent.enabled ? "#eee" : "#888"
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                    }
                }

                Text {
                    text: boatController?.ErrorMessage ?? ""
                    color: "#ff6666"
                    visible: (boatController?.ErrorMessage ?? "") !== ""
                }

                Item { Layout.fillWidth: true }

                Text {
                    text: (boatController?.CanInterfaces ?? []).length === 0 ? "No CAN interfaces found" : ""
                    color: "#888"
                    visible: (boatController?.CanInterfaces ?? []).length === 0
                }
            }

            GridLayout {
                Layout.fillWidth: true
                Layout.fillHeight: true
                columns: 2
                rowSpacing: 15
                columnSpacing: 15

                Card {
                    title: "COGSOGRapid (PGN 129026)"

                    GridLayout {
                        anchors.fill: parent
                        columns: 2
                        columnSpacing: 15
                        rowSpacing: 6

                        Text { text: "COG (deg)"; color: "#888"; font.pixelSize: 12; Layout.preferredWidth: 90 }
                        ValueSpinBox {
                            realValue: boatController?.cog ?? 0
                            onRealValueChanged: if (boatController) boatController.cog = realValue
                            to: 36000
                        }

                        Text { text: "SOG (m/s)"; color: "#888"; font.pixelSize: 12; Layout.preferredWidth: 90 }
                        ValueSpinBox {
                            realValue: boatController?.sog ?? 0
                            onRealValueChanged: if (boatController) boatController.sog = realValue
                            to: 10000
                            realStep: 0.1
                        }
                    }
                }

                Card {
                    title: "Temperature (PGN 130312)"

                    GridLayout {
                        anchors.fill: parent
                        columns: 2
                        columnSpacing: 15
                        rowSpacing: 6

                        Text { text: "Temp (K)"; color: "#888"; font.pixelSize: 12; Layout.preferredWidth: 90 }
                        ValueSpinBox {
                            realValue: boatController?.temperature ?? 0
                            onRealValueChanged: if (boatController) boatController.temperature = realValue
                            to: 50000
                        }
                    }
                }

                Card {
                    title: "Humidity (PGN 130313)"

                    GridLayout {
                        anchors.fill: parent
                        columns: 2
                        columnSpacing: 15
                        rowSpacing: 6

                        Text { text: "Humidity (%)"; color: "#888"; font.pixelSize: 12; Layout.preferredWidth: 90 }
                        ValueSpinBox {
                            realValue: boatController?.humidity ?? 0
                            onRealValueChanged: if (boatController) boatController.humidity = realValue
                            to: 10000
                        }
                    }
                }

                Card {
                    title: "Pressure (PGN 130314)"

                    GridLayout {
                        anchors.fill: parent
                        columns: 2
                        columnSpacing: 15
                        rowSpacing: 6

                        Text { text: "Pressure (Pa)"; color: "#888"; font.pixelSize: 12; Layout.preferredWidth: 90 }
                        ValueSpinBox {
                            realValue: boatController?.pressure ?? 0
                            onRealValueChanged: if (boatController) boatController.pressure = realValue
                            to: 20000000
                            decimals: 0
                            multiplier: 1
                            realStep: 1000
                        }
                    }
                }
            }

            Item { Layout.fillHeight: true }
        }
    }
}
