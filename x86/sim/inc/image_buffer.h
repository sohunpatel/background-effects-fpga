#ifndef __IMAGE_BUFFER_H__
#define __IMAGE_BUFFER_H__

#include <libavutil/frame.h>
#include <pthread.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define BUFFER_SUCCESS 0
#define BUFFER_FULL 1
#define BUFFER_EMPTY 2
#define BUFFER_MEMCPY_FAILIRE 3

#define IMAGE_RES_WIDTH 1920
#define IMAGE_RES_HEIGHT 1080
#define BUFFER_SIZE 10

/**
 * Stucture to parse the pixels in frame buffers for each rgb value
 */
typedef struct {
  uint8_t red;
  uint8_t green;
  uint8_t blue;
} pixel_t;

/**
 * Thread safe implementation of a circular buffer that contains n frames
 *
 * All of the data locking is handled by the functions in this module, so, there
 * is no need to call the lock functions yourself
 */
typedef struct {
  // mutex to handle locking of the data in this object
  pthread_mutex_t mutex;
  // buffer of frames to be passed between threads
  AVFrame *buffer;
  // index of head frame in buffer
  size_t head;
  // index of last frame in buffer
  size_t tail;
  // max capacity of buffer
  size_t max;
  // is the buffer full right now?
  bool full;
} image_buffer_t;

// allocate the memory for the buffer
void image_buffer_init(image_buffer_t *me, size_t size);
// free all memory
void image_buffer_destroy(image_buffer_t *image_buffer);

// push a frame to the buffer
int image_buffer_push(image_buffer_t *image_buffer, uint8_t *image);
// pop a frame from the buffer
pixel_t *image_buffer_pop(image_buffer_t *image_buffer);

#endif // __IMAGE_BUFFER_H__
