/*
 * MIT License
 *
 * Copyright (c) 2022 0x107a
 *
 */

// pool allocator for large consecutive allocations
// allocator rounds to multiples of 8 instead of 16 (glibc ptmalloc)
// this should only be used when we are sure of consecutive allocations, such as
// when allocating tokens or nodes. another benefit, is that we dont have to
// explicitly free all allocated object, we can just simply free the pool. uses
// malloc internally, just bulk allocates for us. no need for calloc/realloc,
// just bloat.
// TODO: add chunk marking capabilities for debugging.
#ifndef ALLOCATOR_H_
#define ALLOCATOR_H_

#include <stdlib.h>
#include <string.h>

// max amount of arenas per allocator
#define MAX_ARENA 32
#define DEFAULT_ARENA_CAPACITY 128

typedef struct chunk Chunk;
struct chunk {
  Chunk *next_chunk;
};

typedef struct arena Arena;
struct arena {
  Chunk *memory;
  Chunk *pointer;
  size_t capacity;
  // each chunk size
  int chunk_size;
  Arena *extend_arena;
};

typedef struct allocator Allocator;
struct allocator {
  Arena memory_arenas[MAX_ARENA];
  // amount of arenas
  char n_arenas;
};

void *memdup(Arena *arena, const void *src, size_t size);
#define ALLOC(arena, type, ...)                                                \
  (type *)memdup(arena, (type[]){__VA_ARGS__}, sizeof(type))
Arena *arena_emplace(Arena *arena, size_t size);
Arena *arena_deplace(Arena *arena);
Arena *arena_extend(Arena *arena);
Allocator *make_allocator(void);
Arena *allocator_register(Allocator *allocator, size_t size);
Arena *allocator_get_arena(Allocator *allocator, size_t size);
void allocator_deregister(Allocator *allocator, size_t size);
void *allocator_malloc(Arena *arena);
void allocator_free(Arena *arena, void *memory);
void allocator_destroy(Allocator *allocator);
#ifdef DEBUG
void allocator_test(void);
#endif // DEBUG
#endif // ALLOCATOR_H_
