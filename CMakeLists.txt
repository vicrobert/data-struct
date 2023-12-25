cmake_minimum_required(VERSION 3.26)
project(data_struct C)

set(CMAKE_C_STANDARD 11)

include_directories(include)

add_executable(data_struct
        include/list.h
        src/topo/toposort.c
        src/expr/expreval.c
        src/expr/operator.c
        src/expr/token.c)
