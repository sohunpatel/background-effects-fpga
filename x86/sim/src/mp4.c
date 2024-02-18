#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavutil/avutil.h>
#include <libswscale/swscale.h>
#include <stdio.h>
#include <stdlib.h>

#include "image_buffer.h"
#include "mp4.h"

void mp4_init(mp4_t *me, char *filename) {
    me->format_context = NULL;
    if (avformat_open_input(&me->format_context, filename, NULL, NULL) != 0) {
        fprintf(stderr, "cannot open input file: %s\n", filename);
        return;
    }

    if (avformat_find_stream_info(me->format_context, NULL) < 0) {
        fprintf(stderr, "Couldn't find stream information.\n");
        return;
    }

    me->codec = NULL;
    me->video_stream = av_find_best_stream(
        me->format_context, AVMEDIA_TYPE_VIDEO, -1, -1, &me->codec, 0);
    if (me->video_stream < 0) {
        fprintf(stderr, "Couldn't find video stream.\n");
        return;
    }

    me->codec_context = avcodec_alloc_context3(me->codec);
    avcodec_parameters_to_context(
        me->codec_context,
        me->format_context->streams[me->video_stream]->codecpar);
    avcodec_open2(me->codec_context, me->codec, NULL);
}

void mp4_destroy(mp4_t *me) {
    avformat_close_input(&me->format_context);
    avcodec_free_context(&me->codec_context);
}

pixel_t *mp4_decode(FILE *input_file) {
    AVFormatContext *formatContext = NULL;

    pixel_t *image = malloc(IMAGE_RES_HEIGHT * IMAGE_RES_WIDTH);

    return image;
}
