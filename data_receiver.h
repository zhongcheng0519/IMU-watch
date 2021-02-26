#ifndef DATA_RECEIVER_H
#define DATA_RECEIVER_H

#include <zmq.hpp>
#include <string>
#include <QThread>

#pragma pack(push, 1)
typedef struct {
    long timestamp;
    float temperature;
    float acc_x;
    float acc_y;
    float acc_z;
    float gyro_x;
    float gyro_y;
    float gyro_z;
}RawData_t;

typedef struct {
    long timestamp;
    float q0;
    float q1;
    float q2;
    float q3;
}RawQData_t;
#pragma pack(pop)

QDebug operator<<(QDebug dbg, const RawData_t &data);

/**
 * @brief The DataReceiver class receives data from sensor through zmq
 */
class DataReceiver : public QThread
{
    Q_OBJECT
public:
    DataReceiver();

    void verbose(bool debug);

    void close();

signals:
    void dataReceived(long ts, float ax, float ay, float az, float gx, float gy, float gz);
    void qdataReceived(long ts, float q0, float q1, float q2, float q3);

protected:
    virtual void run() override;

private:
    std::string _pub_proto_ip{"tcp://127.0.0.1:"};
    std::string _pub_port{"5556"};
    bool _working{false};
    RawData_t _raw_data;
    bool _debug{false};
    bool _connected{false};
};

#endif // DATA_RECEIVER_H
