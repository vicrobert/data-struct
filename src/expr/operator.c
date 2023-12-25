//
// Created by 杨钧博 on 2023/12/25.
//
#include "expreval.h"
#include "token.h"
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
    set_token(dst, sum_str, strlen(sum_str), DIGIT, OP_ADD, 0, sum);
    return dst;
}

token_t * calc_minus(token_t * dst, token_t * op1, token_t * op2) {
    char minus_str[EXPR_LEN_MAX] = {0};
    double op1_num = strtod(op1->lexeme, NULL);
    double op2_num = strtod(op2->lexeme, NULL);
    double minus = op1_num - op2_num;
    sprintf(minus_str, "%f", minus);
    set_token(dst, minus_str, strlen(minus_str), DIGIT, OP_MINUS, 0, minus);
    return dst;
}

token_t * calc_mult(token_t * dst, token_t * op1, token_t * op2) {
    char mult_str[EXPR_LEN_MAX] = {0};
    double op1_num = strtod(op1->lexeme, NULL);
    double op2_num = strtod(op2->lexeme, NULL);
    double mult = op1_num * op2_num;
    sprintf(mult_str, "%f", mult);
    set_token(dst, mult_str, strlen(mult_str), DIGIT, OP_MULT, 0, mult);
    return dst;
}

token_t * calc_div(token_t * dst, token_t * op1, token_t * op2) {
    char div_str[EXPR_LEN_MAX] = {0};
    double op1_num = strtod(op1->lexeme, NULL);
    double op2_num = strtod(op2->lexeme, NULL);
    double div = op1_num / op2_num;
    sprintf(div_str, "%f", div);
    set_token(dst, div_str, strlen(div_str), DIGIT, OP_DIV, 0, div);
    return dst;
}

token_t * calc_remaind(token_t * dst, token_t * op1, token_t * op2) {
    char remaind_str[EXPR_LEN_MAX] = {0};
    double op1_num = strtod(op1->lexeme, NULL);
    double op2_num = strtod(op2->lexeme, NULL);
    double remaind = fmod(op1_num, op2_num);
    sprintf(remaind_str, "%f", remaind);
    set_token(dst, remaind_str, strlen(remaind_str), DIGIT, OP_REMMAIND, 0, remaind);
    return dst;
}

token_t * calc_power(token_t * dst, token_t * op1, token_t * op2) {
    char res_str[EXPR_LEN_MAX] = {0};
    double op1_num = strtod(op1->lexeme, NULL);
    double op2_num = strtod(op2->lexeme, NULL);
    double res = pow(op1_num, op2_num);
    sprintf(res_str, "%f", res);
    set_token(dst, res_str, strlen(res_str), DIGIT, OP_POWER, 0, res);
    return dst;
}


// 三角函数
token_t * calc_sin(token_t * dst, token_t * op1, token_t * op2) {
    UNUSED(op1);
    char sin_str[EXPR_LEN_MAX] = {0};
    double op2_num = strtod(op2->lexeme, NULL);
    double sin_num = sin(op2_num);
    sprintf(sin_str, "%f", sin_num);
    set_token(dst, sin_str, strlen(sin_str), DIGIT, OP_SIN, 0, sin_num);
    return dst;
}

token_t * calc_cos(token_t * dst, token_t * op1, token_t * op2) {
    UNUSED(op2);
    char cos_str[EXPR_LEN_MAX] = {0};
    double op1_num = strtod(op1->lexeme, NULL);
    double cos_num = cos(op1_num);
    sprintf(cos_str, "%f", cos_num);
    set_token(dst, cos_str, strlen(cos_str), DIGIT, OP_COS, 0, cos_num);
    return dst;
}

token_t * calc_tan(token_t * dst, token_t * op1, token_t * op2) {
    UNUSED(op2);
    char tan_str[EXPR_LEN_MAX] = {0};
    double op1_num = strtod(op1->lexeme, NULL);
    double tan_num = sin(op1_num);
    sprintf(tan_str, "%f", tan_num);
    set_token(dst, tan_str, strlen(tan_str), DIGIT, OP_TAN, 0, tan_num);
    return dst;
}

token_t * calc_ctan(token_t * dst, token_t * op1, token_t * op2) {
    UNUSED(op2);
    char ctan_str[EXPR_LEN_MAX] = {0};
    double op1_num = strtod(op1->lexeme, NULL);
    double ctan_num = 1.0 / tan(op1_num);
    sprintf(ctan_str, "%f", ctan_num);
    set_token(dst, ctan_str, strlen(ctan_str), DIGIT, OP_CTAN, 0, ctan_num);
    return dst;
}

// 对数
token_t * calc_log(token_t * dst, token_t * op1, token_t * op2) {
    UNUSED(op2);
    char log_str[EXPR_LEN_MAX] = {0};
    double op1_num = strtod(op1->lexeme, NULL);
    double log_num = sin(op1_num);
    sprintf(log_str, "%f", log_num);
    set_token(dst, log_str, strlen(log_str), DIGIT, OP_LOG, 0, log_num);
    return dst;
}