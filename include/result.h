//
// Created by 杨钧博 on 2023/12/26.
//

#ifndef DATA_STRUCT_RESULT_H
#define DATA_STRUCT_RESULT_H

#define MSG_SIZE 128
#define SUCC 0
#define UNKNOWN_ERR 1
#define SYNTAX_ERR 2

struct _result {
    int code;
    char msg[MSG_SIZE];
    void * data;
};
typedef struct _result result_t;

result_t * success(void * data_ptr);
result_t * error(int code, void * data_ptr);

#endif //DATA_STRUCT_RESULT_H
