//
// Created by 杨钧博 on 2023/12/26.
//

#ifndef DATA_STRUCT_RESULT_H
#define DATA_STRUCT_RESULT_H

#define MSG_SIZE 128
struct _result {
    int code;
    char msg[MSG_SIZE];
};
typedef struct _result result_t;

result_t * success();
result_t * error(int code);

#endif //DATA_STRUCT_RESULT_H
