#include <string>
#include <iostream>
#include <thread>
#include <vector>
#include <mutex>
#include <deque>
#include <tuple>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "unistd.h"

#include <libcaercpp/libcaer.hpp>
#include <libcaercpp/devices/davis.hpp>

#include <evl/core/types.hpp>
#include <evl/core/shutdown.hpp>
#include <evl/core/davisio.hpp>
#include <evl/imgproc/detection.hpp>

#define W 240
#define H 180

void loop_detectRod(EventBuffer *buffer, int lifetime){

    cv::Rect roi(50, 100, 150, 80); //(x, y, w, h), 最初だけ決め打ち
    cv::Point vertex(50, 100);
    cv::namedWindow("filtered", 1);
    //cv::resizeWindow("filtered" , 1000, 1000);
    cv::Mat src = cv::Mat::zeros(H, W, CV_8UC1);
    cv::Mat filtered = cv::Mat::zeros(H, W, CV_8UC1);
    while (1) {
        usleep(1);      // micro sec. calling frequency
        filtered = cv::Mat::zeros( filtered.size(), filtered.type() );
        
        std::vector<EventTuple> v = readBufferOnLifetime(buffer, lifetime);
        detect_rod_tip(v, &roi, &vertex);
        cv::line(filtered, cv::Point(vertex.x, 0), cv::Point(vertex.x, H), 255, 1);
        cv::line(filtered, cv::Point(0, vertex.y), cv::Point(W, vertex.y), 255, 1);
         
        cv::circle(filtered, vertex, 5, 255, -1, CV_AA);
        //cv::resize(filtered, resized, cv::Size(), 5, 5);
        cv::imshow("filtered", filtered);
        cv::waitKey(1);
    }
    cv::destroyAllWindows();
}

int main(){
    int lifetime = 10000;     // micro sec
    int buffersize = 50000;
    EventBuffer buffer(buffersize);

    std::cout << "version "<< CV_VERSION << std::endl;
    std::thread t1(bufferData, &buffer);
    loop_detectRod(&buffer, lifetime);
    t1.join();
    // cv::waitKey(100);
    // cv::destroyAllWindows();

    return 1;
}

