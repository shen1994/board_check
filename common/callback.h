#ifndef CALLBACK_H
#define CALLBACK_H

#include <opencv2/opencv.hpp>
#include "ffmpeg.h"

typedef void* (*thread_type)(void*);

typedef struct camera_params
{
    // base params
    pthread_t tid;
    cv::Mat image;
    bool copy_ok;
    bool is_running;
}CamParamsType;

class Callback{
public:
	Callback(FFMPEGType &_ffmpeg_params);
	~Callback();
	void* camera_run(void* args);
    void camera_start();
    void camera_stop();

public:
	CamParamsType camera_param;
    FFMPEGType ffmpeg_param;
};

#endif