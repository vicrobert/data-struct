//
// Created by 杨钧博 on 2024/1/26.
//

#ifndef DATA_STRUCT_TENSOR_H
#define DATA_STRUCT_TENSOR_H

struct _tensor {
    int dim;
    int * dim_size;
    double * data;
};
typedef struct _tensor tensor_t;
#endif //DATA_STRUCT_TENSOR_H
