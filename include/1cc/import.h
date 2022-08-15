/*
 * MIT License
 *
 * Copyright (c) 2022 0x107a
 *
 */

keyword(KWRD_RETURN, "return", sizeof("return")) keyword(
    KWRD_CONTINUE,
    "continue",
    sizeof(
        "continue")) keyword(KWRD_BREAK,
                             "break",
                             sizeof(
                                 "break")) keyword(KWRD_IF,
                                                   "if",
                                                   sizeof(
                                                       "if")) keyword(KWRD_ELIF,
                                                                      "elif",
                                                                      sizeof(
                                                                          "eli"
                                                                          "f"))
    keyword(KWRD_ELSE, "else", sizeof("else")) keyword(
        KWRD_WHILE,
        "while",
        sizeof(
            "while")) keyword(KWRD_FOR,
                              "for",
                              sizeof(
                                  "for")) keyword(KWRD_DO,
                                                  "do",
                                                  sizeof(
                                                      "do")) keyword(KWRD_STATIC,
                                                                     "static",
                                                                     sizeof(
                                                                         "stati"
                                                                         "c"))
        keyword(KWRD_CONST, "const", sizeof("const")) keyword(
            KWRD_STRUCT,
            "struct",
            sizeof("struct")) keyword(KWRD_ENUM,
                                      "enum",
                                      sizeof("enum")) keyword(KWRD_UNION,
                                                              "union",
                                                              sizeof("union"))
            keyword(KWRD_CASE, "case", sizeof("case")) keyword(
                KWRD_DEFAULT,
                "default",
                sizeof("default")) keyword(KWRD_SWITCH,
                                           "switch",
                                           sizeof(
                                               "switch")) keyword(KWRD_EXTERN,
                                                                  "extern",
                                                                  sizeof(
                                                                      "extern"))
                keyword(KWRD_SIZEOF, "sizeof", sizeof("sizeof")) keyword(
                    KWRD_TYPEDEF,
                    "typedef",
                    sizeof(
                        "typedef")) keyword(KWRD_UNSIGNED,
                                            "unsigned",
                                            sizeof(
                                                "unsigned")) keyword(KWRD_SIGNED,
                                                                     "signed",
                                                                     sizeof(
                                                                         "signe"
                                                                         "d"))
                    keyword(KWRD_GOTO, "goto", sizeof("goto")) keyword(
                        KWRD_REGISTER,
                        "register",
                        sizeof("register")) keyword(KWRD_VOLATILE,
                                                    "volatile",
                                                    sizeof("volatile"))
                        token(TOK_LOGIC_NOT, "!", 1) token(
                            TOK_HASH,
                            "#",
                            1) token(TOK_DOLLAR,
                                     "$",
                                     1) token(TOK_MOD,
                                              "%",
                                              1) token(TOK_AND,
                                                       "&",
                                                       1) token(TOK_LPAREN,
                                                                "(",
                                                                1)
                            token(TOK_RPAREN, ")", 1) token(
                                TOK_MUL,
                                "*",
                                1) token(TOK_ADD,
                                         "+",
                                         1) token(TOK_COMMA,
                                                  ",",
                                                  1) token(TOK_SUB,
                                                           "-",
                                                           1) token(TOK_PERIOD,
                                                                    ".",
                                                                    1)
                                token(TOK_DIV, "/", 1) token(
                                    TOK_COLON,
                                    ":",
                                    1) token(TOK_SEMICOLON,
                                             ";",
                                             1) token(TOK_LT,
                                                      "<",
                                                      1) token(TOK_EQ,
                                                               "=",
                                                               1) token(TOK_GT,
                                                                        ">",
                                                                        1)
                                    token(TOK_QUESTION, "?", 1) token(
                                        TOK_AT,
                                        "@",
                                        1) token(TOK_LBRACK,
                                                 "[",
                                                 1) token(TOK_RBRACK,
                                                          "]",
                                                          1) token(TOK_XOR,
                                                                   "^",
                                                                   1)
                                        token(TOK_LQBRACE, "{", 1) token(
                                            TOK_RQBRACE,
                                            "}",
                                            1) token(TOK_BSLASH,
                                                     "\\",
                                                     1) token(TOK_BIT_NOT,
                                                              "~",
                                                              1) token(TOK_LTE,
                                                                       "<=",
                                                                       2)
                                            token(TOK_GTE, ">=", 2) token(
                                                TOK_EQE,
                                                "==",
                                                2) token(TOK_NTE, "!=", 2)
                                                token(TOK_ADDE, "+=", 2) token(
                                                    TOK_SUBE,
                                                    "-"
                                                    "=",
                                                    2) token(TOK_MULE,
                                                             "*=",
                                                             2) token(TOK_DIVE,
                                                                      "/=",
                                                                      2)
                                                    token(TOK_MODE,
                                                          "%"
                                                          "=",
                                                          2) token(TOK_XORE,
                                                                   "^=",
                                                                   2)
                                                        token(TOK_ANDE,
                                                              "&=",
                                                              2) token(TOK_ORE,
                                                                       "|=",
                                                                       2)
                                                            token(
                                                                TOK_INC,
                                                                "++",
                                                                2) token(TOK_DEC,
                                                                         "--",
                                                                         2)
                                                                token(
                                                                    TOK_ANDAND,
                                                                    "&&",
                                                                    2)
                                                                    token(
                                                                        TOK_OROR,
                                                                        "||",
                                                                        2)
