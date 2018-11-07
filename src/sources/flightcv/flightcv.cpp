//
// Created by PulsarV on 18-9-25.
//

#include <flightcv/flightcv.h>
namespace flight {
    namespace flight_cv {
        Flight_Eye::Flight_Eye(std::string tty_path) {
            this->serial = new serial::Serial;
        }

        Flight_Eye::Flight_Eye(std::string tty_path, std::string model_path) {
            this->serial = new serial::Serial;
            this->serial->openSerial(tty_path.c_str());
            if (!serial->isOpend()) {
                std::cout << "[ERROR] " << "Open TTY device:" << tty_path << "failed" << std::endl;
                return;
            }
        }

        void Flight_Eye::start(std::string video_path) {

            cv::VideoCapture cap(video_path);
//            cv::dnn::readNetFromDarknet("","");
            if (cap.isOpened()) {
                clock_t counter, time_now;
                counter = clock();
                while (1) {
                    cv::Mat frame;
                    cap >> frame;
//                    cv::imshow("frame", frame);
                    int x = 0, y = 0, z = 0;
                    get_target_hough(frame, x, y, z);
                    if (cv::waitKey(10) == 'x')break;
                    time_now = clock();
                    if ((double) ((time_now - counter) / 100000.0) > 1) {
                        counter = clock();
                        std::cout << "Times: " << time_now << std::endl;
                    }
                }
                cv::destroyAllWindows();
            }
        }

        void Flight_Eye::test_image(std::string image_path) {
            cv::Mat image = cv::imread(image_path);
            int x = 0, y = 0, z = 0;
            get_target_hough(image, x, y, z);
            cv::waitKey(0);
            cv::destroyAllWindows();
        }
        void Flight_Eye::init_darknet(std::string cfg_path, std::string model_path, std::string names_path) {

            try{
                darknet=cv::dnn::readNetFromDarknet(cfg_path,model_path);
                darknet.setPreferableBackend(cv::dnn::DNN_BACKEND_DEFAULT);
                darknet.setPreferableTarget(cv::dnn::DNN_TARGET_CPU);
                this->is_load_model=true;
                if (darknet.empty()) {
                    perror("Load Error");
                }
                std::vector<std::string> classNamesVec;
                std::ifstream classNamesFile(names_path);
                if (classNamesFile.is_open()){
                    std::string className="";
                    while(std::getline(classNamesFile,className)){
                        classNamesVec.push_back(className);
                        std::cout<<"[INFO] class "<<className<<std::endl;
                    }
                }
            }catch (std::exception e){
                std::cout<<"[ERROR] "<<"Load error"<<std::endl;
            }
        }

        void Flight_Eye::start(int index) {
//            if(this->is_load_model) {
//                PXCImage *colorIm, *depthIm, *irIm;
//                PXCImage::ImageData depth_data, color_data, ir_data;
//                PXCImage::ImageInfo depth_info, color_info, ir_info;
//
//                PXCSenseManager *sm = PXCSenseManager::CreateInstance();
//
//                sm->QueryCaptureManager()->SetFileName(file, record);
//
//                sm->EnableStream(PXCCapture::STREAM_TYPE_COLOR, 640, 480, 0);
//                sm->EnableStream(PXCCapture::STREAM_TYPE_DEPTH, 640, 480);
//                sm->EnableStream(PXCCapture::STREAM_TYPE_IR, 640, 480);
//                sm->Init();
//                PXCCapture::Device *device = sm->QueryCaptureManager()->QueryDevice();
//

                cv::VideoCapture cap(index);

                if (cap.isOpened()) {
                    clock_t counter, time_now;
                    counter = clock();
                    while (1) {
//                        if (sm->AcquireFrame(true) < PXC_STATUS_NO_ERROR) break;
//
//                        PXCCapture::Sample *sample = sm->QuerySample();
//
//                        irIm = sample->ir;
//                        colorIm = sample->color;
//                        depthIm = sample->depth;
//
//                        if (colorIm->AcquireAccess(PXCImage::ACCESS_READ, PXCImage::PIXEL_FORMAT_RGB24, &color_data) < PXC_STATUS_NO_ERROR)
//                            wprintf_s(L"error\n");
//                        if (depthIm->AcquireAccess(PXCImage::ACCESS_READ, &depth_data) < PXC_STATUS_NO_ERROR)
//                            wprintf_s(L"error\n");
//                        if (irIm->AcquireAccess(PXCImage::ACCESS_READ, PXCImage::PIXEL_FORMAT_RGB24, &ir_data) < PXC_STATUS_NO_ERROR)
//                            wprintf_s(L"error\n");
//                        depth_info = sample->depth->QueryInfo();
//                        color_info = sample->color->QueryInfo();
//                        ir_info = sample->ir->QueryInfo();
//
//                        Mat depth(Size(depth_info.width, depth_info.height), CV_16UC1, (void*)depth_data.planes[0], depth_data.pitches[0] / sizeof(uchar));
//                        Mat color(Size(color_info.width, color_info.height), CV_8UC3, (void*)color_data.planes[0], color_data.pitches[0] / sizeof(uchar));
//                        Mat ir(Size(ir_info.width, ir_info.height), CV_16UC1, (void*)ir_data.planes[0], ir_data.pitches[0] / sizeof(uchar));
//
//                        irIm->ReleaseAccess(&ir_data);
//                        depthIm->ReleaseAccess(&depth_data);
//                        colorIm->ReleaseAccess(&color_data);
//
                        cv::Mat frame;
                        cap >> frame;
//                    cv::imshow("frame", frame);
                        int x = 0, y = 0, z = 0;
                        get_target_hough(frame, x, y, z);
                        if (cv::waitKey(10) == 'x')break;
                        time_now = clock();
                        if ((double) ((time_now - counter) / 100000.0) > 1) {
                            counter = clock();
//                        std::cout << "Times: " << time_now << std::endl;
                        }
//                        sm->ReleaseFrame();
                    }
                    cv::destroyAllWindows();
//                }
            }
            std::cout<<"[INFO] "<<"YOLO Model is not load or load error"<<std::endl;
        }
        void Flight_Eye::detcet_target(cv::Mat src, int &x, int &y, int &z){
            cv::Mat input(src,1/255.F);
            darknet.setInput(input,"data");
        }

        void get_target_yolo(cv::Mat src, int &x, int &y, int &z) {

        }

        void get_target_hough(cv::Mat src, int &x, int &y, int &z) {
            std::vector<cv::Vec3f> circles;
            cv::Mat mid_frame;
            cv::Mat hsv_frame;
//            cv::Mat res_frame;
            cv::Scalar hsv_min(110,50,50);
            cv::Scalar hsv_max(130,255,255);

            cv::Mat mask=cv::Mat::zeros(src.size(),CV_8U);
            cv::inRange(src,hsv_min,hsv_max,mask);
//            cv::Mat element=cv::getStructuringElement(cv::MORPH_RECT,cv::Size(5,5));
//            morphologyEx(dstImage,dstImage,cv::MORPH_ERODE,element,cv::Point(-1,-1),1);
//            morphologyEx(dstImage,dstImage,cv::MORPH_DILATE,element,cv::Point(-1,-1),4);
//            cv::imshow("dst",mask);
//            cv::bitwise_and(src,src,mask);

//            cv::imshow("res_frame",res_frame);

            cv::cvtColor(src,hsv_frame,cv::COLOR_RGB2HSV);
//            cv::imshow("HSV",hsv_frame);
            cv::cvtColor(src, mid_frame, cv::COLOR_BGRA2GRAY);
            double image_avg = get_avg(src);
//            std::cout<<"AVG:"<<(double)255/image_avg-0.4<<std::endl;
            int scale=5 * ((image_avg / 255)>0.2?image_avg / 255:1);
//            std::cout<<"SCALE:"<<(image_avg / 255) <<"   "<<scale<<std::endl;

            cv::blur(mid_frame, mid_frame, cv::Size(scale, scale));
//            cv::threshold(mid_frame, mid_frame, (unsigned int) (image_avg), 255, cv::THRESH_BINARY);
            cv::HoughCircles(mid_frame, circles, CV_HOUGH_GRADIENT, 1.7, 10, 100, 200, 0, 0);
            for (size_t circel = 0; circel < circles.size(); circel += 1) {
                cv::Point center(cvRound(circles[circel][0]), cvRound(circles[circel][1]));
                int radius = cvRound(circles[circel][2]);
                cv::circle(src, center, 3, cv::Scalar(255, 0, 0), -1, 8, 0);
                cv::circle(src, center, radius, cv::Scalar(255, 0, 0), 3, 8, 0);
            }
//            cv::imshow("draw image", src);
//            cv::imshow("mid frame", mid_frame);
        }

        void Flight_Eye::move_center(int x, int y, int position) {
            if (this->serial->isOpend()) {
                if (position > 180)position = 180;
                if (position < 0)position = 0;
                std::string data = std::to_string(position);
                while (!serial->send(data.c_str())) {}
                char bufffer[8] = {0};
                serial->recive(bufffer, 8);
                std::cout << "recive:" << bufffer << std::endl;
                std::cout << "send:" << data << std::endl;
            }
        }

        double get_avg(cv::Mat src) {
            cv::cvtColor(src, src, cv::COLOR_RGB2GRAY);
            return cv::mean(src)[0];
        }
    }
}
