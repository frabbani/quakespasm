#include "fstream.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void file_stream_init(file_stream_t *fs, const char *home, const char *file, int close) {
  if (close && fs->fp)
    fclose(fs->fp);

  strcpy(fs->path, home);
  strcpy(fs->file, file);

  fs->status = FS_UNOPENED;
  fs->fp = NULL;
  fs->size = fs->pos = fs->read = 0;
}

char file_stream_get_char(void *params) {
  file_stream_t *fs = params;
  if (fs->status == FS_ERROR || fs->status == FS_EOF) {
    return '\0';
  }

  if (FS_UNOPENED == fs->fp) {
    char filename[128];
    sprintf(filename, "%s/%s", fs->path, fs->file);

    fs->fp = fopen(filename, "r");
    if (!fs->fp) {
      printf("%s - FAILED to open file '%s'\n", __FUNCTION__, filename);
      fs->status = FS_ERROR;
      return '\0';
    }
    fseek(fs->fp, 0, SEEK_END);
    fs->size = ftell(fs->fp);
    if (0 == fs->size) {
      printf("%s - INVALID FILE SIZE for file '%s'!\n", __FUNCTION__, filename);
      fs->status = FS_ERROR;
      fclose(fs->fp);
      fs->fp = NULL;
      return '\0';
    }
    fs->status = FS_STREAMING;
    fseek(fs->fp, 0, SEEK_SET);
    printf("%s - file '%s' opened for streaming (size=%d)\n", __FUNCTION__, filename, (int) fs->size);
    fs->pos = fs->read = 0;
  }

  if (FS_STREAMING == fs->status) {
    if (fs->pos == fs->read) {
      fs->pos = 0;
      fs->read = fread(fs->buffer, 1, sizeof(fs->buffer), fs->fp);
      if (0 == fs->read) {
        char filename[128];
        sprintf(filename, "%s/%s", fs->path, fs->file);

        fs->status = feof(fs->fp) ? FS_EOF : FS_ERROR;
        fclose(fs->fp);
        fs->fp = NULL;
        printf("%s - file '%s' closed\n", __FUNCTION__, filename);
        return '\0';
      }
    }
    return fs->buffer[fs->pos++];
  }

  return '\0';
}
