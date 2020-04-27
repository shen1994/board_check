#include "ffmpeg.h"

FFMPEG::FFMPEG(std::string rtsp_addr)
{
	params.rtsp_addr = rtsp_addr;
}

FFMPEG::~FFMPEG()
{
	params.video_index = -1;
	av_packet_free(&params.packet);
	av_frame_free(&params.av_frame);
	av_frame_free(&params.av_frame_bgr);
	if(!(nullptr == params.out_buffer))
	{
		av_free(params.out_buffer);
		params.out_buffer = nullptr;
	}
		
	sws_freeContext(params.convert_ctx);
	avcodec_close(params.codec_ctx);
	avformat_close_input(&params.format_ctx);
}

/**
* @brief: init ffmpeg packages and decoders
**/
void FFMPEG::ffmpeg_init()
{
	av_register_all();
	avformat_network_init();

    params.format_ctx = avformat_alloc_context();

	AVDictionary *av_dict_ptr = nullptr;
	av_dict_set(&av_dict_ptr, "rtsp_transport", "tcp", 0);
	av_dict_set(&av_dict_ptr, "stimeout", "2000000", 0);
    av_dict_set(&av_dict_ptr, "max_delay", "500000", 0);

	while (avformat_open_input(&params.format_ctx, params.rtsp_addr.c_str(), nullptr, &av_dict_ptr) != 0)
	{
	    std::cout << "Camera: Can't open the rtsp address." << std::endl;
	    int millisecs = 500;
	    struct timeval tval;
    	tval.tv_sec = millisecs / 1000.;
    	tval.tv_usec = (int)(millisecs * 1000) % 1000000;
    	select(0, nullptr, nullptr, nullptr, &tval);
	}

	av_dict_free(&av_dict_ptr);

	if (avformat_find_stream_info(params.format_ctx, nullptr) < 0)
	{
	    std::cout << "Camera: Can't find stream infomation." << std::endl;
	    return ;
	}

	params.video_index = -1;
	while(params.video_index == -1)
	{
		for (int n = 0; n < params.format_ctx->nb_streams; n++)
		{
			if (params.format_ctx->streams[n]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO)
			{
			    params.video_index = n;
			    break;
			}
		}
		if (params.video_index == -1)
		{
		    std::cout << "Camera: Can't find a video stream." << std::endl;
		    int millisecs = 1000;
	        struct timeval tval;
    		tval.tv_sec = millisecs / 1000.;
    		tval.tv_usec = (int)(millisecs * 1000) % 1000000;
    		select(0, nullptr, nullptr, nullptr, &tval);
		}	 
	}   	

	params.codec_ctx = avcodec_alloc_context3(nullptr);
	avcodec_parameters_to_context(params.codec_ctx, 
									params.format_ctx->streams[params.video_index]->codecpar);
	params.codec = avcodec_find_decoder(params.codec_ctx->codec_id);

	params.codec_ctx->bit_rate = 0;
	params.codec_ctx->time_base.num = 1;
	params.codec_ctx->time_base.den = 10;
	params.codec_ctx->frame_number = 1;

	if (params.codec == nullptr)
	{
	    std::cout << "Camera: Can't find a codec." << std::endl;
	    return ;
	}

	if (avcodec_open2(params.codec_ctx, params.codec, nullptr) < 0)
	{
	    std::cout << "Camera: Can't open a codec." << std::endl;
	    return ;
	}

	AVPixelFormat pixel_format;
	switch(params.codec_ctx->pix_fmt)
	{
	    case AV_PIX_FMT_YUVJ420P:
	    	pixel_format = AV_PIX_FMT_YUV420P;
	    	break;
	    case AV_PIX_FMT_YUVJ422P:
	    	pixel_format = AV_PIX_FMT_YUV422P;
	    	break;
	    case AV_PIX_FMT_YUVJ444P:
	    	pixel_format = AV_PIX_FMT_YUV444P;
	    	break;
	    case AV_PIX_FMT_YUVJ440P:
	    	pixel_format = AV_PIX_FMT_YUV440P;
	    	break;
	    default:
	    	pixel_format = AV_PIX_FMT_YUV420P;
	    	break;
	}

    params.convert_ctx = sws_getContext(params.codec_ctx->width, params.codec_ctx->height,
                                     pixel_format, params.codec_ctx->width, params.codec_ctx->height,
                                     AV_PIX_FMT_BGR24, SWS_BICUBIC, nullptr, nullptr, nullptr);	
}

/**
* @brief: malloc spaces for each camera and get (width, height) from image
**/
void FFMPEG::ffmpeg_malloc_spaces()
{
	params.av_frame = av_frame_alloc();
    params.av_frame_bgr = av_frame_alloc();

    int num_bytes = av_image_get_buffer_size(AV_PIX_FMT_BGR24, params.codec_ctx->width, 
    										params.codec_ctx->height, 1);
    params.out_buffer = (uint8_t *) av_malloc(num_bytes * sizeof(uint8_t));
    av_image_fill_arrays(params.av_frame_bgr->data, params.av_frame_bgr->linesize,
    					params.out_buffer, AV_PIX_FMT_BGR24,
                   		params.codec_ctx->width, params.codec_ctx->height, 1);

    params.packet = (AVPacket *) malloc(sizeof(AVPacket));
    av_new_packet(params.packet, params.codec_ctx->width * params.codec_ctx->height);

    params.width = params.codec_ctx->width;
    params.height = params.codec_ctx->height;
}
