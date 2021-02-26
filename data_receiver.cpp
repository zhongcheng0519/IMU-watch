#include "data_receiver.h"
#include <QDebug>
#include <iostream>

using namespace std;

DataReceiver::DataReceiver()
{

}

void DataReceiver::verbose(bool debug)
{
    _debug = debug;
}

void DataReceiver::close()
{
    _working = false;
    if (this->wait())
    {
        qDebug() << "Listener Closed";
    }
}

void DataReceiver::run()
{
    zmq::context_t context(1);
    zmq::socket_t subscriber(context, ZMQ_SUB);
    string address = _pub_proto_ip + _pub_port;
    subscriber.connect(address);

    subscriber.setsockopt(ZMQ_SUBSCRIBE, NULL, 0);
    int time_out = 1000;
    subscriber.setsockopt(ZMQ_RCVTIMEO, &time_out, 4);
    cout << "Collecting data from " << address << endl;

    this->_working = true;

    while (_working)
    {
        size_t sz = 0;
        try {
            if ((sz = subscriber.recv(&_raw_data, sizeof(RawData_t))))
            {
                if (_debug)
                    qDebug() << this->_raw_data;
                if (sz == sizeof(RawData_t))
                {
                    emit dataReceived(_raw_data.timestamp, _raw_data.acc_x, _raw_data.acc_y, _raw_data.acc_z,
                                      _raw_data.gyro_x, _raw_data.gyro_y, _raw_data.gyro_z);
                }
                else if (sz == sizeof(RawQData_t))
                {
                    RawQData_t qdata;
                    memcpy(&qdata, &_raw_data, sizeof(RawQData_t));
                    emit qdataReceived(qdata.timestamp, qdata.q0, qdata.q1, qdata.q2, qdata.q3);
                }
                else
                {
                    qWarning() << "Undefined error!";
                }

            }
            else {
                throw zmq::error_t();
            }
        }
        catch (zmq::error_t& ee)
        {
            qDebug() << "Listener: Waiting to connect.";
            qDebug() << ee.what();
        }

    }

}

QDebug operator<<(QDebug dbg, const RawData_t &data)
{
    QDebugStateSaver saver(dbg);
    dbg << data.timestamp << " " <<
           data.temperature << " " <<
           data.acc_x << " " <<
           data.acc_y << " " <<
           data.acc_z << " " <<
           data.gyro_x << " " <<
           data.gyro_y << " " <<
           data.gyro_z;

    return dbg;
}
