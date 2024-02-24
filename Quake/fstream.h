#pragma once

typedef enum {
  FS_UNOPENED = 0,
  FS_STREAMING,
  FS_EOF,
  FS_ERROR,
} file_stream_status_t;

typedef struct {
  file_stream_status_t status;
  void *fp;
  int size;
  int pos;
  int read;
  char buffer[1024];
  char file[64];
  char path[64];
} file_stream_t;

void file_stream_init(file_stream_t *fs, const char *home, const char *file, int close);

char file_stream_get_char(void *params);

