#ifndef DETECT_CORNERS_H
#define DETECT_CORNERS_H

#include <opencv2/opencv.hpp>

class Detect{
public:
	Detect(int _board_cows, int _board_rows, int _width, int _height, double _resolution);
	~Detect();
	bool core_detect(cv::Mat &image, std::vector<cv::Point2f> &corners);

public:
	int target_width;
	int target_height;
	double resolution;
	int board_cows;
	int board_rows;
};

#endif