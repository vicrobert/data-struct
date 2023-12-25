//
// Created by 杨钧博 on 2023/12/25.
//

#ifndef DATA_STRUCT_OPERATOR_H
#define DATA_STRUCT_OPERATOR_H

#include "expreval.h"

token_t * calc_add(token_t * dst, token_t * op1, token_t * op2);
token_t * calc_minus(token_t * dst, token_t * op1, token_t * op2);
token_t * calc_mult(token_t * dst, token_t * op1, token_t * op2);
token_t * calc_div(token_t * dst, token_t * op1, token_t * op2);
token_t * calc_remaind(token_t * dst, token_t * op1, token_t * op2);
token_t * calc_power(token_t * dst, token_t * op1, token_t * op2);

#endif //DATA_STRUCT_OPERATOR_H
