/*
 * MIT License
 *
 * Copyright (c) 2022 u03b5
 *
 */

#ifndef HASHMAP_H_
#define HASHMAP_H_

#include <assert.h>
#include <stdlib.h>
#include <string.h>

#define MAX_MAP_SIZE 0x10000
#define TOMBSTONE ((void *)-1)

typedef struct entry Entry;
typedef struct hashmap Hashmap;

struct hashmap {
  Entry *buffer;
  size_t capacity;
  size_t no_in_use;
};

size_t fnv1a_hash(const char *key);
Hashmap *make_hashmap(size_t power);
size_t hashmap_insert(Hashmap *hashmap, const char *key, void *value);
void *hashmap_remove(Hashmap *hashmap, const char *key);
void *hashmap_retrieve(Hashmap *hashmap, const char *key);
void *hashmap_nretrieve(Hashmap *hashmap, const char *key, size_t n);
void hashmap_resize(Hashmap *hashmap);
void hashmap_destroy(Hashmap *hashmap);
#ifdef DEBUG
#include <stdio.h>
void hashmap_dump(Hashmap *hashmap);
#endif // DEBUG
#endif // HASHMAP_H_
