#ifndef ATTITUDE_SOLVER_H
#define ATTITUDE_SOLVER_H

#include <QObject>
#include <QQuaternion>
#include "data_receiver.h"

class AttitudeSolver : public QObject
{
    Q_OBJECT
public:
    explicit AttitudeSolver(QObject *parent = nullptr);

    Q_INVOKABLE void start();
public slots:
    void onDataReceived(long ts, float ax, float ay, float az, float gx, float gy, float gz);
signals:
    void attitudeChanged(qreal aq0, qreal aq1, qreal aq2, qreal aq3);
private:
    DataReceiver* _data_receiver;
    double _q0{1};
    double _q1{0};
    double _q2{0};
    double _q3{0};
    float _lastT{-1};
    double _exInt{0};
    double _eyInt{0};
    double _ezInt{0};
};

#endif // ATTITUDE_SOLVER_H
