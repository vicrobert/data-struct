//
// Created by 杨钧博 on 2023/12/26.
//

#ifndef DATA_STRUCT_RESULT_H
#define DATA_STRUCT_RESULT_H

#define MSG_SIZE 128
#define SUCC 0
#define SUCC_MSG "Success"

#define UNKNOWN_ERR 1
#define UNKNOWN_ERR_MSG "Unknown error at %d %s"

#define SYNTAX_ERR 2
#define SYNTAX_ERR_MSG "Syntax error of '%s' at pos %d"

#define SYNTAX_UNKNOW_ERR 3
#define SYNTAX_UNKNOW_MSG "Syntax error"

#define UNREC_SYMBOL_ERR 4
#define UNREC_SYMBOL_MSG "Unrecognizable symbol '%s' at pos %d"

#define SYMBOL_NEED_ERR 5
#define SYMBOL_NEED_MSG "'%s' is need at pos %d"

#define ILLEGAL_NUM_ERR 6
#define ILLEGAL_NUM_MSG "Illegal number of '%s' at pos %d"


struct _result {
    int code;
    char msg[MSG_SIZE];
    void * data;
};
typedef struct _result result_t;

result_t * success(void * data_ptr);
result_t * error(int code);
result_t * error_seq(int code, const char * msg, int pos);
result_t * error_ch(int code, char c, int pos);

#endif //DATA_STRUCT_RESULT_H
