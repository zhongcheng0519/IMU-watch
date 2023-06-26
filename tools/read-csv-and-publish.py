#!/usr/bin/env python3
import sys
import struct
import zmq
import os
import time
import math


def help():
    print('Usage: python csv-path')


def read_and_publish(csv_path):
    # read file
    with open(csv_path, 'rt') as f:
        lines = f.readlines()

    # init zmq
    context = zmq.Context()
    socket = context.socket(zmq.PUB)
    socket.bind("tcp://*:5556")

    for line in lines:
        seps = line.split(',')

        if len(seps) == 7:
            ts = int(seps[0])
            temperature = 37.5
            ax = float(seps[1])
            ay = float(seps[2])
            az = float(seps[3])
            gx = float(seps[4])*math.pi/180    # rad/s
            gy = float(seps[5])*math.pi/180    # rad/s
            gz = float(seps[6].strip())*math.pi/180    #rad/s
            pack = struct.pack('<Qfffffff', ts, temperature, ax, ay, az, gx, gy, gz)
            print(len(pack), ts, ax, ay, az, gx, gy, gz)
        if len(seps) == 11:
            ts = int(seps[0])
            q0 = float(seps[7])
            q1 = float(seps[8])
            q2 = float(seps[9])
            q3 = float(seps[10])
            pack = struct.pack('<Qffff', ts, q0, q1, q2, q3)
            print(len(pack), ts, q0, q1, q2, q3)

        socket.send(pack)
        time.sleep(0.01)


if __name__ == '__main__':
    print("Read csv file and publish them out.")
    if len(sys.argv) != 2:
        help()
        sys.exit(1)
    csv_path = sys.argv[1]
    if os.path.splitext(csv_path)[1] != '.csv':
        raise ValueError("Extension of the file should be .csv")
    if not os.path.exists(csv_path):
        raise FileExistsError(csv_path)

    read_and_publish(csv_path)
