#include "detect_corners.h"
#include <iostream>

Detect::Detect(int _board_cows, int _board_rows, int _width, int _height, double _resolution)
{
	resolution = _resolution;
	target_width = round((double)_width * resolution);
	target_height = round((double)_height * resolution);
	board_cows = _board_cows;
	board_rows = _board_rows;
}

Detect::~Detect()
{

}

bool Detect::core_detect(cv::Mat &image, std::vector<cv::Point2f> &corners)
{
    bool found_corners = false;
    cv::Mat gray;
    cv::Mat rimage;
    cv::Size board_size = cv::Size(board_cows - 1, board_rows - 1);

    cv::resize(image, rimage, cv::Size(target_width, target_height));
    cv::cvtColor(rimage, gray, CV_BGR2GRAY);   

    corners.clear();
    found_corners = cv::findChessboardCorners(rimage, board_size, corners,
                    	CV_CALIB_CB_ADAPTIVE_THRESH | CV_CALIB_CB_FILTER_QUADS);
    if(found_corners)
    {
        cornerSubPix(gray, corners, cv::Size(5, 5), cv::Size(-1, -1),
                               cv::TermCriteria(CV_TERMCRIT_EPS | CV_TERMCRIT_ITER, 30, 0.1));
       	// drawChessboardCorners(current_gray, board_size, corners, found_corners);
       	for(int i = 0; i < corners.size(); i++)
       	{
       		corners[i].x = corners[i].x / resolution;
       		corners[i].y = corners[i].y / resolution;
       	}
    }

    return found_corners;
}
