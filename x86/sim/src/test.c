#include <libavcodec/avcodec.h>
#include <libavcodec/codec.h>
#include <libavcodec/packet.h>
#include <libavformat/avformat.h>
#include <libavformat/avio.h>
#include <libavutil/avutil.h>
#include <libavutil/error.h>
#include <libavutil/frame.h>
#include <libavutil/imgutils.h>
#include <libavutil/mem.h>
#include <libavutil/pixfmt.h>
#include <libswscale/swscale.h>
#include <stdint.h>
#include <stdio.h>

// function to switch rgb values in AVFrame
void switch_rgb_values(AVFrame *frame) {
  int width = frame->width;
  int height = frame->height;

  // iterate over each pixel
  for (int y = 0; y < height; y++) {
    for (int x = 0; x < width; x++) {
      // get the rgb values of the current pixel
      uint8_t *pixel = &frame->data[0][y * frame->linesize[0] + x * 3];
      uint8_t red = pixel[0];
      pixel[1] = red;
      pixel[2] = red;
    }
  }
}

int main() {
  // open input video file
  AVFormatContext *format_ctx = NULL;
  avformat_open_input(&format_ctx, "../x86/sim/reference.mp4", NULL, NULL);
  avformat_find_stream_info(format_ctx, NULL);

  // find video stream
  int video_stream_idx = -1;
  for (int i = 0; i < format_ctx->nb_streams; i++) {
    if (format_ctx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO) {
      video_stream_idx = i;
      break;
    }
  }

  // allocate AVFrame
  AVFrame *frame = av_frame_alloc();

  // initialize SwsContext for pixel format conversion
  struct SwsContext *sws_ctx = NULL;

  // open output video file
  AVFormatContext *output_format_ctx = NULL;
  avformat_alloc_output_context2(&output_format_ctx, NULL, NULL, "output.mp4");
  AVStream *out_stream = avformat_new_stream(output_format_ctx, NULL);
  AVCodecContext *codec_ctx = avcodec_alloc_context3(NULL);
  avcodec_parameters_to_context(codec_ctx, out_stream->codecpar);

  // open codec
  const AVCodec *codec = avcodec_find_encoder(codec_ctx->codec_id);
  avcodec_open2(codec_ctx, codec, NULL);

  // write file header
  avio_open(&output_format_ctx->pb, "output.mp4", AVIO_FLAG_WRITE);
  if (avformat_write_header(output_format_ctx, NULL) < 0) {
    fprintf(stderr, "cannot write output header");
    return -1;
  }

  // read frames
  AVPacket packet;
  while (av_read_frame(format_ctx, &packet) >= 0) {
    if (packet.stream_index == video_stream_idx) {
      // decode video packet into AVFrame
      avcodec_send_packet(codec_ctx, &packet);
      avcodec_receive_frame(codec_ctx, frame);

      // process frame
      switch_rgb_values(frame);

      // convert frame pixel format for writing
      if (!sws_ctx) {
        sws_ctx =
            sws_getContext(frame->width, frame->height, AV_PIX_FMT_RGB24,
                           codec_ctx->width, codec_ctx->height,
                           codec_ctx->pix_fmt, SWS_BICUBIC, NULL, NULL, NULL);
      }
      AVFrame *converted_frame = av_frame_alloc();
      av_image_alloc(converted_frame->data, converted_frame->linesize,
                     codec_ctx->width, codec_ctx->height, codec_ctx->pix_fmt,
                     1);
      sws_scale(sws_ctx, (const uint8_t *const *)frame->data, frame->linesize,
                0, frame->height, converted_frame->data,
                converted_frame->linesize);

      // encode frame and write to output file
      AVPacket out_packet;
      av_init_packet(&out_packet);
      int rc = avcodec_send_frame(codec_ctx, converted_frame);
      if (rc < 0) {
        fprintf(stderr, "Error encoding frame\n");
        break;
      }
      while (rc >= 0) {
        rc = avcodec_receive_packet(codec_ctx, &out_packet);
        if (rc == AVERROR(EAGAIN) || rc == AVERROR_EOF) {
          break;
        }
        if (rc < 0) {
          fprintf(stderr, "Error encoding frame\n");
          break;
        }
        av_packet_rescale_ts(&out_packet, codec_ctx->time_base,
                             out_stream->time_base);
        out_packet.stream_index = out_stream->index;
        av_write_frame(output_format_ctx, &out_packet);
        av_packet_unref(&out_packet);
      }

      // free converted frame
      av_freep(&converted_frame->data[0]);
      av_frame_free(&converted_frame);
    }

    av_packet_unref(&packet);
  }

  // write file trailer
  av_write_trailer(output_format_ctx);

  // clean up
  av_frame_free(&frame);
  if (sws_ctx) {
    sws_freeContext(sws_ctx);
  }
  avcodec_free_context(&codec_ctx);
  avformat_close_input(&format_ctx);
  avformat_free_context(output_format_ctx);

  return 0;
}
