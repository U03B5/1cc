/*
 * MIT License
 *
 * Copyright (c) 2022 u03b5
 *
 */

#include "1cc/allocator.h"

static size_t round_size(size_t size) {
  return (size + (size % 8)) + sizeof(void *);
}

void *memdup(Arena *arena, const void *src, size_t size) {
  return memcpy(allocator_malloc(arena), src, size);
}

// for extensions
Arena *make_arena(size_t size) {
  return arena_emplace(malloc(sizeof(Arena)), size);
}

void arena_free(Arena *arena) {
  free(arena_deplace(arena));
  return;
}

// arena
Arena *arena_emplace(Arena *arena, size_t size) {
  size = round_size(size);
  Chunk *tmp;
  arena->chunk_size = size;
  arena->capacity = DEFAULT_ARENA_CAPACITY;
  arena->extend_arena = 0;
  tmp = arena->pointer = arena->memory =
      calloc(arena->capacity, arena->chunk_size);
  for (int i = 0; i < arena->capacity - 1; ++i) {
    tmp->next_chunk = (Chunk *)((char *)tmp + arena->chunk_size);
    tmp = tmp->next_chunk;
  }
  tmp->next_chunk = 0;
  return arena;
}

Arena *arena_deplace(Arena *arena) {
  if (arena) {
    arena->chunk_size = arena->capacity = 0;
    if (arena->memory)
      free(arena->memory);
    arena->memory = 0;
  }
  return arena;
}

// extend memory
Arena *arena_extend(Arena *arena) {
  if (arena) {
    while (arena->extend_arena)
      arena = arena->extend_arena;
    return (arena->extend_arena = make_arena(arena->chunk_size));
  }
  return 0;
}

// allocator
Allocator *make_allocator(void) {
  Allocator *allocator = malloc(sizeof(Allocator));
  allocator->n_arenas = 0;
  memset(allocator->memory_arenas, 0, sizeof(Arena) * MAX_ARENA);
  return allocator;
}

Arena *allocator_register(Allocator *allocator, size_t size) {
  size = round_size(size);
  Arena *memory;
  if (allocator && allocator->n_arenas <= MAX_ARENA) {
    memory = allocator->memory_arenas;
    for (int i = 0; i < MAX_ARENA; ++i) {
      if (memory[i].chunk_size == size)
        return &memory[i]; // arena for type already exists
    }
    ++allocator->n_arenas;
    return arena_emplace(&memory[(int)allocator->n_arenas++], size);
  }
  return 0;
}

Arena *allocator_get_arena(Allocator *allocator, size_t size) {
  size = round_size(size);
  Arena *arenas = allocator->memory_arenas;
  for (int i = 0; i < MAX_ARENA; ++i)
    if (arenas[i].chunk_size == size)
      return &arenas[i];
  return 0;
}

void allocator_deregister(Allocator *allocator, size_t size) {
  size = round_size(size);
  Arena *arena = allocator_get_arena(allocator, size), *tmp;
  if (arena) {
    arena_deplace(arena);
    arena = arena->extend_arena;
    while (arena) {
      tmp = arena;
      arena = arena->extend_arena;
      arena_free(tmp);
    }
    --allocator->n_arenas;
  }
  return;
}

void *allocator_malloc(Arena *arena) {
  Chunk *chunk = 0;
  if (arena) {
    if (!arena->pointer)
      arena->pointer = arena_extend(arena)->memory;
    chunk = arena->pointer;
    arena->pointer = arena->pointer->next_chunk;
  }
  return chunk;
}

// we dont have to explicitly free chunks
// just nice to have?
void allocator_free(Arena *arena, void *memory) {
  if (arena && memory) {
    arena->pointer->next_chunk = arena->memory;
    arena->memory = arena->pointer;
  }
  return;
}

// caller must first deregister arenas before destroying allocator
void allocator_destroy(Allocator *allocator) {
  if (allocator)
    free(allocator);
  return;
}
