//
// Created by 杨钧博 on 2023/12/25.
//
#include "expreval.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define UNUSED(x) ((x) = (x))

token_t * calc_add(token_t * dst, token_t * op1, token_t * op2) {
    char sum_str[EXPR_LEN_MAX] = {0};
    double op1_num = strtod(op1->lexeme, NULL);
    double op2_num = strtod(op2->lexeme, NULL);
    double sum = op1_num + op2_num;
    sprintf(sum_str, "%f", sum);
    set_cur_token(dst, sum_str, strlen(sum_str), DIGIT);
    return dst;
}

token_t * calc_minus(token_t * dst, token_t * op1, token_t * op2) {
    char minus_str[EXPR_LEN_MAX] = {0};
    double op1_num = strtod(op1->lexeme, NULL);
    double op2_num = strtod(op2->lexeme, NULL);
    double minus = op1_num - op2_num;
    sprintf(minus_str, "%f", minus);
    set_cur_token(dst, minus_str, strlen(minus_str), DIGIT);
    return dst;
}

token_t * calc_mult(token_t * dst, token_t * op1, token_t * op2) {
    char mult_str[EXPR_LEN_MAX] = {0};
    double op1_num = strtod(op1->lexeme, NULL);
    double op2_num = strtod(op2->lexeme, NULL);
    double mult = op1_num * op2_num;
    sprintf(mult_str, "%f", mult);
    set_cur_token(dst, mult_str, strlen(mult_str), DIGIT);
    return dst;
}

token_t * calc_div(token_t * dst, token_t * op1, token_t * op2) {
    char div_str[EXPR_LEN_MAX] = {0};
    double op1_num = strtod(op1->lexeme, NULL);
    double op2_num = strtod(op2->lexeme, NULL);
    double div = op1_num / op2_num;
    sprintf(div_str, "%f", div);
    set_cur_token(dst, div_str, strlen(div_str), DIGIT);
    return dst;
}

token_t * calc_remaind(token_t * dst, token_t * op1, token_t * op2) {
    char remaind_str[EXPR_LEN_MAX] = {0};
    double op1_num = strtod(op1->lexeme, NULL);
    double op2_num = strtod(op2->lexeme, NULL);
    double remaind = fmod(op1_num, op2_num);
    sprintf(remaind_str, "%f", remaind);
    set_cur_token(dst, remaind_str, strlen(remaind_str), DIGIT);
    return dst;
}

token_t * calc_power(token_t * dst, token_t * op1, token_t * op2) {
    char res_str[EXPR_LEN_MAX] = {0};
    double op1_num = strtod(op1->lexeme, NULL);
    double op2_num = strtod(op2->lexeme, NULL);
    double res = pow(op1_num, op2_num);
    sprintf(res_str, "%f", res);
    set_cur_token(dst, res_str, strlen(res_str), DIGIT);
    return dst;
}

token_t * calc_oppos(token_t * dst, token_t * op1, token_t * op2) {
    UNUSED(op2);
    char oppos_str[EXPR_LEN_MAX] = {0};
    double op1_num = strtod(op1->lexeme, NULL);
    sprintf(oppos_str, "%f", - op1_num);
    set_cur_token(dst, oppos_str, strlen(oppos_str), DIGIT);
    return dst;
}