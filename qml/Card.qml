import QtQuick
import QtQuick.Layouts

Rectangle {
    id: card

    property string title: ""
    default property alias content: contentColumn.data

    Layout.fillWidth: true
    Layout.preferredHeight: 120
    color: "transparent"

    Canvas {
        anchors.fill: parent
        onPaint: {
            var ctx = getContext("2d")
            ctx.strokeStyle = "#666"
            ctx.lineWidth = 2
            ctx.setLineDash([5, 5])
            ctx.beginPath()
            ctx.roundedRect(1, 1, width - 2, height - 2, 8, 8)
            ctx.stroke()
        }
    }

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 12
        spacing: 8

        Text {
            text: card.title
            color: "#aaa"
            font.bold: true
            font.pixelSize: 14
            visible: card.title !== ""
        }

        Item {
            id: contentColumn
            Layout.fillWidth: true
            Layout.fillHeight: true
        }
    }
}
