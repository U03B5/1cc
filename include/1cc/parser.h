/*
 * MIT License
 *
 * Copyright (c) 2022 u03b5
 *
 */

#ifndef PARSER_H_
#define PARSER_H_
#include "1cc/ast.h"
#include "1cc/lexer.h"
#include "1cc/log.h"
#include "1cc/token.h"

typedef struct parser Parser;
struct parser {
  //  Symtab* symbol_table;
  Arena *node_arena;
  // more
};

#endif // PARSER_H_
