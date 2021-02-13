import Qt3D.Core 2.0
import Qt3D.Render 2.0
import Qt3D.Input 2.0
import Qt3D.Extras 2.0

import QtQuick 2.0
import QtQuick.Scene3D 2.0

Entity {
    id: sceneRoot
    property bool showReceivedData: true
    property real yaw: 0.0
    property real pitch: 0.0
    property real roll: 0.0
    property real q0: 0.631430 //1.0
    property real q1: -0.040534 //0.0
    property real q2: -0.067639 //0.0
    property real q3: 0.771413 //0.0

    Connections {
        target: g_attitude_solver
        onAttitudeChanged: {
            q0 = aq0
            q1 = aq1
            q2 = aq2
            q3 = aq3
        }
    }

    Camera {
        id: camera
        projectionType: CameraLens.PerspectiveProjection
        fieldOfView: 45
        aspectRatio: 16/9
        nearPlane : 0.1
        farPlane : 1000.0
        position: Qt.vector3d( 10.0, 5.0, 5.0 )
        upVector: Qt.vector3d( 0.0, 0.0, 1.0 )
        viewCenter: Qt.vector3d( 0.0, 0.0, 0.0 )
    }

    OrbitCameraController {
        camera: camera
    }

    components: [
        RenderSettings {
            activeFrameGraph: ForwardRenderer {
                clearColor: Qt.rgba(0, 0.5, 1, 1)
                camera: camera
            }
        },
        // Event Source will be set by the Qt3DQuickWindow
        InputSettings { }
    ]



/////////////////////////////////////////////////////

    PhongMaterial {
        id: material
        ambient: Qt.rgba(0.7, 0.7, 0.7, 1)
    }

    CuboidMesh {
        id: cuboidMesh
        xExtent: 2
        yExtent: 4
        zExtent: 0.5
    }

    Transform {
        id: sensorTransform
//        rotation: fromEulerAngles(pitch, yaw, roll)
        rotation: {
            if (showReceivedData) {
                return Qt.quaternion(q0, q1, q2, q3)
            } else {
                var s = Math.PI / 360
                var cshy = Math.cos(yaw*s)
                var cshp = Math.cos(pitch*s)
                var cshr = Math.cos(roll*s)
                var snhy = Math.sin(yaw*s)
                var snhp = Math.sin(pitch*s)
                var snhr = Math.sin(roll*s)
                var tq0 = cshr*cshp*cshy + snhr*snhp*snhy
                var tq1 = snhr*cshp*cshy - cshr*snhp*snhy
                var tq2 = cshr*snhp*cshy + snhr*cshp*snhy
                var tq3 = cshr*cshp*snhy - snhr*snhp*cshy
                return Qt.quaternion(tq0, tq1, tq2, tq3)
            }
        }
    }

    Entity {
        id: sensorEntity
        components: [ cuboidMesh, material, sensorTransform ]

        Entity {
            id: xaxisEntity
            PhongMaterial {
                id: xmaterial
                ambient: Qt.rgba(1, 0, 0, 1)
            }

            Transform {
                id: xTransform
                rotation: fromAxisAndAngle(Qt.vector3d(0, 0, 1), -90)
                translation: Qt.vector3d(1.5, 0, 0)
            }

            CylinderMesh {
                id: xMesh
                radius: 0.1
                length: 1
            }

            components: [xMesh, xmaterial, xTransform]
        }

        Entity {
            id: yaxisEntity
            PhongMaterial {
                id: ymaterial
                ambient: Qt.rgba(0, 1, 0, 1)
            }

            Transform {
                id: yTransform
                translation: Qt.vector3d(0, 2.5, 0)
            }

            CylinderMesh {
                id: yMesh
                radius: 0.1
                length: 1
            }

            components: [yMesh, ymaterial, yTransform]
        }

        Entity {
            id: zaxisEntity
            PhongMaterial {
                id: zmaterial
                ambient: Qt.rgba(0, 0, 1, 0)
            }

            Transform {
                id: zTransform
                rotation: fromAxisAndAngle(Qt.vector3d(1, 0, 0), -90)
                translation: Qt.vector3d(0, 0, 0.5)
            }

            CylinderMesh {
                id: zMesh
                radius: 0.1
                length: 1
            }

            components: [zMesh, zmaterial, zTransform]
        }
    }

    //////////////////////////////////////////////////////
    PlaneMesh {
        id: xyPlane
        width: 1000
        height: 1000
//        meshResolution: Qt.size(4, 4)
    }

    PhongAlphaMaterial {
        id: planeMaterial
        alpha: 0.5
        ambient: Qt.rgba(0.5, 0.5, 0, 0)
    }

    Transform {
        id: planeTransform
        rotation: fromAxisAndAngle(Qt.vector3d(1, 0, 0), 90)
    }

    Entity {
        id: worldEntity
        components: [ xyPlane, planeMaterial, planeTransform ]

    }

}

