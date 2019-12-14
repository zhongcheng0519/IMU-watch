#include "attitude_solver.h"
#include <math.h>

AttitudeSolver::AttitudeSolver(QObject *parent) : QObject(parent)
{
    _data_receiver = new DataReceiver();
    connect(_data_receiver, SIGNAL(dataReceived(RawData_t)), this, SLOT(onDataReceived(RawData_t)));
}

void AttitudeSolver::start()
{
    if (_data_receiver == NULL)
    {
        qWarning() << "Data receiver has not been initialized!";
        return;
    }
    _data_receiver->start();
}

void AttitudeSolver::onDataReceived(RawData_t raw_data)
{
    double Kp = 0.0015;
    double Ki = 0.000001;
    double ax = raw_data.acc_x;
    double ay = raw_data.acc_y;
    double az = raw_data.acc_z;
    double gx = raw_data.gyro_x;
    double gy = raw_data.gyro_y;
    double gz = raw_data.gyro_z;
    double acc_norm = sqrt(ax*ax + ay*ay + az*az);

    // norm acc
    ax = ax / acc_norm;
    ay = ay / acc_norm;
    az = az / acc_norm;

    float halfT = 0;
    if (_lastT == -1)
    {
        _lastT = raw_data.timestamp;
        return;
    }
    else
    {
        // ms -> s
        halfT = (raw_data.timestamp - _lastT)/1000/2;
        _lastT = raw_data.timestamp;
    }
    // update q
    _q0 = _q0 + (-_q1*gx - _q2*gy - _q3*gz)* halfT;
    _q1 = _q1 + (_q0*gx + _q2*gz - _q3*gy) * halfT;
    _q2 = _q2 + (_q0*gy - _q1*gz + _q3*gx) * halfT;
    _q3 = _q3 + (_q0*gz + _q1*gy - _q2*gx) * halfT;

    // estimated direction of gravity
    double est_gx = 2*(_q1*_q3 - _q0*_q2);
    double est_gy = 2*(_q0*_q1 + _q2*_q3);
    double est_gz = _q0*_q0 - _q1*_q1 - _q2*_q2 + _q3*_q3;

    // error calculating by cross product
    double ex = (ay*est_gz - az*est_gy);
    double ey = (az*est_gx - ax*est_gz);
    double ez = (ax*est_gy - ay*est_gx);

    // integral error scaled integral gain
    _exInt = _exInt + ex*Ki;
    _eyInt = _eyInt + ey*Ki;
    _ezInt = _ezInt + ez*Ki;

    // adjusted gyroscope measurements
    gx = gx + Kp*ex + _exInt;
    gy = gy + Kp*ey + _eyInt;
    gz = gz + Kp*ez + _ezInt;

    // update new quarternion
    _q0 = _q0 + (-_q1*gx - _q2*gy - _q3*gz)* halfT;
    _q1 = _q1 + (_q0*gx + _q2*gz - _q3*gy) * halfT;
    _q2 = _q2 + (_q0*gy - _q1*gz + _q3*gx) * halfT;
    _q3 = _q3 + (_q0*gz + _q1*gy - _q2*gx) * halfT;

    // norm quarternion
    double qnorm = sqrt(_q0*_q0 + _q1*_q1 + _q2*_q2 + _q3*_q3);
    if (qnorm == 0)
    {
        qWarning() << "Quaternion norm equals 1 unexpectly!";
        return;
    }
    _q0 = _q0 / qnorm;
    _q1 = _q1 / qnorm;
    _q2 = _q2 / qnorm;
    _q3 = _q3 / qnorm;

    emit attitudeChanged(QQuaternion(_q0, _q1, _q2, _q3));
}
