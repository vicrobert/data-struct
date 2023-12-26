//
// Created by 杨钧博 on 2023/12/26.
//
#include "result.h"
#include <stdio.h>

#define ERR_CNT 256
result_t results[ERR_CNT] = {
        {.code = SUCC, .msg = "Success"},
        {.code = UNKNOWN_ERR, .msg = "Unknown error"},
        {.code = SYNTAX_ERR, .msg = "Syntax error"}
};

result_t * success(void * data_ptr) {
    results[0].data = data_ptr;
    return &results[0];
}

result_t * error(int code, void * data_ptr) {
    results[code].data = data_ptr;
    return &results[code];
}

