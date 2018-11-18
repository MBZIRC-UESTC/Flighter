//
// Created by PulsarV on 18-9-25.
//

#ifndef FLIGHTER_FLIGHTCV_H
#define FLIGHTER_FLIGHTCV_H

#include <opencv2/core/mat.hpp>
#include <opencv2/opencv.hpp>
#include <serial/flight_serial.h>

namespace flight {
    namespace flight_cv {
        void get_target_hough(cv::Mat src, int &x, int &y, int &z);


        double get_avg(cv::Mat src);

        class Flight_Eye {
        public:
            Flight_Eye(std::string tty_path);

            Flight_Eye(std::string tty_path,std::string model_path);

            void start(int index);

            void start(std::string video_path);

            void test_image(std::string image_path);

            void move_center(int x,int y,int position=0);

            void init_darknet(std::string cfg_path,std::string model_path,std::string names_path);

            void detcet_target(cv::Mat src, int &x, int &y, int &z);
        private:
            void get_target_yolo(cv::Mat src, int &x, int &y, int &z);
            bool is_load_model=false;
            serial::Serial *serial;
            cv::dnn::Net darknet;
        };
    }
}

#endif //FLIGHTER_FLIGHTCV_H
