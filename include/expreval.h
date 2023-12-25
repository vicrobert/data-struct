//
// Created by 杨钧博 on 2023/12/25.
//

#ifndef DATA_STRUCT_EXPEVAL_H
#define DATA_STRUCT_EXPEVAL_H

#define EXPR_LEN_MAX    1024
#define FUNC_NAME_LEN_MAX   16

enum _token_type {
    NIL = 0,
    DIGIT = 1,
    ALPHABET = 2,
    OP = 3,
    FUNC = 4,
    BAD_TOKEN = 1000
};
typedef enum _token_type token_type_t;

struct _token {
    char lexeme[EXPR_LEN_MAX];
    token_type_t token_type;
};
typedef struct _token token_t;

struct _token_queue {
    token_t queue[EXPR_LEN_MAX];
    int head;
    int tail;
};
typedef struct _token_queue token_queue_t;

struct _token_stack {
    token_t stack[EXPR_LEN_MAX];
    int top;
};
typedef struct _token_stack token_stack_t;

typedef token_t * (op_func_t)(token_t *, token_t *, token_t *);

struct _func_tbl_entry {
    int symbol_code;
    op_func_t * op_func;
};
typedef struct _func_tbl_entry func_tbl_entry_t;

struct _op_tbl_entry {
    char op_code;
    char prior;
};
typedef struct _op_tbl_entry op_tbl_entry_t;

void set_cur_token(token_t * token, const char * lex, int lex_len, token_type_t token_type);

#endif //DATA_STRUCT_EXPEVAL_H
