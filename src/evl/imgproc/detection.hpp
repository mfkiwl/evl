// Copyright 2018 Event Vision Library.
#ifndef SRC_EVL_IMGPROC_DETECTION_HPP_
#define SRC_EVL_IMGPROC_DETECTION_HPP_

#include <vector>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#define W 240
#define H 180

void detect_rod_tip(std::vector<EventTuple>, cv::Rect *roi);
void detect_rod_tip(std::vector<EventTuple> v,
  cv::Rect *roi, cv::Point *vertex);

#endif    // SRC_EVL_IMGPROC_DETECTION_HPP_
