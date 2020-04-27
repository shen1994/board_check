#ifndef PARAMS_H
#define PARAMS_H

#include <opencv2/opencv.hpp>

typedef struct{
    double board_angle;
    cv::Point2f center_point;
    double top_bot_value;
    double left_right_value;
    bool top_bot_dir;
    bool left_right_dir;
    double areas;
    double areas_error;
    bool areas_dir;
}ParamsType;

#endif