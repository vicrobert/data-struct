//
// Created by 杨钧博 on 2023/12/25.
//
#include "expreval.h"
#include "token.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

void set_token(token_t * dst, const char * lex, const int lex_len, const token_type_t token_type,
               const op_code_t code, const int prior) {
    memset(dst, 0, sizeof(token_t));
    strncpy(dst->lexeme, lex, lex_len);
    dst->lexeme[lex_len] = '\0';
    dst->op_code = code;
    dst->op_prior = prior;
    dst->token_type = token_type;
    if (token_type == DIGIT) {
        dst->value = strtod(dst->lexeme, NULL);
    }
}

void set_token_value(token_t * dst, double value) {
    memset(dst, 0, sizeof(token_t));
    dst->token_type = DIGIT;
    dst->value = value;
    sprintf(dst->lexeme, "%f", value);
}