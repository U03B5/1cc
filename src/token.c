/*
 * MIT License
 *
 * Copyright (c) 2022 u03b5
 *
 */

#include "1cc/token.h"

char *token_read(Token *token, char *buffer, size_t length) {
  if (token && token->position) {
    if (buffer && length)
      return strncpy(buffer, token->position, length);
    return strndup(token->position, length);
  }
  return 0;
}

const char *kind2str(int kind) {
  switch (kind) {
  case IDENTIFIER:
    return "IDENTIFIER";
  case CHAR_LITERAL:
    return "CHAR";
  case STRING_LITERAL:
    return "STRING";
  case INTEGER_LITERAL:
    return "INTEGER";
  case FLOAT_LITERAL:
    return "FLOAT";
#define token(id, str, _)                                                      \
  case id:                                                                     \
    return str;
#define keyword(id, str, _)                                                    \
  case id:                                                                     \
    return str;
#include "1cc/import.h"
#undef keyword
#undef token
  }
  return "ERROR";
}

Token *token_dump(Token *token) {
  if (token) {
    printf("TOKEN(%s, %#lx, %ld, %#lx, %d)\n", kind2str(token->kind),
           token->position, token->length, token->value.v, token->line);
  }
  return token;
}
