/*
 * MIT License
 *
 * Copyright (c) 2022 0x107a
 *
 */

#ifndef TYPE_H_
#define TYPE_H_

// enum {};

typedef struct type Type;
struct type {
  int kind;
  int size;
  int align;
};
#endif  // TYPE_H_
