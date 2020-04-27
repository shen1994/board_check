#ifndef FFMPEG_H
#define FFMPEG_H

#include <unistd.h>
#include <sys/select.h>
#include <sys/time.h>
extern "C"
{
#include <libavutil/mathematics.h>
#include <libavutil/imgutils.h>
#include <libswscale/swscale.h>
#include <libavutil/pixdesc.h>
#include <libavdevice/avdevice.h>
}
#include <string>
#include <vector>
#include <iostream>

typedef struct FFMPEGTypeDefine
{
	std::string rtsp_addr;
	int video_index;
	AVFormatContext *format_ctx;
	AVCodecContext *codec_ctx;
	AVCodec *codec;

	uint8_t *out_buffer;
	AVFrame *av_frame, *av_frame_bgr;
	int width, height;
	SwsContext *convert_ctx;
	AVPacket *packet;
}FFMPEGType;

class FFMPEG
{
public:
	FFMPEG(std::string rtsp_addr);
	~FFMPEG();
	void ffmpeg_init();	
	void ffmpeg_malloc_spaces();

public:
	FFMPEGType params;
};

#endif