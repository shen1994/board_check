#ifndef LINE_LINEAR_H
#define LINE_LINEAR_H

#include <vector>
#include <string>
#include <opencv2/opencv.hpp>

typedef struct{
	int w;
	int h;

	int v_n;
	double *v_x_left;
	double *v_y_left;
	double v_a_left;
	double v_b_left;
	double v_dis_left;

	double *v_x_right;
	double *v_y_right;
	double v_a_right;
	double v_b_right;
	double v_dis_right;

	int h_n;
	double *h_x_top;
	double *h_y_top;
	double h_a_top;
	double h_b_top;
	double h_dis_top;

	double *h_x_bot;
	double *h_y_bot;
	double h_a_bot;
	double h_b_bot;
	double h_dis_bot;

	cv::Point2f src_top;
	cv::Point2f src_bot;
}LINETYPE;

class LineLinear{
public:
	LineLinear(int cols, int rows);
	~LineLinear();
	bool line_calculate(const double *x, const double *y, int n_data, double *a, double *b);
	double same_distance(const double *x, int n_data, const double k, const double b);
	double distance_to_points(const cv::Point2f &up_pt, const cv::Point2f &down_pt,
								cv::Point2f &up_pre_pt, cv::Point2f &down_pre_pt, 
								double distance, const double k);
	void set_values_and_calculate(const std::vector<cv::Point2f> &data, std::vector<cv::Point2f> &edge_points);
	double calculate_triangle_areas(const cv::Point2f &a, const cv::Point2f &b, const cv::Point2f &c);
	double calculate_concindence(const std::vector<cv::Point2f> &data, cv::Rect user_rect);
	void tell_angle_to_rotate(const std::vector<cv::Point2f> &data, double *angle);
	void tell_center_point(const std::vector<cv::Point2f> &data, cv::Point2f &center_point);
	void tell_location_error(cv::Point2f &src_center_point, cv::Point2f &tar_center_point, 
							double *width_diff, bool *width_dir, double *height_diff, bool *height_dir);
	double tell_areas(const std::vector<cv::Point2f> &data);
	void tell_areas_error(double src_areas, double tar_areas, double *err, bool *dir);
public:
	LINETYPE params;
};

#endif