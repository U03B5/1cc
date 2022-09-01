/*
 * MIT License
 *
 * Copyright (c) 2022 u03b5
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
#endif // TYPE_H_
