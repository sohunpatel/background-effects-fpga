#ifndef __MP4_H__
#define __MP4_H__

#include <libavcodec/avcodec.h>
#include <libavcodec/packet.h>
#include <libavformat/avformat.h>
#include <stdio.h>

#include "image_buffer.h"

typedef struct {
    AVFormatContext *format_context;
    AVCodecContext *codec_context;
    const AVCodec *codec;
    AVPacket packet;
    int video_stream;
} mp4_t;

void mp4_init(mp4_t *me, char *filename);
void mp4_destroy(mp4_t *me);
pixel_t *mp4_decode(FILE *input_file);

#endif // __MP4_H__
