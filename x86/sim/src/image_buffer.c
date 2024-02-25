#include <assert.h>
#include <pthread.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "image_buffer.h"

void image_buffer_init(image_buffer_t *me, size_t size) {
  pthread_mutex_init(&me->mutex, NULL);
  me->buffer = malloc(IMAGE_RES_WIDTH * IMAGE_RES_HEIGHT * BUFFER_SIZE *
                      sizeof(pixel_t));

  me->max = size;
}

void image_buffer_destroy(image_buffer_t *me) {
  pthread_mutex_destroy(&me->mutex);
  free(me->buffer);
}

static void image_buffer_lock(image_buffer_t *me) {
  pthread_mutex_lock(&me->mutex);
}

static void image_buffer_unlock(image_buffer_t *me) {
  pthread_mutex_unlock(&me->mutex);
}

int image_buffer_push(image_buffer_t *me, uint8_t *image) {
  // if (me->buffer_size <= 0) {
  //     return BUFFER_FULL;
  // }

  // memcpy(me->buffer + me->buffer_size++, image,
  //        IMAGE_RES_WIDTH * IMAGE_RES_HEIGHT * sizeof(pixel_t));

  return BUFFER_SUCCESS;
}

pixel_t *image_buffer_pop(image_buffer_t *me) {
  pixel_t *image = malloc(IMAGE_RES_WIDTH * IMAGE_RES_HEIGHT * sizeof(pixel_t));
  return image;
}
