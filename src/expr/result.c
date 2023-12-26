//
// Created by 杨钧博 on 2023/12/26.
//
#include "result.h"

#define ERR_CNT 256
result_t results[ERR_CNT] = {
        {.code = 0, .msg = "success"},
        {.code = 1, .msg = "syntax error"}
};

result_t * success() {
    return &results[0];
}

result_t * error(int code) {
    return &results[code];
}

