#include "callback.h"

Callback::Callback(FFMPEGType &_ffmpeg_param)
{
    ffmpeg_param = _ffmpeg_param;
    camera_param.is_running = false;
    camera_param.copy_ok = false;
    camera_param.image = cv::Mat(ffmpeg_param.height, ffmpeg_param.width, CV_8UC3);
}

Callback::~Callback()
{
    camera_param.is_running = false;
    camera_param.copy_ok = false;
    camera_param.image.release();
}

/**
* @brief: run threads for each camera
**/
void* Callback::camera_run(void* args)
{
	int read_ret = 0;

    while(this->camera_param.is_running)
    {
        read_ret = av_read_frame(this->ffmpeg_param.format_ctx, this->ffmpeg_param.packet);
        if (read_ret >= 0 && this->ffmpeg_param.packet->stream_index == this->ffmpeg_param.video_index)
        {
            int got_frame = 0;
            int used_flag = 0;
            used_flag = avcodec_send_packet(this->ffmpeg_param.codec_ctx, this->ffmpeg_param.packet);
            if (used_flag < 0 && used_flag != AVERROR(EAGAIN) && used_flag != AVERROR_EOF){}
            else 
            {
                if (used_flag >= 0) this->ffmpeg_param.packet->size = 0;
                used_flag = avcodec_receive_frame(this->ffmpeg_param.codec_ctx, this->ffmpeg_param.av_frame);
                if (used_flag >= 0) got_frame = 1;
            }

            if (got_frame == 1)
            {
                sws_scale(this->ffmpeg_param.convert_ctx, (uint8_t const * const *) this->ffmpeg_param.av_frame->data,
                        this->ffmpeg_param.av_frame->linesize, 0, this->ffmpeg_param.codec_ctx->height, 
                        this->ffmpeg_param.av_frame_bgr->data, this->ffmpeg_param.av_frame_bgr->linesize);

                // here we get one image to deal
                if(!this->camera_param.copy_ok)
                {
                	memcpy(this->camera_param.image.data, this->ffmpeg_param.av_frame_bgr->data[0],
                        	this->ffmpeg_param.width * this->ffmpeg_param.height * 3 * sizeof(uint8_t)); 
                	this->camera_param.copy_ok = true;
                }
            }
        }
        av_packet_unref(this->ffmpeg_param.packet);
    }    
}

void Callback::camera_start()
{
	camera_param.is_running = true;
    thread_type callback = (thread_type)&Callback::camera_run;
    pthread_create(&camera_param.tid, nullptr, callback, this);
}

void Callback::camera_stop()
{
    camera_param.is_running = false;
}