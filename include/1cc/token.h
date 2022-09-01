/*
 * MIT License
 *
 * Copyright (c) 2022 u03b5
 *
 */

#ifndef TOKEN_H_
#define TOKEN_H_

#include <stdio.h>
#include <string.h>

enum {
#define token(id, _, __) id,
#define keyword(id, _, __) id,
#include "1cc/import.h"
#undef token
#undef keyword
  TOKEN_LOAD_SIZE,
  IDENTIFIER,
  CHAR_LITERAL,
  STRING_LITERAL,
  INTEGER_LITERAL,
  FLOAT_LITERAL
};

typedef struct token Token;
struct token {
  int kind;
  char *position;
  size_t length;
  union {
    long i;
    void *v;
  } value;
  // debug information
  int line;
};

char *token_read(Token *token, char *buffer, size_t length);
Token *token_dump(Token *token);
#endif // TOKEN_H_
