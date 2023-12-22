//
// Created by 杨钧博 on 2023/12/22.
//
#include <stdio.h>
#include "stdlib.h"

#define MAX_NODES 100

int init() {
    int topo[MAX_NODES][MAX_NODES] = {0};
    int n1, n2;
    while (1) {
        scanf("%d %d\n", &n1, &n2);
        if (n1 < 0 || n2 < 0 || n1 >= 100 || n2 >= 100) break;
        topo[n1][n2] = 1;
        printf("setted\n");
    }
    for (int i = 0; i < MAX_NODES; i ++) {
        for (int j = 0; j < MAX_NODES; j++) {
            printf("%d", topo[i][j]);
        }
        printf("\n");
    }


    return 0;
}
