//
// Created by PulsarV on 18-9-25.
//
#include <serial/flight_serial.h>

namespace flight {
    namespace serial {
        RC_SERIAL_STATUE Serial::recive(char *buffer, int size) {
            SERIAL_FLAGS empty;
            if (this->isOpend())
                if (read(this->device_point, buffer, size) > 0x0000)
                    return RC_SERIAL_SUCCESS;
            return RC_SERIAL_ERROR;
        }

        int Serial::send(std::string str) {
            int wr_num = write(this->device_point, str.c_str(), str.length());
            if (wr_num)return 1;
            return 0;

        }

        int Serial::send(char *str) {
            int wr_num = write(this->device_point, str, RC_SERIAL_BUFFERSIZE);
            if (wr_num)return 1;
            return 0;

        }

        int Serial::send(long comm) {
            char commond = (char) comm;
            int wr_num = write(this->device_point, &commond, RC_SERIAL_BUFFERSIZE);
            if (wr_num)return 1;
            return 0;

        }

        bool Serial::isOpend() {
            return (this->device_point != RC_SERIAL_ERROR ? true : false);
        }

        int Serial::openSerial(std::string device){
            this->device_point = open(device.c_str(), O_RDWR | O_NOCTTY | O_NDELAY);//O_NDELAY
            struct termios st;
            char ch[RC_SERIAL_MAX_RECIVE_BUFFER_SIZE];
            if (this->device_point < 0) {
                return -1;
            }
        }

        int Serial::openSerial(const char *device) {
            this->device_point = open(device, O_RDWR | O_NOCTTY | O_NDELAY);//O_NDELAY
            struct termios st;
            char ch[RC_SERIAL_MAX_RECIVE_BUFFER_SIZE];
            if (this->device_point < 0) {
                return -1;
            }
        }

        int Serial::openSerial(char *device) {
            this->device_point = open(device, O_RDWR | O_NOCTTY | O_NDELAY);//O_NDELAY
            struct termios st;
            char ch[RC_SERIAL_MAX_RECIVE_BUFFER_SIZE];
            if (this->device_point < 0) {
                return -1;
            }
        }

        int Serial::release() {
            if (this->device_point) {
                tcflush(this->device_point, TCIOFLUSH);
                close(this->device_point);
            }
        }
    }
}
