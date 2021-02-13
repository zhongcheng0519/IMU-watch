#include "attitude_solver.h"
#include <math.h>

AttitudeSolver::AttitudeSolver(QObject *parent) : QObject(parent)
{
    _data_receiver = new DataReceiver();
    _data_receiver->verbose(false);
    connect(_data_receiver, SIGNAL(dataReceived(long,float,float,float,float,float,float)),
            this, SLOT(onDataReceived(long,float,float,float,float,float,float)));
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

void AttitudeSolver::onDataReceived(long ts, float axf, float ayf, float azf, float gxf, float gyf, float gzf)
{
//    double Kp = 0.0015;
    double Kp = 0.25;
//    double Ki = 0.000001;
    double Ki = 0.000001;
    double ax = axf;
    double ay = ayf;
    double az = azf;
    double gx = gxf;
    double gy = gyf;
    double gz = gzf;
//    double gx = 0;
//    double gy = 0;
//    double gz = 0;
    double q0, q1, q2, q3;

    double acc_norm = sqrt(ax*ax + ay*ay + az*az);

    // norm acc
    ax = ax / acc_norm;
    ay = ay / acc_norm;
    az = az / acc_norm;

    float halfT = 0;
    if (_lastT == -1)
    {
        _lastT = ts;
        return;
    }
    else
    {
        // ms -> s
        halfT = (ts - _lastT)/1000/2;
        _lastT = ts;
    }
    // update q
    q0 = _q0 + (-_q1*gx - _q2*gy - _q3*gz)* halfT;
    q1 = _q1 + (_q0*gx + _q2*gz - _q3*gy) * halfT;
    q2 = _q2 + (_q0*gy - _q1*gz + _q3*gx) * halfT;
    q3 = _q3 + (_q0*gz + _q1*gy - _q2*gx) * halfT;

    // estimated direction of gravity
    double est_gx = 2*(q1*q3 - q0*q2);
    double est_gy = 2*(q0*q1 + q2*q3);
    double est_gz = q0*q0 - q1*q1 - q2*q2 + q3*q3;
//    qDebug() << "est_gx, est_gy, est_gz = (" << est_gx << "," << est_gy << "," << est_gz << ")";

    // error calculating by cross product
    double ex = (ay*est_gz - az*est_gy);
    double ey = (az*est_gx - ax*est_gz);
    double ez = (ax*est_gy - ay*est_gx);
    qDebug() << "ex, ey, ez = (" << ex << "," << ey << "," << ez << ")";

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

    emit attitudeChanged(_q0, _q1, _q2, _q3);
}
