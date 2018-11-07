//
//YOLO 测试用例
// Created by PulsarV on 18-11-2.
//
#include <flightcv/flightcv.h>
#include <librealsense2/rs.hpp>
#include <dji_status.hpp>
#include <flight_control_sample.hpp>
#include <GL/glut.h>
#include <boost/thread.hpp>
using namespace cv::dnn;
using namespace cv;
#define PI 3.14159265358

#define RADAR_STEP 0.01
float p1_x=0,p1_y=0;
float p2_x=0,p2_y=0;
float p1step = 1.75f;
//float p1step = 0.0f;
float p2step = 2.25f;
void draw_line(float x1,float y1,float x2,float y2){
    GLfloat line_size=3;
    glLineWidth(line_size);
    glBegin(GL_LINES);
    glVertex2f(x1,y1);
    glVertex2f(x2,y2);
    glEnd();
}
void draw_point(float x,float y, float R,float G,float B, float alpha){
    GLfloat point_size=5;
    glPointSize(point_size);
    glColor4f(R,G,B,alpha);
    glBegin(GL_POINTS);
    glVertex2f(x,y);
    glEnd();
}
void draw_rader() {
    glClear(GL_COLOR_BUFFER_BIT);
    glClearColor(0, 0, 0, 0);
    glColor4f(0,1,0,0);
    int n=1000;
    for (float scale=1;scale>0.1;scale-=0.1){
        glBegin(GL_LINE_LOOP);
        for(int i=0;i<n;i++)
        {
            glVertex2f(scale*cos(2*PI*i/n),scale*sin(2*PI*i/n));   //定义顶点
        }
        glEnd();
    }
    glColor4f(1,0,0,0);
    draw_line(0,0,0,1);
    draw_line(0,0,1,1);
    draw_line(0,0,-1,1);
    draw_line(0,0,1,-1);
    draw_line(0,0,-1,-1);
    draw_line(0,0,0,-1);
    draw_line(0,0,1,0);
    draw_line(0,0,-1,0);
    draw_point(p1_x,p1_y,1,1,1,0);
    draw_point(p2_x,p2_y,1,1,1,0);
    draw_line(0,0,p1_x,p1_y);
    draw_line(0,0,p2_x,p2_y);
    glFlush();
    glutSwapBuffers();
}
float get_angle(float x,float y){
    float trangel=atan(x/y)*180/PI;
    if(y<0 && x>0 || y<0 && x<0)
        trangel=trangel+180;
    if(x<0 && y>0)
        trangel=trangel+360;
    trangel=trangel==360?0:trangel;
}
void TimerFunction(int value) {
    p1_x = sin(PI*p1step);
    p1_y = cos(PI*p1step);
    p2_x = sin(PI*p2step);
    p2_y = cos(PI*p2step);
    if (p1step==2.5+1.75)
        p1step=1.75;
    if (p2step==2.5+2.25)
        p2step=2.25;
    p1step+=RADAR_STEP;
    p2step+=RADAR_STEP;
//    std::cout<<"当前角度1: "<<get_angle(p1_x,p1_y)<<" 当前角度2: "<<get_angle(p2_x,p2_y)<<std::endl;
    glutPostRedisplay(); //标志重新绘制
    glutTimerFunc(1,TimerFunction,1);
}
#define PI 3.14159265358

int main(int argc, char **argv) {

//    glutInit(&argc, argv);
//    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
//    glutInitWindowPosition(100, 100);
//    glutInitWindowSize(640, 640);
//    glMatrixMode(GL_PROJECTION);//设定投影方式
//    glutCreateWindow("Flight Eye");
//    glutDisplayFunc(draw_rader);
//    glutTimerFunc(50,TimerFunction,1);
//    glutMainLoop();
//    flight::flight_cv::Flight_Eye flighter(FL_SERIAL_DEVICE);

//    LinuxSetup linuxEnvironment(2, "./UserConfig.txt");
//    Vehicle*   vehicle = linuxEnvironment.getVehicle();
//    monitoredTakeoff(vehicle);
//    if (vehicle == nullptr)
//    {
//        std::cout << "Vehicle not initialized, exiting.\n";
//        return -1;
//    }
//    moveByPositionOffset(vehicle, 0, 0, 2, 30);
    rs2::colorizer color_map;
    rs2::pipeline pipe;
    pipe.start();
    std::cout << "Load Wight File" << std::endl;
    cv::dnn::Net net = cv::dnn::readNetFromDarknet("../../data/yolov2.cfg",
                                                   "../../data/yolov2_20000.weights");
    if (net.empty()) {
        perror("Load Error");
    }
    std::vector<std::string> classNameVec;
    std::ifstream classNameFile("../../data/voc.names");
    if (classNameFile.is_open()) {
        std::string className = "";
        while (std::getline(classNameFile, className))
            classNameVec.push_back(className);
    }
    std::cout << "Load Done" << std::endl;

    while (true) {
        rs2::frameset data = pipe.wait_for_frames();
        rs2::frame depth = data.get_depth_frame();
        rs2::frame color = data.get_color_frame();
        if (!color || !depth) break;
        cv::Mat frame(cv::Size(640, 480), CV_8UC3, (void *) color.get_data(), cv::Mat::AUTO_STEP);
        cv::cvtColor(frame, frame, cv::COLOR_BGR2RGB);
        cv::Mat depthmat(cv::Size(640, 480), CV_16U, (void *) depth.get_data(), cv::Mat::AUTO_STEP); ///显示
//            cv::imshow("image", frame);
        cv::imshow("depth", depthmat);
        double image_avg = flight::flight_cv::get_avg(frame);
        int scale=5 * ((image_avg / 255)>0.2?image_avg / 255:1);
//        cv::blur(frame, frame, cv::Size(scale, scale));

        cv::Mat inputblob = cv::dnn::blobFromImage(frame, 1 / 255.f, cv::Size(416, 416), cv::Scalar(), true, false);
        net.setInput(inputblob, "data");
        cv::Mat detectionMat = net.forward("detection_out");
        std::stringstream ss;
        ss << "detection time: " << time << " ms";
        putText(frame, ss.str(), Point(20, 20), 0, 0.5, Scalar(0, 0, 255));
        for (int i = 0; i < detectionMat.rows; i++) {
            const int probability_index = 5;
            const int probability_size = detectionMat.cols - probability_index;
            float *prob_array_ptr = &detectionMat.at<float>(i, probability_index);
            size_t objectClass = std::max_element(prob_array_ptr, prob_array_ptr + probability_size) - prob_array_ptr;
            float confidence = detectionMat.at<float>(i, (int) objectClass + probability_index);
            if (confidence > 0.3) {
                float x = detectionMat.at<float>(i, 0);
                float y = detectionMat.at<float>(i, 1);


                float width = detectionMat.at<float>(i, 2);
                float height = detectionMat.at<float>(i, 3);
                int xLeftBottom = static_cast<int>((x - width / 2) * frame.cols);
                int yLeftBottom = static_cast<int>((y - height / 2) * frame.rows);
                int xRightTop = static_cast<int>((x + width / 2) * frame.cols);
                int yRightTop = static_cast<int>((y + height / 2) * frame.rows);
                cv::circle(frame, cv::Point((xRightTop+xLeftBottom)/2,(yRightTop+yLeftBottom)/2), 3, cv::Scalar(255, 0, 0), -1, 8, 0);
                int center_x=(xRightTop+xLeftBottom)/2;
                int center_y=(yRightTop+yLeftBottom)/2;
//                moveByPositionOffset(vehicle, 0, 0, 0, (atan(center_y/center_x)*180)/PI);

                Rect object(xLeftBottom, yLeftBottom,
                            xRightTop - xLeftBottom,
                            yRightTop - yLeftBottom);
                rectangle(frame, object, cv::Scalar(0, 0, 255), 2, 8);
//                if (objectClass < classNameVec.size()) {
//                    ss.str("");
//                    ss << confidence;
//                    String conf(ss.str());
//                    String label = String(classNameVec[objectClass]) + ": " + conf;
//                    int baseLine = 0;
//                    Size labelSize = getTextSize(label, FONT_HERSHEY_SIMPLEX, 0.5, 1, &baseLine);
//                    rectangle(frame, Rect(Point(xLeftBottom, yLeftBottom),
//                                          Size(labelSize.width, labelSize.height + baseLine)),
//                              cv::Scalar(255, 255, 255), CV_FILLED);
//                    putText(frame, label, cv::Point(xLeftBottom, yLeftBottom + labelSize.height),
//                            FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 0, 0));
//                }
            }
        }
        int x, y, z;
//        flight::flight_cv::get_target_hough(frame, x, y, z);
        imshow("YOLO-Detections", frame);
        if (cv::waitKey(5)=='q')
            break;
    }
    cv::destroyAllWindows();
//    monitoredLanding(vehicle);
    return 0;
}
