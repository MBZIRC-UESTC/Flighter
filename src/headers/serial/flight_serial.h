//
// Created by PulsarV on 18-9-25.
//

#ifndef FLIGHTER_FLIGHT_SERIAL_H
#define FLIGHTER_FLIGHT_SERIAL_H
#include <map>
#include <string>
#include <fcntl.h>
#include <iostream>
#include <termios.h>
#include <zconf.h>
#define  RC_SERIAL_MAX_RECIVE_BUFFER_SIZE 2048
#define RC_SERIAL_STATUE int
#define RC_SERIAL_ERROR -1
#define RC_SERIAL_SUCCESS 1
#define RC_SERIAL_BUFFERSIZE 8
namespace flight {
    namespace serial {
        typedef std::map<std::string, std::string> SERIAL_FLAGS;
        typedef std::pair<std::string, std::string> SERIAL_FLAG;

        class Serial {
        public:
            int openSerial(char *device);
            int openSerial(const char *device);
            int openSerial(std::string device);

            int send(std::string str);

            int send(long comm);

            int send(char* str);

            RC_SERIAL_STATUE recive(char *buffer, int size);

            bool isOpend();

            int release();

        private:
            int device_point;
            char *device;
        };
    }
}
#endif //FLIGHTER_FLIGHT_SERIAL_H
