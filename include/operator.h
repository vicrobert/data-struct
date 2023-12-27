//
// Created by 杨钧博 on 2023/12/25.
//

#ifndef DATA_STRUCT_OPERATOR_H
#define DATA_STRUCT_OPERATOR_H

#include "token.h"

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
    OP_LOG = 20,
    OP_FACTOR = 21
};
typedef enum _op_code op_code_t;

#define FUNC_TBL_SIZE 100
typedef token_t * (op_func_t)(token_t *, token_t *, token_t *);
struct _func_tbl_entry {
    op_code_t op_code;
    op_func_t * op_func;
};
typedef struct _func_tbl_entry func_tbl_entry_t;

token_t * calc_add(token_t * dst, token_t * op1, token_t * op2);
token_t * calc_minus(token_t * dst, token_t * op1, token_t * op2);
token_t * calc_mult(token_t * dst, token_t * op1, token_t * op2);
token_t * calc_div(token_t * dst, token_t * op1, token_t * op2);
token_t * calc_remaind(token_t * dst, token_t * op1, token_t * op2);
token_t * calc_power(token_t * dst, token_t * op1, token_t * op2);

token_t * calc_sin(token_t * dst, token_t * op1, token_t * op2);
token_t * calc_cos(token_t * dst, token_t * op1, token_t * op2);
token_t * calc_tan(token_t * dst, token_t * op1, token_t * op2);
token_t * calc_ctan(token_t * dst, token_t * op1, token_t * op2);
token_t * calc_log(token_t * dst, token_t * op1, token_t * op2);
token_t * calc_factor(token_t * dst, token_t * op1, token_t * op2);

#endif //DATA_STRUCT_OPERATOR_H
