#include "line_linear.h"
#include <stdio.h>

LineLinear::LineLinear(int cols, int rows)
{
	params.h_n = cols - 1;
	params.v_n = rows - 1;
	params.h_x_top = (double*)malloc(params.h_n * sizeof(double));
	params.h_y_top = (double*)malloc(params.h_n * sizeof(double));
	params.h_x_bot = (double*)malloc(params.h_n * sizeof(double));
	params.h_y_bot = (double*)malloc(params.h_n * sizeof(double));
	params.v_x_left = (double*)malloc(params.v_n * sizeof(double));
	params.v_y_left = (double*)malloc(params.v_n * sizeof(double));
	params.v_x_right = (double*)malloc(params.v_n * sizeof(double));
	params.v_y_right = (double*)malloc(params.v_n * sizeof(double));
	memset(params.h_x_top, 0.0, params.h_n * sizeof(double));
	memset(params.h_y_top, 0.0, params.h_n * sizeof(double));
	memset(params.v_x_left, 0.0, params.v_n * sizeof(double));
	memset(params.v_y_left, 0.0, params.v_n * sizeof(double));
	memset(params.h_x_bot, 0.0, params.h_n * sizeof(double));
	memset(params.h_y_bot, 0.0, params.h_n * sizeof(double));
	memset(params.v_x_right, 0.0, params.v_n * sizeof(double));
	memset(params.v_y_right, 0.0, params.v_n * sizeof(double));
}

LineLinear::~LineLinear()
{	
	if(params.h_x_top != nullptr)
	{
		free(params.h_x_top);
		params.h_x_top = nullptr;
	}
	if(params.h_y_top != nullptr)
	{
		free(params.h_y_top);
		params.h_y_top = nullptr;
	}
	if(params.h_x_bot != nullptr)
	{
		free(params.h_x_bot);
		params.h_x_bot = nullptr;
	}
	if(params.h_y_bot != nullptr)
	{
		free(params.h_y_bot);
		params.h_y_bot = nullptr;
	}
	if(params.v_x_left != nullptr)
	{
		free(params.v_x_left);
		params.v_x_left = nullptr;
	}
	if(params.v_y_left != nullptr)
	{
		free(params.v_y_left);
		params.v_y_left = nullptr;
	}
	if(params.v_x_right != nullptr)
	{
		free(params.v_x_right);
		params.v_x_right = nullptr;
	}
	if(params.v_y_right != nullptr)
	{
		free(params.v_y_right);
		params.v_y_right = nullptr;
	}
}

/**
* @brief: calcuate y = ax + b
* @return: a, b
**/
bool LineLinear::line_calculate(const double *x, const double *y, int n_data, double *a, double *b)
{
	double t1 = 0., t2 = 0., t3 = 0., t4 = 0.;
    for(int i = 0; i < n_data; i++)  
    {  
        t1 += x[i] * x[i];  
        t2 += x[i];  
        t3 += x[i] * y[i];  
        t4 += y[i];  
    }

    double bot = n_data * t1 - t2 * t2;
    if(bot == 0)
    {  
    	*a = 1.;  
        *b = 0.;  
        return false;
    }  
    else  
    {  
    	*a = (t3 * n_data - t2 * t4) / bot;
    	*b = (t1 * t4 - t2 * t3) / bot;
    	return true;
    } 
}

/**
* @brief: calculate one distance on the line, near corners's distance is same
**/
double LineLinear::same_distance(const double *x, int n_data, const double k, const double b)
{
	double *pred_y = (double*)malloc(n_data * sizeof(double));
	for(int i = 0; i < n_data; i++)
	{
		pred_y[i] = k * x[i] + b;
	}
	double points_distance = 0.0;
	for(int i = 0; i < n_data - 1; i++)
	{
		points_distance += sqrt(pow(pred_y[i+1] - pred_y[i], 2) + pow(x[i+1] - x[i], 2));
	}
	free(pred_y);
	pred_y = nullptr;

	return points_distance / (n_data - 1);
}

double LineLinear::distance_to_points(const cv::Point2f &up_pt, const cv::Point2f &down_pt,
										cv::Point2f &up_pre_pt, cv::Point2f &down_pre_pt, 
										double distance, const double k)
{
	double shift_x = cos(atan(k)) * distance;
	double shift_y = sin(atan(k)) * distance;
	up_pre_pt.x = up_pt.x - shift_x;
	up_pre_pt.y = up_pt.y - shift_y;
	down_pre_pt.x = down_pt.x + shift_x;
	down_pre_pt.y = down_pt.y + shift_y;
}

void LineLinear::set_values_and_calculate(const std::vector<cv::Point2f> &data, std::vector<cv::Point2f> &edge_points)
{
	for(int i = 0; i < params.h_n; i++)
	{
		params.h_x_top[i] = data[i].x;
		params.h_y_top[i] = data[i].y;
		params.h_x_bot[i] = data[params.h_n * (params.v_n - 1) + i].x;
		params.h_y_bot[i] = data[params.h_n * (params.v_n - 1) + i].y;
	}
	for(int i = 0; i < params.v_n; i++)
	{
		params.v_x_left[i] = data[i * params.h_n].x;
		params.v_y_left[i] = data[i * params.h_n].y;
		params.v_x_right[i] = data[i * params.h_n + params.h_n - 1].x;
		params.v_y_right[i] = data[i * params.h_n + params.h_n - 1].y;
	}

	std::vector<cv::Point2f> h_pt;
	std::vector<cv::Point2f> v_pt;
	for(int i = 0; i < 4; i++)
	{
		h_pt.push_back(cv::Point2f(0.0, 0.0));
		v_pt.push_back(cv::Point2f(0.0, 0.0));
	}

	// calculate h direction points
	LineLinear::line_calculate((const double*)params.h_x_top, (const double*)params.h_y_top, params.h_n, 
								&params.h_a_top, &params.h_b_top);
	params.h_dis_top = LineLinear::same_distance((const double*)params.h_x_top, params.h_n, params.h_a_top, params.h_b_top);
	LineLinear::distance_to_points(cv::Point2f(params.h_x_top[0], params.h_y_top[0]), 
								cv::Point2f(params.h_x_top[params.h_n-1], params.h_y_top[params.h_n-1]),
								h_pt[0], h_pt[1], params.h_dis_top, params.h_a_top);

	LineLinear::line_calculate((const double*)params.h_x_bot, (const double*)params.h_y_bot, params.h_n, 
								&params.h_a_bot, &params.h_b_bot);
	params.h_dis_bot = LineLinear::same_distance((const double*)params.h_x_bot, params.h_n, params.h_a_bot, params.h_b_bot);
	LineLinear::distance_to_points(cv::Point2f(params.h_x_bot[0], params.h_y_bot[0]), 
								cv::Point2f(params.h_x_bot[params.h_n-1], params.h_y_bot[params.h_n-1]),
								h_pt[2], h_pt[3], params.h_dis_bot, params.h_a_bot);

	// calculate v direction points
	LineLinear::line_calculate((const double*)params.v_x_left, (const double*)params.v_y_left, params.v_n, 
								&params.v_a_left, &params.v_b_left);
	params.v_dis_left = LineLinear::same_distance((const double*)params.v_x_left, params.v_n, params.v_a_left, params.v_b_left);
	LineLinear::distance_to_points(cv::Point2f(params.v_x_left[params.v_n-1], params.v_y_left[params.v_n-1]), 
								cv::Point2f(params.v_x_left[0], params.v_y_left[0]),
								v_pt[0], v_pt[1], params.v_dis_left, params.v_a_left);

	LineLinear::line_calculate((const double*)params.v_x_right, (const double*)params.v_y_right, params.v_n,
								&params.v_a_right, &params.v_b_right);
	params.v_dis_right = LineLinear::same_distance((const double*)params.v_x_right, params.v_n, params.v_a_right, params.v_b_right);
	LineLinear::distance_to_points(cv::Point2f(params.v_x_right[params.v_n-1], params.v_y_right[params.v_n-1]),
								cv::Point2f(params.v_x_right[0], params.v_y_right[0]),
								v_pt[2], v_pt[3], params.v_dis_right, params.v_a_right);

	std::cout<<params.h_dis_top<<" "<<params.h_dis_bot<<" "<<params.v_dis_left<<" "<<params.v_dis_right<<std::endl;
	
	edge_points.clear();

	edge_points.push_back(cv::Point2f(h_pt[0].x, v_pt[0].y));
	edge_points.push_back(cv::Point2f(h_pt[1].x, v_pt[1].y));
	edge_points.push_back(cv::Point2f(h_pt[2].x, v_pt[2].y));
	edge_points.push_back(cv::Point2f(h_pt[3].x, v_pt[3].y));

	h_pt.clear();
	v_pt.clear();
}

/**
* @brief: calculate triangle areas
* @forlumn: p = (a + b + c) / 2  S=√[p(p-a)(p-b)(p-c)]
**/
double LineLinear::calculate_triangle_areas(const cv::Point2f &a, const cv::Point2f &b, const cv::Point2f &c)
{
	double ad = sqrt(pow(a.x - b.x, 2) + pow(a.y - b.y, 2));
	double bd = sqrt(pow(a.x - c.x, 2) + pow(a.y - c.y, 2));
	double cd = sqrt(pow(b.x - c.x, 2) + pow(b.y - c.y, 2));
	double p = (ad + bd + cd) / 2.0;
	return sqrt(p * (p - ad) * (p - bd) * (p - cd));
}

/**
* @brief: calculate areas concindence
* @special: source is in rule, but target is not, not IOU
**/
double LineLinear::calculate_concindence(const std::vector<cv::Point2f> &data, cv::Rect user_rect)
{
	double src_areas = tell_areas(data);
	double tar_areas = user_rect.width * user_rect.height;

	double coin = 0.0;
	double overlap = 0.0;

	double xl1 = data[0].x;
	double yb1 = data[0].y;
	double xr1 = data[params.h_n * params.v_n - 1].x;
	double yt1 = data[params.h_n * params.v_n - 1].y;
	double xl2 = user_rect.x;
	double yb2 = user_rect.y;
	double xr2 = user_rect.x + user_rect.width;
	double yt2 = user_rect.y + user_rect.height;
    double xmin = std::max(xl1, xl2);
    double ymin = std::max(yb1, yb2);
    double xmax = std::min(xr1, xr2);
    double ymax = std::min(yt1, yt2);
    double width = xmax - xmin;
    double height = ymax - ymin;
    if((width <= 0) || (height <= 0))
        overlap = 0.0;
    else
    	overlap = width * height;
    
    coin = overlap / (src_areas + tar_areas - overlap);

	printf("Areas: source--->%lf, target--->%lf, concidence--->%lf\n", src_areas, tar_areas, coin);
}

/**
* @brief: calculate angle to decide rotate
* @return: angle(double), if left, then direction is (-), else (+)
**/
void LineLinear::tell_angle_to_rotate(const std::vector<cv::Point2f> &data, double *angle)
{
	for(int i = 0; i < params.h_n; i++)
	{
		params.h_x_top[i] = data[i].x;
		params.h_y_top[i] = data[i].y;
		params.h_x_bot[i] = data[params.h_n * (params.v_n - 1) + i].x;
		params.h_y_bot[i] = data[params.h_n * (params.v_n - 1) + i].y;
	}
	for(int i = 0; i < params.v_n; i++)
	{
		params.v_x_left[i] = data[i * params.h_n].x;
		params.v_y_left[i] = data[i * params.h_n].y;
		params.v_x_right[i] = data[i * params.h_n + params.h_n - 1].x;
		params.v_y_right[i] = data[i * params.h_n + params.h_n - 1].y;
	}

	LineLinear::line_calculate(params.h_x_top, params.h_y_top, params.h_n, &params.h_a_top, &params.h_b_top);

	LineLinear::line_calculate(params.h_x_bot, params.h_y_bot, params.h_n, &params.h_a_bot, &params.h_b_bot);

	*angle = fabs(atan(params.h_a_top) - atan(params.h_a_bot)) * 180.;
}

/**
* @brief: calculate board center point
**/
void LineLinear::tell_center_point(const std::vector<cv::Point2f> &data, cv::Point2f &center_point)
{
	std::vector<cv::Point2f> pts;
	pts.push_back(data[0]);
	pts.push_back(data[params.h_n - 1]);
	pts.push_back(data[params.h_n * (params.v_n - 1)]);
	pts.push_back(data[params.h_n * params.v_n - 1]);

	cv::Point2f out_top = cv::Point2f((pts[0].x + pts[1].x) / 2.0, (pts[0].y + pts[1].y) / 2.0);
	cv::Point2f out_bot = cv::Point2f((pts[2].x + pts[3].x) / 2.0, (pts[2].y + pts[3].y) / 2.0);

	double out_x = (out_top.x + out_bot.x) / 2.0;
	double out_y = (out_top.y + out_bot.y) / 2.0;

	center_point = cv::Point2f(out_x, out_y);
}

/**
* @brief: calculate board center Point error
* @special: if width_dir < 0, then tell user source is in left of target
*           if height_dir < 0, then tell user source is in top of target
**/
void LineLinear::tell_location_error(cv::Point2f &src_center_point, cv::Point2f &tar_center_point, 
									double *width_diff, bool *width_dir, double *height_diff, bool *height_dir)
{
	*width_diff = src_center_point.y - tar_center_point.y;

	if(*width_diff > 0)
		*width_dir = false;
	else
		*width_dir = true;
	
	*width_diff = fabs(*width_diff);

	*height_diff = src_center_point.x - tar_center_point.x;
	if(*height_diff > 0)
		*height_dir = false;
	else
		*height_dir = true;
	*height_diff = fabs(*height_diff);
}

/**
* @brief: calculate source quard areas
**/
double LineLinear::tell_areas(const std::vector<cv::Point2f> &data)
{
	cv::Point2f rect_1 = data[0];
	cv::Point2f rect_2 = data[params.h_n - 1];
	cv::Point2f rect_3 = data[params.h_n * (params.v_n - 2)];
	cv::Point2f rect_4 = data[params.h_n * params.v_n - 1];
	double half_a = LineLinear::calculate_triangle_areas(rect_1, rect_2, rect_3);
	double half_b = LineLinear::calculate_triangle_areas(rect_4, rect_2, rect_3);
	return half_a + half_b;
}

/**
* @brief: calculate areas errors
**/
void LineLinear::tell_areas_error(double src_areas, double tar_areas, double *err, bool *dir)
{
	*err = src_areas - tar_areas;
	if(*err > 0)
	{
		*dir = true;
	}
	else
	{
		*dir = false;
	}
	*err = fabs(*err);
}
