/*
 * MIT License
 *
 * Copyright (c) 2022 0x107a
 *
 */

#include "1cc/lexer.h"

#define IS_UPPER_ASCII(c) ('A' <= c && c <= 'Z')
#define IS_LOWER_ASCII(c) ('a' <= c && c <= 'z')
#define IS_IDENT_ASCII(c) (IS_UPPER_ASCII(c) || IS_LOWER_ASCII(c) || c == '_')
#define IS_INTEGER(c) ('0' <= c && c <= '9')
#define IS_HEXADECIMAL(c) \
  (IS_INTEGER(c) || ('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z'))
#define IS_OCTAL(c) ('0' <= c && c <= '7')
#define IS_NEWLINE(c) (c == '\n' || c == 13)
#define IS_SKIP(c) (IS_NEWLINE(c) || c == ' ' || c == 9)
#define IS_OPERATOR(c)                                                     \
  ((33 <= c && c <= 47) || (58 <= c && c <= 64) || (91 <= c && c <= 94) || \
   (123 <= c && c <= 126))

// constant token table used internally within the lexer
const Token* g_token_table[] = {
#define token(id, key, len) \
  &(Token){.kind = id, .position = key, .length = len, .value.v = 0},
#define keyword(id, key, len) \
  &(Token){.kind = id, .position = key, .length = len, .value.v = 0},
#include "1cc/import.h"
#undef keyword
#undef token
    &(Token){0, 0, 0, 0}};

Lexer* make_lexer(Allocator* allocator) {
  Lexer* lexer = malloc(sizeof(Lexer));
  lexer->source = make_stack(0, 0);
  lexer->token_allocator = allocator;
  lexer->token_arena = allocator_register(allocator, sizeof(Token));
  lexer->token_map = make_hashmap(TOKEN_LOAD_SIZE + 1);
#define token(id, key, _) \
  hashmap_insert(lexer->token_map, key, (void*)g_token_table[id]);
#define keyword(id, key, _) \
  hashmap_insert(lexer->token_map, key, (void*)g_token_table[id]);
#include "1cc/import.h"
#undef keyword
#undef token
  return lexer;
}

void lexer_destroy(Lexer* lexer) {
  Stack* source_stack;
  if (lexer) {
    source_stack = lexer->source;
    allocator_deregister(lexer->token_allocator, sizeof(Token));
    hashmap_destroy(lexer->token_map);
    for (Stack* tmp = stack_pop(&source_stack); tmp != 0; tmp = stack_pop(&source_stack)) {
      if (tmp->value)
        source_destroy(tmp->value);
      free(tmp);
    }
    stack_destroy(lexer->source);
    free(lexer);
  }
  return;
}

void lexer_register(Lexer* lexer, Source* source) {
  if (source_read(source))
    stack_push(&lexer->source, source);
  else
    free(source);
  return;
}

static Token* tokenize_integer(Lexer* lexer) {
  Source* source = lexer->source->value;
  char* cursor = source->cursor;
  int base = 10, is_floating = 0;
  if (*cursor == '0') {
    if (cursor[1] == 'x' || cursor[1] == 'X') {
      if (IS_HEXADECIMAL(cursor[2])) {
        source->cursor += 2;
        base = 16;
      } else {
        LOG_ERROR("Invalid hexadecimal numeral %d:%s\n", source->line,
                  source->path);
        return 0;
      }
    } else if (IS_OCTAL(cursor[1])) {
      ++source->cursor;
      base = 8;
    } else if (IS_INTEGER(cursor[1]))
      LOG_ERROR(
          "Invalid numeral, may not start with 0 unless denoting base system "
          "%d:%s\n",
          source->line, source->path);
  }
  while (*source->cursor) {
    if (*source->cursor == '.') {
      if (is_floating || base != 10 || !IS_INTEGER(source->cursor[1])) {
        break;
      } else {
        ++source->cursor;
        is_floating = 1;
      }
    }
    if (IS_INTEGER(*source->cursor))
      ++source->cursor;
    else
      break;
  }
  return ALLOC(lexer->token_arena, Token,
               {.kind = (is_floating) ? FLOAT_LITERAL : INTEGER_LITERAL,
                .position = cursor,
                .length = source->cursor - cursor,
                .value.i = base,
                .line = source->line});
}

// TODO: finish this at a later stage
// its so boring implementing this. ill get at it someday
static int handle_escaped_char(Lexer* lexer) {
  Source* source = lexer->source->value;
  switch (*source->cursor) {}
  return 0;
}

static Token* tokenize_char(Lexer* lexer) {
  Source* source = lexer->source->value;
  char* cursor = source->cursor;
  int result = (*cursor == '\\') ? handle_escaped_char(lexer) : *++cursor;
  if (*++cursor == '\'')
    return (result) ? ALLOC(lexer->token_arena, Token,
                            {.kind = CHAR_LITERAL,
                             .position = cursor,
                             .length = result,
                             .value.i = 0,
                             .line = source->line})
                    : 0;
  else
    return 0;
}

static Token* tokenize_string(Lexer* lexer) {
  Source* source = lexer->source->value;
  char* cursor = ++source->cursor;
  int length = 0;
  for (; *source->cursor != '\"'; ++source->cursor) {
    if (*source->cursor == 0 || *source->cursor == '\n') {
      LOG_ERROR("Unterminated string literal on line %d of %s\n", source->line,
                source->path);
      return 0;
    }
    if (*cursor == '\\') {
      ++source->cursor;
      int result = handle_escaped_char(lexer);
      if (result)
        length += result;
      else
        return 0;
    } else
      ++length;
  }
  ++source->cursor;
  return ALLOC(lexer->token_arena, Token,
               {.kind = STRING_LITERAL,
                .position = cursor,
                .length = length + 1,
                .value.v = 0,
                .line = source->line});
}

static Token* tokenize_ident(Lexer* lexer) {
  Source* source = lexer->source->value;
  char* cursor = source->cursor;
  size_t length;
  Token* kwrd_token;
  while (*source->cursor) {
    if (IS_IDENT_ASCII(*source->cursor))
      ++source->cursor;
    else
      break;
  }
  length = source->cursor - cursor;
  kwrd_token = (Token*)hashmap_nretrieve(lexer->token_map, cursor, length);
  return (kwrd_token) ? (kwrd_token)
                      : (ALLOC(lexer->token_arena, Token,
                               {.kind = IDENTIFIER,
                                .position = cursor,
                                .length = length,
                                .value.i = 0,
                                .line = source->line}));
}

static Token* tokenize_operator(Lexer* lexer) {
  Source* source = lexer->source->value;
  char* cursor = source->cursor;
  Token* tmp = 0;
  if (IS_OPERATOR(*cursor)) {
    ++source->cursor;
    switch (*(cursor + 1)) {
      case '=':
        goto token_retrieve;
      case '+':
        goto token_retrieve;
      case '-':
        goto token_retrieve;
      case '&':
        goto token_retrieve;
      case '|':
      token_retrieve:
        tmp = (Token*)hashmap_nretrieve(lexer->token_map, cursor, 2);
        break;
      default:
        break;
    }
    return (tmp) ? (++source->cursor, tmp)
                 : ((Token*)hashmap_nretrieve(lexer->token_map, cursor, 1));
  }
  return tmp;
}

static Token* lexer_get_internal(Lexer* lexer) {
  char c;
  Source* source = lexer->source->value;
  if (*source->cursor) {
    while (IS_SKIP(*source->cursor)) {
      if (IS_NEWLINE(*source->cursor))
        ++source->line;
      ++source->cursor;
    }
    c = *source->cursor;
    switch (c) {
      case '\'':
        ++source->cursor;
        return tokenize_char(lexer);
      case '"':
        ++source->cursor;
        return tokenize_string(lexer);
      case 0:
        return 0;
      default:
        break;
    }
    if (IS_INTEGER(c))
      return tokenize_integer(lexer);
    if (IS_IDENT_ASCII(c))
      return tokenize_ident(lexer);
    if (IS_OPERATOR(c))
      return tokenize_operator(lexer);
    LOG_WARNING("Unrecognized Token %d found on line %d of %s %s:%d\n",
                *source->cursor, source->line, source->path, __FILE__,
                __LINE__);
  }
  return 0;
}

Token* lexer_get(Lexer* lexer) {
  Source* source;
  Token* tmp = 0;
  if (lexer->source->value) {
    source = lexer->source->value;
    if (source->peek_token_cache) {
      tmp = source->peek_token_cache;
      source->peek_token_cache = 0;
      return tmp;
    } else
      return source->prev_token_cache = lexer_get_internal(lexer);
  }
  return 0;
}

Token* lexer_prev(Lexer* lexer) {
  Source* source;
  if (lexer && lexer->source->value) {
    source = lexer->source->value;
    return (source->prev_token_cache) ? (source->prev_token_cache) : 0;
  }
  return 0;
}

Token* lexer_peek(Lexer* lexer) {
  Source* source;
  if (lexer && lexer->source->value) {
    source = lexer->source->value;
    if (!source->peek_token_cache) {
      if (source->cursor >= (char*)(source->contents + source->size))
        return 0;
      source->peek_token_cache = lexer_get_internal(lexer);
      source->cursor -= source->peek_token_cache->length;
    }
    return source->peek_token_cache;
  }
  return 0;
}
