#include<stdio.h>

/*#if defined(WIN32) && !defined(__cplusplus)  
#define inline __inline  
#endif */ 	

#include "libavformat/avformat.h"
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>

int main(int argc, char* argv[])
{
	//init all codec / format 
	av_register_all();

	AVFormatContext * pFormatCtx = NULL;
	if (avformat_open_input(&pFormatCtx, "file:D:/Work/Project/C_CPP/FFmpegTutorial/Debug/test.mp4", NULL, NULL) != 0)
	{
		return -1;
	}

	if (avformat_find_stream_info(pFormatCtx, NULL) < 0)
	{
		return -1;
	}

	av_dump_format(pFormatCtx, 0, "file:D:/Work/Project/C_CPP/FFmpegTutorial/Debug/test.mp4", 0);

	unsigned i;
	AVCodecContext *pCodecCtxOrig = NULL;
	AVCodecContext *pCodecCtx = NULL;
	
	int videoStream = -1;
	for(i = 0; i < pFormatCtx->nb_streams; i++)
	{
		if (pFormatCtx->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO) 
		{
			videoStream = i;
		}
	}

	if (-1 == videoStream) {
		return -1;
	}

	pCodecCtxOrig = pFormatCtx->streams[videoStream]->codec;

	AVCodec *pCodec = NULL;
	pCodec = avcodec_find_decoder(pCodecCtxOrig->codec_id);
	if (pCodec == NULL) 
	{
		return -1;
	}

	pCodecCtx = avcodec_alloc_context3(pCodec);
	AVCodecParameters *pAVCodecParam = avcodec_parameters_alloc();
	avcodec_parameters_from_context(pAVCodecParam, pCodecCtxOrig);
	avcodec_parameters_to_context(pCodecCtx, pAVCodecParam);

	if(avcodec_open2(pCodecCtx, pCodec, NULL) < 0)
	{
		return -1;
	}

	AVFrame *pframe = av_frame_alloc();
	
	//int numBytes = avpicture_get_size(AV_PIX_FMT_RGB24, pCodecCtx->width , pCodecCtx->height);
	int numBytes = av_image_get_buffer_size(AV_PIX_FMT_RGB24, pCodecCtx->width, pCodecCtx->height, 16);
	uint8_t *buffer = (uint8_t*)av_malloc(numBytes);

	//avpicture_fill((AVPicture*) pframe, buffer, AV_PIX_FMT_RGB24, pCodecCtx->width, pCodecCtx->height);
	//av_image_fill_arrays(buffer, 16, pframe->buf->);
	av_image_copy_to_buffer(buffer, numBytes, pframe->data[4], pframe->linesize[4], AV_PIX_FMT_RGB24, pCodecCtx->width, pCodecCtx->height, 16);
	return 0;
}