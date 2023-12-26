//
// Created by 杨钧博 on 2023/12/25.
//

#ifndef DATA_STRUCT_TOKEN_H
#define DATA_STRUCT_TOKEN_H

#include "expreval.h"

#define OP_TBL_SIZE 512

token_t op_token_tbl[OP_TBL_SIZE];

void init_op_token_tbl();
void set_token(token_t * dst, const char * lex, const int lex_len, const token_type_t token_type,
               const op_code_t code, const int prior);
void set_token_value(token_t * dst, double value);
token_t * map_op_token_tbl(char * lex);

#endif //DATA_STRUCT_TOKEN_H
