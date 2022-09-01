/*
 * MIT License
 *
 * Copyright (c) 2022 0x107a
 *
 */

#ifndef AST_H_
#define AST_H_
#include "1cc/log.h"
#include "1cc/symbol.h"
#include "1cc/token.h"
#include "1cc/type.h"

// TODO: ast node kind def
enum {
  NODE_PROGRAM,
  NODE_BLOCK,
  NODE_EXPRSTMT,
  NODE_EMPTYSTMT,
  NODE_IFSTMT,
  NODE_SWITCHSTMT,
  NODE_CASESTMT,
  NODE_DEFAULTSTMT,
  NODE_FORSTMT,
  NODE_WHILESTMT,
  NODE_DOSTMT,
  NODE_BREAKSTMT,
  NODE_CONTINUESTMT,
  NODE_PASSSTMT,
  NODE_BINARYEXPR,
  NODE_UNARYEXPR,
  NODE_RELEXPR, // relational expression
  NODE_ASSIGNEXPR,
  NODE_CONDEXPR,
  NODE_CALLEXPR,
  NODE_LITERAL,
  NODE_IDENTIFIER,
  NODE_VAR
};

typedef struct node Node;
struct node {
  int kind;
  Type *type;
  Token *token;
  // additional information for various nodes
  union {
    Node *body;
    // function call
    struct {
      Node *arguments;
      Type *return_type;
    };
    // conditional or loop
    struct {
      Node *init;
      Node *step;
      // conditional
      Node *cond;
      Node *then;
      Node *els;
    };
    // expression
    struct {
      Node *lhs;
      Node *rhs;
    };
    // return statement
    struct {
      Node *return_value;
    };
    // case or default
    struct {
      Node *next_case;
      Node *default_case;
    };
  };
  Node *next;
};

#endif // AST_H_
