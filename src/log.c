/*
 * MIT License
 *
 * Copyright (c) 2022 0x107a
 *
 */

#include "1cc/log.h"

static Log g_log_handler;

const char* g_log_levels[] = {
    [LEVEL_SILENT] = BGRN "[SILENT]",   [LEVEL_INFO] = BCYN "[INFO]",
    [LEVEL_WARNING] = BYEL "[WARNING]", [LEVEL_ERROR] = BRED "[ERROR]",
    [LEVEL_FATAL] = BMAG "[FATAL]",     0};

Lhandle* make_lhandle(FILE* handle, int level, Lhandle* prev) {
  Lhandle* tmp = malloc(sizeof(Lhandle));
  tmp->handle = handle;
  tmp->level = level;
  tmp->next = 0;
  tmp->prev = prev;
  return tmp;
}

int log_register(const char* key, FILE* file, int level) {
  Lhandle *tmp, *handle = 0;
  if (g_log_handler.head) {
    for (tmp = g_log_handler.head; tmp->next; tmp = tmp->next)
      ;
    tmp->next = make_lhandle(file, level, tmp);
  } else
    handle = g_log_handler.head = make_lhandle(file, level, g_log_handler.head);
  if (handle)
    return hashmap_insert(&g_log_handler.handles, key, handle);
  return 0;
}

void log_deregister(const char* key) {
  Lhandle* tmp = hashmap_remove(&g_log_handler.handles, key);
  if (tmp) {
    tmp->prev = tmp->next;
    fclose(tmp->handle);
    free(tmp);
  }
  return;
}

void log_set_level(const char* key, int level) {
  Lhandle* handle = hashmap_retrieve(&g_log_handler.handles, key);
  if (handle)
    handle->level = level;
  return;
}

void log_internal(int level, const char* fmt, ...) {
  va_list args;
  va_start(args, fmt);
  // TODO: remove newline
  Lhandle* tmp;
  for (tmp = g_log_handler.head; tmp; tmp = tmp->next) {
    if (LEVEL_SILENT <= tmp->level && tmp->level <= LEVEL_FATAL) {
      if (tmp->level >= level) {
        fprintf(
            tmp->handle, "%s%s ",
            g_log_levels[level],  // potentially vulnerable, but i dont care.
            COLOR_RESET);
        vfprintf(tmp->handle, fmt, args);
      }
    }
  }
  va_end(args);
  return;
}

void log_shutdown(void) {
  Lhandle* tmp = g_log_handler.head;
  while (tmp) {
    g_log_handler.head = tmp->next;
    free(tmp);
    tmp = g_log_handler.head;
  }
  return;
}
