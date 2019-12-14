import QtQuick 2.0
import QtQuick.Controls 2.0

Item {
    id: root
    width: row.implicitWidth
    height: row.implicitHeight
    property alias title: sliderText.text
    property alias value: slider.value

    Row {
        id: row
        Text {
            id: sliderText
            text: "title"
            font.pointSize: 14
            anchors.verticalCenter: parent.verticalCenter
        }

        Slider {
            id: slider
            from: -180
            to: 180
            value: 0
        }

        Text {
            id: sliderValue
            width: 80
            text: slider.value.toFixed(1)
            horizontalAlignment: Text.AlignRight
            font.pointSize: 14
            anchors.verticalCenter: parent.verticalCenter
        }
    }
}
