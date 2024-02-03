//
// Created by 杨钧博 on 2024/1/26.
//
#include "tensor.h"
#include <stdio.h>
#include <stdlib.h>

tensor_t * _tensor_init(int data_size) {
    tensor_t * tensor_ptr = malloc(sizeof(tensor_t));
    tensor_ptr->data = malloc(data_size * sizeof(double));
    tensor_ptr->dim_size = malloc(1 * sizeof(int));
    tensor_ptr->dim = 1;
    tensor_ptr->dim_size[0] = data_size;
    return  tensor_ptr;
}

void _tensor_free(tensor_t ** tensor_ptr) {
    free((* tensor_ptr)->data);
    free((* tensor_ptr)->dim_size);
    free((* tensor_ptr));
    (* tensor_ptr) = NULL;
}


