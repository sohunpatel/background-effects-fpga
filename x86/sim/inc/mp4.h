#ifndef __MP4_H__
#define __MP4_H__

#include <libavcodec/avcodec.h>
#include <libavcodec/packet.h>
#include <libavformat/avformat.h>
#include <stdio.h>

#include "image_buffer.h"

/**
 * Contains the required data to read, write, and manipulate .mp4 files
 */
typedef struct {
  // i/o context for the format of the .mp4 file
  AVFormatContext *format_context;
  // main codec api
  AVCodecContext *codec_context;
  // static structure to contain codec
  const AVCodec *codec;
  // placeholder for each packet being decoded
  // TODO: make sure this needs to be here, this kind of seems like a
  //       temporary variable for each iteration through frames
  AVPacket packet;
  // video stream
  // TODO: determine what this actually refers to
  int video_stream;
} mp4_t;

// initialize all the memory to hold the mp4 file
void mp4_init(mp4_t *me, char *filename);
// free all of the memory
void mp4_destroy(mp4_t *me);
// decode for the next frame
// TODO: rethink the function signature. i believe pixel_t should be replaced
//       with AVFrame and the arguments should just be mp4_t
pixel_t *mp4_decode(FILE *input_file);

#endif // __MP4_H__
