#include <librealsense2/rs.hpp> // Include RealSense Cross Platform API
#include<iostream>
#include<flightcv/flightcv.h>
#include <pthread.h>
#include <sys/types.h>
#include <csignal>

cv::VideoWriter writer;
cv::VideoWriter dwriter;
cv::VideoWriter cwriter;
void singal_ctrlc(int singo){
    writer.release();
    dwriter.release();
    cwriter.release();
    std::cout<<"\n Camera Stoped"<<std::endl;
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
    cwriter.open(file_name+"c.avi",CV_FOURCC('M','J','P','G'),25.0,cv::Size(640,480));
    pipe.start();
    while (true) {
        rs2::frameset data = pipe.wait_for_frames();
        rs2::frame depth = data.get_depth_frame().apply_filter(color_map);
        rs2::frame color = data.get_color_frame();
        if (!color || !depth) continue;
        cv::Mat image(cv::Size(640, 480), CV_8UC3, (void *) color.get_data(), cv::Mat::AUTO_STEP);
        cv::cvtColor(image,image,cv::COLOR_BGR2RGB);
        const int w = depth.as<rs2::video_frame>().get_width();
        const int h = depth.as<rs2::video_frame>().get_height();
        cv::Mat depthmat(cv::Size(w, h), CV_8UC3, (void*)depth.get_data(), cv::Mat::AUTO_STEP);
        cv::resize(depthmat,depthmat,cv::Size(640,480));
        cwriter<<depthmat;
        cv::cvtColor(depthmat,depthmat,CV_RGB2GRAY);
        for (int i = 0; i < depthmat.rows; ++i) {
            for (int j = 0; j < depthmat.cols; ++j){
                if(depthmat.at<u_char>(i, j)<=150){
                    depthmat.at<u_char>(i, j)=0;
                } else{
                    depthmat.at<u_char>(i, j)=255;
                }
            }
        }
        cv::cvtColor(depthmat,depthmat,CV_GRAY2RGB);
        int sum_x=0,sum_y=0,area=1;
        for(int x=0;x<depthmat.rows;x+=1){
            for (int y = 0; y < depthmat.cols; y+=1) {
                if(depthmat.at<uchar>(x,y)>100){
                    sum_x+=x;
                    sum_y+=y;
                    area+=1;
                }
            }
        }
        int center_x=sum_x/area,center_y=sum_y/area;
        std::cout<<center_x<<" "<<center_y<<std::endl;
//        std::cout<<max<<std::endl;
//        cv::imshow("image",image);
//        cv::imshow("depth",depthm);
        dwriter<<depthmat;
        writer<<image;
        cv::waitKey(10);
//        if(cv::waitKey(10)=='q'){
//            break;
//        }
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
