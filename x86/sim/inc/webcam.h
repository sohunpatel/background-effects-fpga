#ifndef __WEBCAM_H__
#define __WEBCAM_H__

#include <bits/pthreadtypes.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef struct buffer {
  //  data containing image data
  uint8_t *start;
  // ammount of data in the buffer
  size_t length;
} buffer_t;

typedef struct webcam {
  // device name
  char *name;
  // file descriptor we are reading from
  int fd;
  // all buffers used for webcam handling
  buffer_t *buffers;
  // number of buffers being used
  uint8_t nbuffers;

  // current frame
  buffer_t frame;
  // webcam handler thread
  pthread_t thread;
  // lock to ensure that this is thread-safe
  pthread_mutex_t mtx_frame;

  // definitions of frame format
  uint16_t width;
  uint16_t height;
  uint8_t colorspace;
  char formats[16][5];
  bool streaming;
} webcam_t;

// initialize a webcam
webcam_t *webcam_open(const char *dev);
// destroy a webcam
void webcam_close(webcam_t *me);
// resize the webcam
void webcam_resize(webcam_t *me, uint16_t width, uint16_t height);
// start streaming the webcam video stream
void webcam_stream(webcam_t *me, bool flag);
// grab a frame from the  webcam
buffer_t webcam_grab(webcam_t *me);

#endif // !__WEBCAM_H__
