/*
 * MIT License
 *
 * Copyright (c) 2022 0x107a
 *
 */

#ifndef LEXER_H_
#define LEXER_H_
#include <stdlib.h>
#include <string.h>

#include "1cc/allocator.h"
#include "1cc/hashmap.h"
#include "1cc/log.h"
#include "1cc/source.h"
#include "1cc/vector.h"
#include "1cc/token.h"

typedef struct lexer Lexer;

Lexer* make_lexer(Allocator* allocator);
void lexer_destroy(Lexer* lexer);
void lexer_register(Lexer* lexer, Source* path);
Token* lexer_get(Lexer* lexer);
Token* lexer_prev(Lexer* lexer);
Token* lexer_peek(Lexer* lexer);
#endif  // LEXER_H_
