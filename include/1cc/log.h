/*
 * MIT License
 *
 * Copyright (c) 2022 0x107a
 *
 */

#ifndef LOG_H_
#define LOG_H_
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "1cc/color.h"
#include "1cc/hashmap.h"

enum { LEVEL_SILENT, LEVEL_INFO, LEVEL_WARNING, LEVEL_ERROR, LEVEL_FATAL };

typedef struct lhandle Lhandle;
struct lhandle {
  FILE* handle;
  int level;
  // we should not have too many outputs
  // ideally, stdout/stderr should be the only ones
  Lhandle* next;
  Lhandle* prev;
};

typedef struct log Log;
struct log {
  Lhandle* head;
  Hashmap handles;
};

Lhandle* make_lhandle(FILE* handle, int level, Lhandle* prev);
int log_register(const char* key, FILE* file, int level);
void log_deregister(const char* key);
void log_set_level(const char* key, int level);
void log_shutdown(void);

// log_internal handled by macro to prevent overflow of static const string
// arrays
void log_internal(int level, const char* fmt, ...);
#define LOG_SILENT(fmt, ...) log_internal(LEVEL_SILENT, fmt, __VA_ARGS__);
#define LOG_INFO(fmt, ...) log_internal(LEVEL_INFO, fmt, __VA_ARGS__);
#define LOG_WARNING(fmt, ...) log_internal(LEVEL_WARNING, fmt, __VA_ARGS__);
#define LOG_ERROR(fmt, ...) log_internal(LEVEL_ERROR, fmt, __VA_ARGS__);
#define LOG_FATAL(fmt, ...) log_internal(LEVEL_FATAL, fmt, __VA_ARGS__);
// zero overhead, you pay for what you use
// if we want to debug and trace compile steps, then we can via LOG_DEBUG macro.
#ifdef DEBUG
#define LOG_DEBUG(fmt, ...) LOG_SILENT(fmt, __VA_ARGS__);
#else
#define LOG_DEBUG(...)
#endif  // DEBUG
#endif  // LOG_H_
