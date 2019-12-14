#ifndef DATA_RECEIVER_H
#define DATA_RECEIVER_H

#include <zmq.hpp>
#include <string>
#include <QThread>

#pragma pack(push, 1)
typedef struct{
    long timestamp;
    float temperature;
    float acc_x;
    float acc_y;
    float acc_z;
    float gyro_x;
    float gyro_y;
    float gyro_z;
}RawData_t;
#pragma pack(pop)

QDebug operator<<(QDebug dbg, const RawData_t &data);

/**
 * @brief The DataReceiver class receives data from sensor through zmq
 */
class DataReceiver : public QThread
{
public:
    DataReceiver();

    void verbose(bool debug);

    void close();

signals:
    void dataReceived(RawData_t raw_data);

protected:
    void run() override;

private:
    std::string _pub_proto_ip{"tcp://192.168.31.111:"};
    std::string _pub_port{"5556"};
    bool _working{false};
    RawData_t _raw_data;
    bool _debug{false};
    bool _connected{false};
};

#endif // DATA_RECEIVER_H
