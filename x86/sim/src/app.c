#include "app.h"
#include "mp4.h"
#include <asm-generic/errno-base.h>
#include <libavcodec/avcodec.h>
#include <libavcodec/packet.h>
#include <libavutil/error.h>
#include <libavutil/frame.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

void app_init(app_t *me, CLIArgs args) {
    me->filepath_in = args.file_in;
    me->filepath_out = args.file_out;

    mp4_init(&me->input_file, args.file_in);

    if (args.file_out) {
        me->filepath_out = args.file_out;
        mp4_init(&me->output_file, me->filepath_out);
    } else {
        me->filepath_out = NULL;
    }
}

void app_destroy(app_t *me) {
    mp4_destroy(&me->input_file);
    if (me->filepath_out) {
        mp4_destroy(&me->output_file);
    }
}

void app_run(app_t *me) {
    printf("Starting conversion\n");

    int n_frames = 0;

    AVPacket packet;
    while (av_read_frame(me->input_file.format_context, &packet) >= 0) {
        if (packet.stream_index == me->input_file.video_stream) {
            AVFrame *frame = av_frame_alloc();
            int response =
                avcodec_send_packet(me->input_file.codec_context, &packet);
            if (response < 0 || response == AVERROR(EAGAIN) ||
                response == AVERROR_EOF) {
                break;
            }
            response =
                avcodec_receive_frame(me->input_file.codec_context, frame);
            if (response == AVERROR(EAGAIN) || response == AVERROR_EOF) {
                av_frame_free(&frame);
                continue;
            }

            // process frame
            int width = frame->width;
            int height = frame->height;

            // Iterate over each pixel
            for (int y = 0; y < height; y++) {
                for (int x = 0; x < width; x++) {
                    uint8_t *pixel =
                        &frame->data[0][y * frame->linesize[0] + x * 3];
                    uint8_t red = pixel[0];
                    pixel[1] = red;
                    pixel[2] = red;
                }
            }
            n_frames++;

            av_frame_free(&frame);
        }
    }

    printf("There we %i frames\n", n_frames);
}
