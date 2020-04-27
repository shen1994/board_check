#include <iostream>

#include "ffmpeg.h"
#include "callback.h"
 
int main(int argc, char *argv[])
{
    //if(argc < 7)
    //{
    //    printf("args are too less. existing.\n");
    //    return -1;
    //}

    int image_counter = 0;
    bool found_corners = false;
    std::vector<cv::Point2f> corners;
    cv::Size board_size = cv::Size(atoi(argv[2]), atoi(argv[3]));
    cv::Mat gray, rimage, target_image;
    cv::Size target_size = cv::Size(atoi(argv[6]), atoi(argv[5]));

    // state one object to read streams from camera
    FFMPEG ffmpeg_obj(argv[1]);
    ffmpeg_obj.ffmpeg_init();
    ffmpeg_obj.ffmpeg_malloc_spaces();

    // start one thread to capture images
    Callback camera_callback(ffmpeg_obj.params);
    camera_callback.camera_start();

    cv::namedWindow("capture-window", CV_WINDOW_NORMAL);

    while(true)
    {
        if(camera_callback.camera_param.copy_ok)
        {
            corners.clear();
            rimage.release(); gray.release();
            cv::resize(camera_callback.camera_param.image, rimage, cv::Size(640, 480));
            cv::cvtColor(rimage, gray, CV_BGR2GRAY);
            found_corners = cv::findChessboardCorners(gray, board_size, corners,
                        CV_CALIB_CB_ADAPTIVE_THRESH | CV_CALIB_CB_FILTER_QUADS);
            if(found_corners)
            {
                drawChessboardCorners(rimage, board_size, corners, found_corners);
            }
            if(cv::waitKey(30) > 0)
            {
                target_image.release();
                std::string image_path(argv[4]);
                cv::resize(camera_callback.camera_param.image, target_image, target_size);
                cv::imwrite(image_path + "/image" + std::to_string(image_counter) + ".png", target_image);
                printf("Now saving image: %d\n", image_counter);
                image_counter += 1;
            }
            cv::imshow("capture-window", rimage);
            camera_callback.camera_param.copy_ok = false;     
        }
        cv::waitKey(1);
    }

    // destroy all windows and release resources
    cv::destroyWindow("capture-window");
 
    return 0;
}
