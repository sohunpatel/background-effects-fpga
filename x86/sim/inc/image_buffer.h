#ifndef __IMAGE_BUFFER_H__
#define __IMAGE_BUFFER_H__

#include <pthread.h>
#include <stdint.h>

#define BUFFER_SUCCESS        0
#define BUFFER_FULL           1
#define BUFFER_EMPTY          2
#define BUFFER_MEMCPY_FAILIRE 3

#define IMAGE_RES_WIDTH  1920
#define IMAGE_RES_HEIGHT 1080
#define BUFFER_SIZE      10

typedef struct {
  uint8_t red;
  uint8_t green;
  uint8_t blue;
} pixel_t;

typedef struct {
  pthread_mutex_t mutex;
  pixel_t *buffer;
  int buffer_size;
  int buffer_idx;
} image_buffer_t;

void image_buffer_init(image_buffer_t *image_buffer);
void image_buffer_destroy(image_buffer_t *image_buffer);
void image_buffer_lock(image_buffer_t *iamge_buffer);
void image_buffer_unlock(image_buffer_t *iamge_buffer);
int image_buffer_push(image_buffer_t *image_buffer, uint8_t *image);
pixel_t *image_buffer_pop(image_buffer_t *image_buffer);

#endif // __IMAGE_BUFFER_H__
