#include <librealsense2/rs.hpp> // Include RealSense Cross Platform API
#include<iostream>
#include<flightcv/flightcv.h>

int main(int argc, char *argv[]) try {
    rs2::colorizer color_map;
    rs2::pipeline pipe;
    pipe.start();
    while (1) {
        ///等待一帧数据，默认等待5s
        rs2::frameset data = pipe.wait_for_frames();
        rs2::frame depth = data.get_depth_frame();
        rs2::frame color = data.get_color_frame();
        if (!color || !depth) break;
        cv::Mat image(cv::Size(640, 480), CV_8UC3, (void *) color.get_data(), cv::Mat::AUTO_STEP);
        cv::cvtColor(image,image,cv::COLOR_BGR2RGB);
        cv::Mat depthmat(cv::Size(640, 480), CV_16U, (void *) depth.get_data(), cv::Mat::AUTO_STEP); ///显示

        cv::imshow("image", image);
        cv::imshow("depth", depthmat);
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
