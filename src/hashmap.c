/*
 * MIT License
 *
 * Copyright (c) 2022 0x107a
 *
 */

#include "1cc/hashmap.h"

struct entry {
  const char *key;
  void *value;
};

static size_t round_p2(size_t n) {
  --n;
  n |= n >> 1;
  n |= n >> 2;
  n |= n >> 4;
  n |= n >> 8;
  n |= n >> 16;
  n |= n >> 32;
  return ++n;
}

size_t fnv1a_hash(const char *key) {
  size_t hash = 0x811c9dc5;
  for (size_t i = 0; key[i] != 0; ++i) {
    hash ^= key[i];
    hash *= 0x01000193;
  }
  return hash;
}

Hashmap *make_hashmap(size_t size) {
  Hashmap *hashmap = 0;
  if (size && size < MAX_MAP_SIZE) {
    if (!((size != 0) && !(size & (size - 1)) && size > 3)) {
      size = round_p2(size);
      if (size >= MAX_MAP_SIZE)
        return 0;
    }
    hashmap = malloc(sizeof(Hashmap));
    hashmap->capacity = size;
    hashmap->no_in_use = 0;
    hashmap->buffer = calloc(hashmap->capacity, sizeof(Entry));
  }
  return hashmap;
}

size_t hashmap_insert(Hashmap *hashmap, const char *key, void *value) {
  Entry *buffer;
  size_t q = 1, capacity, hash = 0;
  if (hashmap && hashmap->buffer && key) {
    if (hashmap->no_in_use >= hashmap->capacity)
      hashmap_resize(hashmap);
    buffer = hashmap->buffer;
    capacity = hashmap->capacity;
    hash = fnv1a_hash(key) & (capacity - 1);
    while (buffer[hash].key) {
      if (buffer[hash].key != TOMBSTONE)
        if (!strcmp(key, buffer[hash].key))
          return hash; // already exists in hashmap
      hash = (hash + (q / 2) + (q * q) / 2) & (capacity - 1);
      ++q;
    }
    buffer[hash].key = key;
    buffer[hash].value = value;
    hashmap->no_in_use++;
  }
  return hash;
}

void *hashmap_remove(Hashmap *hashmap, const char *key) {
  Entry *buffer;
  void *value;
  size_t q = 1, capacity, hash;
  if (hashmap && hashmap->buffer) {
    buffer = hashmap->buffer;
    capacity = hashmap->capacity;
    hash = fnv1a_hash(key) & (capacity - 1);
    while (buffer[hash].key) {
      if (buffer[hash].key != TOMBSTONE) {
        if (!strcmp(key, buffer[hash].key)) {
          buffer[hash].key = TOMBSTONE;
          value = buffer[hash].value;
          buffer[hash].value = 0;
          hashmap->no_in_use--;
          return value;
        }
      }
      hash = (hash + (q / 2) + (q * q) / 2) & (capacity - 1);
      ++q;
    }
  }
  return 0;
}

void *hashmap_retrieve(Hashmap *hashmap, const char *key) {
  Entry *buffer;
  size_t q = 1, capacity, hash;
  if (hashmap && hashmap->buffer) {
    buffer = hashmap->buffer;
    capacity = hashmap->capacity;
    hash = fnv1a_hash(key) & (capacity - 1);
    while (buffer[hash].key) {
      if (buffer[hash].key != TOMBSTONE) {
        if (!strcmp(key, buffer[hash].key))
          return buffer[hash].value;
      }
      hash = (hash + (q / 2) + (q * q) / 2) & (capacity - 1);
      ++q;
    }
  }
  return 0;
}

void *hashmap_nretrieve(Hashmap *hashmap, const char *key, size_t n) {
  char *new_key;
  void *value = 0;
  if (hashmap) {
    new_key = strndup(key, n);
    value = hashmap_retrieve(hashmap, new_key);
    free(new_key);
  }
  return value;
}

void hashmap_resize(Hashmap *hashmap) {
  Entry *old_buffer;
  size_t old_capacity;
  if (hashmap && hashmap->buffer) {
    old_capacity = hashmap->capacity;
    old_buffer = hashmap->buffer;
    hashmap->capacity *= 2;
    hashmap->buffer = calloc(hashmap->capacity, sizeof(Entry));
    for (size_t i = 0; i < old_capacity; ++i) {
      if (old_buffer[i].key && old_buffer[i].key != TOMBSTONE)
        hashmap_insert(hashmap, old_buffer[i].key, old_buffer[i].value);
    }
    free(old_buffer);
  }
  return;
}

void hashmap_destroy(Hashmap *hashmap) {
  if (hashmap) {
    if (hashmap->buffer)
      free(hashmap->buffer);
    free(hashmap);
  }
  return;
}

#ifdef DEBUG
void hashmap_dump(Hashmap *hashmap) {
  Entry *buffer;
  if (hashmap && hashmap->buffer) {
    printf("Hashmap Info:\nCapacity: %ld\nN in use: %ld\n", hashmap->capacity,
           hashmap->no_in_use);
    buffer = hashmap->buffer;
    for (size_t i = 0; i < hashmap->capacity; ++i) {
      if (buffer[i].key && buffer[i].key != TOMBSTONE)
        printf("Entry #%ld:\nKey: %s\nValue: %#lx\n", i, buffer[i].key,
               buffer[i].value);
    }
  }
}
#endif // DEBUG
