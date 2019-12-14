QT += qml quick 3dinput

SOURCES += \
    main.cpp \
    data_receiver.cpp \
    attitude_solver.cpp

OTHER_FILES += \
    SceneEntity.qml \
    main.qml

RESOURCES += \
    resources.qrc

HEADERS += \
    data_receiver.h \
    attitude_solver.h
