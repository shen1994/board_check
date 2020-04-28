#include <iostream>

#include "params.h"
#include "config.h"
#include "ffmpeg.h"
#include "callback.h"
#include "line_linear.h"
#include "detect_corners.h"
#include "draw_ui.h"
 
int main( )
{ 

    //state config for camera and image params
    Config options;
    options.Init((char *) "config.ini");

    // state one object to read streams from camera
    FFMPEG ffmpeg_obj(options.params.rtsp_addr);
    ffmpeg_obj.ffmpeg_init();
    ffmpeg_obj.ffmpeg_malloc_spaces();

    int board_cows = options.params.cows, board_rows = options.params.rows;
    float board_pixel = options.params.pixels;

    int board_shift_x = options.params.shift_x, board_shift_y = options.params.shift_y;
    int board_iner_cows = board_cows - 1;
    int board_iner_rows = board_rows - 1;
    int board_width = (int)(board_iner_cows * board_pixel);
    int board_height = (int)(board_iner_rows * board_pixel);
    int board_x = (int)((ffmpeg_obj.params.width - board_iner_cows * board_pixel) / 2.0f - board_shift_x);
    int board_y = (int)((ffmpeg_obj.params.height - board_iner_rows * board_pixel) / 2.0f - board_shift_y);
    cv::Rect user_rect(board_x, board_y, board_width, board_height);
    cv::Point2f tar_center_point(board_x + board_width / 2.0, board_y + board_height / 2.0);
    double tar_areas = board_iner_cows * board_iner_rows * board_pixel * board_pixel;

    // state one object to resize image and detect corners
    std::vector< cv::Point2f > corners; corners.clear();
    std::vector< cv::Point3f > objs; objs.clear();
    cv::Mat K_double, D_double, K_float, D_float;
    for (int i = 0; i < board_iner_rows; i++)
      for (int j = 0; j < board_iner_cows; j++)
        objs.push_back(cv::Point3f((float)j * options.params.square_size, 
            (float)i * options.params.square_size, 0.));
    Detect detect_obj(board_cows, board_rows, ffmpeg_obj.params.width, ffmpeg_obj.params.height, 1.0);
    cv::Mat target_points, image_points;
    cv::Mat(objs).convertTo(target_points, CV_32F);
    cv::FileStorage fs(options.params.calib_file.c_str(), cv::FileStorage::READ);
    fs["K"] >> K_double; fs["D"] >> D_double;
    K_double.convertTo(K_float, CV_64F);
    D_double.convertTo(D_float, CV_64F);
    //printf("%f\n", K.ptr<double>(0)[0]);

    // state one object to calculate angle, center point, area, etc.
    LineLinear linear_obj(board_cows, board_rows);

    // state one object to draw ui for users
    UI ui_obj = UI();

    // define params to calculate and guide user to adjust
    ParamsType device_params;

    // start one thread to capture images
    Callback camera_callback(ffmpeg_obj.params);
    camera_callback.camera_start();

    cv::namedWindow("chess-check", CV_WINDOW_NORMAL);

    while(true)
    {
        if(camera_callback.camera_param.copy_ok)
        {
            bool found_corners = false;
            // camera_callback.camera_param.image = cv::imread("../test/image0.png", 1);
            found_corners = detect_obj.core_detect(camera_callback.camera_param.image, corners);

            // draw default ui
            //ui_obj.ui_draw_target(camera_callback.camera_param.image, user_rect, 
            //            tar_center_point, board_cows - 1, board_rows - 1);
            ui_obj.ui_draw_default(camera_callback.camera_param.image);

            if(found_corners)
            {
                // pnp calculation
                cv::Mat rvec, tvec, Rmatrix, Tmatrix, RInv_temp, RInv;
                cv::Mat cam_loc, cen_loc, cen_world_loc;
                cv::Mat(corners).convertTo(image_points, CV_64F);
                cv::solvePnP(target_points, image_points, K_float, D_float, rvec, tvec);
                cv::Rodrigues(rvec, Rmatrix);

                double theta_x = atan2(Rmatrix.at<double>(2, 1), Rmatrix.at<double>(2, 2)) * 57.2958f;
                double theta_y = atan2(-Rmatrix.at<double>(2, 0), 
                    sqrt(Rmatrix.at<double>(2, 1) * Rmatrix.at<double>(2, 1) + Rmatrix.at<double>(2, 2) * Rmatrix.at<double>(2, 2)))* 57.2958;
                double theta_z = atan2(Rmatrix.at<double>(1, 0), Rmatrix.at<double>(0, 0))* 57.2958;

                // camera location
                RInv_temp = Rmatrix.inv();
                RInv_temp.convertTo(RInv, CV_64F);
                tvec.convertTo(Tmatrix, CV_64F);
                cam_loc = RInv * Tmatrix;

                float cam_x2 = (cam_loc.at<double>(0)) * (cam_loc.at<double>(0));
                float cam_y2 = (cam_loc.at<double>(1)) * (cam_loc.at<double>(1));
                float cam_z2 = (cam_loc.at<double>(2)) * (cam_loc.at<double>(2));
                float cam_dist = sqrt(cam_x2 + cam_y2 + cam_z2);
                cam_dist = fabs(cam_loc.at<double>(2));

                // copy params to device params
                device_params.distance = cam_dist;
                device_params.raw_angle = theta_x;
                device_params.pitch_angle = theta_y;
                device_params.yoll_angle = theta_z;
                //printf("distance: %f\n", fabs(cam_loc.at<double>(2)));

                // calculate angles
                //linear_obj.tell_angle_to_rotate(corners, &device_params.board_angle);

                // calculate center points
                //linear_obj.tell_center_point(corners, device_params.center_point);

                // tell top and bot, left and right direction
                //linear_obj.tell_location_error(device_params.center_point, tar_center_point, 
                //            &device_params.top_bot_value, &device_params.top_bot_dir, 
                //            &device_params.left_right_value, &device_params.left_right_dir);

                // calculate areas
                //device_params.areas = linear_obj.tell_areas(corners);
                //linear_obj.tell_areas_error(device_params.areas, tar_areas, 
                //            &device_params.areas_error, &device_params.areas_dir);

                // calculate scores --- > toDo

                // draw related ui
                ui_obj.ui_draw_text(camera_callback.camera_param.image, device_params);
                ui_obj.ui_draw_params(camera_callback.camera_param.image, device_params);
                ui_obj.ui_draw_source(camera_callback.camera_param.image, corners, 
                            device_params.center_point, board_cows - 1, board_rows - 1);
            }
            cv::Mat rimage;
            cv::resize(camera_callback.camera_param.image, rimage, cv::Size(1280, 720));
            cv::imshow("chess-check", rimage);
            camera_callback.camera_param.copy_ok = false;     
        }
        cv::waitKey(1);
    }

    // destroy all windows and release resources
    cv::destroyWindow("chess-check");
 
    return 0;
}
