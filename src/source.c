/*
 * MIT License
 *
 * Copyright (c) 2022 0x107a
 *
 */

#include "1cc/source.h"

Source *make_source(const char *path) {
  Source *source = calloc(1, sizeof(Source));
  if (source) {
    source->path = path;
    source->line = 0;
    source->peek_token_cache = source->prev_token_cache = 0;
  }
  return source;
}

Source *source_read(Source *source) {
  struct stat fattr;
  int fd;
  if (source) {
    fd = open(source->path, O_RDONLY);
    if (stat(source->path, &fattr) != -1) {
      if (S_ISREG(fattr.st_mode)) {
        source->line = 1;
        source->size = fattr.st_size;
        source->contents = source->cursor =
            calloc(sizeof(char), fattr.st_size + 16);
        if (read(fd, source->contents, fattr.st_size) > 0)
          return source;
        else {
          if (source->contents)
            free(source->contents);
          LOG_ERROR("Could not read contents of file: %s\n", source->path);
        }
      } else
        LOG_ERROR("File path provided is not a normal file: %s\n",
                  source->path);
    } else
      LOG_ERROR("Could not stat file of path: %s\n", source->path);
  }
  return 0;
}

// does not free linked list, free single entry
void source_destroy(Source *source) {
  if (source) {
    if (source->contents)
      free(source->contents);
    free(source);
  }
  return;
}
