//
// Created by 杨钧博 on 2023/12/25.
//
#include "operator.h"
#include "token.h"
#include <string.h>
#include <math.h>

const func_tbl_entry_t op_func_tbl[FUNC_TBL_SIZE] = {
        {OP_NULL, NULL}, //PADDING
        {OP_ADD, calc_add},
        {OP_MINUS, calc_minus},
        {OP_MULT, calc_mult},
        {OP_DIV, calc_div},
        {OP_REMMAIND, calc_remaind},
        {OP_LBRAC, NULL},
        {OP_RBRAC, NULL},
        {OP_POWER, calc_power},
        /** 9-20 reserved for extending **/
        {OP_SIN, calc_sin},
        {OP_COS, calc_cos},
        {OP_TAN, calc_tan},
        {OP_CTAN, calc_ctan},
        /** 9-20 end **/
        {OP_NULL, NULL}, //PADDING
        {OP_NULL, NULL}, //PADDING
        {OP_NULL, NULL}, //PADDING
        {OP_NULL, NULL}, //PADDING
        {OP_NULL, NULL}, //PADDING
        {OP_NULL, NULL}, //PADDING
        {OP_NULL, NULL}, //PADDING
        {OP_LOG, calc_log},
        {OP_FACTOR, calc_factor}
};

token_t op_token_tbl[OP_TBL_SIZE] = { 0 };
void init_op_token_tbl() {
    // op code
    op_token_tbl['+'].op_code = OP_ADD;
    op_token_tbl['+'].token_type = BIN_OP;
    op_token_tbl['+'].op_prior = 1;
    op_token_tbl['+'].lexeme[0] = '+';
    op_token_tbl['-'].op_code = OP_MINUS;
    op_token_tbl['-'].token_type = BIN_OP;
    op_token_tbl['-'].op_prior = 1;
    op_token_tbl['-'].lexeme[0] = '-';
    op_token_tbl['*'].op_code = OP_MULT;
    op_token_tbl['*'].token_type = BIN_OP;
    op_token_tbl['*'].op_prior = 2;
    op_token_tbl['*'].lexeme[0] = '*';
    op_token_tbl['/'].op_code = OP_DIV;
    op_token_tbl['/'].token_type = BIN_OP;
    op_token_tbl['/'].op_prior = 2;
    op_token_tbl['/'].lexeme[0] = '/';
    op_token_tbl['%'].op_code = OP_REMMAIND;
    op_token_tbl['%'].token_type = BIN_OP;
    op_token_tbl['%'].op_prior = 2;
    op_token_tbl['%'].lexeme[0] = '%';
    op_token_tbl['('].op_code = OP_LBRAC;
    op_token_tbl['('].token_type = OP;
    op_token_tbl['('].op_prior = 0;
    op_token_tbl['('].lexeme[0] = '(';
    op_token_tbl[')'].op_code = OP_RBRAC;
    op_token_tbl[')'].token_type = OP;
    op_token_tbl[')'].op_prior = 0;
    op_token_tbl[')'].lexeme[0] = ')';
    op_token_tbl['^'].op_code = OP_POWER;
    op_token_tbl['^'].token_type = BIN_OP;
    op_token_tbl['^'].op_prior = 2;
    op_token_tbl['^'].lexeme[0] = '^';
    op_token_tbl['!'].op_code = OP_FACTOR;
    op_token_tbl['!'].token_type = UN_OP;
    op_token_tbl['!'].op_prior = 2;
    op_token_tbl['!'].lexeme[0] = '!';
    //from pos 256 to start high-level function
    op_token_tbl[256].op_code = OP_SIN;
    op_token_tbl[256].token_type = UN_OP;
    op_token_tbl[256].op_prior = 1;
    op_token_tbl[257].op_code = OP_COS;
    op_token_tbl[257].token_type = UN_OP;
    op_token_tbl[257].op_prior = 1;
    op_token_tbl[258].op_code = OP_TAN;
    op_token_tbl[258].token_type = UN_OP;
    op_token_tbl[258].op_prior = 1;
    op_token_tbl[259].op_code = OP_CTAN;
    op_token_tbl[259].token_type = UN_OP;
    op_token_tbl[259].op_prior = 1;
    op_token_tbl[267].op_code = OP_LOG;
    op_token_tbl[267].token_type = BIN_OP;
    op_token_tbl[267].op_prior = 1;
    strncpy(op_token_tbl[256].lexeme, "sin", 3);
    strncpy(op_token_tbl[257].lexeme, "cos", 3);
    strncpy(op_token_tbl[258].lexeme, "tan", 3);
    strncpy(op_token_tbl[259].lexeme, "ctan", 4);
    strncpy(op_token_tbl[267].lexeme, "log", 3);
}

token_t * map_op_token_tbl(char * lex) {
    int i;
    if (op_token_tbl[lex[0]].op_code != OP_NULL)
        return &op_token_tbl[lex[0]];
    for (i = 256; i < OP_TBL_SIZE; i ++) {
        if (!strncmp(lex, op_token_tbl[i].lexeme, strlen(lex))) {
            return &op_token_tbl[i];
        }
    }
    return NULL;
}
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

//阶乘
token_t * calc_factor(token_t * dst, token_t * op1, token_t * op2) {
    UNUSED(op2);
    double res = 1;
    for (int i = 2; i <= op1->value; i ++)
        res *= i;
    set_token_value(dst, res);
    return dst;
}