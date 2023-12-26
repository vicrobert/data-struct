//
// Created by 杨钧博 on 2023/12/25.
//
#include "expreval.h"
#include "token.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define UNUSED(x) ((x) == (x))

token_t * calc_add(token_t * dst, token_t * op1, token_t * op2) {
    set_token_value(dst, op1->value + op2->value);
    return dst;
}

token_t * calc_minus(token_t * dst, token_t * op1, token_t * op2) {
    set_token_value(dst, op1->value - op2->value);
    return dst;
}

token_t * calc_mult(token_t * dst, token_t * op1, token_t * op2) {
    set_token_value(dst, op1->value * op2->value);
    return dst;
}

token_t * calc_div(token_t * dst, token_t * op1, token_t * op2) {
    set_token_value(dst, op1->value / op2->value);
    return dst;
}

token_t * calc_remaind(token_t * dst, token_t * op1, token_t * op2) {
    set_token_value(dst, fmod(op1->value, op2->value));
    return dst;
}

token_t * calc_power(token_t * dst, token_t * op1, token_t * op2) {
    set_token_value(dst, pow(op1->value, op2->value));
    return dst;
}


// 三角函数
token_t * calc_sin(token_t * dst, token_t * op1, token_t * op2) {
    UNUSED(op1);
    set_token_value(dst, sin(op1->value));
    return dst;
}

token_t * calc_cos(token_t * dst, token_t * op1, token_t * op2) {
    UNUSED(op2);
    set_token_value(dst, cos(op1->value));
    return dst;
}

token_t * calc_tan(token_t * dst, token_t * op1, token_t * op2) {
    UNUSED(op2);
    set_token_value(dst, tan(op1->value));
    return dst;
}

token_t * calc_ctan(token_t * dst, token_t * op1, token_t * op2) {
    UNUSED(op2);
    set_token_value(dst, 1 / tan(op1->value));
    return dst;
}

// 对数
token_t * calc_log(token_t * dst, token_t * op1, token_t * op2) {
    UNUSED(op2);
    set_token_value(dst, log(op1->value));
    return dst;
}