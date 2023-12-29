//
// Created by 杨钧博 on 2023/12/26.
//
#include "result.h"
#include <stdio.h>
#include <string.h>

#define ERR_CNT 256
result_t results[ERR_CNT] = {
        {.code = SUCC},
        {.code = UNKNOWN_ERR},
        {.code = SYNTAX_ERR},
        {.code = SYNTAX_UNKNOW_ERR},
        {.code = UNREC_SYMBOL_ERR},
        {.code = SYMBOL_NEED_ERR},
        {.code = ILLEGAL_NUM_ERR}
};
const char *result_msg[ERR_CNT] = {
        SUCC_MSG,
        UNKNOWN_ERR_MSG,
        SYNTAX_ERR_MSG,
        SYNTAX_UNKNOW_MSG,
        UNREC_SYMBOL_MSG,
        SYMBOL_NEED_MSG,
        ILLEGAL_NUM_MSG
};

result_t * success(void * data_ptr) {
    results[0].data = data_ptr;
    return &results[0];
}

result_t * error(int code) {
    strcpy(results[code].msg, result_msg[code]);
    printf("%s\n", results[code].msg);
    return &results[code];
}

result_t * error_seq(int code, const char * msg, int pos) {
    sprintf(results[code].msg, result_msg[code], msg != NULL ? msg : "", pos);
    printf("%s\n", results[code].msg);
    return &results[code];
}

result_t * error_ch(int code, char c, int pos) {
    char str[2];
    sprintf(str, "%c", c);
    sprintf(results[code].msg, result_msg[code], str, pos);
    printf("%s\n", results[code].msg);
    return &results[code];
}

