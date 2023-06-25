import QtQuick 2.0
import QtQuick.Scene3D 2.0
import QtQuick.Controls 2.0

Item {
    width: 800
    height: 600

    Component.onCompleted: {
        console.log("IMU-watch start...")
    }

    Scene3D {
        id: scene3d
        anchors.fill: parent
        focus: true
        aspects: ["input", "logic"]
        cameraAspectRatioMode: Scene3D.AutomaticAspectRatio

        SceneEntity {
            yaw: yawSlider.value     // rotate about z
            pitch: pitchSlider.value // rotate about y
            roll: rollSlider.value   // rotate about x
        }
    }

    Column {
        id: sliders
        anchors.right: parent.right
        anchors.rightMargin: 0
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 0

        TextedSlider {
            id: yawSlider
            anchors.right: parent.right
            anchors.rightMargin: 0
            title: "yaw"
        }

        TextedSlider {
            id: pitchSlider
            anchors.right: parent.right
            anchors.rightMargin: 0
            title: "pitch"
        }

        TextedSlider {
            id: rollSlider
            anchors.right: parent.right
            anchors.rightMargin: 0
            title: "roll"
        }

    }

}

