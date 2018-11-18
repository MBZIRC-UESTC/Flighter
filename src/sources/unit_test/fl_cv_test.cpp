#include <librealsense2/rs.hpp> // Include RealSense Cross Platform API
#include<iostream>
#include<flightcv/flightcv.h>
#include <pthread.h>
#include <sys/types.h>
#include <csignal>

cv::VideoWriter writer;
cv::VideoWriter dwriter;
void singal_ctrlc(int singo){
    writer.release();
    dwriter.release();
    std::cout<<"Stoped"<<std::endl;
    exit(0);
}
int main(int argc, char *argv[]) try {
    rs2::colorizer color_map;
    rs2::pipeline pipe;
    signal(SIGINT,singal_ctrlc);
    std::string file_name;
    long file_time=clock();
    file_name="./save/"+std::to_string(file_time);
    writer.open(file_name+".avi",CV_FOURCC('M','J','P','G'),25.0,cv::Size(640,480));
//    writer.open("appsrc ! autovideoconvert ! omxh265enc ! matroskamux ! filesink location="+file_name+".avi",CV_FOURCC('M','J','P','G'),25.0,cv::Size(640,480));
//    dwriter.open("appsrc ! autovideoconvert ! omxh265enc ! matroskamux ! filesink location="+file_name+"d.avi",CV_FOURCC('M','J','P','G'),25.0,cv::Size(640,480));
    dwriter.open(file_name+"d.avi",CV_FOURCC('M','J','P','G'),25.0,cv::Size(640,480));
    pipe.start();
    int count_fps=0;
    while (true) {
        rs2::frameset data = pipe.wait_for_frames();
        rs2::frame depth = data.get_depth_frame();
        rs2::frame color = data.get_color_frame();
        if (!color || !depth) continue;
        cv::Mat image(cv::Size(640, 480), CV_8UC3, (void *) color.get_data(), cv::Mat::AUTO_STEP);
        cv::cvtColor(image,image,cv::COLOR_BGR2RGB);
        cv::Mat depthmat(cv::Size(640, 480), CV_16U, (void *) depth.get_data(), cv::Mat::AUTO_STEP);
        cv::cvtColor(depthmat,depthmat,CV_GRAY2RGB);
//        cv::imshow("sss",image);
//        cv::imshow("dep",depthmat);
        dwriter<<depthmat;
        writer<<image;
        cv::waitKey(10);
    }
    return EXIT_SUCCESS;
} catch (const rs2::error &e) {
    std::cerr << "RealSense error calling " << e.get_failed_function() << "(" << e.get_failed_args() << "):\n    "
              << e.what() << std::endl;
    return EXIT_FAILURE;
} catch (const std::exception &e) {
    std::cerr << "Error1 : " << e.what() << std::endl;
    return EXIT_FAILURE;
}
