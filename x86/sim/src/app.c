#include <asm-generic/errno-base.h>
#include <libavcodec/avcodec.h>
#include <libavcodec/packet.h>
#include <libavutil/error.h>
#include <libavutil/frame.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "app.h"
#include "mp4.h"

/**
 * Initialize the app_t data structure
 *   @param[in]  me    the app data structure that you would like to intialize
 *   @param[in]  args  the parsed and organized cli arguments containing
 *                     filepaths
 *
 * Any call of this function must be followed be a call of app_destory(me) where
 * me is the same object used here
 */
void app_init(app_t *me, CLIArgs args) {
  // add filepaths to app stucture
  me->filepath_in = args.file_in;
  me->filepath_out = args.file_out;

  // initialize the input .mp4 file
  mp4_init(&me->input_file, args.file_in);

  if (args.file_out) {
    // if the filepath for an output file exists, add the path the app data
    // and initialize the .mp4 file
    me->filepath_out = args.file_out;
    mp4_init(&me->output_file, me->filepath_out);
  } else {
    // setting the output path will allow us to use me->filepath_out as a
    // check to see if we need to write to a file
    me->filepath_out = NULL;
  }
}

/**
 * Destory the app data structure and free any allocated memory
 *   @param[in]  me  the app data structure to be destroyed
 */
void app_destroy(app_t *me) {
  mp4_destroy(&me->input_file);
  if (me->filepath_out) {
    mp4_destroy(&me->output_file);
  }
}

/**
 * Handles all of multithreading done to ensure that this function can be run
 * within a 30 Hz frequency that my webcam captures at
 *   @param[in]  me  app data to run algos with
 *
 * TODO: Right now, for simplicity, this function is a single threaded function.
 *       This is inefficient and I should implement this with CUDA and/or
 *       multithreading so that I can keep up with my webcam (30 Hz).
 */
void app_run(app_t *me) {
  // TODO: change this message to something more meangingful or remove it
  printf("Starting conversion\n");

  // TODO: remove this code. this was here as an original place holder
  int n_frames = 0;

  // captures the current packet that i am manipulating
  AVPacket packet;
  // iterates through all of the frames until i reach EOF
  while (av_read_frame(me->input_file.format_context, &packet) >= 0) {
    // make sure that i am reading from the correct video stream
    // TODO: understand exactly what a video stream is in this context
    if (packet.stream_index == me->input_file.video_stream) {
      // send raw packet data to a decoder
      int response = avcodec_send_packet(me->input_file.codec_context, &packet);
      // break if there is an error
      if (response < 0 || response == AVERROR(EAGAIN) ||
          response == AVERROR_EOF) {
        break;
      }
      AVFrame *frame = av_frame_alloc();
      // receive decoded frame from the decoder
      response = avcodec_receive_frame(me->input_file.codec_context, frame);
      // break if there is an error, making sure to free the frame if so
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
          // Get the appropriate (x,y) pixel
          uint8_t *pixel = &frame->data[0][(y * frame->linesize[0]) + (x * 3)];
          // convert the green and blue values to red (should look
          // like a red filter)
          uint8_t red = pixel[0];
          pixel[1] = red;
          pixel[2] = red;
        }
      }

      n_frames++;

      // if there are no errors, ensure that the frame is freed
      av_frame_free(&frame);
    }
  }

  // TODO: change this message to something more meaningful or remove it
  printf("There we %i frames\n", n_frames);
}
