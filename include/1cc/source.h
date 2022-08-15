/*
 * MIT License
 *
 * Copyright (c) 2022 0x107a
 *
 */

#ifndef FILE_H_
#define FILE_H_
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#include "1cc/log.h"
#include "1cc/token.h"

typedef struct source Source;
struct source {
  const char* path;
  char* contents;
  char* cursor;
  int line;
  size_t size;
  Token* peek_token_cache;
  Token* prev_token_cache;
};

Source* make_source(const char* path);
Source* source_read(Source* source);
void source_destroy(Source* source);
#endif  // FILE_H_
