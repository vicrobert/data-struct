//
// Created by 杨钧博 on 2023/12/25.
//
#include "expreval.h"
#include <string.h>

void set_cur_token(token_t * token, const char * lex, int lex_len, token_type_t token_type) {
    memset(token, 0, sizeof(token_t));
    strncpy(token->lexeme, lex, lex_len);
    token->token_type = token_type;
}