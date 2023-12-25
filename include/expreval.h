//
// Created by 杨钧博 on 2023/12/25.
//

#ifndef DATA_STRUCT_EXPEVAL_H
#define DATA_STRUCT_EXPEVAL_H

#define EXPR_LEN_MAX    1024

enum _token_type {
    NIL = 0,
    DIGIT = 1,
    ALPHABET = 2,
    OP = 3,
    BAD_TOKEN = 1000
};
typedef enum _token_type token_type_t;

enum _op_code {
    OP_NULL = 0,
    OP_ADD = 1,
    OP_MINUS = 2,
    OP_MULT = 3,
    OP_DIV = 4,
    OP_REMMAIND = 5,
    OP_LBRAC = 6,
    OP_RBRAC = 7,
    OP_POWER = 8,
    OP_SIN = 9,
    OP_COS = 10,
    OP_TAN = 11,
    OP_CTAN = 12,
    /** 13-19 are reserved for extending **/
    OP_LOG = 20
};
typedef enum _op_code op_code_t;

#define OP_LEX_MAXLEN 8
struct _token {
    token_type_t token_type;
    op_code_t op_code;
    int op_prior;
    double value;
    char lexeme[OP_LEX_MAXLEN];
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

#define FUNC_TBL_SIZE 100
typedef token_t * (op_func_t)(token_t *, token_t *, token_t *);
struct _func_tbl_entry {
    op_code_t op_code;
    op_func_t * op_func;
};
typedef struct _func_tbl_entry func_tbl_entry_t;

#endif //DATA_STRUCT_EXPEVAL_H
